#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <datastorage.h>
namespace Ui {
  class MainWindow;
}
struct list_t{
  QString str;
  int n;
};

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
  void openAbout();
  //Чтение данных из входных файлов
 int  ReadAllBom(QMap <QString, QList<TData> > &allBom);
  //Получение данных со склада
  listStringInt_t readSklad();
  // поиск элементов из BOM файла в файле со складом.
 QList<QStringList> operationSearch( QMap <QString, QList<TData> > &allBom, listStringInt_t &dataSklad);

};

#endif // MAINWINDOW_H
