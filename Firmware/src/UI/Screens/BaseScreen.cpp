#include "BaseScreen.h"

#include "../Theme.h"

void BaseScreen::Create(lv_obj_t* parent)
{
    _screenRoot = parent;

    lv_obj_remove_flag(_screenRoot, LV_OBJ_FLAG_SCROLLABLE);

    // Set a basic background style for the root
    lv_obj_set_style_bg_color(parent, MEDIUM_GRAY_BACKGROUND, 0);
    lv_obj_set_style_bg_opa(parent, LV_OPA_COVER, 0);
    lv_obj_set_style_pad_all(parent, 0, 0);
    lv_obj_set_style_border_width(parent, 0, 0);

    // Use a grid layout for the base structure
    // Columns: [Left Side] [Center Area] [Right Side]
    // Rows:    [Top Bar] [Middle Content] [Bottom Bar]

    static int32_t col_dsc[] = {60, LV_GRID_FR(1), 60, LV_GRID_TEMPLATE_LAST};
    static int32_t row_dsc[] = {LV_GRID_CONTENT, LV_GRID_FR(1), LV_GRID_CONTENT, LV_GRID_TEMPLATE_LAST};

    lv_obj_set_layout(parent, LV_LAYOUT_GRID);
    lv_obj_set_grid_dsc_array(parent, col_dsc, row_dsc);

    // --- Create Containers ---

    // Top Container: Spans all 3 columns in the first row
    _topContainer = lv_obj_create(parent);
    lv_obj_remove_style_all(_topContainer);
    lv_obj_remove_flag(_topContainer, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_width(_topContainer, lv_pct(100));
    lv_obj_set_grid_cell(_topContainer, LV_GRID_ALIGN_STRETCH, 0, 3, LV_GRID_ALIGN_STRETCH, 0, 1);
    lv_obj_set_layout(_topContainer, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(_topContainer, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(_topContainer, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_height(_topContainer, LV_SIZE_CONTENT);

    // Left Container: Middle row, first column
    _leftContainer = lv_obj_create(parent);
    lv_obj_remove_style_all(_leftContainer);
    lv_obj_remove_flag(_leftContainer, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_grid_cell(_leftContainer, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 1, 1);
    lv_obj_set_layout(_leftContainer, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(_leftContainer, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(_leftContainer, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER);

    // Center Container: Middle row, second column
    _centerContainer = lv_obj_create(parent);
    lv_obj_remove_style_all(_centerContainer);
    lv_obj_remove_flag(_centerContainer, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_width(_centerContainer, lv_pct(100));
    lv_obj_set_grid_cell(_centerContainer, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, 1, 1);
    lv_obj_set_layout(_centerContainer, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(_centerContainer, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(_centerContainer, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    // Right Container: Middle row, third column
    _rightContainer = lv_obj_create(parent);
    lv_obj_remove_style_all(_rightContainer);
    lv_obj_remove_flag(_rightContainer, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_grid_cell(_rightContainer, LV_GRID_ALIGN_STRETCH, 2, 1, LV_GRID_ALIGN_STRETCH, 1, 1);
    lv_obj_set_layout(_rightContainer, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(_rightContainer, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(_rightContainer, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER);

    // Bottom Container: Spans all 3 columns in the last row
    _bottomContainer = lv_obj_create(parent);
    lv_obj_remove_style_all(_bottomContainer);
    lv_obj_remove_flag(_bottomContainer, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_width(_bottomContainer, lv_pct(100));
    lv_obj_set_grid_cell(_bottomContainer, LV_GRID_ALIGN_STRETCH, 0, 3, LV_GRID_ALIGN_STRETCH, 2, 1);
    lv_obj_set_layout(_bottomContainer, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(_bottomContainer, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(_bottomContainer, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_height(_bottomContainer, LV_SIZE_CONTENT);
}
