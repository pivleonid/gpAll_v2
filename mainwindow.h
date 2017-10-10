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

 private slots:
  void openBoms(void);

};

#endif // MAINWINDOW_H
