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
#define EXCEL_COLOR_RED 5287936
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
      ui->tableWidget->resizeColumnsToContents();



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

//    ui->progressBar->setValue(0);
//    QCoreApplication::processEvents();
//    ActiveWord word;
//    if(!word.wordConnect()){
//        QMessageBox msgBox;
//        msgBox.setText("Word не установлен");
//        msgBox.exec();
//        return;
//    }
//    QString path = QApplication::applicationDirPath() + "/test.docx";
//    QAxObject* doc = word.documentOpen(path);
//    if(doc == NULL){
//        QMessageBox msgBox;
//        msgBox.setText("Не найден word- шаблон");
//        msgBox.exec();
//        return;
//    }




    QMap<QString, QList<QStringList> > dataBom;
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
    //word.findReplaseLabel("[Dat]" , dt, false);


   // заполнение таблицы
//    word.setVisible();
//    QStringList listLabel = word.tableGetLabels(1, 3);
//    ui->progressBar->setValue(80);
//    QCoreApplication::processEvents();
//    if (word.tableFill(datInWord,listLabel,1,3) < 0){
//        mesOut("Ошибка заполнения таблицы!");
//        word.setVisible();
//        return;
//    }
//    ui->progressBar->setValue(100);
//    QCoreApplication::processEvents();
//    word.setVisible();
//    int u;
//    u++;


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
//--#-- Поиск строки и колонок из которых считываются данные
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
//--#-- Конец -- Поиск строки и колонок из которых считываются данные
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
                excel.sheetCellColorInsert(sheet, data, i, partNumber);
                varPerem << data.toString();

                int g = varPerem.count();
                if(varPerem[g-2] == "" && varPerem[g-3] == "" )
                    break;
            }
            excel.workbookClose(ex1);

            QTableWidgetItem* item = ui->tableWidget->item(co, 1);
            QTableWidgetItem* item1 = ui->tableWidget->item(co, 2);
            QStringList perrem;
            perrem << item->text();
            perrem << item1->text();
            peremTabl << varPerem << perrem;
            ui->progressBar->setValue(proc);
            QCoreApplication::processEvents();
            proc += proc;
            co++;
        }

     //В peremTable лежат : [0],[2] ... набор всех данных из BOM. = {Ref Designator, patNumber, QTY, color}
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

        int c = (*it).indexOf(",");
        if(c > 0){
            (*it).remove(c, (*it).count() - c);
        }
        c = (*it).indexOf("-");
        if(c > 0){
            (*it).remove(c, (*it).count() - c);
        }
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
                int c = (*it1).indexOf(",");
                if(c > 0){
                    (*it1).remove(c, (*it1).count() - c);
                }
                c = (*it1).indexOf("-");
                if(c > 0){
                    (*it1).remove(c, (*it1).count() - c);
                }
                (*it1).remove(QRegExp("[^A-Za-zА-Яа-я]"));
                //name = name.remove(QRegExp("[^A-Za-zА-Яа-я]"));
                if( it == *it1){
                    mapVarLisrBegin[it].append (*(it1 + 1));
                    mapVarLisrBegin[it].append (*(it1 + 2)); //кол-во элемнтов
                }
                it1 = it1 + 3;// т.к. след. значение через 3
            }
        }
