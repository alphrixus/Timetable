#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "pagehandler.h"
#include <QDockWidget>
#include <QFile>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    textBrowser = new QTextBrowser();
    qtextDoc = new QTextDocument();
    file.setFileName("lastDownloaded.txt");
    dockWidget = new QDockWidget(tr("№ группы:"), this);
    if (file.open(QIODevice::ReadOnly))
    {
        qInfo() << "Считываю текст последней загрузки...";
        text = file.readAll();
    }
    else
    {
        qDebug() << "Ошибка чтения файла";
    }
    file.close();

    dockWidget->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::RightDockWidgetArea);
    this->qtextDoc->setHtml(text);
    this->textBrowser->setDocument(qtextDoc);
    this->dockWidget->setWidget(textBrowser);
    this->addDockWidget(Qt::BottomDockWidgetArea,dockWidget);
    dockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    this->resizeDocks({dockWidget},{500},Qt::Vertical);

    connect(&thread, SIGNAL(started()), &pageHandler, SLOT(run()));
    connect(&pageHandler, &PageHandler::finish, &thread, &QThread::terminate);
    connect(&pageHandler, &PageHandler::finish, this, &MainWindow::showText);
    pageHandler.moveToThread(&thread);
    pageHandler.Qtd->moveToThread(&thread);
    pageHandler.accessManager.moveToThread(&thread);
 }

MainWindow::~MainWindow()
{
    delete ui;
    delete textBrowser;
    delete qtextDoc;
    delete dockWidget;
}
void MainWindow::showText()
{
    this->text = pageHandler.text;
    this->qtextDoc->setHtml(text);
    this->textBrowser->setDocument(qtextDoc);
    this->dockWidget->setWidget(textBrowser);
    addDockWidget(Qt::BottomDockWidgetArea, dockWidget);
    dockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        qInfo() << "Записываю в файл последней загрузки";
        stream << text;
        file.flush();
        file.close();
    }

}
void MainWindow::on_findButton_clicked()
{
    this->dockWidget->setWindowTitle("№ группы: "+ui->lineGroupEdit->text());
    pageHandler.url = "https://schedule.tsu.tula.ru/?group="+ui->lineGroupEdit->text();
    this->parse();
}
void MainWindow::parse()
{
    thread.start();
}



