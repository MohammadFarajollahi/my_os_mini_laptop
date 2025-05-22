
void ClockSetting() {
  if (change_menu == 1) {
    change_menu = 0;
    Serial.println("Clock Setting");
    CreatClockSetting();
  }
}



lv_obj_t *ta_year, *ta_month, *ta_day;
lv_obj_t *ta_hour, *ta_minute, *ta_second;
lv_obj_t *dd_weekday;

const char *weekdays[] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

void send_time_to_stm32(lv_event_t *e) {
  String msg = "clockSet:";

  msg += String(lv_textarea_get_text(ta_year)) + ",";
  msg += String(lv_textarea_get_text(ta_month)) + ",";
  msg += String(lv_textarea_get_text(ta_day)) + ",";
  msg += String(lv_textarea_get_text(ta_hour)) + ",";
  msg += String(lv_textarea_get_text(ta_minute)) + ",";
  msg += String(lv_textarea_get_text(ta_second)) + ",";
  msg += String(lv_dropdown_get_selected(dd_weekday));

  stm32_serial.println(msg);
  Serial.println("🔁 ارسال شد: " + msg);
}

void CreatClockSetting() {
  // سال، ماه، روز
  ta_year = lv_textarea_create(lv_scr_act());
  lv_textarea_set_placeholder_text(ta_year, "Year");
  lv_obj_align(ta_year, LV_ALIGN_TOP_LEFT, 10, 10);
  lv_textarea_set_max_length(ta_year, 4);

  ta_month = lv_textarea_create(lv_scr_act());
  lv_textarea_set_placeholder_text(ta_month, "Month");
  lv_obj_align_to(ta_month, ta_year, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
  lv_textarea_set_max_length(ta_month, 2);

  ta_day = lv_textarea_create(lv_scr_act());
  lv_textarea_set_placeholder_text(ta_day, "Day");
  lv_obj_align_to(ta_day, ta_month, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
  lv_textarea_set_max_length(ta_day, 2);

  // ساعت، دقیقه، ثانیه
  ta_hour = lv_textarea_create(lv_scr_act());
  lv_textarea_set_placeholder_text(ta_hour, "Hour");
  lv_obj_align(ta_hour, LV_ALIGN_LEFT_MID, 10, 0);
  lv_textarea_set_max_length(ta_hour, 2);

  ta_minute = lv_textarea_create(lv_scr_act());
  lv_textarea_set_placeholder_text(ta_minute, "Minute");
  lv_obj_align_to(ta_minute, ta_hour, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
  lv_textarea_set_max_length(ta_minute, 2);

  ta_second = lv_textarea_create(lv_scr_act());
  lv_textarea_set_placeholder_text(ta_second, "Second");
  lv_obj_align_to(ta_minute, ta_minute, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
  lv_textarea_set_max_length(ta_second, 2);

  // انتخاب روز هفته
  dd_weekday = lv_dropdown_create(lv_scr_act());
  lv_dropdown_set_options_static(dd_weekday,
                                 "Saturday\n"
                                 "Sunday\n"
                                 "Monday\n"
                                 "Tuesday\n"
                                 "Wednesday\n"
                                 "Thursday\n"
                                 "Friday");
  lv_obj_align(dd_weekday, LV_ALIGN_BOTTOM_LEFT, 10, -30);

  // دکمه ارسال
  lv_obj_t *btn_send = lv_btn_create(lv_scr_act());
  lv_obj_align(btn_send, LV_ALIGN_BOTTOM_RIGHT, -10, -10);
  lv_obj_t *label = lv_label_create(btn_send);
  lv_label_set_text(label, "Send");
  lv_obj_center(label);
  lv_obj_add_event_cb(btn_send, send_time_to_stm32, LV_EVENT_CLICKED, NULL);
}