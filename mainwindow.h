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
 void operationSumRefDez(QMap<QString, QList<QStringList>> & mapVarLisrCont);
  listStringInt_t operationSklad();
 void operationSearch( QMap<QString, QList<QStringList>> & dataSkladAndSum);

};

#endif // MAINWINDOW_H