//==--mapVarLisrBegin харнит [key] - [refDez1][qnt1][refdez2][qnt2]...
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

    /*
//--//--mapVarListEnd - [key] = [0][0] refDez1
                                [0][1] кол-во1
                                [0][2] refDez2
                                [0][3] кол-во2
                                .....
                                [1] - [0] - кол-во в BOM 1
                                [1] - [1] - %
                                [2][0] refDezk
                                [2][1] кол-воk
                                ...
                                [3][0]- кол-во в BOM 2
                                [3][1]- %

    */
    mapVarLisrBegin.clear();
     QMap<QString, QList<QStringList> > mapVarList;
    foreach (QString key, allPrefix)
        mapVarList[key];
 //-------------------------------------------------------------------------------------------
 //По каждому элементу в кол-ве должно быть учтнено кол-во BOM'ов и %
 //-------------------------------------------------------------------------------------------
    foreach (QString key, mapVarLisrEnd.keys()) {
        QStringList varKN;
        int contIntKey = mapVarLisrEnd[key].count();
        for(int i = 0; i < contIntKey;i = i + 2){
            QStringList temp1 = mapVarLisrEnd[key][i + 1];
            int cont = temp1[0].toInt();
            int percent = temp1[1].toInt();
            //пробегаюсь по значению ключа
            for( QStringList::iterator it = mapVarLisrEnd[key][i].begin(); it < mapVarLisrEnd[key][i].end(); it++){
                varKN << *it; //refDez
                it++;
                QString temp = *it; //кол-во
                int numb = temp.toInt();
                numb *= cont;
                int numb1 = numb;
                double p = ceil(numb * percent/100);
                numb = numb + p;
                varKN << QString::number(numb1); //без процентов
                varKN << QString::number(numb); //с учетом процента
                int k;
                k++;

            }
            mapVarList[key] << varKN;
            varKN.clear();
        }
    }
 //-------------------------------------------------------------------------------------------
   /* Какие данны в mapVarList?
    mapVarList[key]  ----[0] [0] --- [partNumber]
                         [0] [1] --- [number] //состоящее из кол-ва qty кол-ва BOM файлов
                         [0] [2] --- [number] //состоящее из кол-ва qty кол-ва BOM файлов и %
                          ...
                          //следующий BOM
                         [1][0]
                         ...
*/
 //-------------------------------------------------------------------------------------------
    int countBOM = 0;
    QMap<QString, QStringList > mapStringList; // из Qlist<QStringList> сделаю QStringList
    foreach (QString key, mapVarList.keys()) { //пробегаюсь по ключам
        QStringList varKN;
        //сколько значений существует по ключу == Cколько всего BOM файлов?
         countBOM = mapVarList[key].count();
        for(int i = 0; i < countBOM; i++){
            foreach (auto var, mapVarList[key][i]) {
                varKN << var;
            }
        }
        if(countBOM <= 0)
            return -3;
        mapStringList[key] << varKN;
        varKN.clear();
    }


    //Чтобы сохранить предыдущую логику слхраняю все в QMap<QString, QList<QStringList>>
    //теперь надо найти одинаковые эл-ты и просуммировать их

                int  k ; //для отладки

      QMap<QString, QList<QStringList>>     out;
    foreach (QString key, mapStringList.keys()) {
        QStringList RefDez;
        QVector<int> qty;
        QVector<int> qtyPer; // с процентами
        int countList = mapStringList[key].count();
        RefDez << mapStringList[key][0];
        qty << mapStringList[key][1].toInt();
        qtyPer << mapStringList[key][2].toInt();
        for(int i = 3; i < countList; i +=3 ){ //поиск начиная с 3 эл-та, через 3

            int ind = RefDez.indexOf(mapStringList[key][i]);
            //индекс найден
            if(ind >= 0){
                qty[ind] += mapStringList[key][i + 1].toInt();
                qtyPer[ind] += mapStringList[key][i + 2].toInt();

            }
            else{
                RefDez << mapStringList[key][i];
                qty    << mapStringList[key][i + 1].toInt();
                qtyPer << mapStringList[key][i + 2].toInt();
            }
        }
        //пробежались по ключу- сохраним данные
        if( (RefDez.count() != qty.count()) && (qtyPer.count() != qty.count()) )
            return -4;
        for(int i = 0; i < RefDez.count(); i++){
            QStringList per;
            per << RefDez[i] << QString::number(qty[i]) << QString::number(qtyPer[i]);
            out[key] << per;
        }

    }
    mapVarLisrCont = out;

