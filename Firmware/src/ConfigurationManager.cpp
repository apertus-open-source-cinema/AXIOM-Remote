#include "ConfigurationManager.h"
#include <pico/unique_id.h>
#include <cstring>
#include <algorithm>
// #include <hardware/flash.h>  // Flash functions not available

ConfigurationManager::ConfigurationManager() {
    loadDefaults();
}

uint32_t ConfigurationManager::calculateChecksum(const uint8_t* data, size_t size) {
    uint32_t checksum = 0;
    for (size_t i = 0; i < size; ++i) {
        checksum = (checksum << 5) + checksum + data[i]; // Simple hash
    }
    return checksum;
}

bool ConfigurationManager::validateConfig(const ConfigHeader& header, const ConfigEntry* entries) {
    if (header.magic != MAGIC_NUMBER) {
        return false;
    }

    // Calculate checksum of entries
    uint32_t calculated_checksum = calculateChecksum(
        reinterpret_cast<const uint8_t*>(entries),
        header.num_entries * sizeof(ConfigEntry)
    );

    return header.checksum == calculated_checksum;
}

void ConfigurationManager::loadDefaults() {
    // Set default configuration values
    cache_[ConfigKey::DISPLAY_BRIGHTNESS] = 100;
    cache_[ConfigKey::USB_TIMEOUT_MS] = 1000U;
    cache_[ConfigKey::THEME_MODE] = 0; // 0 = default theme
    cache_[ConfigKey::POWER_SAVE_MODE] = false;
    cache_[ConfigKey::LAST_SCREEN_ID] = 0;
}

bool ConfigurationManager::load() {
    // Flash functions not available - just use defaults
    // Read configuration from flash
    // uint8_t buffer[CONFIG_SIZE];
    // flash_read(CONFIG_FLASH_OFFSET, buffer, CONFIG_SIZE);

    // const ConfigHeader* header = reinterpret_cast<const ConfigHeader*>(buffer);
    // const ConfigEntry* entries = reinterpret_cast<const ConfigEntry*>(buffer + sizeof(ConfigHeader));

    // if (!validateConfig(*header, entries)) {
    //     printf("Invalid configuration in flash, using defaults\n");
    //     loaded_ = true; // Mark as loaded even with defaults
    //     return true;
    // }

    // // Load entries into cache
    // for (uint32_t i = 0; i < header->num_entries; ++i) {
    //     const ConfigEntry& entry = entries[i];
    //     switch (entry.type) {
    //         case 0: cache_[entry.key] = entry.value.int_val; break;
    //         case 1: cache_[entry.key] = entry.value.uint_val; break;
    //         case 2: cache_[entry.key] = entry.value.float_val; break;
    //         case 3: cache_[entry.key] = entry.value.bool_val; break;
    //         case 4: cache_[entry.key] = std::string(entry.value.string_val); break;
    //     }
    // }

    loaded_ = true;
    printf("Configuration loaded (simulated - no flash storage)\n");
    return true;
}

