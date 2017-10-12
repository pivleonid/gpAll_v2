#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
  class MainWindow;
}
struct list_t{
  QString str;
  int n;
};
//QList<list_t> sklad;
typedef QList<list_t> listStringInt_t;

class MainWindow : public QMainWindow
{
  Q_OBJECT



public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
  void mesOut(QString);



private:
  Ui::MainWindow *ui;
  QStringList fileName_DATAs;
  QString fileName_DATA;

 private slots:
  //функции, привязанные к кнопкам
  void openBoms();
  void openSklad();
  void clear();
  void generate();
  //вспомогательные функции
  //Получение и суммирование всех BOM файлов
 void operationSumRefDez(QMap<QString, QList<QStringList>> & mapVarLisrCont);
  //Получение данных со склада
  listStringInt_t operationSklad();
  // поиск элементов из BOM файла в файле со складом.
  // dataSkladAndSum = [ключ] -> [0] == "partNumber"
  //                             [1] == "Кол-во всех элементов во всех BOM'ах"
  //                             [2] == "Кол-во этих элементов на складе"
 QList<QStringList> operationSearch( QMap<QString, QList<QStringList>> & dataSkladAndSum, listStringInt_t &dataSklad);

};

#endif // MAINWINDOW_H
