
lv_obj_t *time_label;
lv_obj_t *day_label;
lv_obj_t *date_label;

void update_time(const char *time, const char *day, const char *date) {
  lv_label_set_text(time_label, time);  // ساعت
  lv_label_set_text(day_label, day);    // روز هفته
  lv_label_set_text(date_label, date);  // تاریخ
}



void clock_() {

  // ایجاد پس‌زمینه
  lv_obj_t *screen = lv_scr_act();
  lv_obj_set_style_bg_color(screen, lv_color_hex(0xF2F2F2), 0);

  // ایجاد کنتینر برای لیبل‌ها
  lv_obj_t *info_container = lv_obj_create(screen);
  lv_obj_set_size(info_container, 190, 90);
  lv_obj_align(info_container, LV_ALIGN_TOP_LEFT, 10, 10);
  lv_obj_set_style_bg_color(info_container, lv_color_hex(0xF2F2F2), 0);
  lv_obj_set_style_border_width(info_container, 2, 0);
  lv_obj_set_style_border_color(info_container, lv_color_hex(0xF2F2F2), 0);
  lv_obj_clear_flag(info_container, LV_OBJ_FLAG_SCROLLABLE);  // غیرفعال کردن اسکرول

  // لیبل ساعت
  time_label = lv_label_create(info_container);
  lv_obj_set_style_text_font(time_label, &lv_font_unscii_16, 0);
  lv_obj_set_style_text_color(time_label, lv_color_hex(0x000000), 0);
  lv_obj_align(time_label, LV_ALIGN_TOP_LEFT, 10, 5);
  lv_label_set_text(time_label, "10:45:30");

  // لیبل روز
  day_label = lv_label_create(info_container);
  lv_obj_set_style_text_font(day_label, &lv_font_unscii_8, 0);
  lv_obj_set_style_text_color(day_label, lv_color_hex(0xAAAAAA), 0);
  lv_obj_align(day_label, LV_ALIGN_TOP_LEFT, 10, 35);
  lv_label_set_text(day_label, "Tuesday");

  // لیبل تاریخ
  date_label = lv_label_create(info_container);
  lv_obj_set_style_text_font(date_label, &lv_font_unscii_8, 0);
  lv_obj_set_style_text_color(date_label, lv_color_hex(0xAAAAAA), 0);
  lv_obj_align(date_label, LV_ALIGN_TOP_LEFT, 10, 50);
  lv_label_set_text(date_label, "2024-12-04");
}


//////////////////////////////////////////////////////////////////////////////////////////////
void clock_read() {
  if (main_sec == 1) {
    main_sec = 0;
    // update_time(clock_hour, clock_minute, clock_second); //  update_time(10, seconds / 60, seconds % 60); // ساعت 10:MM:SS
    static int seconds = 0;
    seconds++;
    char time[9], day[10], date[11];
    sprintf(time, "%02d:%02d:%02d", clock_hour, clock_minute, clock_second);
    sprintf(day, "Wednesday");    // روز ثابت برای مثال
    sprintf(date, "2024-12-04");  // تاریخ ثابت برای مثال
    update_time(time, day, date);
  }
}


