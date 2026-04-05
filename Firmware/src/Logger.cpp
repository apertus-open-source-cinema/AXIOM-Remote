#include "Logger.h"
#include <pico/time.h>
#include <cstring>
#include <algorithm>
// #include <hardware/flash.h>  // Flash functions not available in this SDK version

// Static member initialization
Logger* Logger::instance_ = nullptr;

ConsoleLogSink::ConsoleLogSink() {
    // Check if USB is connected initially
    // usb_connected_ = stdio_usb_connected();  // Function not available
    usb_connected_ = true;  // Assume USB is connected for now
}

void ConsoleLogSink::write(const LogEntry& entry) {
    if (!usb_connected_) {
        return; // Don't try to write to disconnected USB
    }

    const char* level_str = "UNKWN";
    switch (entry.level) {
        case LogLevel::DEBUG: level_str = "DEBUG"; break;
        case LogLevel::INFO: level_str = "INFO "; break;
        case LogLevel::WARNING: level_str = "WARN "; break;
        case LogLevel::ERROR: level_str = "ERROR"; break;
        case LogLevel::CRITICAL: level_str = "CRIT "; break;
    }

    printf("[%s] %s: %s", level_str, entry.module, entry.message);

    if (entry.line > 0) {
        printf(" (line %u)", entry.line);
    }
    printf("\n");
}

FlashLogSink::FlashLogSink() {
    initialize();
}

void FlashLogSink::initialize() {
    if (initialized_) return;

    // Flash functions not available - simulate with in-memory storage
    // Read the next entry index from flash
    // uint32_t index_buffer[1];
    // flash_read(LOG_FLASH_OFFSET, reinterpret_cast<uint8_t*>(index_buffer), sizeof(uint32_t));
    // next_entry_index_ = index_buffer[0];

    next_entry_index_ = 0;  // Start from 0

    // Validate index
    if (next_entry_index_ >= MAX_ENTRIES) {
        next_entry_index_ = 0;
    }

    initialized_ = true;
}

void FlashLogSink::writeToFlash(const FlashLogEntry& entry) {
    // Flash functions not available - just simulate writing
    // uint32_t entry_offset = LOG_FLASH_OFFSET + sizeof(uint32_t) +
    //                        (next_entry_index_ * sizeof(FlashLogEntry));

    // if (entry_offset + sizeof(FlashLogEntry) > LOG_FLASH_OFFSET + LOG_SIZE) {
    //     // Log is full, wrap around
    //     next_entry_index_ = 0;
    //     entry_offset = LOG_FLASH_OFFSET + sizeof(uint32_t);
    // }

    // Write the entry
    // flash_range_program(entry_offset, reinterpret_cast<const uint8_t*>(&entry), sizeof(FlashLogEntry));

    // Update the index
    next_entry_index_ = (next_entry_index_ + 1) % MAX_ENTRIES;
    // uint32_t index_buffer[1] = {next_entry_index_};
    // flash_range_program(LOG_FLASH_OFFSET, reinterpret_cast<uint8_t*>(index_buffer), sizeof(uint32_t));

    printf("FlashLogSink: Would write log entry to flash (simulated)\n");
}

void FlashLogSink::write(const LogEntry& entry) {
    FlashLogEntry flash_entry;
    flash_entry.timestamp = entry.timestamp;
    flash_entry.level = entry.level;
    flash_entry.line = entry.line;

    // Copy strings with bounds checking
    std::strncpy(flash_entry.module, entry.module, sizeof(flash_entry.module) - 1);
    flash_entry.module[sizeof(flash_entry.module) - 1] = '\0';

    std::strncpy(flash_entry.message, entry.message, sizeof(flash_entry.message) - 1);
    flash_entry.message[sizeof(flash_entry.message) - 1] = '\0';

    writeToFlash(flash_entry);
}

void FlashLogSink::flush() {
    // Flash writes are synchronous, so nothing to flush
}

bool FlashLogSink::getLogEntry(uint32_t index, LogEntry& entry) {
    if (index >= MAX_ENTRIES) {
        return false;
    }

    // Flash functions not available - return empty entry
    // uint32_t entry_offset = LOG_FLASH_OFFSET + sizeof(uint32_t) +
    //                        (index * sizeof(FlashLogEntry));

    // FlashLogEntry flash_entry;
    // flash_read(entry_offset, reinterpret_cast<uint8_t*>(&flash_entry), sizeof(FlashLogEntry));

    // Basic validation - check if entry has valid timestamp
    // if (flash_entry.timestamp == 0 || flash_entry.timestamp == 0xFFFFFFFF) {
    //     return false; // Empty or erased entry
    // }

    // entry.level = flash_entry.level;
    // entry.timestamp = flash_entry.timestamp;
    // entry.module = flash_entry.module; // Note: this points to flash memory
    // entry.message = flash_entry.message; // Note: this points to flash memory
    // entry.line = flash_entry.line;

    return false; // No flash storage available
}

uint32_t FlashLogSink::getLogEntryCount() const {
    return MAX_ENTRIES; // Always return max, let caller check validity
}

void FlashLogSink::clearLog() {
    // Flash functions not available - just reset index
    // Erase the entire log area
    // flash_range_erase(LOG_FLASH_OFFSET, LOG_SIZE);

    // Reset index
    next_entry_index_ = 0;
    // uint32_t index_buffer[1] = {0};
    // flash_range_program(LOG_FLASH_OFFSET, reinterpret_cast<uint8_t*>(index_buffer), sizeof(uint32_t));

    printf("FlashLogSink: Would clear flash log (simulated)\n");
}

Logger::Logger() {
    console_sink_ = std::make_unique<ConsoleLogSink>();
    flash_sink_ = std::make_unique<FlashLogSink>();
}

Logger* Logger::getInstance() {
    if (!instance_) {
        instance_ = new Logger();
    }
    return instance_;
}

const char* Logger::levelToString(LogLevel level) const {
    switch (level) {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR: return "ERROR";
        case LogLevel::CRITICAL: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

void Logger::writeToSinks(const LogEntry& entry) {
    if (console_sink_) {
        console_sink_->write(entry);
    }
    if (flash_sink_) {
        flash_sink_->write(entry);
    }
}

void Logger::log(LogLevel level, const char* module, const char* message,
                 uint32_t line, const char* file) {
    if (level < min_level_) {
        return; // Filter out messages below minimum level
    }

    LogEntry entry;
    entry.level = level;
    entry.timestamp = to_ms_since_boot(get_absolute_time());
    entry.module = module ? module : "UNKNOWN";
    entry.message = message ? message : "";
    entry.line = line;

    writeToSinks(entry);
}