bool ConfigurationManager::save() {
    if (!loaded_) {
        return false;
    }

    // Flash functions not available - just simulate saving
    // Prepare data to write
    // uint8_t buffer[CONFIG_SIZE] = {0};
    // ConfigHeader* header = reinterpret_cast<ConfigHeader*>(buffer);
    // ConfigEntry* entries = reinterpret_cast<ConfigEntry*>(buffer + sizeof(ConfigHeader));

    // header->magic = MAGIC_NUMBER;
    // header->version = 1;
    // header->num_entries = cache_.size();

    // // Fill entries
    // size_t entry_idx = 0;
    // for (const auto& pair : cache_) {
    //     if (entry_idx >= (CONFIG_SIZE - sizeof(ConfigHeader)) / sizeof(ConfigEntry)) {
    //         break; // Not enough space
    //     }

    //     ConfigEntry& entry = entries[entry_idx++];
    //     entry.key = pair.first;

    //     std::visit([&entry](const auto& value) {
    //         using T = std::decay_t<decltype(value)>;
    //         if constexpr (std::is_same_v<T, int32_t>) {
    //             entry.type = 0;
    //             entry.value.int_val = value;
    //         } else if constexpr (std::is_same_v<T, uint32_t>) {
    //             entry.type = 1;
    //             entry.value.uint_val = value;
    //         } else if constexpr (std::is_same_v<T, float>) {
    //             entry.type = 2;
    //             entry.value.float_val = value;
    //         } else if constexpr (std::is_same_v<T, bool>) {
    //             entry.type = 3;
    //             entry.value.bool_val = value;
    //         } else if constexpr (std::is_same_v<T, std::string>) {
    //             entry.type = 4;
    //             std::strncpy(entry.value.string_val, value.c_str(), sizeof(entry.value.string_val) - 1);
    //         }
    //     }, pair.second);
    // }

    // header->num_entries = entry_idx;
    // header->checksum = calculateChecksum(
    //     reinterpret_cast<const uint8_t*>(entries),
    //     entry_idx * sizeof(ConfigEntry)
    // );

    // // Write to flash (erase sector first)
    // flash_range_erase(CONFIG_FLASH_OFFSET, CONFIG_SIZE);
    // flash_range_program(CONFIG_FLASH_OFFSET, buffer, CONFIG_SIZE);

    printf("Configuration saved (simulated - no flash storage)\n");
    return true;
}

void ConfigurationManager::resetToDefaults() {
    cache_.clear();
    loadDefaults();
}

// Template implementations
template<typename T>
bool ConfigurationManager::get(ConfigKey key, T& value) const {
    auto it = cache_.find(key);
    if (it == cache_.end()) {
        return false;
    }

    // Manual type checking instead of exceptions
    const ConfigValue& val = it->second;
    if constexpr (std::is_same_v<T, int32_t>) {
        if (std::holds_alternative<int32_t>(val)) {
            value = std::get<int32_t>(val);
            return true;
        }
    } else if constexpr (std::is_same_v<T, uint32_t>) {
        if (std::holds_alternative<uint32_t>(val)) {
            value = std::get<uint32_t>(val);
            return true;
        }
    } else if constexpr (std::is_same_v<T, float>) {
        if (std::holds_alternative<float>(val)) {
            value = std::get<float>(val);
            return true;
        }
    } else if constexpr (std::is_same_v<T, bool>) {
        if (std::holds_alternative<bool>(val)) {
            value = std::get<bool>(val);
            return true;
        }
    } else if constexpr (std::is_same_v<T, std::string>) {
        if (std::holds_alternative<std::string>(val)) {
            value = std::get<std::string>(val);
            return true;
        }
    }
    return false;
}

template<typename T>
bool ConfigurationManager::set(ConfigKey key, const T& value) {
    cache_[key] = value;
    return true;
}

// Convenience method implementations
bool ConfigurationManager::getInt(ConfigKey key, int32_t& value) const {
    return get(key, value);
}

bool ConfigurationManager::getUInt(ConfigKey key, uint32_t& value) const {
    return get(key, value);
}

bool ConfigurationManager::getFloat(ConfigKey key, float& value) const {
    return get(key, value);
}

bool ConfigurationManager::getBool(ConfigKey key, bool& value) const {
    return get(key, value);
}

bool ConfigurationManager::getString(ConfigKey key, std::string& value) const {
    return get(key, value);
}

bool ConfigurationManager::setInt(ConfigKey key, int32_t value) {
    return set(key, value);
}

bool ConfigurationManager::setUInt(ConfigKey key, uint32_t value) {
    return set(key, value);
}

bool ConfigurationManager::setFloat(ConfigKey key, float value) {
    return set(key, value);
}

bool ConfigurationManager::setBool(ConfigKey key, bool value) {
    return set(key, value);
}

bool ConfigurationManager::setString(ConfigKey key, const std::string& value) {
    return set(key, value);
}