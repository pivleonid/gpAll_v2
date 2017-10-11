#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
  class MainWindow;
}

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
  void operationSumRefDez();

  void operationSklad();

};

#endif // MAINWINDOW_H
