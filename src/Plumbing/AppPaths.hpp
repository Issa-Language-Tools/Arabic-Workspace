#pragma once

#include <QString>

namespace AppPaths {

// Returns the per-OS app data directory, creating it (and migrating legacy
// db/config files out of the working directory) if needed.
QString appDataDir();

}  // namespace AppPaths