/*  QMap < QString,QList<QStringList >>
    mapVarList[key]  ----[0] [0] --- [partNumber]
                         [0] [1] --- [number] //состоящее из кол-ва qty кол-ва BOM файлов
                         [0] [2] --- [number] //состоящее из кол-ва qty кол-ва BOM файлов и %
                          ...
                          //следующий BOM
                         [1][0]
                         ...
*/

    if( !countBOM ) // если нуль- то ошибка
        return -5;
    QList <QStringList> RefDez;
    //QVector<int> qty(countBOM);
    QList<QMap < QString,QStringList >> map;

    bool flag = false;
    foreach (QString key, mapVarList.keys()) {
        for(int i = 0; i < countBOM; i++){
            //mapVarList[key][i] - в нем лежат qstringlist
            QStringList a;
            foreach (auto var, mapVarList[key][i]) {
                a << var;
                k++;
            }
            QMap<QString, QStringList> b;
            b[key].append(a);
            if(flag == false){

                map.insert(i, b);
                if( i == countBOM -1)
                    flag = true;
                continue;
            }

            map[i][key].append(a);
            // RefDez.append( a );
             k++;
        }
    }
 // в каждом map[i] хранятся по ключу элементы
    foreach (QString key, mapVarList.keys()) {
    for (int i = 0; i < countBOM ; i++){
            for(int j = 0; j < map[i][key].count(); j+=3){
        bool flagFound = false;
                for (int z = 0; z < out[key].count(); z++){
                    QString a = map[i][key][j];
                    QString b = out[key][z][0];
                    int c = out[key][z].count() + i + 1;
                    int p = map[i][key][j].indexOf(out[key][z][0]);
                    k++;
                    if(p >= 0){
                        out[key][z] << map[i][key][p + 1];
                        flagFound = true;
                        continue;

                    }
                    else{
                        if( flagFound == true)
                            continue;
                         if (z == out[key].count() - 1)
                            out[key][z] << "0";

                    }
                    k++;
                }
            }
        }
    }


    return 1;


//    QMap<QString, QStringList > mapStringListNew;
//    foreach (QString key, mapStringList.keys()) { //пробегаюсь по ключам
//        mapStringListNew[key] << mapStringList[key][0];
//        mapStringListNew[key] << mapStringList[key][1];
//        for(QStringList::iterator itNew = mapStringListNew[key].begin(); itNew < mapStringListNew[key].end(); itNew++){
//            for ( QStringList::iterator it = mapStringList[key].begin() + 2; it <mapStringList[key].end(); it++){
//                QString i = *it;
//                QString inew = *itNew;
//                if( (*itNew).toInt() == 0){//т.е. не число
//                    if(*itNew == *it){
//                        i = *(it + 1);
//                        inew = *(itNew + 1);
//                        int num1 = i.toInt();
//                        int num2 = inew.toInt();
//                        num1 = num1 + num2;
//                        inew = *(itNew + 1);
//                        itNew =  mapStringListNew[key].erase(itNew+1);
//                         inew = *(itNew);
//                        mapStringListNew[key].insert(itNew, QString::number(num1));
//                        i = *it;
//                        inew = *itNew;
//                        int k =0 ;
//                        k++;
//                    }
//                    if(*itNew != *it){
//                        //если это последний, то добавляю в конец списка
//                        if(  (itNew + 1) <= mapStringListNew[key].end() ){
//                            mapStringListNew[key] << *it;
//                            i  = *(it + 1);
//                            mapStringListNew[key] << (*(it + 1));
//                        }
//                        int k;
//                        k++;
//                    }
//                }
//            }
//            itNew++;
//            if(itNew == mapStringListNew[key].end())
//                    break;

//        }
//    }


    ///
    ///old version
    ///
//    //
//    mapVarLisrBegin.clear();
//    mapVarLisrEnd.clear();


//    bool flag = true;
//    foreach (QString key, mapVarLisrEnd.keys ()) {
//        mapVarLisrBegin[key];
//        mapVarLisrEnd[key];
//    }
//    //----------------------//
//    foreach (QString key, mapVarList.keys()) {
//        int contIntKey = mapVarList[key].count();
//        for(int i = 0; i < contIntKey;i = i + 2){
//            QStringList varK;
//            QList <QStringList> varKN;
//            for( QStringList::iterator it = mapVarList[key][i].begin(); it <mapVarList[key][i].end(); it++){

