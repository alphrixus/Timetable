#ifndef PAGEHANDLER_H
#define PAGEHANDLER_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QWidget>
#include <QTextDocument>

class PageHandler : public QObject
{
    Q_OBJECT
public:
    PageHandler(QObject *parent = nullptr);
    QNetworkAccessManager accessManager;
    QTextDocument* Qtd;
    QString text, url;

signals:
    void finish();
public slots:
    void get(QString location);
    void post(QString location, QByteArray data);
    void run();

private slots:
    void readyRead();
    void authenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator);
    void encrypted(QNetworkReply *reply);
    void finished(QNetworkReply *reply);
    void preSharedKeyAuthenticationRequired(QNetworkReply *reply, QSslPreSharedKeyAuthenticator *authenticator);
    void proxyAuthenticationRequired(const QNetworkProxy &proxy, QAuthenticator *authenticator);
    void sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);

private:
    QByteArray data;
};

#endif // PAGEHANDLER_H
