#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qfiledialog.h"
#include "activeexcel.h"
#include "activeword.h"
#include "qmessagebox.h"
#include "qaxobject.h"



MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(openBoms()));
  connect(ui->pushButton_2,SIGNAL(clicked(bool)),this,SLOT(openSklad()));
  connect(ui->pushButton_3,SIGNAL(clicked(bool)),this,SLOT(generate()));
  connect(ui->pushButton_4,SIGNAL(clicked(bool)),this,SLOT(clear()));
   ui->progressBar->setValue(0);



}



void MainWindow::openSklad(){

   fileName_DATA = QFileDialog::getOpenFileName(this, tr("Open Excel File"),"", tr("(*.xlsx *.xls)"));
  if(fileName_DATA.count() == 0){
      mesOut("Не выбралы BOM файлы");
      return;
    }
  ui->label_2->setText(fileName_DATA);

}


void MainWindow::openBoms(){
  fileName_DATAs = QFileDialog::getOpenFileNames(this, tr("Open Excel File"),"", tr("(*.xlsx *.xls)"));
  if(fileName_DATAs.count() == 0){
      mesOut("Не выбралы BOM файлы");
      return;
    }
  foreach (auto var, fileName_DATAs) {
      ui->textEdit->insertPlainText(var + "\n");
    }

}

void MainWindow::clear(){
  ui->label_2->clear();
  ui->textEdit->clear();
  fileName_DATAs.clear();
  fileName_DATA.clear();
}

