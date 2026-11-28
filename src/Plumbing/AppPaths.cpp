#include "AppPaths.hpp"

#include <QDir>
#include <QStandardPaths>

namespace AppPaths {

QString appDataDir() {
  QString dir =
    QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  if (dir.isEmpty()) {
    dir = QDir::homePath() + "/.language-workspace";
  }

  QDir().mkpath(dir);
  return dir;
}

}  // namespace AppPaths
