#ifndef BINDTEXTUREPATCH_H
#define BINDTEXTUREPATCH_H

// DDS format structure
struct DDSFormat {
    quint32 dwSize;
    quint32 dwFlags;
    quint32 dwHeight;
    quint32 dwWidth;
    quint32 dwLinearSize;
    quint32 dummy1;
    quint32 dwMipMapCount;
    quint32 dummy2[11];
    struct {
        quint32 dummy3[2];
        quint32 dwFourCC;
        quint32 dummy4[5];
    } ddsPixelFormat;
};

// compressed texture pixel formats
#define FOURCC_DXT1  0x31545844
#define FOURCC_DXT2  0x32545844
#define FOURCC_DXT3  0x33545844
#define FOURCC_DXT4  0x34545844
#define FOURCC_DXT5  0x35545844

#ifndef GL_COMPRESSED_RGB_S3TC_DXT1_EXT
#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT   0x83F0
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT  0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT  0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT  0x83F3
#endif

#ifndef GL_GENERATE_MIPMAP_SGIS
#define GL_GENERATE_MIPMAP_SGIS       0x8191
#define GL_GENERATE_MIPMAP_HINT_SGIS  0x8192
#endif




typedef void (APIENTRY *pfn_glCompressedTexImage2DARB) (GLenum, GLint, GLenum, GLsizei,
                                                        GLsizei, GLint, GLsizei, const GLvoid *);
static pfn_glCompressedTexImage2DARB qt_glCompressedTexImage2DARB = 0;

typedef QHash<QString, DdsData> QGLDDSCache;
Q_GLOBAL_STATIC(QGLDDSCache, qgl_dds_cache)
;

static void forgetChachedTextures(){
  QGLDDSCache::iterator it;
  for (it=qgl_dds_cache()->begin(); it!=qgl_dds_cache()->end(); it++){
    unsigned int b = it.value().bind;
    if (b>0) glDeleteTextures(1,&b);
  }
  qgl_dds_cache()->clear();
}

static bool myBindTexture(const QString &fileName, DdsData &data)
{
    if (!qt_glCompressedTexImage2DARB) {

      QGLContext cx(QGLFormat::defaultFormat());
      qt_glCompressedTexImage2DARB = (pfn_glCompressedTexImage2DARB) cx.getProcAddress(QLatin1String("glCompressedTexImage2DARB"));

    }

    QGLDDSCache::const_iterator it = qgl_dds_cache()->constFind(fileName);
    if (it != qgl_dds_cache()->constEnd()) {
      data = it.value();
      glBindTexture(GL_TEXTURE_2D, data.bind);
      return true;
    }

    QFile f(fileName);
    f.open(QIODevice::ReadOnly);

    data.sx = data.sy = data.bind = data.mipmap = data.filesize = data.ddxversion=0;

    char tag[4];
    f.read(&tag[0], 4);
    if (strncmp(tag,"DDS ", 4) != 0) {
        qWarning("QGLContext::bindTexture(): not a DDS image file.");

        return false;
    }

    DDSFormat ddsHeader;
    f.read((char *) &ddsHeader, sizeof(DDSFormat));

    int factor = 4;
    int bufferSize = 0;
    int blockSize = 16;
    GLenum format;

    switch(ddsHeader.ddsPixelFormat.dwFourCC) {
    default:
        qWarning("QGLContext::bindTexture() DDS image format not supported.");
        return false;
    case FOURCC_DXT1:
        format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
        factor = 2;
        blockSize = 8;
        data.ddxversion=1;
        break;
    case FOURCC_DXT3:
        format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
        data.ddxversion=3;
        break;
    case FOURCC_DXT5:
        format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
        data.ddxversion=5;
        break;
    }

    if (!ddsHeader.dwLinearSize) {
        //qWarning("QGLContext::bindTexture() DDS image size is not valid.");
        //return 0;
        ddsHeader.dwLinearSize = ddsHeader.dwHeight * ddsHeader.dwWidth * factor / 4;
    }

    if (ddsHeader.dwMipMapCount == 0) ddsHeader.dwMipMapCount=1;

    data.mipmap=ddsHeader.dwMipMapCount;

    if (ddsHeader.dwMipMapCount > 1)
        bufferSize = ddsHeader.dwLinearSize * factor;
    else
        bufferSize = ddsHeader.dwLinearSize;

    data.filesize=bufferSize+4+sizeof(ddsHeader);
    data.sx = ddsHeader.dwWidth;
    data.sy = ddsHeader.dwHeight;

    GLubyte *pixels = (GLubyte *) malloc(bufferSize*sizeof(GLubyte));
    f.seek(ddsHeader.dwSize + 4);
    f.read((char *) pixels, bufferSize);
    f.close();

    GLuint tx_id;
    glGenTextures(1, &tx_id);

    data.bind=tx_id;

    glBindTexture(GL_TEXTURE_2D, tx_id);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if (((1<<(ddsHeader.dwMipMapCount-1))>=(int)ddsHeader.dwWidth)|| ((1<<(ddsHeader.dwMipMapCount-1))>=(int)ddsHeader.dwWidth))
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    else
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    int size;
    int offset = 0;
    int w = ddsHeader.dwWidth;
    int h = ddsHeader.dwHeight;

    if (!qt_glCompressedTexImage2DARB) {
        qWarning("QGLContext::bindTexture(): The GL implementation does not support texture"
                 "compression extensions.");
        return false;
    }
    // load mip-maps
    for(int i = 0; i < (int) ddsHeader.dwMipMapCount; ++i) {
        if (w == 0) w = 1;
        if (h == 0) h = 1;

        size = ((w+3)/4) * ((h+3)/4) * blockSize;
        //glCompressedTexImage2DARB
        qt_glCompressedTexImage2DARB
                                    (GL_TEXTURE_2D, i, format, w, h, 0,
                                     size, pixels + offset);
        offset += size;

        // half size for each mip-map level
        w = w/2;
        h = h/2;
    }

    free(pixels);

    qgl_dds_cache()->insert(fileName, data);
    return true;
}




#endif // BINDTEXTUREPATCH_H
