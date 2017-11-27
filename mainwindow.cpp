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
  connect(ui->action,SIGNAL(triggered(bool)), this, SLOT(openAbout()));

   ui->progressBar->setValue(0);
    ui->tableWidget->setColumnWidth(0, 550);

}
//------------------------------------------------------------------------
void MainWindow::openSklad(){

   fileName_DATA = QFileDialog::getOpenFileName(this, tr("Open Excel File"),"", tr("(*.xlsx *.xls)"));
  if(fileName_DATA.count() == 0){
      mesOut("Не выбралы BOM файлы");
      return;
    }
  ui->label_2->setText(fileName_DATA);

}
//------------------------------------------------------------------------
void MainWindow::openBoms(){
  fileName_DATAs = QFileDialog::getOpenFileNames(this, tr("Open Excel File"),"", tr("(*.xlsx *.xls)"));
  if(fileName_DATAs.count() == 0){
      mesOut("Не выбралы BOM файлы");
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
//      ui->tableWidget->resizeColumnsToContents();



}
//------------------------------------------------------------------------
void MainWindow::clear(){
  ui->label_2->clear();
  for(int i = ui->tableWidget->rowCount(); i >= 0; i--)
      ui->tableWidget->removeRow(i);
  fileName_DATAs.clear();
  fileName_DATA.clear();
  ui->progressBar->setValue(0);
}
//------------------------------------------------------------------------
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
    int err = 0;
    err = operationSumRefDez(dataBom);
    if(err < 0){
        if(err == -10) // не критичная ошибка
           goto m1;
        return;

    }
    m1:
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
//------------------------------------------------------------------------
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
  QAxObject* ex1 = excel.workbookOpen(QVariant(fileName_DATA));
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
int  MainWindow::operationSumRefDez(QMap<QString, QList<QStringList>>& mapVarLisrCont){

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

    int co = 0; //счетчик строк QTableWidget
    QList<QStringList> peremTabl;
    proc = 60 / proc; // для подсчета процента
        for (QStringList::iterator str = strList.begin(); str < strList.end(); str++) {

            QAxObject* ex1 = excel.workbookOpen(QVariant(*str));

            if(ex1 == NULL){
                mesOut("Невозможно открыть BOM:\n" + *str
                       +"\nПроверте возможность редактирования");
                return -2;
            }
            QString name = excel.sheetName().toString();
            QAxObject* sheet = excel.workbookSheetActive(name);

            QStringList varPerem;
            QVariant data;

            int line = 0;// номер строки, в которой есть  Ref Designator, Part Number и QTY (кол-во)
            int refDez = 0;
            int partNumber = 0;
            int qty = 0;
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
               return -1;
            }
            ///ЧТЕНИЕ ДАННЫХ!!!
            for(int i = line + 1;  ; i++){
                if (excel.sheetCellInsert(sheet, data, i, refDez))
                    varPerem << data.toString();
                else   {mesOut("Ошибка обработки BOM данных!"); flagErr = true;}
                if (excel.sheetCellInsert(sheet, data, i, partNumber)){
                    varPerem << data.toString();
                }
                else   {mesOut("Ошибка обработки BOM данных!"); flagErr = true;}
                if (excel.sheetCellInsert(sheet, data, i, qty)){
                    varPerem << data.toString();
                }
                else {mesOut("Ошибка обработки BOM данных!"); flagErr = true;}

                int g = varPerem.count();
                if(varPerem[g-1] == "" && varPerem[g-2] == "" )
                    break;
            }
            excel.workbookClose(ex1);

            QTableWidgetItem* item = ui->tableWidget->item(co, 1);
            QStringList perrem;
            perrem << item->text();
            peremTabl << varPerem << perrem;
            ui->progressBar->setValue(proc);
            QCoreApplication::processEvents();
            proc += proc;
            co++;
        }

     //В peremTable лежат : [0],[2] ... набор всех данных из BOM.
    //                      [1],[3] кол-во эл-тов и цвет, переведенный в количество

    //--начало формирования Всех префиксов
        QStringList Prefix, allPrefix;
        for(int i = 0; i < peremTabl.count(); i = i + 2){
            for (QStringList::iterator it = peremTabl[i].begin(); it < peremTabl[i].end(); it++) {
                Prefix << *it;
                it++;
            }
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
    QMap<QString, QStringList > mapVarLisrBegin;
    QMap<QString, QList<QStringList> > mapVarLisrEnd;
    foreach (QString key, allPrefix) {
        mapVarLisrEnd[key];
        mapVarLisrBegin[key];
    }

    for(int i = 0; i < peremTabl.count(); i = i + 2){
        mapVarLisrBegin.clear();
        foreach (auto it, allPrefix) {

            for (QStringList::iterator it1 = peremTabl[i].begin();it1 < peremTabl[i].end(); it1++) {
                QString name = *it1;
                name = name.remove(QRegExp("[^A-Za-zА-Яа-я]"));
                if( it == name){
                    mapVarLisrBegin[it].append (*(it1 + 1));
                }
                it1++;// т.к. след. значение через 1
            }
        }
        //Добавить к контейнеру хотелки Бориса

        foreach (QString key, mapVarLisrBegin.keys()) {
            //пробегаюсь по значению ключа
            QStringList ver;
            for( QStringList::iterator it = mapVarLisrBegin[key].begin(); it < mapVarLisrBegin[key].end(); it++){
                ver <<  *it;
        }
            mapVarLisrEnd[key] << ver;
            ver.clear();
            ver = peremTabl[i+1];
            mapVarLisrEnd[key] << ver;
            ver.clear();

        }
    }
    //--mapVarListEnd - [key] = [0] - что за элемент - хранит qstringlist эл-тов
    //                          [1] - [0] - кол-во в BOM 1
    //                          [1] - [1] - %
    //                          [2] - что за элемент- хранит qstringlist эл-тов
    //                          [1] - [0] - кол-во в BOM 2
    //                          [1] - [1] - %
    // и т.д.
 //-------------------------------------------------------------------------------------------

  QMap<QString, QList<QStringList> > mapVarLisrCont1, mapVarLisrCont2; //выходная переменная из этого алгоритма
    bool flag = true;
    foreach (QString key, mapVarLisrEnd.keys ()) {
        mapVarLisrCont1[key];
         mapVarLisrCont2[key];
    }
    foreach (QString key, mapVarLisrEnd.keys()) {
        int contIntKey = mapVarLisrEnd[key].count();
        for(int i = 0; i <contIntKey;i = i+2){
            QStringList varK;
            QList <QStringList> varKN;
            for( QStringList::iterator it = mapVarLisrEnd[key][i].begin(); it <mapVarLisrEnd[key][i].end(); it++){

                varK << *it;
            }
            for (QStringList::iterator it = varK.begin(); it < varK.end(); it++) {
                //  Первый влет- по ключу запиывается первый элемент
                if(it == varK.begin()){
                    QStringList peremen;
                    peremen << *it << "1";
                    varKN.append (peremen);
                    it++;
                    continue;
                }
                QString a = (*it); // для отладки
                flag = true;
                //Если по такому ключу и с таким partnumber содержится элемент- кол-во + 1
                for (QList<QStringList>::iterator var = varKN.begin();var < varKN.end(); var++)
                {
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
                    varKN.append (peremen);
                    it++;
                    continue;
                }
            }

            int sumBOM = mapVarLisrEnd[key][i+1][0].toInt();
            int percent = mapVarLisrEnd[key][i+1][1].toInt();
            for (QList<QStringList>::iterator var = varKN.begin();var < varKN.end(); var++)
            {

                int number = (*var).at(1).toInt();
                number = number * sumBOM;
                (*var)[1] = QString::number(number);
                double num = ceil( (number * percent)/100 );
                number = number + num;
                (*var).append(QString::number(number));
            }
             mapVarLisrCont1[key] << varKN;

        }//закрывает цикл for(int i = 0; i <contIntKey;i = i+2)
    }
    // mapVarLisrCont1 == [key] - [эл-т1][кол-во][%]
    //                            [эл-т2][кол-во][%]
    //--Просуммирую одинаковые элементы

    foreach (QString key, mapVarLisrCont1.keys()) {
        for (QList<QStringList>::iterator it = mapVarLisrCont1[key].begin(); it < mapVarLisrCont1[key].end(); it++) {
            //  Первый влет- по ключу запиывается первый элемент
            if(it == mapVarLisrCont1[key].begin()){
                mapVarLisrCont2[key].append (*it);
               // it++;
                continue;
            }
            QString a = (*it).at(0); // для отладки
            flag = true;
            //Если по такому ключу и с таким partnumber содержится элемент
            for (QList<QStringList>::iterator var =   mapVarLisrCont2[key].begin();var <   mapVarLisrCont2[key].end(); var++)
            {
                if( (*var).at(0) == a ){
                    int number = (*it).at(1).toInt() + (*var).at(1).toInt();
                    int percent = (*it).at(2).toInt() + (*var).at(2).toInt();

                    (*var)[1] = QString::number(number);
                    (*var)[2] = QString::number(percent);
                    flag = false;
                    break;
                }

            }
            //если элемент уже был найден- смотрю следующее значение
            //если не был найден -  вношу этот элемент
            if(flag == true){
                mapVarLisrCont2[key].append (*it);
                //it++;
                continue;
            }
        }
    }

mapVarLisrCont = mapVarLisrCont2;

if(flagErr == true)
    return -10;
}
//------------------------------------------------------------------------
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
          peremen << *it << QString::number(0);
          dataBomOut[key].append (peremen);

         }
    }
  //    dataBomOut == [key] - [эл-т1][кол-во][%][кол-во на складе]
  //                          [эл-т2][кол-во][%][кол-во на складе]
  //формирование выходного файла
  QList<QStringList> inWord;
  // по ключам
  foreach (QString key, dataBomOut.keys ()) {
      // по значению ключей
      QStringList keys;
      keys << "" << key << "" << ""  << "" << "" << "" << "";
      inWord << keys;
      keys.clear();
      for( QList<QStringList>::iterator it = dataBomOut[key].begin(); it < dataBomOut[key].end(); it++){
                  //partNumber   //кол-во в BOM   //+%          //на складе
          int numBoms = (*it).at(1).toInt();
          int percent = (*it).at(2).toInt();
          int skladD = (*it).at(3).toInt();
          int res = skladD - numBoms;
          int resPercent = skladD - percent;
          QString zakyp, zakypPer, ost, ostPer;
          if(res < 0){
              zakyp = QString::number( abs(res) );
              ost = QString::number(0);
          }
          if(res > 0){
              zakyp = "-";
              ost = QString::number(res);
          }
          if(resPercent < 0){
              zakypPer = QString::number( abs(resPercent) );
              ostPer = QString::number(0);
          }
          if(resPercent > 0){
              zakypPer = "-";
              ostPer = QString::number(resPercent);
          }
          keys << (*it).at(0) << (*it).at(1) << (*it).at(2) << (*it).at(3)
                  //закупить
               << zakyp << ost << zakypPer << ostPer;
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
               (*var) << "" << "Прочие" << "" << ""<< "" << "" << "" << "";
                continue;
            }
            QString str = keys[index];
            QString a = tem[str];
             ( *(var)).clear();
            (*var) << "" << a << "" << ""<< "" << "" << "" << "";
            int c;
            c++;
        }
    }
    return inWord;

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


