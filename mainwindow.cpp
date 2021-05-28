#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTableWidgetItem>
#include <QTableWidget>
#include <QPainter>
#include <QtMath>
#include <QtDebug>
#include <QMessageBox>

int n, cost = 0, ind = 0;
bool rep = false;
int array[1000];
int completed[100];
int min = 10000;

std::vector<std::vector<int>> matrix;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPalette palette;

    palette.setColor(QPalette::Window,QColor(56, 57, 58));
    this->setPalette(palette);

    ui->tableWidget->setRowCount(num);
    ui->tableWidget->setColumnCount(num);
    ui->tableWidget->horizontalHeader()->setDefaultSectionSize(35); //H
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(35);   //W
    matrix.assign(num, std::vector<int>(num));

    for (int i = 0; i < num; ++i)
    {
        for (int j = 0; j < num; ++j)
        {
            if (i != j)
            {
                QTableWidgetItem *item = new QTableWidgetItem("0");
                ui->tableWidget->setItem(i, j, item);
            }
            else
            {
                QTableWidgetItem *item = new QTableWidgetItem("*");
                item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable);
                ui->tableWidget->setItem(i, j, item);
            }
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnClear_clicked()
{
    matrix.assign(num, std::vector<int>(num));
    for (int i = 0; i < num; ++i)
        for (int j = 0; j < num; ++j)
        {
            if (i != j)
            {
                QTableWidgetItem *item = new QTableWidgetItem("0");
                ui->tableWidget->setItem(i, j, item);
            }
            else
            {
                QTableWidgetItem *item = new QTableWidgetItem("*");
                item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable);
                ui->tableWidget->setItem(i, j, item);
            }
        }
    ui->label_result->setText("");
}


void MainWindow::paintEvent(QPaintEvent *event)
{
    if(ris==true)
    {
        ris = false;
        QPainter painter(ui->openGLW);

        front=QBrush(QColor(0,200,255));
        back=QBrush(QColor(176, 255, 251));
        textPen=QPen(QColor(255,255,255));

        painter.setBrush(front);
        painter.setFont(glFont);
        painter.setPen(textPen);
        textPen.setWidth(2);

        painter.begin(ui->openGLW);


        for (int i = 0; i < num; ++i)
        {
            double p = (2 * M_PI) / num*i + M_PI * 3.0/2.0; // Расположение вершин от точки (pointOx;pointOy) в полярной системе координат
            double xc = pointOx + R * cos(p);               //Координаты вершины по x
            double yc = pointOy + R * sin(p);               //Координаты вершины по y
            QString text; text.setNum(i+1);                 //Текст в вершине


            textPen.setColor(QColor(155, 155, 250));
            painter.drawEllipse(QRectF(xc , yc, 2*r, 2*r));

            textPen.setColor(QColor(255, 255, 250));
            painter.drawText(QRect(xc+20, yc+20, 40, 30),Qt::AlignCenter, text);
            for (int j = 0; j < num; ++j)
            {
                if (matrix[i][j] != 0)
                {
                    textPen.setColor(QColor(255, 255, 250));
                    double p2 = (2 * M_PI) / num*j + M_PI * 3.0/2.0;
                    double xc2 = pointOx + R * cos(p2);
                    double yc2 = pointOx + R * sin(p2);
                    double l = sqrt(pow((xc2 - xc), 2) + pow((yc2 - yc), 2));
                    int xl  = (xc2 - xc)/l*r + xc;            // Координаты х по первой точке
                    int yl  = (yc2 - yc)/l*r + yc;            // Координаты y по первой точке
                    int xl2 = (xc - xc2)/l*r + xc2;           // Координаты x по второй точке
                    int yl2 = (yc - yc2)/l*r + yc2;           // Координаты y по второй точке
                    painter.drawLine(xl + r, yl + r, xl2 + r, yl2 + r);

                    double b = 0;

                    if      (xc2==xl2 && yl2<yc2) b=M_PI*3/2;
                    else if (yc2==yl2 && xl2<xc2) b=M_PI;
                    else if (xc2==xl2 && yl2>yc2) b=M_PI_2;
                    else if (yc2==yl2 && xl2>xc2) b=0;
                    else if (xl2>xc2 && yl2<yc2)  b=atan((yc2-yl2)/(xc2-xl2))+M_PI*2;
                    else if (xl2<xc2 && yl2<yc2)  b=atan((yc2-yl2)/(xc2-xl2))+M_PI;
                    else if (xl2<xc2 && yl2>yc2)  b=atan((yc2-yl2)/(xc2-xl2))+M_PI;
                    else if (xl2>xc2 && yl2>yc2)  b=atan((yc2-yl2)/(xc2-xl2));

                    int xs1=cos(b + M_PI/8)*ls+xl2;
                    int ys1=sin(b + M_PI/8)*ls+yl2;
                    int xs2=cos(b - M_PI/8)*ls+xl2;
                    int ys2=sin(b - M_PI/8)*ls+yl2;
                    painter.drawLine(xl2 + r, yl2 + r, xs1 + r, ys1 + r);
                    painter.drawLine(xl2 + r, yl2 + r, xs2 + r, ys2 + r);

                    double cx = (xl2 + xl) / 2;
                    double cy = (yl2 + yl) / 2;
                    cx = (cx + xl)/2;
                    cy = (cy + yl)/2;
                    cx = (cx + xl2)/2;
                    cy = (cy + yl2)/2;

                    text.setNum(matrix[i][j]);
                    textPen.setColor(QColor(155, 155, 250));
                    if (matrix[i][j] == matrix[j][i])
                    {
                        painter.drawText(QRectF((xl2 + xl) / 2 + 18, (yl2 + yl) / 2 + 27, 20, 15), Qt::AlignCenter, text);
                    }
                    else
                    {
                        painter.drawText(QRectF(cx + 25, cy + 25, 20, 15), Qt::AlignCenter, text);
                    }

                }
            }
        }
        painter.end();
    }
}

