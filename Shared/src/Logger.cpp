#include "shared/Logger.hpp"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace shared {

std::mutex Logger::mutex_;
std::ofstream Logger::stream_;

void Logger::Init(const std::string& file_path) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (stream_.is_open()) {
        stream_.close();
    }
    stream_.open(file_path, std::ios::app);
}

void Logger::Info(const std::string& message) {
    Write("INFO", message);
}

void Logger::Error(const std::string& message) {
    Write("ERROR", message);
}

void Logger::Debug(const std::string& message) {
    Write("DEBUG", message);
}

void Logger::Write(const std::string& level, const std::string& message) {
    std::lock_guard<std::mutex> lock(mutex_);

    const auto now = std::chrono::system_clock::now();
    const std::time_t now_c = std::chrono::system_clock::to_time_t(now);

    std::ostringstream oss;
    oss << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S") << " ["
        << level << "] " << message;

    if (stream_.is_open()) {
        stream_ << oss.str() << '\n';
    }

    std::cout << oss.str() << std::endl;
}

}  // namespace shared
