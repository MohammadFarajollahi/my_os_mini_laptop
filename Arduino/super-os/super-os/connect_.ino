

// تابع تغییر وضعیت اتصال
void update_connection_status(bool connected) {
  if (connected) {
    //btn_count = 2;
    lv_obj_del(label_create[4]);
    free((void *)img_dsc[4].data);  //main free cach
    //free((void *)img_dsc[2].data);  //main free cach
    set_label(4, 80, 80, 310, -16, "/desktop_pic/stm32_connect.bmp");
  } else {
    //btn_count = 2;
    lv_obj_del(label_create[3]);
    free((void *)img_dsc[3].data);  //main free cach
    //free((void *)img_dsc[2].data);  //main free cach
    set_label(3, 80, 80, 310, -16, "/desktop_pic/stm32_disconnect.bmp");
  }


  if (Network_ready == 1) {
    lv_obj_del(label_create[5]);
    free((void *)img_dsc[5].data);  //main free cach
    set_label(5, 80, 80, 265, -20, "/desktop_pic/anten.bmp");
  }
  if (Network_ready == 0) {
    lv_obj_del(label_create[6]);
    free((void *)img_dsc[6].data);  //main free cach
    set_label(6, 80, 80, 265, -20, "/desktop_pic/anten2.bmp");
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