
void home_screen() {
  //input_touch();
  if (change_menu == 1) {
    change_menu = 0;
    Serial.println("desktop start ...");
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x000000), 0);
    create_desktop_icon();
    create_battery_shape();  // ایجاد نمایشگر باتری
    clock_();
    update_connection_status(false);  // قطع
    main_timer = 1;
    main_sec = 0;
    batt_sec = 0;
  }
  battery_read();
  clock_read();
  stm32_connect_check();
}


void event_handler_button1_desktop(lv_event_t *e) {
  Serial.println("button1");
  count_files = 0;
  btn_count = 0;
  //***files list***
  listDir(SD, "/desktop_pic", 0);
  Serial.print("file_count:");
  Serial.println(count_files);
  for (int i = 0; i <= count_files - 1; i++) {
    free((void *)img_dsc[i].data);  //main free cach
  }
  lv_obj_del(menu_container);
  lv_obj_clean(lv_scr_act());
  menu_select = "mainMenu";
  change_menu = 1;
  Serial.println("desktop delete");
}


void create_desktop_icon() {
  btn_count = 0;
  menu_container = lv_obj_create(lv_scr_act());
  lv_obj_set_size(menu_container, 480, 319);                        // اندازه کانتینر
  lv_obj_align(menu_container, LV_ALIGN_CENTER, 0, 0);              // مرکز قرار دادن
  lv_obj_set_scroll_dir(menu_container, LV_DIR_VER);                // تنظیم اسکرول به سمت چپ و راست
  lv_obj_set_scroll_snap_x(menu_container, LV_SCROLL_SNAP_CENTER);  // اسکرول به سمت مرکز
  set_button("Menu", 80, 80, 10, 200, 26, 264, "/desktop_pic/menu.bmp");
  set_label(80, 80, 310, -20, "/desktop_pic/stm32_disconnect.bmp");
  set_label(80, 80, 310, -20, "/desktop_pic/stm32_connect.bmp");
}
