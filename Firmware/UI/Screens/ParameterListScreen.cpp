#include "ParameterListScreen.h"

ParameterListScreen::ParameterListScreen(IUSBDevice* usbDevice) :
    IScreen(usbDevice), _header("Parameter Menu"), _cancelButton("Cancel"), _setButton("Set"),
    _backgroundColor((uint16_t)Color565::White), _textColor((uint16_t)Color565::Black),
    _highlightColor((uint16_t)Color565::AXIOM_Orange), _highlightTextColor((uint16_t)Color565::White),
    _backgroundPressedColor(utils::RGB565(0, 128, 255)), _textPressedColor((uint16_t)Color565::White),
    _highlightIndex(0), _pressedIndex(-1), _previousOptionIndex(0), _optionLineHeight(35)
{
    //_cancelButton.SetHandler(&CancelButtonHandler);
    _bottomButtonBar.SetButton(ButtonPosition::Left, &_cancelButton);

    // This is the primary button in this menu
    //_setButton.SetHandler(&SetButtonHandler);
    _setButton.SetBackgroundColor((uint16_t)Color565::AXIOM_Orange);
    _bottomButtonBar.SetButton(ButtonPosition::Right, &_setButton);
}

void ParameterListScreen::SetOptions(const char* optionlabels[], uint8_t optioncount)
{
    _optionCount = optioncount;
    for (int8_t i = 0; i < optioncount; i++)
    {
        _optionLabels[i] = optionlabels[i];
        //_parameterMenuItem[i].SetHorizontalTextMargin(_horizontalTextMargin);
    }
}

void ParameterListScreen::SetSetButtonPressed(bool pressed)
{
    _setButton.SetHighlighted(pressed);
}

void ParameterListScreen::SetCancelButtonPressed(bool pressed)
{
    _cancelButton.SetHighlighted(pressed);
}

void ParameterListScreen::SetHighlighted(uint8_t highlightindex)
{
    if (highlightindex < _optionCount)
    {
        _highlightIndex = highlightindex;
    }
}

void ParameterListScreen::SetPressed(uint8_t pressedindex)
{
    if (pressedindex < _optionCount)
    {
        _pressedIndex = pressedindex;
    }
}

void ParameterListScreen::UnpressAll()
{
    _pressedIndex = -1;
}

void ParameterListScreen::UpdateChoice(uint8_t choiceindex)
{
    if (choiceindex < _optionCount)
    {
        _previousOptionIndex = choiceindex;
    }
}

uint8_t ParameterListScreen::GetHighlightIndex()
{
    return _highlightIndex;
}

void ParameterListScreen::SetHeader(const char* value)
{
    _header = value;
}

const char* ParameterListScreen::GetHeader() const
{
    return _header;
}

void ParameterListScreen::Draw(IPainter* painter)
{
    painter->Fill(_backgroundColor);
    DrawHeader(painter);

    // draw options
    painter->SetFont(Font::FreeSans12pt7b);
    uint8_t heightcenter = GlobalSettings::LCDHeight / 2 - _optionLineHeight / 2;
    uint8_t fontcenter = _optionLineHeight / 2 + painter->GetCurrentFontHeight() / 2;
    for (int8_t i = 0; i < _optionCount; i++)
    {
        if (_pressedIndex == i)
        {
            painter->DrawFillRectangle(20, heightcenter, GlobalSettings::LCDWidth - 40, _optionLineHeight,
                                       _backgroundPressedColor);
            painter->DrawText(30, heightcenter + fontcenter, _optionLabels[i], _textPressedColor,
                              TextAlign::TEXT_ALIGN_LEFT, GlobalSettings::LCDWidth);
        } else if (_highlightIndex == i)
        {
            painter->DrawFillRectangle(20, heightcenter, GlobalSettings::LCDWidth - 40, _optionLineHeight,
                                       _highlightColor);
            painter->DrawText(30, heightcenter + fontcenter, _optionLabels[i], _highlightTextColor,
                              TextAlign::TEXT_ALIGN_LEFT, GlobalSettings::LCDWidth);
        } else if ((i - _highlightIndex <= 2) && (i - _highlightIndex >= -2))
        {
            painter->DrawText(30, heightcenter + (i - _highlightIndex) * _optionLineHeight + fontcenter,
                              _optionLabels[i], _textColor, TextAlign::TEXT_ALIGN_LEFT, GlobalSettings::LCDWidth);
        }

        // draw current (old) value indicator circle
        if (_previousOptionIndex == i)
        {
            if (_highlightIndex == i)
            {
                painter->DrawFillCircle(25, heightcenter + (i - _highlightIndex) * _optionLineHeight + fontcenter - 7,
                                        3, _highlightTextColor);
            } else
            {
                painter->DrawFillCircle(25, heightcenter + (i - _highlightIndex) * _optionLineHeight + fontcenter - 7,
                                        3, _textColor);
            }
        }
    }

    DrawBottomButtonBar(painter);
}

static void CancelButtonHandler(void* sender);
static void SetButtonHandler(void* sender);

void ParameterListScreen::DrawHeader(IPainter* painter)
{
    // header background
    painter->DrawFillRectangle(0, 0, GlobalSettings::LCDWidth, 30, (uint16_t)Color565::Black);

    //  header title
    painter->SetFont(Font::FreeSans12pt7b);
    painter->DrawText(0, 22, _header, (uint16_t)Color565::White, TextAlign::TEXT_ALIGN_CENTER,
                      GlobalSettings::LCDWidth);

    // header separation lines
    painter->DrawLine(0, 29, GlobalSettings::LCDWidth - 1, 29, (uint16_t)Color565::AXIOM_Orange);
}
