#include "glwidgets.h"
#include "mainwindow.h"

#include <QtGui>

QString IniData::tokenFullName(int k){

  switch (k){
  case MESH: return QString(MainWindow::tr("Mesh"));
  case TEXTURE: return MainWindow::tr("Texture");
  case SHADER: return MainWindow::tr("Shader");
  case MATERIAL: return MainWindow::tr("Material");
  case SKELETON: return MainWindow::tr("Skeleton");
  case ANIMATION: return MainWindow::tr("Animation");
  case BODY: return MainWindow::tr("Collision");
  default:  return MainWindow::tr("???");
  }
}

QString IniData::tokenPlurName(int k){
  switch (k){
  case MESH: return MainWindow::tr("Meshes");
  case TEXTURE: return MainWindow::tr("Textures");
  case SHADER: return MainWindow::tr("Shaders");
  case MATERIAL: return MainWindow::tr("Materials");
  case SKELETON: return MainWindow::tr("Skeletons");
  case ANIMATION: return MainWindow::tr("Animations");
  case BODY: return MainWindow::tr("Collisions");
  default:  return MainWindow::tr("???");
  }
}


void MainWindow::about()
{
  /*
  QDialog msg(this);
  QBoxLayout l(QBoxLayout::LeftToRight);
  msg.setWindowTitle("OpenBrf");
  msg.setLayout(&l);
  QPixmap p = QIcon(":/openBrf.ico").pixmap(256,256);
  QLabel logo;
  logo.setPixmap( p );
  msg.layout()->addWidget(&logo);
  QLabel lab(*/

 QMessageBox msg(QMessageBox::Information,"OpenBrf",
  //QMessageBox::about(this, ("Open-Brf"),
              QString(tr(
               "<p>"
               "&nbsp; &nbsp; <b>OpenBrf</b><br>"
               "&nbsp; &nbsp; by <b>%2</b></p><p>"
               "&nbsp; &nbsp; <b>ver %6</b><br>"
               "&nbsp; &nbsp; (%1)<br></p>"
               "<p><i>Testing, bug reporting, suggestions by:</i> %3</p>"
               "<p><i>Additional art by:</i> %4</p>"
               "<p><i>Translations by:</i> %5</p>")
             ).arg(__DATE__)
              .arg("[mtarini] --- Marco Tarini")
              .arg(" <br>[amade], [Andrde Cuyne], [Barf], [Brutus], [Caba`drin], "
                   "[captain lust], [cdvader], [Chel], [DaBlade], [Dain Ironfoot], "
                   "[Darwin], [dreamterror], [dunde], [ealabor], [eierkopf], "
                   "[EvolutiveBrain], "
                   "[fedeita], [Fafhrd], [Fei Dao], [foxyman], [Freddex], [FrisianDude], "
                   "[Geroj], [GetAssista], [giles], [Gothic Knight], [Hatonastick], "
                   "[havarez], "
                   "[Highelf], [Highlander], [HokieBT], [Jai], [killkhergit], "
                   "[Konar], [Llew], "
                   "[LordRaglan], [Lumos], [MadVader], [Mandible], [Mekelan], "
                   "[Merlkir], [mr.master], "
                   "[mysstick], "
                   "[octoburn], [pagan], [Percus], [qlithe], [RATMdude92], [Red River], "
                   "[Septa Scarabae], [Silesian], [Shik], [Silver Wolf], [Spak], [Swyter], "
                   "[Triglav], [Tul], [Ursca], [Vlejundo], [Vornne], [WilliamBerne], "
                   "[yellowmosquito], [Yoshiboy], [xenoargh]")
              .arg("<br>[amade], [Swyter]!")
              .arg(QString("<br> [foxyman] <i>(%1)</i><br>"
                           " [Swyter] <i>(Español)</i><br>"
                           " [Vlejundo] <i>(Deutsche)</i><").arg(tr("additional code and Chinese")))
              .arg("0.0.56")
            ,QMessageBox::Ok, this);
    msg.setLocale(QLocale::system());
  //msg.layout()->addWidget(&lab);

  msg.setIconPixmap( windowIcon().pixmap(128,128) );
  msg.exec();
}


void MainWindow::optionAutoFixTextureShowInfo(){
   QMessageBox::about(this, "OpenBRF",
            tr("<b>What is this autofix DXT texture option?</b><br>"
               "<p>"
               "Many DDS texture creation programs/plugins around will output DXT1 textures "
               "with a minor error in the header.</p><p>"
               "This error confuses me (OpenBRF) but not Mount and Blade (or many other programs).</p><p>"
               "(When I cannot read a texture for this or any other problem, I display a chekerboard pattern instead).</p><p>"
               "If you want, I can silently fix this error every time I encounter it "
               "(I'll shamelessly write on the texture dss files on disk).</p>"
               ));

}

void MainWindow::aboutCheckboard(){
  int t = glWidget->lastMatErr.type;
  QString mot;
  QString longmot;
  QString cure;
  bool ingame;
  switch (t){
  case 1:
    mot = QString(tr("I don't know what the material <i>\"%1\"</i> is.")).arg(glWidget->lastMatErr.matName);
    longmot = QString(tr("I've scanned in all file \"%1\" and didn't find a "
      "<i>load_mod_resource</i> or <i>load_resource</i> command that pointed me to a brf file "
      "that contained any such material")).arg(mabPath+"/Modules/"+modName+"/module.ini");
    cure = QString(tr("<br>- double check material name of the mesh<br>"
                   "<b>or</b><br>"
                   "- find the brf-file with the material, or create one<br>"
                   "- add a line <i>load_<b>mod</b>_resource</i> in module.ini, with a text editor,<br>"
                   "- (note the <i>mod</i> part)!<br>"
                   "- save module.ini<br>"
                   "- come back, and refresh Module [f5]"));
    ingame=true;
    break;
  case 2:
    mot = QString(tr("I cannot find the file \"%1\" on disk.")).arg(glWidget->lastMatErr.texName);
    longmot = QString(tr("I've looked in folders <br>%1<br> and <br>%2<br> and <br>%3<br> but it wasn't there...<br>"
                      "Maybe it is was tiff texture? (I don't understand them).")).arg(
        glWidget->texturePath[0]).arg(glWidget->texturePath[1]).arg(glWidget->texturePath[2]);
    cure = QString(tr("<br>- double check DiffuesA texture name of the material<br>"
                   "- (hint: remember you can navigate with ctrl-left/right)<br>"
                   "<b>or</b>"
                   "<br>- make sure the missing texture file in mod texture folder!"
                   "<br>- put it there if it is missing")
                   );
    ingame=true;
    break;
  case 3:
    mot = QString(tr("I cannot understand the texture format of  file \"%1\".")).arg(glWidget->lastMatErr.texName);
    longmot = QString(tr("I'm supposed to understand .dds textures of formats DXT1 (maybe), DXT3, and DXT5.<br>"
                      "But some kinds of DXT1 texture confuse me, and too big textures too."
                      ));
    cure = QString(tr("Maybe just accept the fact... it should still show the texture in game."));
    ingame=false;
  }
  //if (t!=0)
  QMessageBox::about(this, tr("Open-Brf"),
     QString(tr("<i>I could not display the real texture because:</i><br><b>%1</b><br><br>%2<br><br><b>Cure: </b>%3")).arg(mot).arg(longmot).arg(cure));
}




