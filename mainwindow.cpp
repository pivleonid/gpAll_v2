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


#define EXCEL_COLOR_GREEN 5287936
#define EXCEL_COLOR_YELLOW 65535
#define EXCEL_COLOR_RED 255
#define EXCEL_COLOR_WHITE 16777215
#define EXCEL_COLOR_BLUE 12611584


MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(openBoms()));
  connect(ui->pushButton_2,SIGNAL(clicked(bool)),this,SLOT(openSklad()));
  connect(ui->pushButton_3,SIGNAL(clicked(bool)),this,SLOT(generate()));
  connect(ui->pushButton_4,SIGNAL(clicked(bool)),this,SLOT(clear()));
  connect(ui->action,SIGNAL(triggered(bool)), this, SLOT(openAbout()));

   ui->progressBar->setValue(0);

   //ui->pushButton_3->setEnabled(false);

   //ui->tableWidget->setColumnWidth(0, 550);


}
//------------------------------------------------------------------------
void MainWindow::openSklad(){

   fileName_DATA = QFileDialog::getOpenFileName(this, tr("Open Excel File"),"", tr("(*.xlsx *.xls)"));
   ui->label_2->setText(fileName_DATA);

}
//------------------------------------------------------------------------
void MainWindow::openBoms(){
  fileName_DATAs = QFileDialog::getOpenFileNames(this, tr("Open Excel File"),"", tr("(*.xlsx *.xls)"));
  if(fileName_DATAs.count() == 0){
      return;
    }


  foreach (auto var, fileName_DATAs) {
      int rowCont = ui->tableWidget->rowCount();
      ui->tableWidget->insertRow(rowCont);// вставка строки
      QTableWidgetItem* item = new QTableWidgetItem(); // выделяем память под ячейку
      item->setText(var); // вставляем текст
      ui->tableWidget->setItem(rowCont, 0, item); // вставляем ячейку
      QCoreApplication::processEvents();
      QTableWidgetItem* item1 = new QTableWidgetItem();
      item1->setText("1");
      ui->tableWidget->setItem(rowCont, 1, item1);
      QCoreApplication::processEvents();
       QTableWidgetItem* item2 = new QTableWidgetItem();
      item2->setText("20");
      ui->tableWidget->setItem(rowCont, 2, item2);
      rowCont++;
  }
  // Ресайзим колонки по содержимому
      //ui->tableWidget->resizeColumnsToContents();

    ui->pushButton_3->setEnabled(true);

}
//------------------------------------------------------------------------
void MainWindow::clear(){
  ui->label_2->clear();
  for(int i = ui->tableWidget->rowCount(); i >= 0; i--)
      ui->tableWidget->removeRow(i);
  fileName_DATAs.clear();
  fileName_DATA.clear();
  ui->progressBar->setValue(0);

  ui->pushButton_3->setEnabled(false);



}
//------------------------------------------------------------------------
void  MainWindow::generate(){

    ui->progressBar->setValue(0);
    QCoreApplication::processEvents();
    ActiveWord word;
    if(!word.wordConnect())
       mesOut("Word не установлен");

    QString path = QApplication::applicationDirPath() + "/test.docx";
    QAxObject* doc = word.documentOpen(path);
    if(doc == NULL)
        mesOut("Не найден word- шаблон");

//-----Чтение всех BOM файлов
    QMap <QString, QList<TData> > allBom;
    int err = 0;
    err = ReadAllBom(allBom);

    if(err < 0)
        mesOut("Error №" + QString::number(err));

//------Конец Чтения


//Чтение данных со склада
    listStringInt_t sklad;
    sklad = readSklad();

//Поиск элементов из BOM'ов в складских архивах
    QList<QStringList> tableDat = operationSearch(allBom, sklad);


    int BomCount = ui->tableWidget->rowCount();




    int b = tableDat[0].count() - 1;
    for (int i = 0; i < b  ; i++){
        if(i == BomCount){
            word.tableAddColumn(1, i+1, "Сумма эл-тов", "[" + QString::number(i) + "]" , 1);
            continue;
        }
        if(i == BomCount*2 + 1){
            word.tableAddColumn(1, i+1, "Сумма эл-тов + %", "[" + QString::number(i) + "]" , 1);
            continue;
        }
        if(i > BomCount && i < BomCount*2+1){
            word.tableAddColumn(1, i+1,  QString::number(i-BomCount)+ " + %", "[" + QString::number(i) + "]" , 1);
            continue;
        }
        if(i == b - 1){
            word.tableAddColumn(1, i+1, "Закупить", "[" + QString::number(i) + "]" , 1);
            continue;
        }
        if(i == b - 2){
            word.tableAddColumn(1, i+1, "Наличие на складе", "[" + QString::number(i) + "]" , 1);
            continue;
        }
        word.tableAddColumn(1, i+1, QString::number(i+1), "[" + QString::number(i) + "]" , 1);
    }
    word.tableAutoFitWindow(1);

    QStringList listLabel;
   int ret = word.tableGetLabels(1, 2, listLabel);
    ui->progressBar->setValue(80);
    QCoreApplication::processEvents();


    ret = word.tableFill(tableDat,listLabel,1,2) ;

    //


    // добавляю пустую строчку



     //1 колонка - это partNumber последняя - сумма всех
    for(int i = 0; i < BomCount; i++){


       QTableWidgetItem* item = ui->tableWidget->item(i, 0);
       QString bomName = item->text();
        ret = word.tableAddLineWithText(1,1, QString::number(i+1) + ":\n"+ bomName);
       if(ret < 0)
         qDebug()<< "ret = "<<ret<<"; i = " << i;
       if(i == 0)
           word.tableMergeCell(1, bomName, b + 1, 0);

    }

    QDateTime dt = QDateTime::currentDateTime();
    word.tableAddLineWithText(1,1, dt.toString());

    foreach (auto key, allBom.keys()) {
        word.tableMergeCell(1, key, b + 1, 0);
    }
    //
    word.setVisible();
    ui->progressBar->setValue(100);
    QCoreApplication::processEvents();


    //







}
//------------------------------------------------------------------------
listStringInt_t MainWindow::readSklad(){
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

  QAxObject* ex1 = excel.workbookOpen(QVariant( this->ui->label_2->text()));
  if(ex1 == NULL)
    mesOut("Невозможно открыть файл склада:\n" + fileName_DATA
           +"\nПроверте возможность редактирования");
  //
  QStringList names =  excel.sheetsList();
  QStringList varPerem;

  foreach (QString name, names) {
      QAxObject* sheetActiv = excel.workbookSheetActive(name);
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
      excel.workbookClose(ex1);
      ui->progressBar->setValue(70);
      QCoreApplication::processEvents();
      return sklad;

}
//------------------------------------------------------------------------
int  MainWindow::ReadAllBom(QMap <QString, QList<TData> > &allBom){


    QMap <QString, QList<TData> > data;

    bool flagErr = false; // флаг выставляется при "Ошибка обработки BOM данных!"
    //для чтения из таблицы и подсчета процента завершения работы excel'я
    int proc = ui->tableWidget->rowCount();
    QStringList strList; //путь к файлу. Только первая колонка
    for(int i = 0; i < proc ; i++){
        QTableWidgetItem* item = ui->tableWidget->item(i, 0);
        if(NULL != item)
            strList.append(item->text());

    }

    ActiveExcel excel;
    if(!excel.excelConnect()){
        mesOut("Не установлен excel");
        return -1;
    }

    int co = 0; //счетчик строк QTableWidget- сколько файлов загрузили

    proc = 60 / proc; // для подсчета процента

    dataStorage storage;
    int k = 0; //для отладки

//--------- пробег по всем эл-там таблицы
    for (QStringList::iterator str = strList.begin(); str < strList.end(); str++) {

        QAxObject* ex1 = excel.workbookOpen(QVariant(*str));

        if(ex1 == NULL){
            mesOut("Невозможно открыть BOM:\n" + *str
                   +"\nПроверте возможность редактирования");
            return -2;
        }
        QString name = excel.sheetName().toString();
        QAxObject* sheet = excel.workbookSheetActive(name);


        QVariant data;
        //--#-- Поиск строки и колонок из которых считываются данные
        int line = 0;// номер строки, в которой есть  Ref Designator, Part Number и QTY (кол-во)
        int refDez = 0;
        int partNumber = 0;
        int qty = 0;
        int color = 0;
        //В каждом документе в первом столбце есть #, после которой идут элементы
        for (int i = 1; i < 100 ; i++){
            if (excel.sheetCellInsert(sheet, data, i, 1)){
                if (data.toString() == "#"){
                    line = i;
                    //строка найдена, теперь ищу колонки
                    for (int i = 1;i <100 ; i++){
                        if (excel.sheetCellInsert(sheet, data, line, i)){
                            if(data == "Ref Designator")
                                refDez = i;
                            if(data == "Part Number")
                                partNumber = i;
                            if(data == "QTY")
                                qty = i;
                            if((refDez != 0) && (partNumber != 0) && (qty != 0))
                                break;
                        }
                        else { mesOut("Ошибка обработки BOM данных!"); flagErr = true;}
                    }
                    break;
                }
            }
            else { mesOut("Ошибка обработки BOM данных!") ;flagErr = true;}
        }
        if(line * refDez * partNumber * qty == 0){
            mesOut("В файле:\n" + *str + "проверить наличие столбцов:\n #, Ref Designator, Part Number и QTY");
            return -3;
        }
        //--#-- Конец -- Поиск строки и колонок из которых считываются данные
        ///ЧТЕНИЕ ДАННЫХ!!!

        QTableWidgetItem* item = ui->tableWidget->item(co, 1);
        QTableWidgetItem* item1 = ui->tableWidget->item(co, 2);
        int partNumbCount = (item->text()).toInt();
        int percent = (item1->text()).toInt();
        for(int i = line + 1;  ; i++){
            QVariant refN, partNumberN, qtyN ;
            if (excel.sheetCellInsert(sheet, data, i, refDez))
                refN = data;
            else   {mesOut("Ошибка обработки BOM данных!"); flagErr = true;}
            if (excel.sheetCellInsert(sheet, data, i, partNumber)){
                partNumberN = data;
            }
            else   {mesOut("Ошибка обработки BOM данных!"); flagErr = true;}
            if (excel.sheetCellInsert(sheet, data, i, qty)){
                qtyN = data;
            }
            else {mesOut("Ошибка обработки BOM данных!"); flagErr = true;}
            if(excel.sheetCellColorInsert(sheet, data, i, qty)){
                color = data.toInt();
            }
            else {mesOut("Не могу прочитать цвет ячейки!"); flagErr = true;}



            if(refN.toString() == "" && partNumberN.toString() == "" && qtyN.toString() == "" )
                break;
            switch (color) {
            case EXCEL_COLOR_WHITE:
                color = 5;
                break;
            case EXCEL_COLOR_BLUE:
                color = 0;
                break;
            case EXCEL_COLOR_RED:
                color = 150;
                break;
            case EXCEL_COLOR_YELLOW:
                color = 1000;
                break;
            case EXCEL_COLOR_GREEN:
                color = 70;
                break;
            default:
                color = 5;
                mesOut("Цвет partNumber'a: " + partNumberN.toString() + " не стандартный" +
                       "\nПо умолчанию равен безцвеному - х5");
                break;
            }

            storage.insert(refN.toString(), partNumberN.toString(), qtyN.toInt(),partNumbCount , percent,co, color );



        }
        excel.workbookClose(ex1);
         QMap <QString, QList<TData> > data1 = storage.ret();


        ui->progressBar->setValue(proc);
        QCoreApplication::processEvents();
        proc = proc * (co+1);
        co++;
    }
//------конец пробега по всем таблицам
    data = storage.ret();
     //Взять BOM и сравнить все counts !!!
    // Во всех partNumer хранятся одинаковое кол-во эл-тов?
    foreach (auto key, data.keys()) {
        for (int i = 0; i < data[key].count(); i++){
            if (data[key][i].counts.count() != co)
                return -4;
        }

    }
    //Взять BOM и сравнить все проценты !!!
   // Во всех partNumer хранятся одинаковое кол-во эл-тов?
   foreach (auto key, data.keys()) {
       for (int i = 0; i < data[key].count(); i++){
           if (data[key][i].perCent.count() != co)
               return -5;
       }

   }
    //просуммирую все эл-ты, создав еще одну колонку для элементов
    foreach (auto key, data.keys()) {

        for (int i = 0; i < data[key].count(); i++){
            int per = 0;
            for(int j = 0; j < co; j++){
                 per += data[key][i].counts[j];

            }
             data[key][i].counts << per;
        }

    }
    //просуммирую все эл-ты, создав еще одну колонку для процентов
    foreach (auto key, data.keys()) {

        for (int i = 0; i < data[key].count(); i++){
            int per = 0;
            for(int j = 0; j < co; j++){
                 per += data[key][i].perCent[j];

            }
             data[key][i].perCent << per;
        }

    }

    //Здесь заменим ключи на соответствующие имена:
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
   // QMap <QString, QList<TData> > data;
    foreach (auto key1, data.keys()) {
        foreach (auto key2, tem.keys()) {
            if(key1 == key2){
                allBom[tem[key2]]; //копирую ключи
                //Сколько TData? == кол-ву refDez

                for(int i = 0; i < data[key1].count() ; i++){
                    //Сколько элементов?
                    QList<int> count, perCount;
                    for(int j = 0; j < data[key1][i].counts.count() ; j++){
                        count << data[key1][i].counts[j];
                        perCount << data[key1][i].perCent[j];
                    }
                    TData per;
                    per.counts << count;
                    per.perCent << perCount;
                    per.part = data[key1][i].part;
                    per.color = data[key1][i].color;
                     allBom[tem[key2]] << per;
                     int k;
                     k++;

                }

            }
        }
    }


    return 0;
}




//------------------------------------------------------------------------
QList<QStringList> MainWindow::operationSearch(QMap <QString, QList<TData> > &allBom, listStringInt_t &sklad){
    foreach (auto key, allBom.keys()) {

        for (int i = 0; i < allBom[key].count(); i++){
          QString partNumber =  allBom[key][i].part;
          //поиск по складу
          bool flagFound = false;
          for(int j = 0; j < sklad.count(); j++){
             QString s1=  sklad[j].str;
             if(sklad[j].str.contains(partNumber, Qt::CaseInsensitive) == true){
                  flagFound = true;
                   allBom[key][i].perCent << sklad[j].n;
              }

          }
          if( flagFound == false)
              allBom[key][i].perCent << 0;


        }
    }
    //теперь это дело надо перевести в QList<QStringList>  и засандалить в Word таблицу
    QList<QStringList> tableDat;

    foreach (auto key, allBom.keys()) {
        QStringList per;
        QString str;
        str = key; //str.prepend("["); str.append("]");
        per << str;
        //кол-во колонок
        int colomns = allBom[key][0].counts.count();
        for(int i = 1; i < allBom[key][0].counts.count() + allBom[key][0].perCent.count() + 2 ; i++ ) ///+ проценты
            per << "";
        tableDat << per;
        per.clear();

        //i == кол-во TData
        for(int i = 0; i < allBom[key].count(); i++){
            per << allBom[key][i].part;
            for(int j = 0; j < allBom[key][i].counts.count(); j++){

                per << QString::number(allBom[key][i].counts[j]);
            }

            for(int j = 0; j < allBom[key][i].perCent.count(); j++)
                   per << QString::number(allBom[key][i].perCent[j]);
            //Закупить
            int max = allBom[key][i].perCent.count();
            //allBom[key][i].perCent[max -1] == склад
            //allBom[key][i].perCent[max -2] == Rez + %
            int x = abs(allBom[key][i].perCent[max-1] - allBom[key][i].perCent[max - 2]);
            if(x < allBom[key][i].color)
                x = allBom[key][i].color;
            //синий цвет
            if(allBom[key][i].color == 0)
                x = 0;
            per << QString::number(x);
            tableDat << per;
            per.clear();
        }


    }
 return tableDat;
}






//------------------------------------------------------------------------
void MainWindow::openAbout(){
  ActiveWord word;
  if(!word.wordConnect()){
      QMessageBox msgBox;
      msgBox.setText("Word не установлен");
      msgBox.exec();
      return;
    }
 QString path = QApplication::applicationDirPath() + "/Описание.docx";

  //QString path = "D:/projects/gp/PEZ.docx";
  QAxObject* doc1 = word.documentOpen(path);
  if(doc1 == NULL){
      QMessageBox msgBox;
        msgBox.setText("Описание не найдено");
        msgBox.exec();
        word.setVisible();
    return;
    }
   word.setVisible();
}
//------------------------------------------------------------------------
MainWindow::~MainWindow()
{
  delete ui;
}
//------------------------------------------------------------------------
void MainWindow::mesOut(QString mes){
       QMessageBox msgBox;
          msgBox.setText(mes);
          msgBox.exec();

}


