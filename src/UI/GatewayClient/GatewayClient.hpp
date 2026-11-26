#pragma once

#include <QObject>
#include <string>

class QNetworkAccessManager;
class QUrl;
class QString;

class GatewayClient : public QObject {
  Q_OBJECT

public:
  void lookup(const std::string word);

  explicit GatewayClient(QObject* parent = nullptr);
  ~GatewayClient();

signals:
  void resultsReady(const QJsonArray& results);
  void errorOccured(const QString error);

private:
  QNetworkAccessManager* m_NetworkManager;
  QUrl* m_baseUrl;
};
