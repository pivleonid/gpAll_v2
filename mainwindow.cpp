#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qfiledialog.h"
#include "activeexcel.h"
#include "activeword.h"
#include "qmessagebox.h"
#include "qaxobject.h"
#include "qtextcodec.h"
#include "qdatetime.h"
#include "qinputdialog.h"


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

  bool ok;
  int num = QInputDialog::getInt(0, "Внимание!", "Введите количество образцов", 0 , 1, 100, 1, &ok);
  if(!ok)
      return;
  foreach (auto var, fileName_DATAs) {
      ui->textEdit->insertPlainText(var + "    -    "+ QString::number(num) +"\n");
    }

}

void MainWindow::clear(){
  ui->label_2->clear();
  ui->textEdit->clear();
  fileName_DATAs.clear();
  fileName_DATA.clear();
}

void MainWindow::generate(){

    ui->progressBar->setValue(0);
    QCoreApplication::processEvents();
    ActiveWord word;
    if(!word.wordConnect()){
        QMessageBox msgBox;
        msgBox.setText("Word не установлен");
        msgBox.exec();
        return;
    }
    QString path = QApplication::applicationDirPath() + "/test.docx";
    QAxObject* doc = word.documentOpen(path);
    if(doc == NULL){
        QMessageBox msgBox;
        msgBox.setText("Не найден word- шаблон");
        msgBox.exec();
        return;
    }


    QMap<QString, QList<QStringList> > dataBom, dataBomOut;
    operationSumRefDez(dataBom);
    listStringInt_t dataSklad;
    dataSklad = operationSklad();
    QList<QStringList> datInWord;
    datInWord = operationSearch(dataBom, dataSklad);

    QString dt = QDateTime::currentDateTime().toString();
    word.findReplaseLabel("[Dat]" , dt, false);


    //заполнение таблицы
    QStringList listLabel = word.tableGetLabels(1, 3);
    ui->progressBar->setValue(80);
    QCoreApplication::processEvents();
    if (word.tableFill(datInWord,listLabel,1,3) < 0){
        mesOut("Ошибка заполнения таблицы!");
        word.setVisible();
        return;
    }
    ui->progressBar->setValue(100);
    QCoreApplication::processEvents();
    word.setVisible();
    int u;
    u++;


}



listStringInt_t MainWindow::operationSklad(){
    ui->progressBar->setValue(60);
    QCoreApplication::processEvents();
  listStringInt_t errMes;
  list_t list{"", 0};
  errMes.append(list);
  ActiveExcel excel;
  if(!excel.excelConnect()){
      mesOut("Не установлен excel");
      return errMes;
    }
  QAxObject* ex1 = excel.documentOpen(QVariant(fileName_DATA));
  if(ex1 == NULL)
    mesOut("Невозможно открыть файл склада:\n" + fileName_DATA
           +"\nПроверте возможность редактирования");
  //
  QStringList names =  excel.sheetsList();
  QStringList varPerem;

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
                  mesOut("Ошибка обработки данных со склада!");
                  return errMes;
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
              mesOut("Ошибка обработки данных со склада!");
              return errMes;
            }
          if (excel.sheetCellInsert(sheetActiv, data, i, 6))
            varPerem << data.toString();
          else{
              mesOut("Ошибка обработки данных со склада!");
              return errMes;
            }
          int g = varPerem.count();
          if(varPerem[g-1] == "" && varPerem[g-2] == "" )
            break;

        }

    }
  listStringInt_t sklad;
  for (QStringList::iterator it = varPerem.begin(); it < varPerem.end() - 1; it += 2) {

      QString a = *it;
      int num  = (*(it + 1)).toInt();
      list_t list;
      list.str = a;
      list.n = num;
      sklad.append(list);

    }
      excel.documentClose(ex1);
      ui->progressBar->setValue(70);
      QCoreApplication::processEvents();
      return sklad;

}

