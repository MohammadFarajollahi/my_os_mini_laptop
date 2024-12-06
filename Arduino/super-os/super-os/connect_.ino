

// تابع تغییر وضعیت اتصال
void update_connection_status(bool connected) {
  if (connected) {
    btn_count = 1;
    lv_obj_del(label_create[btn_count]);
    free((void *)img_dsc[1].data);  //main free cach
    //free((void *)img_dsc[2].data);  //main free cach
    set_label(80, 80, 310, -20, "/desktop_pic/stm32_connect.bmp");
  } else {
    btn_count = 1;
    lv_obj_del(label_create[btn_count]);
    free((void *)img_dsc[1].data);  //main free cach
    //free((void *)img_dsc[2].data);  //main free cach
    set_label(80, 80, 310, -20, "/desktop_pic/stm32_disconnect.bmp");
  }
}

void stm32_connect_check() {
  // مثال تغییر وضعیت اتصال
  static bool is_connected = false;
  static unsigned long last_change = 0;
  if (millis() - last_change > 500) {  // تغییر وضعیت هر 3 ثانیه
    is_connected = !is_connected;
    update_connection_status(is_connected);
    last_change = millis();
  }
}