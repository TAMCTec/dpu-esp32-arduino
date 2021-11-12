#include "lv_helper.h"

// Set Display Rotation
// 0: Portrait, 1: Landscape, 2: Portrait Reverse, 3: Landscape Reverse
#define DISPLAY_ROTATION 0

#define OPERATER_PLUS = 0
#define OPERATER_MINUS = 1
#define OPERATER_MULTIPLY = 2
#define OPERATER_DIVIDE = 3
#define OPERATER_PERCENT = 4


lv_obj_t *textarea_obj;
int currentOperate = -1;

void draw_main();

static const char * btnm_map[] = {
  "AC", "2", "%", "/", "\n",
  "7", "8", "9", "*", "\n",
  "4", "5", "6", "-", "\n",
  "1", "2", "3", "+", "\n",
  "0", ".", "=", "", "\n"
};

void setup(void) {
  Serial.begin(115200);
  Serial.println("Calculator Starts!");
  lh_init(DISPLAY_ROTATION);
  draw_main();
}

void loop() {
  lh_handler();
}

void draw_main() {
  textarea_obj = lv_textarea_create(lv_scr_act());
  lv_textarea_set_one_line(textarea_obj, true);
  lv_textarea_set_cursor_click_pos(textarea_obj, true);
  lv_obj_set_width(textarea_obj, 240);
  lv_obj_set_align(textarea_obj, LV_ALIGN_TOP_MID);
  lv_obj_set_pos(textarea_obj, 0, 0);
  // lv_obj_add_event_cb(textarea_obj, ta_event_cb, LV_EVENT_ALL, NULL);

  lv_obj_t * btnm1 = lv_btnmatrix_create(lv_scr_act());
  lv_btnmatrix_set_map(btnm1, btnm_map);
  lv_btnmatrix_set_btn_width(btnm1, 16, 2);        /*Make "Action1" twice as wide as "Action2"*/
  lv_btnmatrix_set_btn_ctrl(btnm1, 1, LV_BTNMATRIX_CTRL_CHECKABLE);
  lv_obj_align(btnm1, LV_ALIGN_CENTER, 0, 0);
  lv_obj_add_event_cb(btnm1, event_handler, LV_EVENT_ALL, NULL);
}

static void event_handler(lv_event_t * e) {
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * obj = (lv_obj_t *)lv_event_get_target(e);
  if(code == LV_EVENT_VALUE_CHANGED) {
    uint32_t id = lv_btnmatrix_get_selected_btn(obj);
    const char * txt = lv_btnmatrix_get_btn_text(obj, id);

    Serial.printf("%s was pressed\n", txt);
  }
  if(code == LV_EVENT_DRAW_PART_BEGIN) {
    lv_obj_draw_part_dsc_t * dsc = (lv_obj_draw_part_dsc_t *)lv_event_get_param(e);

    /*Change the right column color*/
    if(dsc->id % 4 == 3 || dsc->id == 18) {
      dsc->rect_dsc->radius = 0;
      if(lv_btnmatrix_get_selected_btn(obj) == dsc->id)  dsc->rect_dsc->bg_color = lv_palette_darken(LV_PALETTE_BLUE, 3);
      else dsc->rect_dsc->bg_color = lv_palette_main(LV_PALETTE_BLUE);
      dsc->label_dsc->color = lv_color_white();
    }
    /*Change the top left three button color to gray*/
    else if(dsc->id == 2) {
        dsc->rect_dsc->radius = LV_RADIUS_CIRCLE;
      if(lv_btnmatrix_get_selected_btn(obj) == dsc->id)  dsc->rect_dsc->bg_color = lv_palette_darken(LV_PALETTE_RED, 3);
      else dsc->rect_dsc->bg_color = lv_palette_main(LV_PALETTE_RED);

      dsc->label_dsc->color = lv_color_white();
    }
    else if(dsc->id == 3) {
      dsc->label_dsc->opa = LV_OPA_TRANSP; /*Hide the text if any*/
    }
  }
}
