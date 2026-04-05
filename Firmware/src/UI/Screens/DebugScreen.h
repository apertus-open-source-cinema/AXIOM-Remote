#pragma once

#include "BaseScreen.h"

class DebugScreen : public BaseScreen
{
public:
    explicit DebugScreen(AppContext* context);
    void Create(lv_obj_t* parent) override;

private:
    void UpdateLabels();
    lv_obj_t* _encoderPosLabel;
    lv_obj_t* _encoderBtnLabel;
};
