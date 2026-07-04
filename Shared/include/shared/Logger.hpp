#pragma once

#include <fstream>
#include <mutex>
#include <string>

namespace shared {

class Logger {
  public:
    static void Init(const std::string& file_path);
    static void Info(const std::string& message);
    static void Error(const std::string& message);
    static void Debug(const std::string& message);

  private:
    static void Write(const std::string& level, const std::string& message);

    static std::mutex mutex_;
    static std::ofstream stream_;
};

}  // namespace shared
