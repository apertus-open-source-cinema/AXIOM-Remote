#include "EventSystem.h"
#include "ConfigurationManager.h" // For ConfigKey definition
#include <algorithm>

void EventSystem::cleanupExpiredListeners() {
    for (auto& pair : listeners_) {
        auto& vec = pair.second;
        vec.erase(
            std::remove_if(vec.begin(), vec.end(),
                [](const std::weak_ptr<EventListener>& weak_ptr) {
                    return weak_ptr.expired();
                }),
            vec.end()
        );
    }
}

void EventSystem::addListener(EventType type, std::weak_ptr<EventListener> listener) {
    listeners_[type].push_back(listener);
}

void EventSystem::addListener(EventType type, std::shared_ptr<EventListener> listener) {
    owned_listeners_.push_back(listener);
    listeners_[type].push_back(listener);
}

void EventSystem::removeListener(EventType type, EventListener* listener) {
    auto it = listeners_.find(type);
    if (it != listeners_.end()) {
        auto& vec = it->second;
        vec.erase(
            std::remove_if(vec.begin(), vec.end(),
                [listener](const std::weak_ptr<EventListener>& weak_ptr) {
                    auto shared = weak_ptr.lock();
                    return !shared || shared.get() == listener;
                }),
            vec.end()
        );
    }

    // Also remove from owned listeners
    owned_listeners_.erase(
        std::remove_if(owned_listeners_.begin(), owned_listeners_.end(),
            [listener](const std::shared_ptr<EventListener>& shared_ptr) {
                return shared_ptr.get() == listener;
            }),
        owned_listeners_.end()
    );
}

void EventSystem::fireEvent(const Event& event) {
    // Cleanup expired listeners periodically
    static uint32_t cleanup_counter = 0;
    if (++cleanup_counter % 100 == 0) { // Cleanup every 100 events
        cleanupExpiredListeners();
    }

    auto it = listeners_.find(event.type);
    if (it != listeners_.end()) {
        for (auto& weak_listener : it->second) {
            if (auto listener = weak_listener.lock()) {
                listener->onEvent(event);
            }
        }
    }
}

void EventSystem::fireButtonEvent(uint8_t button_id, bool pressed) {
    Event event;
    event.type = pressed ? EventType::BUTTON_PRESSED : EventType::BUTTON_RELEASED;
    event.data.button.button_id = button_id;
    event.data.button.pressed = pressed;
    event.timestamp = 0; // TODO: Add proper timestamp
    fireEvent(event);
}

void EventSystem::fireEncoderEvent(int8_t delta) {
    Event event;
    event.type = EventType::ENCODER_ROTATED;
    event.data.encoder.delta = delta;
    event.timestamp = 0;
    fireEvent(event);
}

void EventSystem::fireTouchEvent(uint16_t x, uint16_t y, bool pressed) {
    Event event;
    event.type = EventType::SCREEN_TOUCHED;
    event.data.touch.x = x;
    event.data.touch.y = y;
    event.data.touch.pressed = pressed;
    event.timestamp = 0;
    fireEvent(event);
}

void EventSystem::fireConfigEvent(ConfigKey key) {
    Event event;
    event.type = EventType::CONFIG_CHANGED;
    event.data.config.key = key;
    event.timestamp = 0;
    fireEvent(event);
}

void EventSystem::fireNavigationEvent(NavigationEvent::Direction direction) {
    Event event;
    event.type = EventType::UI_NAVIGATION;
    event.data.navigation.direction = direction;
    event.timestamp = 0;
    fireEvent(event);
}