int least(int m_city)
{
    int nc = 999;
    int min = 999, kmin;

    for (int i = 0; i < n; i++)
    {
        if (matrix[m_city][i] != 0 && completed[i] == 0)
            if (matrix[m_city][i] + matrix[i][m_city] < min)
            {
                min = matrix[i][0] + matrix[m_city][i];
                kmin = matrix[m_city][i];
                nc = i;
            }
    }

    if (min != 999)
        cost += kmin;

    return nc;
}

void mincost(int m_city)
{
    int n_city;

    completed[m_city] = 1;
    array[ind] = m_city + 1;
    ind++;
    n_city = least(m_city);

    if (n_city == 999)
    {
        n_city = 0;
        array[ind] = n_city + 1;
        ind++;
        cost += matrix[m_city][n_city];
        return;
    }

    mincost(n_city);
}

void MainWindow::on_btnResult_clicked()
{
    int size = num;
    n = size;
    ui->label_result->setText("");
    for(int i = 0; i < size; ++i)
    {
       for(int j = 0; j < size; ++j)
       {
           QTableWidgetItem *item = new QTableWidgetItem();
           item = ui->tableWidget->item(i, j);
           if(item->text()!='*')
           {
              matrix[i][j] = item->text().toInt();
           }
           else
           {
               matrix[i][j] = 0;
           }
       }
       completed[i] = 0;
    }
    mincost(0);
    int cities = 0;
    for (int i = 0; i < num; ++i)
    {
        if (completed[i]!=0)
        {
            cities+=1;
        }
    }
    ui->label_result->setText(QString::number(cost));
    cost = 0;
    rep = 1;
    ris = true;
    repaint();
}



void MainWindow::on_btnCreate_clicked()
{
    for(int i = 0; i < num; ++i)
    {
        for(int j = 0; j < num; ++j)
        {
            QTableWidgetItem *item = new QTableWidgetItem();
            item = ui->tableWidget->item(i,j);
            if(item->text()!='*')
            {
                matrix[i][j] = item->text().toInt();
            }
            else{
                matrix[i][j] = 0;
            }
        }

    }

    ris = true;
    repaint();
}

void MainWindow::on_btnAdd_clicked()
{
    num++;
    matrix.assign(num, std::vector<int>(num));


    ui->tableWidget->setRowCount(num);
    ui->tableWidget->setColumnCount(num);
    ui->tableWidget->horizontalHeader()->setDefaultSectionSize(35); //H
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(35);   //W
    for (int i = 0; i < num; ++i)
    {
        for (int j = 0; j < num; ++j)
        {
            int count = matrix[i][j];
            if (i == num-1 || j == num-1)
                count = 0;
            if (i != j)
            {
                QTableWidgetItem *item = new QTableWidgetItem(tr("%1").arg(count));
                ui->tableWidget->setItem(i, j, item);
            }
            else
            {
                QTableWidgetItem *item = new QTableWidgetItem("*");
                item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable);
                ui->tableWidget->setItem(i, j, item);
            }
        }
    }
    ris = true;
    ui->openGLW->update();
}

void MainWindow::on_btnDel_clicked()
{
    if (num != 1)
        num--;
    matrix.assign(num, std::vector<int>(num));


    ui->tableWidget->setRowCount(num);
    ui->tableWidget->setColumnCount(num);
    ui->tableWidget->horizontalHeader()->setDefaultSectionSize(35); //H
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(35);   //W
    for (int i = 0; i < num; ++i)
    {
        for (int j = 0; j < num; ++j)
        {
            int count = matrix[i][j];
            if (i == num-1 || j == num-1)
                count = 0;
            if (i != j)
            {
                QTableWidgetItem *item = new QTableWidgetItem(tr("%1").arg(count));
                ui->tableWidget->setItem(i, j, item);
            }
            else
            {
                QTableWidgetItem *item = new QTableWidgetItem("*");
                item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable);
                ui->tableWidget->setItem(i, j, item);
            }
        }
    }
    ris = true;
    ui->openGLW->update();
}

void MainWindow::on_pushButton_clicked()
{
    ris = true;
    ui->openGLW->update();
}
