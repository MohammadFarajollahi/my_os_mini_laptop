
void main_menu() {

  if (change_menu == 1) {
    change_menu = 0;
    tft.fillScreen(TFT_WHITE);
    tft.setCursor(10, 10);
    tft.setTextFont(1);
    tft.setTextSize(2);
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
    tft.println("Wait...");
    create_menu();
  }
  if (change_menu == 0) {
    lv_timer_handler(); /* let the GUI do its work */
    lv_task_handler();
  }
}

// lv_obj_clean(menu_container);
// lv_obj_clean(lv_scr_act());
void event_handler_button1(lv_event_t *e) {
  Serial.println("button1");
  cash_size();
  print_memory_usage();
}

void event_handler_button2(lv_event_t *e) {
  Serial.println("button2");
}

void event_handler_button3(lv_event_t *e) {
  Serial.println("button3");
}

void event_handler_button4(lv_event_t *e) {
  Serial.println("button4");
}

void event_handler_button5(lv_event_t *e) {
  Serial.println("button5");
}

void event_handler_button6(lv_event_t *e) {
  Serial.println("button6");
}


void event_handler_button7(lv_event_t *e) {
  Serial.println("button7");
}

void event_handler_button8(lv_event_t *e) {
  Serial.println("button8");
}

void event_handler_button9(lv_event_t *e) {
  Serial.println("button9");
}

void event_handler_button10(lv_event_t *e) {
  Serial.println("button10");
}

void event_handler_button11(lv_event_t *e) {
  Serial.println("button11");
}

void event_handler_button12(lv_event_t *e) {
  Serial.println("button12");
}

void event_handler_button13(lv_event_t *e) {
  Serial.println("button13");
}

void event_handler_button14(lv_event_t *e) {
  Serial.println("button14");
}

void event_handler_button15(lv_event_t *e) {
  Serial.println("button15");
}

void event_handler_button16(lv_event_t *e) {
  Serial.println("button16: Exit");

  // File myFolder;
  // myFolder = SD.open("/");  // نام پوشه مورد نظر را وارد کنید
  // if (myFolder) {
  //   myFolder.close();  // بسته شدن پوشه
  //   Serial.println("Folder closed.");
  // } else {
  //   Serial.println("Failed to open the folder.");
  // }
  count_files = 0;
  btn_count = 0;
  //***files list***
  listDir(SD, "/menu_pic", 0);
  Serial.print("file_count:");
  Serial.println(count_files);
  if (menu_container) {
    for (int i = 0; i <= count_files - 1; i++) {
      Serial.print("btn");
      Serial.print(String(i));
      Serial.println(" del");
      lv_img_cache_invalidate_src(&img_dsc[i]);
      lv_img_cache_invalidate_src(&img[i]);
      lv_img_cache_invalidate_src(&btn[i]);
      lv_obj_del(img[i]);
      img[i] = NULL;
      lv_obj_del(btn[i]);
      btn[i] = NULL;
      free((void *)img_dsc[i].data);  //
      //free((void *)img[i].data);      //

      lv_timer_handler(); /* let the GUI do its work */
      lv_task_handler();
    }


    lv_obj_clean(menu_container);

    lv_obj_del(menu_container);

    lv_img_cache_invalidate_src(img_dsc);
    lv_obj_clean(lv_scr_act());
    menu_select = 1;
    change_menu = 1;

    //lv_obj_add_flag(menu_screen, LV_OBJ_FLAG_HIDDEN); // مخفی کردن به‌جای حذف
  }
}


void event_handler_button17(lv_event_t *e) {
}


