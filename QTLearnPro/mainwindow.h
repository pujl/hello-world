#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void    UpdateSize();
private slots:
    //退出程序
    void QuitApplication();

private:
    QString ReadQssFile(const QString& filePath);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
