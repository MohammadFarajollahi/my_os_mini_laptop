

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
  bool is_connected = false;
  // مثال تغییر وضعیت اتصال
  if (check_stm32_count == 0) {
    check_stm32_count = 1;
    //is_connected = !is_connected;
    if (stm32_ready == 1) is_connected = true;
    if (stm32_ready == 0) is_connected = false;
    update_connection_status(is_connected);
  }
}