void create_menu() {
  // if (menu_container) {
  //   lv_obj_clean(menu_container);  // پاک کردن صفحه فعلی (اختیاری)
  //   lv_obj_del(menu_container);
  //   menu_container = NULL;
  //   lv_img_cache_invalidate_src(NULL);  // خالی کردن کش تصویر برای آزادسازی حافظه
  //   lv_refr_now(NULL);                  // به‌روزرسانی صفحه
  //   print_memory_usage();               // نمایش وضعیت حافظه برای دیباگ
  // }                                     // به‌روزرسانی صفحه


  count_files = 0;
  btn_count = 0;
  //***files list***
  listDir(SD, "/menu_pic", 0);
  Serial.print("file_count:");
  Serial.println(count_files);

  // if (menu_container) {
  //   //menu_container = lv_obj_create(lv_scr_act());
  //   // lv_obj_set_size(menu_container, 480, 320);                        // اندازه کانتینر
  //   lv_obj_align(menu_container, LV_ALIGN_CENTER, 0, 0);  // مرکز قرار دادن
  //   //lv_obj_set_scroll_dir(menu_container, LV_DIR_VER);                // تنظیم اسکرول به سمت چپ و راست
  //   //lv_obj_set_scroll_snap_x(menu_container, LV_SCROLL_SNAP_CENTER);  // اسکرول به سمت مرکز
  // } else {

  menu_container = lv_obj_create(lv_scr_act());
  lv_obj_set_size(menu_container, 480, 320);                        // اندازه کانتینر
  lv_obj_align(menu_container, LV_ALIGN_CENTER, 0, 0);              // مرکز قرار دادن
  lv_obj_set_scroll_dir(menu_container, LV_DIR_VER);                // تنظیم اسکرول به سمت چپ و راست
  lv_obj_set_scroll_snap_x(menu_container, LV_SCROLL_SNAP_CENTER);  // اسکرول به سمت مرکز
  //lv_obj_t *btn[count_files + 1];
  //lv_obj_t *img[count_files + 1];

  for (btn_count = 0; btn_count < count_files; btn_count++) {
    btn[btn_count] = lv_btn_create(menu_container);
    lv_obj_set_size(btn[btn_count], 79, 79);  // اندازه دکمه‌ها
                                              // ایجاد تصویر درون دکمه
    img[btn_count] = lv_img_create(btn[btn_count]);

    // بارگذاری تصویر از SD با LovyanGFX

    String img_path = "/menu_pic/" + files[btn_count];  // مسیر تصویرها
    //String img_path = "/1.bmp";  // مسیر تصویرها
    uint16_t *img_data = loadImageFromSD(img_path.c_str());
    Serial.println(img_path);
    if (img_data != nullptr) {

      // ساختن تصویر برای LVGL

      img_dsc[btn_count].data = (uint8_t *)img_data;
      img_dsc[btn_count].data_size = img_size;
      img_dsc[btn_count].header.cf = LV_IMG_CF_TRUE_COLOR;  // فرمت تصویر
      img_dsc[btn_count].header.w = 80;                     // عرض تصویر
      img_dsc[btn_count].header.h = 80;                     // ارتفاع تصویر
      lv_obj_align(img[btn_count], LV_ALIGN_CENTER, 0, 0);

      // img_dsc->data = imageData;
      // img_dsc->data_size = fileSize;
      // img_dsc->header.always_zero = 0;
      // img_dsc->header.w = 100;  // عرض تصویر
      // img_dsc->header.h = 50;   // ارتفاع تصویر
      // img_dsc->header.cf = LV_IMG_CF_TRUE_COLOR;

      //lv_img_cache_invalidate_src(img[btn_count]);

      // تنظیم تصویر در LVGL
      lv_img_set_src(img[btn_count], &img_dsc[btn_count]);
      // lv_img_cache_invalidate_src(&img_dsc[btn_count]);
      // lv_img_cache_invalidate_src(&img[btn_count]);
    } else {
      Serial.println("Failed to load image");
    }
    lv_obj_t *label = lv_label_create(menu_container);
    // موقعیت‌دهی دکمه‌ها به صورت افقی
    //////////////////////////////satr1/////////////////////////////////////////
    if (btn_count == 0) {
      lv_obj_set_pos(btn[btn_count], 10, 5);  // فاصله بین آیتم‌ها
      lv_obj_add_event_cb(btn[btn_count], event_handler_button1, LV_EVENT_CLICKED, NULL);
      lv_label_set_text(label, "Terminal");            // تنظیم متن
      lv_obj_align(label, LV_ALIGN_TOP_LEFT, 18, 69);  // قرار دادن متن در مرکز صفحه
    }

    if (btn_count == 1) {
      lv_obj_set_pos(btn[btn_count], 130, 5);  // فاصله بین آیتم‌ها
      lv_obj_add_event_cb(btn[btn_count], event_handler_button2, LV_EVENT_CLICKED, NULL);
      lv_label_set_text(label, "Serial monitor");       // تنظیم متن
      lv_obj_align(label, LV_ALIGN_TOP_LEFT, 123, 69);  // قرار دادن متن در مرکز صفحه
    }

    if (btn_count == 2) {
      lv_obj_set_pos(btn[btn_count], 250, 5);  // فاصله بین آیتم‌ها
      lv_obj_add_event_cb(btn[btn_count], event_handler_button3, LV_EVENT_CLICKED, NULL);
      lv_label_set_text(label, "PAVAN");                // تنظیم متن
      lv_obj_align(label, LV_ALIGN_TOP_LEFT, 265, 69);  // قرار دادن متن در مرکز صفحه
    }

    if (btn_count == 3) {
      lv_obj_set_pos(btn[btn_count], 360, 5);  // فاصله بین آیتم‌ها
      lv_obj_add_event_cb(btn[btn_count], event_handler_button4, LV_EVENT_CLICKED, NULL);
      lv_label_set_text(label, "Server");               // تنظیم متن
      lv_obj_align(label, LV_ALIGN_TOP_LEFT, 376, 69);  // قرار دادن متن در مرکز صفحه
    }
    ////////////////////////////////////////////satr2//////////////////////////////////////////////////
    if (btn_count == 4) {
      lv_obj_set_pos(btn[btn_count], 10, 100);  // فاصله بین آیتم‌ها
      lv_obj_add_event_cb(btn[btn_count], event_handler_button5, LV_EVENT_CLICKED, NULL);
      lv_label_set_text(label, "Chat");                 // تنظیم متن
      lv_obj_align(label, LV_ALIGN_TOP_LEFT, 31, 162);  // قرار دادن متن در مرکز صفحه
    }

    if (btn_count == 5) {
      lv_obj_set_pos(btn[btn_count], 130, 100);  // فاصله بین آیتم‌ها
      lv_obj_add_event_cb(btn[btn_count], event_handler_button6, LV_EVENT_CLICKED, NULL);
      lv_label_set_text(label, "Clock");                 // تنظیم متن
      lv_obj_align(label, LV_ALIGN_TOP_LEFT, 152, 162);  // قرار دادن متن در مرکز صفحه
    }

    if (btn_count == 6) {
      lv_obj_set_pos(btn[btn_count], 250, 100);  // فاصله بین آیتم‌ها
      lv_obj_add_event_cb(btn[btn_count], event_handler_button7, LV_EVENT_CLICKED, NULL);
      lv_label_set_text(label, "Music player");          // تنظیم متن
      lv_obj_align(label, LV_ALIGN_TOP_LEFT, 247, 162);  // قرار دادن متن در مرکز صفحه
    }

    if (btn_count == 7) {
      lv_obj_set_pos(btn[btn_count], 360, 100);  // فاصله بین آیتم‌ها
      lv_obj_add_event_cb(btn[btn_count], event_handler_button8, LV_EVENT_CLICKED, NULL);
      lv_label_set_text(label, "Setting");               // تنظیم متن
      lv_obj_align(label, LV_ALIGN_TOP_LEFT, 376, 162);  // قرار دادن متن در مرکز صفحه
    }
    ///////////////////////////////////satr3///////////////////////////////////////////////////////
    if (btn_count == 8) {
      lv_obj_set_pos(btn[btn_count], 10, 205);  // فاصله بین آیتم‌ها
      lv_obj_add_event_cb(btn[btn_count], event_handler_button9, LV_EVENT_CLICKED, NULL);
      lv_label_set_text(label, "Robot control");       // تنظیم متن
      lv_obj_align(label, LV_ALIGN_TOP_LEFT, 2, 262);  // قرار دادن متن در مرکز صفحه
    }

    if (btn_count == 9) {
      lv_obj_set_pos(btn[btn_count], 130, 200);  // فاصله بین آیتم‌ها
      lv_obj_add_event_cb(btn[btn_count], event_handler_button10, LV_EVENT_CLICKED, NULL);
      lv_label_set_text(label, "note");                  // تنظیم متن
      lv_obj_align(label, LV_ALIGN_TOP_LEFT, 150, 262);  // قرار دادن متن در مرکز صفحه
    }

    if (btn_count == 10) {
      lv_obj_set_pos(btn[btn_count], 250, 200);  // فاصله بین آیتم‌ها
      lv_obj_add_event_cb(btn[btn_count], event_handler_button11, LV_EVENT_CLICKED, NULL);
      lv_label_set_text(label, "Video player");          // تنظیم متن
      lv_obj_align(label, LV_ALIGN_TOP_LEFT, 247, 262);  // قرار دادن متن در مرکز صفحه
    }

    if (btn_count == 11) {
      lv_obj_set_pos(btn[btn_count], 360, 200);  // فاصله بین آیتم‌ها
      lv_obj_add_event_cb(btn[btn_count], event_handler_button12, LV_EVENT_CLICKED, NULL);
      lv_label_set_text(label, "Calculator");            // تنظیم متن
      lv_obj_align(label, LV_ALIGN_TOP_LEFT, 368, 262);  // قرار دادن متن در مرکز صفحه
    }

    ///////////////////////////////////satr4///////////////////////////////////////////////////////
    if (btn_count == 12) {
      lv_obj_set_pos(btn[btn_count], 10, 305);  // فاصله بین آیتم‌ها
      lv_obj_add_event_cb(btn[btn_count], event_handler_button13, LV_EVENT_CLICKED, NULL);
      lv_label_set_text(label, "Phone");                // تنظیم متن
      lv_obj_align(label, LV_ALIGN_TOP_LEFT, 30, 362);  // قرار دادن متن در مرکز صفحه
    }

    if (btn_count == 13) {
      lv_obj_set_pos(btn[btn_count], 130, 305);  // فاصله بین آیتم‌ها
      lv_obj_add_event_cb(btn[btn_count], event_handler_button14, LV_EVENT_CLICKED, NULL);
      lv_label_set_text(label, "Messages");              // تنظیم متن
      lv_obj_align(label, LV_ALIGN_TOP_LEFT, 138, 362);  // قرار دادن متن در مرکز صفحه
    }

    if (btn_count == 14) {
      lv_obj_set_pos(btn[btn_count], 250, 300);  // فاصله بین آیتم‌ها
      lv_obj_add_event_cb(btn[btn_count], event_handler_button15, LV_EVENT_CLICKED, NULL);
      lv_label_set_text(label, "Gallery");               // تنظیم متن
      lv_obj_align(label, LV_ALIGN_TOP_LEFT, 265, 362);  // قرار دادن متن در مرکز صفحه
    }

    if (btn_count == 15) {
      lv_obj_set_pos(btn[btn_count], 360, 300);  // فاصله بین آیتم‌ها
      lv_obj_add_event_cb(btn[btn_count], event_handler_button16, LV_EVENT_CLICKED, NULL);
      lv_label_set_text(label, "Exit");                  // تنظیم متن
      lv_obj_align(label, LV_ALIGN_TOP_LEFT, 378, 362);  // قرار دادن متن در مرکز صفحه
    }
    //****************************SATR5*************************************************
    if (btn_count == 16) {
      lv_obj_set_pos(btn[btn_count], 10, 400);  // فاصله بین آیتم‌ها
      lv_obj_add_event_cb(btn[btn_count], event_handler_button17, LV_EVENT_CLICKED, NULL);
      lv_label_set_text(label, "Exit");                 // تنظیم متن
      lv_obj_align(label, LV_ALIGN_TOP_LEFT, 30, 462);  // قرار دادن متن در مرکز صفحه
    }
    //files[btn_count].close();
  }

  // }
}