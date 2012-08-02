//#include "brfdata.h"
#include "glwidgets.h"
//#include "selector.h"
#include "mainwindow.h"
//#include "ui_mainwindow.h"
//#include "ui_guipanel.h"
//#include "vcgmesh.h"
//#include "askBoneDialog.h"
//#include "askSkelDialog.h"
//#include "askTexturenameDialog.h"
//#include "askModErrorDialog.h"
//#include "askTransformDialog.h"
//#include "askCreaseDialog.h"

#include <QtGui>

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

 QMessageBox msg(QMessageBox::Information,tr("OpenBrf"),
  //QMessageBox::about(this, tr("Open-Brf"),
            tr("<p>&nbsp; &nbsp; <b>ver 0.0.33 beta</b><br>"
               "&nbsp; &nbsp; (%1)<br></p>"
               "<p>&nbsp; &nbsp; by [mtarini] --- Marco Tarini<br></p>"
               "<p>&nbsp; &nbsp; Testing, bug reporting, suggestions by: <br>[amade], &nbsp; [André de Cuyne],"
               "[Barf], [Brutus], [captain lust], "
               "[cdvader], [Chel], [DaBlade], [Dain Ironfoot], "
               "[Darwin], [dreamterror], [eierkopf], [EvolutiveBrain], [Fei Dao], [Freddex], [FrisianDude], [Geroj], "
               "[GetAssista], [giles], [Highelf], [HokieBT], [killkhergit], [Konar], [Llew], "
               "[LordRaglan], [Lumos], "
               "[Merlkir], [mysstick], "
               "[octoburn], [pagan], [Percus], [RATMdude92], [Red River], [Septa Scarabae], [Swyter], "
               "[Triglav], [Tul], [Ursca], [yellowmosquito]</p>"
               "<p>Additional art by: [amade], [Swyter]!</p>"
             ).arg(__DATE__)//,this);
            ,QMessageBox::Ok);

  //msg.layout()->addWidget(&lab);

  msg.setIconPixmap( windowIcon().pixmap(128,128) );
  msg.exec();
}

void MainWindow::optionAutoFixTextureShowInfo(){
   QMessageBox::about(this, tr("Open-Brf"),
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
    mot = QString("I don't know what the material <i>\"%1\"</i> is.").arg(glWidget->lastMatErr.matName);
    longmot = QString("I've scanned in all file \"%1\" and didn't find a "
      "<i>load_mod_resource</i> or <i>load_resource</i> command that pointed me to a brf file "
      "that contained any such material").arg(mabPath+"/Modules/"+modName+"/module.ini");
    cure = QString("<br>- double check material name of the mesh<br>"
                   "<b>or</b><br>"
                   "- find the brf-file with the material, or create one<br>"
                   "- add a line <i>load_<b>mod</b>_resource</i> in module.ini, with a text editor,<br>"
                   "- (note the <i>mod</i> part)!<br>"
                   "- save module.ini<br>"
                   "- come back, and refresh Module [f5]");
    ingame=true;
    break;
  case 2:
    mot = QString("I cannot find the file \"%1\" on disk.").arg(glWidget->lastMatErr.texName);
    longmot = QString("I've looked in folders <br>%1<br> and <br>%2<br> and <br>%3<br> but it wasn't there...<br>"
                      "Maybe it is was tiff texture? (I don't understand them).").arg(
        glWidget->texturePath[0]).arg(glWidget->texturePath[1]).arg(glWidget->texturePath[2]);
    cure = QString("<br>- double check DiffuesA texture name of the material<br>"
                   "- (hint: remember you can navigate with ctrl-left/right)<br>"
                   "<b>or</b>"
                   "<br>- make sure the missing texture file in mod texture folder!"
                   "<br>- put it there if it is missing"
                   );
    ingame=true;
    break;
  case 3:
    mot = QString("I cannot understand the texture format of  file \"%1\".").arg(glWidget->lastMatErr.texName);
    longmot = QString("I'm supposed to understand .dds textures of formats DXT1 (maybe), DXT3, and DXT5.<br>"
                      "But some kinds of DXT1 texture confuse me, and too big textures too."
                      "The problem could be fixed if you enable the autofix DXT option above.");
    cure = QString("Maybe just accept the fact... it should still show the texture in game.");
    ingame=false;
  }
  //if (t!=0)
  QMessageBox::about(this, tr("Open-Brf"),
     QString("<i>I could not display the real texture because:</i><br><b>%1</b><br><br>%2<br><br><b>Cure: </b>%3").arg(mot).arg(longmot).arg(cure));
}




