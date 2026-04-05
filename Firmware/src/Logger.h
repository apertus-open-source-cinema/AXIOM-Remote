#pragma once

#include <cstdint>
#include <cstdio>
#include <string>
#include <memory>

// Log levels
enum class LogLevel {
    DEBUG = 0,
    INFO = 1,
    WARNING = 2,
    ERROR = 3,
    CRITICAL = 4
};

// Log entry structure
struct LogEntry {
    LogLevel level;
    uint32_t timestamp;
    const char* module;
    const char* message;
    uint32_t line;
};

// Abstract log sink interface
class LogSink {
public:
    virtual ~LogSink() = default;
    virtual void write(const LogEntry& entry) = 0;
    virtual void flush() {}
};

// Console log sink (USB serial)
class ConsoleLogSink : public LogSink {
private:
    bool usb_connected_ = false;

public:
    ConsoleLogSink();
    void write(const LogEntry& entry) override;
    void setUsbConnected(bool connected) { usb_connected_ = connected; }
};

// Flash log sink (for persistent logging)
class FlashLogSink : public LogSink {
private:
    static constexpr uint32_t LOG_FLASH_OFFSET = 0x200000; // 2MB offset
    static constexpr uint32_t LOG_SIZE = 65536; // 64KB log area
    static constexpr uint32_t MAX_ENTRIES = 512;

    struct FlashLogEntry {
        uint32_t timestamp;
        LogLevel level;
        char module[16];
        char message[64];
        uint32_t line;
    };

    uint32_t next_entry_index_ = 0;
    bool initialized_ = false;

    void initialize();
    void writeToFlash(const FlashLogEntry& entry);

public:
    FlashLogSink();
    void write(const LogEntry& entry) override;
    void flush() override;

    // Log retrieval
    bool getLogEntry(uint32_t index, LogEntry& entry);
    uint32_t getLogEntryCount() const;
    void clearLog();
};

// Logger class
class Logger {
private:
    static Logger* instance_;
    LogLevel min_level_ = LogLevel::INFO;
    std::unique_ptr<ConsoleLogSink> console_sink_;
    std::unique_ptr<FlashLogSink> flash_sink_;

    Logger();
    ~Logger() = default;

    const char* levelToString(LogLevel level) const;
    void writeToSinks(const LogEntry& entry);

public:
    // Singleton access
    static Logger* getInstance();

    // Configuration
    void setMinLevel(LogLevel level) { min_level_ = level; }
    LogLevel getMinLevel() const { return min_level_; }

    void setUsbConnected(bool connected) {
        if (console_sink_) {
            console_sink_->setUsbConnected(connected);
        }
    }

    // Logging methods
    void log(LogLevel level, const char* module, const char* message,
             uint32_t line = 0, const char* file = nullptr);

    // Convenience methods
    void debug(const char* module, const char* message, uint32_t line = 0) {
        log(LogLevel::DEBUG, module, message, line);
    }

    void info(const char* module, const char* message, uint32_t line = 0) {
        log(LogLevel::INFO, module, message, line);
    }

    void warning(const char* module, const char* message, uint32_t line = 0) {
        log(LogLevel::WARNING, module, message, line);
    }

    void error(const char* module, const char* message, uint32_t line = 0) {
        log(LogLevel::ERROR, module, message, line);
    }

    void critical(const char* module, const char* message, uint32_t line = 0) {
        log(LogLevel::CRITICAL, module, message, line);
    }

    // Log retrieval for debugging
    bool getFlashLogEntry(uint32_t index, LogEntry& entry) {
        return flash_sink_ ? flash_sink_->getLogEntry(index, entry) : false;
    }

    uint32_t getFlashLogEntryCount() const {
        return flash_sink_ ? flash_sink_->getLogEntryCount() : 0;
    }

    void clearFlashLog() {
        if (flash_sink_) {
            flash_sink_->clearLog();
        }
    }
};

// Logging macros for convenience
#define LOG_DEBUG(module, message) Logger::getInstance()->debug(module, message, __LINE__)
#define LOG_INFO(module, message) Logger::getInstance()->info(module, message, __LINE__)
#define LOG_WARNING(module, message) Logger::getInstance()->warning(module, message, __LINE__)
#define LOG_ERROR(module, message) Logger::getInstance()->error(module, message, __LINE__)
#define LOG_CRITICAL(module, message) Logger::getInstance()->critical(module, message, __LINE__)