#include "pagehandler.h"
#include "qthread.h"
#include <QNetworkRequest>
#include <QAuthenticator>
#include <QString>
#include <QTextDocument>
#include <QTextBlock>

PageHandler::PageHandler(QObject *parent) : QObject(parent)
{
    this->Qtd = new QTextDocument;
    connect (&accessManager, &QNetworkAccessManager::authenticationRequired,this,&PageHandler::authenticationRequired);
    connect (&accessManager, &QNetworkAccessManager::encrypted,this,&PageHandler::encrypted);
    connect (&accessManager, &QNetworkAccessManager::finished,this,&PageHandler::finished);
    connect (&accessManager, &QNetworkAccessManager::preSharedKeyAuthenticationRequired,this,&PageHandler::preSharedKeyAuthenticationRequired);
    connect (&accessManager, &QNetworkAccessManager::proxyAuthenticationRequired,this,&PageHandler::proxyAuthenticationRequired);
    connect (&accessManager, &QNetworkAccessManager::sslErrors,this,&PageHandler::sslErrors);
}

void PageHandler::get(QString location)
{
    qInfo() << "Ожидание ответа от сервера...";
    QNetworkReply* reply = accessManager.get(QNetworkRequest(QUrl(location)));
    connect (reply, &QNetworkReply::readyRead,this, &PageHandler::readyRead);
}

void PageHandler::post(QString location, QByteArray data)
{
    qInfo() << "Получение данных с сервера...";

    QNetworkRequest request = QNetworkRequest(QUrl(location));
    request.setHeader(QNetworkRequest::ContentTypeHeader,"text/plain");
    QNetworkReply* reply = accessManager.post(request, data);
    connect (reply, &QNetworkReply::readyRead,this, &PageHandler::readyRead);
}

void PageHandler::run()
{
    this->get(this->url);
}

void PageHandler::readyRead()
{
    qInfo() << "Готов к передаче";
    QNetworkReply* reply = qobject_cast < QNetworkReply*>(sender());
    if (reply) this->data = reply->readAll();
    this->text = QString::fromUtf8(this->data);
    int j = 0;
    j = text.indexOf("<u>");
    text.remove(0,j);
    qInfo() << QThread::currentThreadId() << " handler id";
    this->finish();
}
void PageHandler::authenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator)
{
    Q_UNUSED(authenticator);
    qInfo() << "Прозводится аутентификация";
}

void PageHandler::encrypted(QNetworkReply *reply)
{  
    qInfo() << "Производится шифрование";
}

void PageHandler::finished(QNetworkReply *reply)
{
    qInfo() << "Завершение";
}

void PageHandler::preSharedKeyAuthenticationRequired(QNetworkReply *reply, QSslPreSharedKeyAuthenticator *authenticator)
{
    qInfo() << "Обмен ключами аутентификации";
}

void PageHandler::proxyAuthenticationRequired(const QNetworkProxy &proxy, QAuthenticator *authenticator)
{
    qInfo() << "Обмен прокси";
}

void PageHandler::sslErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{
    qInfo() << "ssl ошибка";
}



