#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextBrowser>
#include "pagehandler.h"
#include "qthread.h"
#include <QFile>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    PageHandler pageHandler;
    QFile file;

private slots:
    void on_findButton_clicked();
    void parse();
    void showText();

private:
    Ui::MainWindow *ui;
    QTextBrowser*  textBrowser;
    QDockWidget *dockWidget;
    QTextDocument* qtextDoc;
    QThread thread;
    QString text = "Файл последней загрузки пустой/отсутствует";
};
#endif // MAINWINDOW_H
