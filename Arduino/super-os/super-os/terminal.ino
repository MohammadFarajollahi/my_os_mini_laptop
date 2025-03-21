

void Terminal_() {
  if (change_menu == 1) {
    change_menu = 0;
    Serial.println("Create terminal...");
    create_terminal();
    main_timer = 0;
    main_sec = 0;
    batt_sec = 0;
  }
}

// تابع برای اجرای دستورات ترمینال
void run_command(lv_event_t *e) {
  const char *cmd = lv_textarea_get_text(command_input);  // دریافت متن ورودی

  // اضافه کردن دستور به خروجی ترمینال
  lv_textarea_add_text(terminal_output, "\n> ");
  lv_textarea_add_text(terminal_output, cmd);
  lv_textarea_add_text(terminal_output, "\n");

  // اجرای دستورات (در اینجا به سادگی نمایش یک پیغام)
  if (strcmp(cmd, "clear") == 0) {
    lv_textarea_set_text(terminal_output, "");  // پاک کردن خروجی
  } else {
    lv_textarea_add_text(terminal_output, "Command executed: ");
    lv_textarea_add_text(terminal_output, cmd);
    lv_textarea_add_text(terminal_output, "\n");
  }

  lv_textarea_set_text(command_input, "");  // پاک کردن ورودی پس از اجرا
}

void btn_exit(lv_event_t *e) {
  Serial.println("Exit");
  count_files = 0;
  btn_count = 0;
  lv_obj_clean(lv_scr_act());
  menu_select = "mainMenu";
  change_menu = 1;
}

void create_terminal() {
  // ایجاد فیلد متن برای نمایش خروجی ترمینال
  terminal_output = lv_textarea_create(lv_scr_act());
  lv_obj_set_size(terminal_output, 470, 200);                    // تنظیم سایز خروجی ترمینال
  lv_obj_align(terminal_output, LV_ALIGN_TOP_MID, 0, 10);        // قرارگیری در بالای صفحه
  lv_textarea_set_text(terminal_output, "Terminal Ready...\n");  // متن اولیه

  // ایجاد فیلد ورودی برای دریافت دستورات
  command_input = lv_textarea_create(lv_scr_act());
  lv_obj_set_size(command_input, 470, 40);                   // تنظیم سایز ورودی دستورات
  lv_obj_align(command_input, LV_ALIGN_BOTTOM_MID, 0, -60);  // موقعیت در پایین صفحه

  // ایجاد کیبورد
  keyboard = lv_keyboard_create(lv_scr_act());
  lv_obj_set_size(keyboard, 470, 140);                // سایز کیبورد
  lv_obj_align(keyboard, LV_ALIGN_BOTTOM_MID, 0, 0);  // موقعیت در پایین
  lv_keyboard_set_textarea(keyboard, command_input);  // اتصال کیبورد به فیلد ورودی

  // تنظیم رویداد برای اجرای دستورات هنگام فشردن کلید 'Enter' در کیبورد
  lv_obj_add_event_cb(command_input, run_command, LV_EVENT_READY, NULL);


  //exit button
  btn[0] = lv_btn_create(lv_scr_act());
  lv_obj_set_size(btn[0], 70, 30);
  lv_obj_align(btn[0], LV_ALIGN_TOP_LEFT, 400, 10);
  lv_obj_add_event_cb(btn[0], btn_exit, LV_EVENT_CLICKED, NULL);
  lv_obj_t *label = lv_label_create(btn[0]); /*Add a label to the button*/
  lv_label_set_text(label, "Exit");          /*Set the labels text*/
  lv_obj_center(label);
}