void  MainWindow::operationSumRefDez(QMap<QString, QList<QStringList>>& mapVarLisrCont){

   QString str = ui->textEdit->toPlainText();
    QStringList strList1 = str.split('\n');
    QStringList strList;
    foreach (auto var, strList1) {
         strList.append(var.split("    -    "));
    }
    //последний иногда почему- то пустой- очищаю
    if( strList.at(strList.count() - 1) == ""){
        qDebug() << "WFT?";
        strList.removeLast();
    }


    int proc = 50/(strList.count() - 1) ;


  ActiveExcel excel;
  if(!excel.excelConnect()){
      mesOut("Не установлен excel");
     // return errMes;
    }
  //все BOM'ы
  QStringList var;
  for (QStringList::iterator str = strList.begin(); str < strList.end(); str++) {

      QAxObject* ex1 = excel.documentOpen(QVariant(*str));
      qDebug() << *str;
      if(ex1 == NULL)
        mesOut("Невозможно открыть BOM:\n" + *str
               +"\nПроверте возможность редактирования");
      //
      QString name = excel.sheetName().toString();
      QAxObject* sheet = excel.documentSheetActive(name);



      QStringList varPerem, varPeremNew;
      QVariant data;
      // 20 секунд на чтение

      ///ЧТЕНИЕ ДАННЫХ!!!
      for(int i = 2;  ; i++){
          if (excel.sheetCellInsert(sheet, data, i, 2))
            varPerem << data.toString();
          else{
              mesOut("Ошибка обработки BOM данных!");
            //  return errMes;
            }
          if (excel.sheetCellInsert(sheet, data, i, 3)){
            varPerem << data.toString();
          }
          else{
              mesOut("Ошибка обработки BOM данных!");
            //  return errMes;
            }
          int g = varPerem.count();
                if(varPerem[g-1] == "" && varPerem[g-2] == "" )
                 break;

        }
str++;
      qDebug() << *str;
      int num = (*(str)).toInt(); //хранится кол-во элементов


      for(int i = 0; i < num; i++)
          varPeremNew += varPerem;
      var << varPeremNew;
      excel.documentClose(ex1);

      ui->progressBar->setValue(proc);
      QCoreApplication::processEvents();
      proc += proc;
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
        // QMap<QString, QList<QStringList> > mapVarLisrCont;
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

}



QList<QStringList> MainWindow::operationSearch(QMap<QString, QList<QStringList> > &dataSkladAndSum, listStringInt_t &dataSklad){
  QMap<QString, QList<QStringList> > dataBomOut;
  //Заполняю ключами
  foreach (QString key, dataSkladAndSum.keys ()) {
      dataBomOut[key];
    }

  bool flag = false; //найден ли элемент?
  // поиск данных в складе
  //пробегаюсь по всем ключам
  foreach (QString key, dataSkladAndSum.keys ()) {
      //пробегаюсь по значению ключа
      for( QList<QStringList>::iterator it = dataSkladAndSum[key].begin(); it < dataSkladAndSum[key].end(); it++){

          QString dBOM = (*it).at(0);
          //бегу по складским данным
          //var = {QString, int}
          foreach (auto var, dataSklad) {
              QString str = var.str;
              if(str.contains(dBOM, Qt::CaseInsensitive) == true){ //Если строка содержит подстроку
                  //на складе  элемент найден
                  QStringList peremen;
                  peremen << *it << QString::number(var.n);
                  dataBomOut[key].append (peremen);
                  flag = true;
                  break;
                }
            }
          if(flag == true){
              flag = false;
              continue;// следующий элемент
          }
          //на складе  элемент не найден
          QStringList peremen;
          peremen << *it << QString::number(1);
          dataBomOut[key].append (peremen);

         }
    }

  //формирование выходного файла
  QList<QStringList> inWord;
  // по ключам
  foreach (QString key, dataBomOut.keys ()) {
      // по значению ключей
      QStringList keys;
      keys << "" << key << "" << "";
      inWord << keys;
      keys.clear();
      for( QList<QStringList>::iterator it = dataBomOut[key].begin(); it < dataBomOut[key].end(); it++){

          keys << (*it).at(0) << (*it).at(1) << (*it).at(2) << QString::number((*it).at(1).toInt() - (*it).at(2).toInt());
          inWord << keys;
          keys.clear();
        }
    }


  dataSkladAndSum.clear();
  dataSkladAndSum = dataBomOut;


  //замена меток. [C] - конденсаторы.
    QString  str;
    QFile file("Названия групп.txt");
    QTextCodec *codec = QTextCodec::codecForName("CP1251");
    if(file.open(QIODevice::ReadOnly |QIODevice::Text)){
        while (!file.atEnd()){
            QByteArray line = file.readLine();
            QTextCodec *codec = QTextCodec::codecForName("CP1251");
            str += codec->toUnicode(line);
        }
    }
    else qDebug()<< "don't open file";
    QStringList splitStr = str.split("\n");
    QMap<QString, QString> tem;
       foreach (auto st, splitStr) {
           QStringList split = st.split("-");
           tem[split[0]] = split[1];
       }
       QList<QString> keys = tem.keys();
    for (QList<QStringList>::iterator var = inWord.begin(); var < inWord.end(); var++) {
        if((*var).at(0) == (*var).at(2)){
            QString b = (*(var )).at(1);
            int index = keys.indexOf(b);
            if( index < 0){
                ( *(var)).clear();
               (*var) << "" << "Прочие" << "" << "";
                continue;
            }
            QString str = keys[index];
            QString a = tem[str];
             ( *(var)).clear();
            (*var) << "" << a << "" << "";
            int c;
            c++;
        }
    }
    return inWord;

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