//                varK << *it; //хранит по ключу refDez1 nuber1 refDez2 ...
//            }
//            for (QStringList::iterator it = varK.begin(); it < varK.end(); it++) {
//                //  Первый влет- по ключу запиывается первый элемент
//                if(it == varK.begin()){
//                    QStringList peremen;
//                    peremen << *it << "1";
//                    varKN.append (peremen);
//                    it++;
//                    continue;
//                }
//                QString a = (*it); // для отладки
//                flag = true;
//                //Если по такому ключу и с таким partnumber содержится элемент- кол-во + 1
//                for (QList<QStringList>::iterator var = varKN.begin();var < varKN.end(); var++)
//                {
//                    if( (*var).at(0) == a ){
//                        int number = (*var).at(1).toInt();
//                        ++number;
//                        (*var)[1] = QString::number(number);
//                        flag = false;
//                        break;
//                    }

//                }
//                //если элемент уже был найден- смотрю следующее значение
//                //если не был найден -  вношу этот элемент
//                if(flag == true){
//                    QStringList peremen;
//                    peremen << *it << "1";
//                    varKN.append (peremen);
//                    it++;
//                    continue;
//                }
//            }

//            int sumBOM = mapVarLisrEnd[key][i+1][0].toInt();
//            int percent = mapVarLisrEnd[key][i+1][1].toInt();
//            for (QList<QStringList>::iterator var = varKN.begin();var < varKN.end(); var++)
//            {

//                int number = (*var).at(1).toInt();
//                number = number * sumBOM;
//                (*var)[1] = QString::number(number);
//                double num = ceil( (number * percent)/100 );
//                number = number + num;
//                (*var).append(QString::number(number));
//            }
//             mapVarLisrBegin[key] << varKN;

//        }//закрывает цикл for(int i = 0; i <contIntKey;i = i+2)
//    }
//    // mapVarLisrCont1 == [key] - [эл-т1][кол-во][%]
//    //                            [эл-т2][кол-во][%]
//    //--Просуммирую одинаковые элементы

//    foreach (QString key, mapVarLisrBegin.keys()) {
//        for (QList<QStringList>::iterator it = mapVarLisrBegin[key].begin(); it < mapVarLisrBegin[key].end(); it++) {
//            //  Первый влет- по ключу запиывается первый элемент
//            if(it == mapVarLisrBegin[key].begin()){
//                mapVarLisrEnd[key].append (*it);
//               // it++;
//                continue;
//            }
//            QString a = (*it).at(0); // для отладки
//            flag = true;
//            //Если по такому ключу и с таким partnumber содержится элемент
//            for (QList<QStringList>::iterator var =   mapVarLisrEnd[key].begin();var <   mapVarLisrEnd[key].end(); var++)
//            {
//                if( (*var).at(0) == a ){
//                    int number = (*it).at(1).toInt() + (*var).at(1).toInt();
//                    int percent = (*it).at(2).toInt() + (*var).at(2).toInt();

//                    (*var)[1] = QString::number(number);
//                    (*var)[2] = QString::number(percent);
//                    flag = false;
//                    break;
//                }

//            }
//            //если элемент уже был найден- смотрю следующее значение
//            //если не был найден -  вношу этот элемент
//            if(flag == true){
//                mapVarLisrEnd[key].append (*it);
//                //it++;
//                continue;
//            }
//        }
//    }

//mapVarLisrCont = mapVarLisrEnd;

//if(flagErr == true)
//    return -10;

}
//------------------------------------------------------------------------
QList<QStringList> MainWindow::operationSearch(QMap<QString, QList<QStringList> > &dataSkladAndSum, listStringInt_t &dataSklad){
  QMap<QString, QList<QStringList> > dataBomOut;
  //Заполняю ключами
  int a = 1;
  a++;
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


