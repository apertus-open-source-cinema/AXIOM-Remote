#pragma once

#include "ParameterScreen.h"
#include <vector>
#include <string>

class ListParameterScreen : public ParameterScreen
{
private:
    std::vector<std::string> _options;
    int _currentIndex = 0;
    lv_obj_t* _list = nullptr;
    lv_obj_t* _currentValueLabel = nullptr;
    bool _wrapAround = true;

public:
    ListParameterScreen(AppContext* context, const std::string& title, const std::string& parameterName)
        : ParameterScreen(context, title, parameterName)
    {
    }

    void SetOptions(const std::vector<std::string>& options)
    {
        _options = options;
        if (_currentIndex >= (int)_options.size()) _currentIndex = 0;
    }

    void SetWrapAround(bool wrap) { _wrapAround = wrap; }

    void SetCurrentValue(int value) override
    {
        if (value >= 0 && value < (int)_options.size())
        {
            _currentIndex = value;
            UpdateDisplay();
        }
    }

    int GetCurrentValue() const override { return _currentIndex; }

    void CreateParameterUI(lv_obj_t* parent) override
    {
        // Create current value display
        lv_obj_t* valueContainer = lv_obj_create(parent);
        lv_obj_remove_style_all(valueContainer);
        lv_obj_set_size(valueContainer, lv_pct(70), 50);
        lv_obj_align(valueContainer, LV_ALIGN_CENTER, 0, 0);
        lv_obj_add_style(valueContainer, &g_style_lightgray_rounded_box, 0);

        _currentValueLabel = lv_label_create(valueContainer);
        lv_obj_add_style(_currentValueLabel, &g_style_value_dark, 0);
        lv_label_set_text(_currentValueLabel, _options.empty() ? "No options" : _options[_currentIndex].c_str());
        lv_obj_center(_currentValueLabel);

        UpdateDisplay();
    }

    bool HandleKnob(int8_t delta) override
    {
        if (delta > 0) {
            for (int i = 0; i < delta; ++i) NextOption();
        } else if (delta < 0) {
            for (int i = 0; i < -delta; ++i) PreviousOption();
        }
        return true;
    }

    bool HandleParameterAction(ButtonID id, ButtonState state) override
    {
        if (state != ButtonState::Released) return false;

        switch (id)
        {
        case ButtonID::BUTTON_1: // Previous
        case ButtonID::BUTTON_7: // Left side
            PreviousOption();
            return true;
        case ButtonID::BUTTON_3: // Next
        case ButtonID::BUTTON_9: // Right side
            NextOption();
            return true;
        default:
            return false;
        }
    }

private:
    void PreviousOption()
    {
        if (_options.empty()) return;

        int newIndex = _currentIndex - 1;
        if (_wrapAround) {
            newIndex = (newIndex + _options.size()) % _options.size();
        } else {
            if (newIndex < 0) newIndex = 0;
        }
        _currentIndex = newIndex;

        UpdateDisplay();
        if (_onValueChanged) _onValueChanged(_currentIndex);
    }

    void NextOption()
    {
        if (_options.empty()) return;
        
        int newIndex = _currentIndex + 1;
        if (_wrapAround) {
            newIndex = newIndex % _options.size();
        } else {
            if (newIndex >= (int)_options.size()) newIndex = _options.size() - 1;
        }
        _currentIndex = newIndex;

        UpdateDisplay();
        if (_onValueChanged) _onValueChanged(_currentIndex);
    }

    void UpdateDisplay()
    {
        if (_currentValueLabel && !_options.empty())
        {
            lv_label_set_text(_currentValueLabel, _options[_currentIndex].c_str());
        }
    }
};