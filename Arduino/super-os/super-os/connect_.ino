

// تابع تغییر وضعیت اتصال
void update_connection_status(bool connected) {
  if (connected) {

  } else {
  }
}

void stm32_connect() {
  btn[1] = lv_btn_create(menu_container);
  lv_obj_set_size(btn[1], 80, 80);  // اندازه دکمه‌ها
  img[1] = lv_img_create(btn[1]);
  String img_path = "/desktop_pic/stm32_connect.bmp";  // مسیر تصویرها
  uint16_t *img_data = loadImageFromSD(img_path.c_str());
  Serial.println(img_path);
  if (img_data != nullptr) {


    img_dsc[1].data = (uint8_t *)img_data;
    img_dsc[1].data_size = img_size;
    img_dsc[1].header.cf = LV_IMG_CF_TRUE_COLOR;  // فرمت تصویر
    img_dsc[1].header.w = 80;                     // عرض تصویر
    img_dsc[1].header.h = 80;                     // ارتفاع تصویر
    lv_obj_align(img[1], LV_ALIGN_CENTER, 0, 0);
    // تنظیم تصویر در LVGL
    lv_img_set_src(img[1], &img_dsc[1]);
  } else {
    Serial.println("Failed to load image");
  }
  lv_obj_set_pos(btn[1], 390, 25);  // فاصله بین آیتم‌ها
  //lv_obj_add_event_cb(btn[1], event_handler_button1_desktop, LV_EVENT_CLICKED, NULL);
  // تنظیم وضعیت اتصال اولیه
  update_connection_status(false);  // قطع
}

void stm32_connect_check() {
  // مثال تغییر وضعیت اتصال
  static bool is_connected = false;
  static unsigned long last_change = 0;
  if (millis() - last_change > 3000) {  // تغییر وضعیت هر 3 ثانیه
    is_connected = !is_connected;
    update_connection_status(is_connected);
    last_change = millis();
  }
}