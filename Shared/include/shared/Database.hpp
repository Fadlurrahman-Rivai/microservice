#pragma once

#include <mysqlx/xdevapi.h>

#include <memory>
#include <string>

#include "shared/Config.hpp"

namespace shared {

class Database {
  public:
    explicit Database(const DatabaseConfig& config);

    bool IsConnected() const;
    mysqlx::Session& Session();

  private:
    std::unique_ptr<mysqlx::Session> session_;
};

}  // namespace shared
