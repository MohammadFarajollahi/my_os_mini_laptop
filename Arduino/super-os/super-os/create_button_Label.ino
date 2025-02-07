
void set_button(String label_name, int button_size_x, int button_size_y, int button_pos_x, int button_pos_y, int label_pos_x, int label_pos_y, String file_path_) {
  btn[btn_count] = lv_btn_create(menu_container);
  lv_obj_set_size(btn[btn_count], button_size_x - 1, button_size_y - 1);  // اندازه دکمه‌ها
  img[btn_count] = lv_img_create(btn[btn_count]);
  String img_path = file_path_;  // مسیر تصویرها
  uint16_t *img_data = loadImageFromSD(img_path.c_str());
  Serial.println(img_path);
  if (img_data != nullptr) {
    img_dsc[btn_count].data = (uint8_t *)img_data;
    img_dsc[btn_count].data_size = img_size;
    img_dsc[btn_count].header.cf = LV_IMG_CF_TRUE_COLOR;  // فرمت تصویر
    img_dsc[btn_count].header.w = button_size_x;          // عرض تصویر
    img_dsc[btn_count].header.h = button_size_y;          // ارتفاع تصویر
    lv_obj_align(img[btn_count], LV_ALIGN_CENTER, 0, 0);
    // تنظیم تصویر در LVGL
    lv_img_set_src(img[btn_count], &img_dsc[btn_count]);
  } else {
    Serial.println("Failed to load image");
  }
  lv_obj_t *label = lv_label_create(menu_container);
  lv_obj_set_pos(btn[btn_count], button_pos_x, button_pos_y);  // فاصله بین آیتم‌ها
  if (btn_count == 0) lv_obj_add_event_cb(btn[btn_count], event_handler_button1_desktop, LV_EVENT_CLICKED, NULL);
  lv_label_set_text(label, label_name.c_str());                      // تنظیم متن
  lv_obj_align(label, LV_ALIGN_TOP_LEFT, label_pos_x, label_pos_y);  // قرار دادن متن در مرکز صفحه
  ++btn_count;
}


//*******************************************************************label_creat*****************************************************************************
void set_label(int button_size_x, int button_size_y, int button_pos_x, int button_pos_y, String file_path_) {
  label_create[btn_count] = lv_label_create(menu_container);
  lv_obj_set_size(label_create[btn_count], button_size_x - 1, button_size_y - 1);  // اندازه دکمه‌ها
  img[btn_count] = lv_img_create(label_create[btn_count]);
  String img_path = file_path_;  // مسیر تصویرها
  uint16_t *img_data = loadImageFromSD(img_path.c_str());
  Serial.println(img_path);
  if (img_data != nullptr) {
    img_dsc[btn_count].data = (uint8_t *)img_data;
    img_dsc[btn_count].data_size = img_size;
    img_dsc[btn_count].header.cf = LV_IMG_CF_TRUE_COLOR;  // فرمت تصویر
    img_dsc[btn_count].header.w = button_size_x;          // عرض تصویر
    img_dsc[btn_count].header.h = button_size_y;          // ارتفاع تصویر
    lv_obj_align(img[btn_count], LV_ALIGN_CENTER, 0, 0);
    // تنظیم تصویر در LVGL
    lv_img_set_src(img[btn_count], &img_dsc[btn_count]);
  } else {
    Serial.println("Failed to load image");
  }
  lv_obj_align(label_create[btn_count], LV_ALIGN_TOP_LEFT, button_pos_x, button_pos_y);  // مکان لیبل
  ++btn_count;
}