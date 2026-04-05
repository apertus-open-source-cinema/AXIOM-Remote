#pragma once

#include <functional>
#include <unordered_map>
#include <vector>
#include <memory>
#include <cstdint>

// Forward declaration for ConfigKey
enum class ConfigKey;

// Event types
enum class EventType {
    BUTTON_PRESSED,
    BUTTON_RELEASED,
    ENCODER_ROTATED,
    SCREEN_TOUCHED,
    USB_CONNECTED,
    USB_DISCONNECTED,
    POWER_LOW,
    CONFIG_CHANGED,
    UI_NAVIGATION,
    CUSTOM_EVENT = 1000
};

// Event data structures
struct ButtonEvent {
    uint8_t button_id;
    bool pressed;
};

struct EncoderEvent {
    int8_t delta; // Positive for clockwise, negative for counter-clockwise
};

struct TouchEvent {
    uint16_t x, y;
    bool pressed;
};

struct ConfigEvent {
    ConfigKey key;
    // Value can be retrieved from ConfigurationManager
};

struct NavigationEvent {
    enum class Direction { UP, DOWN, LEFT, RIGHT, ENTER, BACK };
    Direction direction;
};

// Generic event data union
union EventData {
    ButtonEvent button;
    EncoderEvent encoder;
    TouchEvent touch;
    ConfigEvent config;
    NavigationEvent navigation;
    uint32_t custom_data;
};

// Event structure
struct Event {
    EventType type;
    EventData data;
    uint32_t timestamp;
};

// Event listener interface
class EventListener {
public:
    virtual ~EventListener() = default;
    virtual void onEvent(const Event& event) = 0;
};

// Event system for decoupling components
class EventSystem {
private:
    std::unordered_map<EventType, std::vector<std::weak_ptr<EventListener>>> listeners_;
    std::vector<std::shared_ptr<EventListener>> owned_listeners_; // For automatic cleanup

    void cleanupExpiredListeners();

public:
    EventSystem() = default;
    ~EventSystem() = default;

    // Register a listener for an event type
    void addListener(EventType type, std::weak_ptr<EventListener> listener);
    void addListener(EventType type, std::shared_ptr<EventListener> listener);

    // Remove a listener
    void removeListener(EventType type, EventListener* listener);

    // Fire an event to all registered listeners
    void fireEvent(const Event& event);

    // Convenience methods for common events
    void fireButtonEvent(uint8_t button_id, bool pressed);
    void fireEncoderEvent(int8_t delta);
    void fireTouchEvent(uint16_t x, uint16_t y, bool pressed);
    void fireConfigEvent(ConfigKey key);
    void fireNavigationEvent(NavigationEvent::Direction direction);
};