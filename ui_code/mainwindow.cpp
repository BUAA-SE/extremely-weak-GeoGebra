#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QFileDialog>
#include<QFile>
#include<QPainter>
#include<string>
#include<QDebug>
#include<QListWidgetItem>
#include<sstream>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->widget->installEventFilter(this);
    path = "input.txt";
    //添加圆
    connect(ui->addButton1,&QPushButton::clicked,[=](){

       if(ui->cir_r->value()>0){
           line_qt cir;
           cir.type = 'C';
           cir.x1 = ui->cir_x->value();
           cir.y1 = ui->cir_y->value();
           cir.x2 = ui->cir_r->value();
           string name = "C "+to_string(cir.x1)+" "+to_string(cir.y1)+" "+to_string(cir.x2);
           QString  name2= QString::fromStdString(name);
           QListWidgetItem * item = new QListWidgetItem(name2);
           ui->listWidget->addItem(item);
       }
    });


    //添加直线
    connect(ui->addButton2,&QPushButton::clicked,[=](){
        char type;
        if(ui->isLine_L->isChecked()) type = 'L';
        else if(ui->is_Line_R->isChecked()) type = 'R';
        else if(ui->isLine_S->isChecked()) type = 'S';
        else return;
        line_qt l;
        l.type = type;
        l.x1  = ui->line_x1->value();
        l.y1 = ui->line_y1->value();
        l.x2 = ui->line_x2->value();
        l.y2 = ui->line_y2->value();
        string name = "";
        name.append(1,l.type);
        name = name+" "+to_string(l.x1)+" "+to_string(l.y1)+" "+to_string(l.x2)+" "+to_string(l.y2);
        QString  name2= QString::fromStdString(name);
        QListWidgetItem * item = new QListWidgetItem(name2);
        ui->listWidget->addItem(item);
    });

    //删除
    connect(ui->del_button,&QPushButton::clicked,[=](){
        int row = ui->listWidget->currentRow();//获取当前鼠标所选行
        ui->listWidget->takeItem(row);//删除该行
    });


    //清空
    connect(ui->clear_button,&QPushButton::clicked,[=](){

        ui->listWidget->clear();
    });


    //运行
    connect(ui->run_button,&QPushButton::clicked,[=](){
            int size =ui->listWidget->count();
            string str;
            vector<string> v;
            stringstream ss;
            char type;
            double x1,y1,x2,y2 = 0;
           // core.init();

            lines.clear();
           for(int i=0;i<size;i++)
           {
               line_qt l;
              ss.clear();
               str = ui->listWidget->item(i)->text().toStdString();
               ss<<str;
               ss>>type;
               if(type=='C'){
                   ss>>x1>>y1>>x2;
               }
               else{
                   ss>>x1>>y1>>x2>>y2;
               }
               l.type = type;
               l.x1 = x1;
               l.x2 = x2;
               l.y1 = y1;
               l.y2 = y2;
               lines.push_back(l);
               v.push_back(str);
               //addL(l);
           }
           int inter_num;
          // computationalGeometry cg;
           inter_num = count_int(v);
           QString st = QString::fromStdString(to_string(inter_num));
           ui->lineEdit->setText(st);
         //  cg.test();
           update();

    });

    //文件
    connect(ui->fileButton,&QPushButton::clicked,[=](){
        QString path1 = QFileDialog::getOpenFileName(this,"打开文件","C:\\Users");
        string path2 = path1.toStdString();
       ifstream in;
        in.open(path2,ios::in);
        if(!in) return;
        ui->listWidget->clear();
        int n;
        in>>n;
        char type;
       // cout<<"n: "<<n<<endl;
        double x1,x2,y1,y2 = 0;
        for(int i = 0;i<n;i++){
            in>>type;
           // cout<<"tp"<<type<<i<<endl;
            if(type=='C'){
                in>>x1>>y1>>x2;
                string name = "C "+to_string(x1)+" "+to_string(y1)+" "+to_string(x2);
                QString  name2= QString::fromStdString(name);
                QListWidgetItem * item = new QListWidgetItem(name2);
                ui->listWidget->addItem(item);
            }
            else{
                in>>x1>>y1>>x2>>y2;
                string name = "";
                name.append(1,type);
              //  cout<<"tp"<<type<<endl;
               // cout<<name<<endl;
                name = name+" "+to_string(x1)+" "+to_string(y1)+" "+to_string(x2)+" "+to_string(y2);
                QString  name2= QString::fromStdString(name);
                QListWidgetItem * item = new QListWidgetItem(name2);
                ui->listWidget->addItem(item);
            }

        }
        in.close();
    });


}
/*
int Core::line_draw() {
    //line & line
    vec.clear();
    for (int i = 1; i <= cnt_l; ++i)
        for (int j = i + 1; j <= cnt_l; ++j) {
            if (cG.lineIntersectionWithLine(line[i], line[j])) {
                vec.push_back(cG.globalIntersection);
            }
        }
    //circle & circle
    for (int i = 1; i <= cnt_c; ++i)
        for (int j = i + 1; j <= cnt_c; ++j) {
            cG.circleIntersectionWithCircle(circle[i], circle[j], vec);
        }
    //line & circle
    for (int i = 1; i <= cnt_l; ++i)
        for (int j = 1; j <= cnt_c; ++j) {
            cG.lineIntersectionWithCircle(line[i], circle[j], vec);
        }
    sort(vec.begin(), vec.end());
    auto new_end = unique(vec.begin(), vec.end());
    vec.erase(new_end, vec.end());
    return (int) vec.size();
}
*/
MainWindow::~MainWindow()
{
    delete ui;
}


bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->widget && event->type() == QEvent::Paint)
    {
        myPaint();
    }
    return QWidget::eventFilter(watched,event);
}

int MainWindow::addL(line_qt l){
    int k = 0;
    if(l.type=='C'){
        //k = core.addCircle(Point(l.x1,l.y1),l.x2);
    }
    else{
       // k = core.addLine(Point(l.x1,l.y1),Point(l.x2,l.y2),l.type);
    }
    return k;
}

void MainWindow::be_Line_L(line_qt ori,line_qt &l){
   double x1 = -100000,y1 = 0,x2 = 100000,y2 = 0;
   if(ori.type == 'L'){
       if(ori.x1==ori.x2){
           x1 = ori.x1;
           y1 = -100000;
           x2 = x1;
           y2 = 100000;
       }
       else{
           double k = (ori.y2-ori.y1)/(ori.x2-ori.x1);
           double b = ori.y2 - k*ori.x2;
           //qDebug()<<"k "<<k<<endl;
           //qDebug()<<"b "<<b<<endl;
           y1 = k*x1+b;
           y2 = k*x2+b;
       }
   }
   else if(ori.type == 'R'){
       if(ori.x1==ori.x2){
           x1 = ori.x1;
           y1 = ori.y1;
           x2 = x1;
           y2 = ori.y2>ori.y1?100000:-100000;
       }
       else{
           double k = (ori.y2-ori.y1)/(ori.x2-ori.x1);
           double b = ori.y2 - k*ori.x2;
           y1 = ori.y1;
           x2 = ori.y2>ori.y1?x2:x1;
           x1 = ori.x1;
           y2 = k*x2+b;

       }
   }
   else return;
   l.type = ori.type;
   l.x1 = x1;
   l.x2 = x2;
   l.y1 = y1;
   l.y2 = y2;
 //  qDebug()<<"x1 "<<x1<<endl;
  // qDebug()<<"y1 "<<y1<<endl;
  // qDebug()<<"x2 "<<x2<<endl;
  // qDebug()<<"y2 "<<y2<<endl;
}


void MainWindow::myPaint()
{
    int width = ui->widget->width()-1;
    int height = ui->widget->height()-1;
    //画边框
    QPainter painter(ui->widget);
    QBrush brush("#D3D3D3");
    QPen pen(QColor(0, 0, 0));
    //pen.setWidth(3);
    brush.setStyle(Qt::CrossPattern);
    painter.setBrush(brush);
    painter.setPen(pen);
    painter.drawRect(0,0,width,height);

    //画坐标轴
    QPen pen2(QColor(0, 0, 0));
    pen2.setWidth(2);
    painter.setPen(pen2);
    painter.drawLine(0,height/2,width,height/2);
    painter.drawLine(width,height/2,width-7,height/2-7);
    painter.drawLine(width,height/2,width-7,height/2+7);

    painter.drawLine(width/2,0,width/2,height);
    painter.drawLine(width/2,0,width/2-7,7);
    painter.drawLine(width/2,0,width/2+7,7);

    //画线
    QPen pen3(Qt::cyan);
    pen3.setWidth(2);
    painter.setPen(pen3);

    line_qt temp,tmp2;
    for(int i = 0;i<int(this->lines.size());i++){
        temp = this->lines.at(i);
        switch(temp.type){
        case 'C':
            painter.drawEllipse(QPointF(width/2+temp.x1,(height/2-temp.y1)),temp.x2,temp.x2);
            break;
        case 'L':
            be_Line_L(temp,tmp2);
            painter.drawLine(width/2+tmp2.x1,height/2-tmp2.y1,width/2+tmp2.x2,height/2-tmp2.y2);
            break;
        case 'R':
            be_Line_L(temp,tmp2);
            painter.drawLine(width/2+tmp2.x1,height/2-tmp2.y1,width/2+tmp2.x2,height/2-tmp2.y2);
            break;
        case 'S':
            painter.drawLine(width/2+temp.x1,height/2-temp.y1,width/2+temp.x2,height/2-temp.y2);
        default:
            break;
        }

    }
}
