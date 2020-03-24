#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "line_qt.h"
#include <vector>
#include<fstream>
#include"calc.h"
using namespace std;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    bool eventFilter(QObject *watched, QEvent *event);
    void myPaint();
    vector<line_qt> lines;
    //Core core2;
    //Circle c;
    //computationalGeometry c2;
    //skipList c3;
    string path;
   // Line l;
    int addL(line_qt l);
    //void MainWindow::myPaint()
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void be_Line_L(line_qt ori,line_qt &l);
};
#endif // MAINWINDOW_H
