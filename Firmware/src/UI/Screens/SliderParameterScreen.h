#pragma once

#include "ParameterScreen.h"
#include <string>

class SliderParameterScreen : public ParameterScreen
{
private:
    int _minValue = 0;
    int _maxValue = 100;
    int _currentValue = 50;
    int _step = 1;
    std::string _unit = "";

protected:
    lv_obj_t* _slider = nullptr;
    lv_obj_t* _valueLabel = nullptr;

public:
    SliderParameterScreen(AppContext* context, const std::string& title, const std::string& parameterName)
        : ParameterScreen(context, title, parameterName)
    {
    }

    void SetRange(int min, int max, int step = 1)
    {
        _minValue = min;
        _maxValue = max;
        _step = step;
        if (_currentValue < _minValue) _currentValue = _minValue;
        if (_currentValue > _maxValue) _currentValue = _maxValue;
    }

    void SetUnit(const std::string& unit) { _unit = unit; }

    void SetCurrentValue(int value) override
    {
        if (value >= _minValue && value <= _maxValue)
        {
            _currentValue = value;
            UpdateDisplay();
        }
    }

    int GetCurrentValue() const override { return _currentValue; }

    void CreateParameterUI(lv_obj_t* parent) override
    {
        // Create value display
        lv_obj_t* valueContainer = lv_obj_create(parent);
        lv_obj_remove_style_all(valueContainer);
        lv_obj_set_size(valueContainer, lv_pct(90), 80);
        lv_obj_align(valueContainer, LV_ALIGN_TOP_MID, 0, 50);
        lv_obj_add_style(valueContainer, &g_style_lightgray_rounded_box, 0);

        _valueLabel = lv_label_create(valueContainer);
        lv_obj_add_style(_valueLabel, &g_style_value_dark, 0);
        lv_obj_center(_valueLabel);

        // Create slider
        _slider = lv_slider_create(parent);
        lv_obj_set_size(_slider, lv_pct(80), 20);
        lv_obj_align(_slider, LV_ALIGN_CENTER, 0, 20);
        lv_slider_set_range(_slider, _minValue, _maxValue);
        lv_slider_set_value(_slider, _currentValue, LV_ANIM_OFF);

        lv_obj_add_event_cb(_slider, [](lv_event_t* e) {
            auto screen = static_cast<SliderParameterScreen*>(lv_event_get_user_data(e));
            lv_obj_t* slider = static_cast<lv_obj_t*>(lv_event_get_target(e));
            int value = lv_slider_get_value(slider);
            screen->_currentValue = value;
            screen->UpdateDisplay();
            if (screen->_onValueChanged) screen->_onValueChanged(value);
        }, LV_EVENT_VALUE_CHANGED, this);

        UpdateDisplay();
    }

    bool HandleParameterAction(ButtonID id, ButtonState state) override
    {
        if (state != ButtonState::Released) return false;

        switch (id)
        {
        case ButtonID::BUTTON_3: // Increase
            IncreaseValue();
            return true;
        case ButtonID::BUTTON_2: // Fine adjustment down
            DecreaseValue(_step);
            return true;
        case ButtonID::BUTTON_6: // Fine adjustment up
            IncreaseValue(_step);
            return true;
        default:
            return false;
        }
    }

    bool HandleKnob(int8_t delta) override
    {
        if (delta > 0) {
            IncreaseValue();
        } else if (delta < 0) {
            DecreaseValue();
        }
        return true;
    }

private:
    void DecreaseValue(int amount = -1)
    {
        if (amount == -1) amount = _step * 10; // Coarse adjustment
        _currentValue = std::max(_minValue, _currentValue - amount);
        UpdateDisplay();
        if (_slider) lv_slider_set_value(_slider, _currentValue, LV_ANIM_ON);
        if (_onValueChanged) _onValueChanged(_currentValue);
    }

    void IncreaseValue(int amount = -1)
    {
        if (amount == -1) amount = _step * 10; // Coarse adjustment
        _currentValue = std::min(_maxValue, _currentValue + amount);
        UpdateDisplay();
        if (_slider) lv_slider_set_value(_slider, _currentValue, LV_ANIM_ON);
        if (_onValueChanged) _onValueChanged(_currentValue);
    }

    void UpdateDisplay()
    {
        if (_valueLabel)
        {
            char buffer[32];
            if (_unit.empty())
                snprintf(buffer, sizeof(buffer), "%d", _currentValue);
            else
                snprintf(buffer, sizeof(buffer), "%d %s", _currentValue, _unit.c_str());
            lv_label_set_text(_valueLabel, buffer);
        }
    }
};
