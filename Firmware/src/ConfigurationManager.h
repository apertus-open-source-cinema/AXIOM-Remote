#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <variant>

// Configuration keys
enum class ConfigKey {
    DISPLAY_BRIGHTNESS,
    USB_TIMEOUT_MS,
    THEME_MODE,
    POWER_SAVE_MODE,
    LAST_SCREEN_ID
};

// Configuration value types
using ConfigValue = std::variant<int32_t, uint32_t, float, bool, std::string>;

// Configuration manager for persistent settings
class ConfigurationManager {
private:
    static constexpr uint32_t CONFIG_FLASH_OFFSET = 0x100000; // 1MB offset
    static constexpr uint32_t CONFIG_SIZE = 4096; // 4KB config area
    static constexpr uint32_t MAGIC_NUMBER = 0x41584D;

    struct ConfigHeader {
        uint32_t magic;
        uint32_t version;
        uint32_t checksum;
        uint32_t num_entries;
    };

    struct ConfigEntry {
        ConfigKey key;
        uint8_t type; // 0=int32, 1=uint32, 2=float, 3=bool, 4=string
        union {
            int32_t int_val;
            uint32_t uint_val;
            float float_val;
            bool bool_val;
            char string_val[64];
        } value;
    };

    std::unordered_map<ConfigKey, ConfigValue> cache_;
    bool loaded_ = false;

    uint32_t calculateChecksum(const uint8_t* data, size_t size);
    bool validateConfig(const ConfigHeader& header, const ConfigEntry* entries);
    void loadDefaults();

public:
    ConfigurationManager();
    ~ConfigurationManager() = default;

    bool load();
    bool save();
    void resetToDefaults();

    // Generic get/set methods
    template<typename T>
    bool get(ConfigKey key, T& value) const;

    template<typename T>
    bool set(ConfigKey key, const T& value);

    // Convenience methods for common types
    bool getInt(ConfigKey key, int32_t& value) const;
    bool getUInt(ConfigKey key, uint32_t& value) const;
    bool getFloat(ConfigKey key, float& value) const;
    bool getBool(ConfigKey key, bool& value) const;
    bool getString(ConfigKey key, std::string& value) const;

    bool setInt(ConfigKey key, int32_t value);
    bool setUInt(ConfigKey key, uint32_t value);
    bool setFloat(ConfigKey key, float value);
    bool setBool(ConfigKey key, bool value);
    bool setString(ConfigKey key, const std::string& value);
};