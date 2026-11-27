#include "GatewayClient.hpp"

#include <qurl.h>

#include <QJsonArray>
#include <QJsonObject>
#include <QString>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

void GatewayClient::lookup(const std::string word) {
  QNetworkRequest request(*m_baseUrl);
  request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

  QJsonObject body;
  body["word"] = word.c_str();
  QNetworkReply* reply = m_NetworkManager->post(request, QJsonDocument(body).toJson());

  connect(reply, &QNetworkReply::finished, this, [this, reply]() {
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError) {
      emit errorOccured(reply->errorString());
      return;
    }

    const QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    emit resultsReady(doc.array());
  });
}

GatewayClient::GatewayClient(QObject* parent) : QObject(parent) {
  // TODO: start gateway service

  m_NetworkManager = new QNetworkAccessManager(this);
  m_baseUrl = new QUrl("http://127.0.0.1:8420/lookup");
}

GatewayClient::~GatewayClient() {
  //  TODO: stop running gateway service

  delete m_baseUrl;
}
