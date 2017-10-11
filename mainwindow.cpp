#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qfiledialog.h"
#include "activeexcel.h"
#include "activeword.h"
#include "qmessagebox.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(openBoms()));
   ui->progressBar->setValue(0);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::openBoms(){

  QStringList fileName_DATA = QFileDialog::getOpenFileNames(this, tr("Open Excel File"),"", tr("(*.xlsx *.xls)"));
  if(fileName_DATA.count() == 0){
      mesOut("Не выбралы BOM файлы");
      return;
    }
  ActiveExcel excel;
  if(!excel.excelConnect()){
      mesOut("Не установлен excel");
      return;
    }
  //все BOM'ы
  QStringList var;
  foreach (auto str, fileName_DATA) {

      QAxObject* ex1 = excel.documentOpen(QVariant(str));
      if(ex1 == NULL)
        mesOut("Невозможно открыть BOM:\n" + str
               +"\nПроверте возможность редактирования");
      //
      QVariant name = excel.sheetName();
      QAxObject* sheet = excel.documentSheetActive(name);



      QStringList varPerem;
      QVariant data;
      // 20 секунд на чтение

      ui->progressBar->setValue(0);
      ///ЧТЕНИЕ ДАННЫХ!!!
      for(int i = 2;  ; i++){
          if (excel.sheetCellInsert(sheet, data, i, 2))
            varPerem << data.toString();
          else{
              mesOut("Ошибка обработки данных!");
              return;
            }
          if (excel.sheetCellInsert(sheet, data, i, 3))
            varPerem << data.toString();
          else{
              mesOut("Ошибка обработки данных!");
              return;
            }
          int g = varPerem.count();
                if(varPerem[g-1] == "" && varPerem[g-2] == "" )
                 break;

        }
      var << varPerem;
      excel.documentClose(ex1);
    }
//--начало формирования Всех префиксов
  QStringList Prefix, allPrefix;
      for (QStringList::iterator it = var.begin(); it < var.end(); it++) {
          Prefix << *it;
          it++;
      }
      for (QStringList::iterator it = Prefix.begin(); it < Prefix.end(); it++) {
          (*it).remove(QRegExp("[^A-Za-zА-Яа-я]"));
      }
      //Удаляю дубликаты
      for (QStringList::iterator it = Prefix.begin(); it < Prefix.end() -1;it++) {
          if( allPrefix.indexOf(*it) < 0 && (*it) != "")
            allPrefix << *it;
        }
//--Конец формирования Всех префиксов
     //Формирование QMAP
      QMap<QString, QStringList > mapVarLisr;
             foreach (auto it, allPrefix) {
                 for (QStringList::iterator it1 = var.begin();it1 < var.end(); it1++) {
                     QString name = *it1;
                     name = name.remove(QRegExp("[^A-Za-zА-Яа-я]"));
                     if( it == name)
                         mapVarLisr[it].append (*(it1 + 1));
                     it1++;// т.к. след. значение через 1
                 }
             }
     //Сортировака QMAP--заполнение ключами
         QMap<QString, QList<QStringList> > mapVarLisrCont;
         foreach (QString key, mapVarLisr.keys ()) {
             mapVarLisrCont[key];
           }
         bool flag = true;
    //Сортировака QMAP
         foreach (QString key, mapVarLisr.keys ()) {
             for( QStringList::iterator it = mapVarLisr[key].begin(); it < mapVarLisr[key].end(); it++){
                 //Первый влет
                if(it == mapVarLisr[key].begin()){
                   QStringList peremen;
                   peremen << *it << "1";
                   mapVarLisrCont[key].append (peremen);
                   continue;
                 }
                 QString a = (*it);
                 flag = true;
                 for (QList<QStringList>::iterator var = mapVarLisrCont[key].begin(); var < mapVarLisrCont[key].end(); var++) {
                     QString z =   (*var).at(0);
                     if( (*var).at(0) == a ){
                         int number = (*var).at(1).toInt();
                         ++number;
                         (*var)[1] = QString::number(number);
                         flag = false;
                         break;
                       }

                   }
                 if(flag == true){
                 QStringList peremen;
                 peremen << *it << "1";
                 mapVarLisrCont[key].append (peremen);
                 continue;
                   }
               }
           }

             int i = 0;
             i++;
}


void MainWindow::mesOut(QString mes){
       QMessageBox msgBox;
          msgBox.setText(mes);
          msgBox.exec();

}