void MainWindow::generate(){

  //-- Складские данные
  ActiveExcel excel;
  if(!excel.excelConnect()){
      mesOut("Не установлен excel");
      return;
    }
  QAxObject* ex1 = excel.documentOpen(QVariant(fileName_DATA));
  if(ex1 == NULL)
    mesOut("Невозможно открыть файл склада:\n" + fileName_DATA
           +"\nПроверте возможность редактирования");
  //
  QStringList names =  excel.sheetsList();
  QStringList varPerem;
  QVariant data;

  foreach (QString name, names) {
      QAxObject* sheetActiv = excel.documentSheetActive(name);
      int colType = 0;
      int colCol = 0;
      int rol = 0;
      bool flagEnd = false;
      QVariant data, data1;
      //Поиск слов "Тип" и  "Кол-во" с 1 по 3 строчку
      for(int i = 2; i < 4  ; i++){
          if (flagEnd == true)
            break;
          //бегу по строчке, пока не встречу подряд 3 пустых ячейки
          for(int j = 1; ; j++){
              if (excel.sheetCellInsert(sheetActiv, data, i, j)){
                  if( data.toString() == "Тип" )
                    colType = j;
                  if( data.toString() == "Кол-во" )
                    colCol = j;
                  if(colType != 0 && colCol != 0 ){
                      rol = i;
                      flagEnd = true;
                      break;
                    }
                  excel.sheetCellInsert(sheetActiv, data1, i, j + 1);
                  if( data.toString() == data1.toString() ){
                      flagEnd = true;
                      break;
                    }


                }
              else{
                  mesOut("Ошибка обработки данных!");
                  return;
                }
            }

        }
      //если не найдена или строка или столбец или строчка- пропуск листа
      if(colType == 0 || colCol == 0 || rol == 0)
        continue;
      for(int i = 3;  ; i++){
          if (excel.sheetCellInsert(sheetActiv, data, i, 4))
            varPerem << data.toString();
          else{
              mesOut("Ошибка обработки данных!");
              return;
            }
          if (excel.sheetCellInsert(sheetActiv, data, i, 6))
            varPerem << data.toString();
          else{
              mesOut("Ошибка обработки данных!");
              return;
            }
          int g = varPerem.count();
          if(varPerem[g-1] == "" && varPerem[g-2] == "" )
            break;

        }

    }

  struct list_t{
    QString str;
    int n;
  };
  QList<list_t> sklad;

  for (QStringList::iterator it = varPerem.begin(); it < varPerem.end() - 1; it += 2) {

      QString a = *it;
      int num  = (*(it + 1)).toInt();
      list_t list;
      list.str = a;
      list.n = num;
      sklad.append(list);

    }
  excel.documentClose(ex1);
   //-- BOM данные

  ActiveExcel excel1;
  if(!excel1.excelConnect()){
      mesOut("Не установлен excel1");
      return;
    }
  //все BOM'ы
  QStringList var;
  foreach (auto str, fileName_DATAs) {

      QAxObject* ex1 = excel1.documentOpen(QVariant(str));
      if(ex1 == NULL)
        mesOut("Невозможно открыть BOM:\n" + str
               +"\nПроверте возможность редактирования");
      //
      QString name = excel1.sheetName().toString();
      QAxObject* sheet = excel1.documentSheetActive(name);



      QStringList varPerem;
      QVariant data;
      // 20 секунд на чтение

      ui->progressBar->setValue(0);
      ///ЧТЕНИЕ ДАННЫХ!!!
      for(int i = 2;  ; i++){
          if (excel1.sheetCellInsert(sheet, data, i, 2))
            varPerem << data.toString();
          else{
              mesOut("Ошибка обработки данных!");
              return;
            }
          if (excel1.sheetCellInsert(sheet, data, i, 3))
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
      excel1.documentClose(ex1);
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
             // Вид mapVarLisrCont - [C] -> [partNumber][кол-во элементов]
                                   //    -> [partNumber][кол-во элементов]
         QMap<QString, QList<QStringList> > mapVarLisrCont;
         //копирую ключи
         foreach (QString key, mapVarLisr.keys ()) {
             mapVarLisrCont[key];
           }
         bool flag = true;
    //---Сортировака QMAP
         //пробегаюсь по всем ключам
         foreach (QString key, mapVarLisr.keys ()) {
             //пробегаюсь по значению ключа
             for( QStringList::iterator it = mapVarLisr[key].begin(); it < mapVarLisr[key].end(); it++){
                 //Первый влет- по ключу запиывается первый элемент
                if(it == mapVarLisr[key].begin()){
                   QStringList peremen;
                   peremen << *it << "1";
                   mapVarLisrCont[key].append (peremen);
                   continue;
                 }
                 QString a = (*it); // для отладки
                 flag = true;
                 //Если по такому ключу и с таким partnumber содержится элемент- кол-во + 1
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
                 //если элемент уже был найден- смотрю следующее значение
                 //если не был найден -  вношу этот элемент
                 if(flag == true){
                 QStringList peremen;
                 peremen << *it << "1";
                 mapVarLisrCont[key].append (peremen);
                 continue;
                   }
               }
           }
          mapVarLisr.clear();// очищая предыдущий контейнер
             int i = 0;
             i++;


}



void MainWindow::operationSklad(){
  ActiveExcel excel;
  if(!excel.excelConnect()){
      mesOut("Не установлен excel");
      return;
    }
  QAxObject* ex1 = excel.documentOpen(QVariant(fileName_DATA));
  if(ex1 == NULL)
    mesOut("Невозможно открыть файл склада:\n" + fileName_DATA
           +"\nПроверте возможность редактирования");
  //
  QStringList names =  excel.sheetsList();
  QStringList varPerem;
  QVariant data;

  foreach (QString name, names) {
      QAxObject* sheetActiv = excel.documentSheetActive(name);
      int colType = 0;
      int colCol = 0;
      int rol = 0;
      bool flagEnd = false;
      QVariant data, data1;
      //Поиск слов "Тип" и  "Кол-во" с 1 по 3 строчку
      for(int i = 2; i < 4  ; i++){
          if (flagEnd == true)
            break;
          //бегу по строчке, пока не встречу подряд 3 пустых ячейки
          for(int j = 1; ; j++){
              if (excel.sheetCellInsert(sheetActiv, data, i, j)){
                  if( data.toString() == "Тип" )
                    colType = j;
                  if( data.toString() == "Кол-во" )
                    colCol = j;
                  if(colType != 0 && colCol != 0 ){
                      rol = i;
                      flagEnd = true;
                      break;
                    }
                  excel.sheetCellInsert(sheetActiv, data1, i, j + 1);
                  if( data.toString() == data1.toString() ){
                      flagEnd = true;
                      break;
                    }


                }
              else{
                  mesOut("Ошибка обработки данных!");
                  return;
                }
            }

        }
      //если не найдена или строка или столбец или строчка- пропуск листа
      if(colType == 0 || colCol == 0 || rol == 0)
        continue;
      for(int i = 3;  ; i++){
          if (excel.sheetCellInsert(sheetActiv, data, i, 4))
            varPerem << data.toString();
          else{
              mesOut("Ошибка обработки данных!");
              return;
            }
          if (excel.sheetCellInsert(sheetActiv, data, i, 6))
            varPerem << data.toString();
          else{
              mesOut("Ошибка обработки данных!");
              return;
            }
          int g = varPerem.count();
          if(varPerem[g-1] == "" && varPerem[g-2] == "" )
            break;

        }

    }

  struct list_t{
    QString str;
    int n;
  };
  QList<list_t> sklad;

  for (QStringList::iterator it = varPerem.begin(); it < varPerem.end() - 1; it += 2) {

      QString a = *it;
      int num  = (*(it + 1)).toInt();
      list_t list;
      list.str = a;
      list.n = num;
      sklad.append(list);

    }
      excel.documentClose(ex1);

}

void MainWindow::operationSumRefDez(void){

  ActiveExcel excel;
  if(!excel.excelConnect()){
      mesOut("Не установлен excel");
      return;
    }
  //все BOM'ы
  QStringList var;
  foreach (auto str, fileName_DATAs) {

      QAxObject* ex1 = excel.documentOpen(QVariant(str));
      if(ex1 == NULL)
        mesOut("Невозможно открыть BOM:\n" + str
               +"\nПроверте возможность редактирования");
      //
      QString name = excel.sheetName().toString();
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
             // Вид mapVarLisrCont - [C] -> [partNumber][кол-во элементов]
                                   //    -> [partNumber][кол-во элементов]
         QMap<QString, QList<QStringList> > mapVarLisrCont;
         //копирую ключи
         foreach (QString key, mapVarLisr.keys ()) {
             mapVarLisrCont[key];
           }
         bool flag = true;
    //---Сортировака QMAP
         //пробегаюсь по всем ключам
         foreach (QString key, mapVarLisr.keys ()) {
             //пробегаюсь по значению ключа
             for( QStringList::iterator it = mapVarLisr[key].begin(); it < mapVarLisr[key].end(); it++){
                 //Первый влет- по ключу запиывается первый элемент
                if(it == mapVarLisr[key].begin()){
                   QStringList peremen;
                   peremen << *it << "1";
                   mapVarLisrCont[key].append (peremen);
                   continue;
                 }
                 QString a = (*it); // для отладки
                 flag = true;
                 //Если по такому ключу и с таким partnumber содержится элемент- кол-во + 1
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
                 //если элемент уже был найден- смотрю следующее значение
                 //если не был найден -  вношу этот элемент
                 if(flag == true){
                 QStringList peremen;
                 peremen << *it << "1";
                 mapVarLisrCont[key].append (peremen);
                 continue;
                   }
               }
           }
          mapVarLisr.clear();// очищая предыдущий контейнер
             int i = 0;
             i++;
}

















MainWindow::~MainWindow()
{
  delete ui;
}


void MainWindow::mesOut(QString mes){
       QMessageBox msgBox;
          msgBox.setText(mes);
          msgBox.exec();

}
