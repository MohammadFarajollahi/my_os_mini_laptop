
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
    stm32_ready = 0;
    //Network_ready = 0;
  }
  battery_read();
  clock_read();
  stm32_connect_check();
  stm32();
}


void event_handler_button1_desktop(lv_event_t *e) {
  Serial.println("button1");
  count_files = 0;
  btn_count = 0;
  //***files list***
  // listDir(SD, "/desktop_pic", 0);
  // Serial.print("file_count:");
  // Serial.println(count_files);
  for (int i = 1; i <= 6; i++) {
    free((void *)img_dsc[i].data);  //main free cach
  }
  lv_obj_del(menu_container);
  lv_obj_clean(lv_scr_act());
  menu_select = "mainMenu";
  change_menu = 1;
  Serial.println("desktop delete");
}


void ServerButton(lv_event_t *e) {
  Serial.println("ServerButton");
  count_files = 0;
  btn_count = 0;
  //***files list***
  // listDir(SD, "/desktop_pic", 0);
  // Serial.print("file_count:");
  // Serial.println(count_files);
  for (int i = 1; i <= 6; i++) {
    free((void *)img_dsc[i].data);  //main free cach
  }
  lv_obj_del(menu_container);
  lv_obj_clean(lv_scr_act());
  menu_select = "server";
  change_menu = 1;
  Serial.println("desktop delete");
}

void create_desktop_icon() {
  SerialState = 1;
  btn_count = 0;
  menu_container = lv_obj_create(lv_scr_act());
  lv_obj_set_size(menu_container, 480, 319);                        // اندازه کانتینر
  lv_obj_align(menu_container, LV_ALIGN_CENTER, 0, 0);              // مرکز قرار دادن
  lv_obj_set_scroll_dir(menu_container, LV_DIR_VER);                // تنظیم اسکرول به سمت چپ و راست
  lv_obj_set_scroll_snap_x(menu_container, LV_SCROLL_SNAP_CENTER);  // اسکرول به سمت مرکز
  set_button(1, "Menu", 80, 80, 10, 200, 26, 264, "/desktop_pic/menu.bmp");
  set_button(2, "Server", 80, 80, 120, 200, 130, 264, "/menu_pic/server.bmp");
  set_label(3, 80, 80, 310, -16, "/desktop_pic/stm32_disconnect.bmp");
  set_label(4, 80, 80, 310, -16, "/desktop_pic/stm32_connect.bmp");
  set_label(5, 80, 80, 265, -20, "/desktop_pic/anten.bmp");
  set_label(6, 80, 80, 265, -20, "/desktop_pic/anten2.bmp");

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
  //terminalp
  lcdShow = 1;
  lv_style_init(&styleTerminal);
  lv_style_set_text_font(&styleTerminal, &lv_font_unscii_8);  // تنظیم فونت
  lv_style_set_bg_color(&styleTerminal, lv_color_hex(0x000000));
  lv_style_set_text_color(&styleTerminal, lv_color_hex(0x00FF00));

  terminal = lv_textarea_create(menu_container);
  lv_obj_add_style(terminal, &styleTerminal, 0);
  lv_obj_set_size(terminal, 240, 200);  // سایز ترمینال
  lv_obj_set_pos(terminal, 210, 80);
  lv_textarea_set_text(terminal, "");                 // Clear terminal
  lv_textarea_set_text(terminal, "ٌwait for Data\n");  // متن اولیه

  //*****read Auto*****
  readSdCard("/AutoServerChek.txt");
  lcd_show2(SdCardMessage);
  Serial.println(SdCardMessage);
  Serial.println(fileContent);
  if (fileContent != "") lcd_show2(fileContent);
  if (fileContent == "Auto on") {
    AutoServerChek = 1;
    lcd_show2("auto Server is on");
    Serial.println("auto Server is on");
    loadFromFile();
    for (int i = 0; i < 50; i++) {
      NumberTest[i] = "";
    }

    lcd_show2("input Numbers-->");
    FoundNumbers = 0;
    int NumtestCount = 0;
    for (int i = 0; i < phoneCount; i++) {
      if (phoneNumbers[i].checked) {
        NumberTest[NumtestCount] = phoneNumbers[i].number;
       // String sss = "Number" + String(i) + ":" + phoneNumbers[i].number;
       String sss = "Number" + String(NumtestCount) + ":" + NumberTest[NumtestCount];
        lcd_show2(sss);
        ++NumtestCount;
        ++FoundNumbers;
      }
    }
    String CCc = "Number Counts:" + String(FoundNumbers);
    lcd_show2(CCc);
  }
  if (fileContent == "Auto off") {
    AutoServerChek = 0;
    lcd_show2("auto Server is off");
    Serial.println("auto Server is off");
  }
}
