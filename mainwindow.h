#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QOpenGLWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

    double num    = 3.0;           // Кол-во вершин
    int pointOx = 200 + num * 15;//Отступ по оси x
    int pointOy = 200 + num * 15;//Отступ по оси y
    int R       = num * 50;      //Радиус окружности на которой расположены вершины
    int r       = 40;          //Радиус вершины
    int ls      = 15;

    QBrush front;
    QBrush back;
    QFont glFont;
    QPen textPen;
    ~MainWindow();
    bool ris = false;
private slots:

    void on_btnClear_clicked();

    void on_btnCreate_clicked();

    void on_btnAdd_clicked();

    void on_btnDel_clicked();

    void on_btnResult_clicked();

    void on_pushButton_clicked();

protected:

    void paintEvent(QPaintEvent *event) override;

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
