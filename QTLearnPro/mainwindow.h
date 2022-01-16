#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tswnd.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public:
    void    UpdateSize();

private slots:
    //退出程序
    void quitApplication();
    void updateTimerLabel();
private:
    QString ReadQssFile(const QString& filePath);

private:
    Ui::MainWindow *ui;

    TsWnd   *m_TsWnd;
};
#endif // MAINWINDOW_H
