
//////////////////////////////////////////////////////////////////////////////////////////
lv_obj_t *terminal;                                       /* Object for textarea (terminal) */
lv_obj_t *btn_clear;                                      /* Object for clear button */
lv_obj_t *btn_color;                                      /* Object for color change button */
lv_obj_t *btn_baud;                                       /* Object for baud rate change button */
lv_obj_t *btn_save;                                       /* Object for save to SD button */
lv_obj_t *label_clear;                                    /* Object for clear button label */
lv_obj_t *label_color;                                    /* Object for color button label */
lv_obj_t *label_baud;                                     /* Object for baud rate button label */
lv_obj_t *label_save;                                     /* Object for save button label */
lv_color_t textColor = lv_color_hex(0xFFFFFF);            // Default color (white)
int baudRate = 9600;                                      // Default baud rate
int colorIndex = 0;                                       // Index to track current color
int baudRates[] = { 9600, 19200, 38400, 57600, 115200 };  // Array of baud rates
int baudIndex = 0;                                        // Index to track current baud rate
String logData = "";                                      // ذخیره داده‌های ورودی

lv_obj_t *fileNameInput;  // ورودی نام فایل
lv_obj_t *btn_keyboard;   // دکمه کیبورد
lv_obj_t *kb;             // کیبورد مجازی
String fileName;          // متغیر ذخیره نام فایل
lv_obj_t *btn_confirm;       // دکمه تأیید
lv_coord_t originalX, originalY; // ذخیره موقعیت اصلی فیلد ورودی

void SerialMonitor() {
  if (change_menu == 1) {
    change_menu = 0;
    Serial.println("Create SerialMonitor...");
    create_SerialMonitor();
    main_timer = 0;
    main_sec = 0;
    batt_sec = 0;
  }

  // Check if data is available on Serial
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');  // Read input until newline

    // Update the terminal with the new text
    lv_textarea_add_text(terminal, input.c_str());
    lv_textarea_add_text(terminal, "\n");  // Add new line
  }

  // خواندن نام فایل از ورودی
  const char *text = lv_textarea_get_text(fileNameInput);
  if (text[0] != '\0') {
    fileName = text;  // ذخیره نام فایل در متغیر
  }
}



// تن
// Event handler for clear button
void event_handler_clear(lv_event_t *e) {
  lv_textarea_set_text(terminal, "");  // Clear terminal
}

// Event handler for color button
void event_handler_color(lv_event_t *e) {
  // Define an array of colors
  lv_color_t colors[] = { lv_color_hex(0xFFFFFF), lv_color_hex(0xFF0000), lv_color_hex(0x00FF00), lv_color_hex(0x0000FF), lv_color_hex(0x000000) };
  colorIndex = (colorIndex + 1) % 5;                               // Cycle through colors
  textColor = colors[colorIndex];                                  // Update text color
  lv_obj_set_style_text_color(terminal, textColor, LV_PART_MAIN);  // Apply new color to terminal
}

// Event handler for baud rate button
void event_handler_baud(lv_event_t *e) {
  // Cycle through baud rates
  baudIndex = (baudIndex + 1) % 5;
  baudRate = baudRates[baudIndex];
  Serial.end();            // Stop current Serial communication
  Serial.begin(baudRate);  // Start with new baud rate

  // Show the updated baud rate on terminal
  lv_textarea_add_text(terminal, "Baud rate changed to: ");
  lv_textarea_add_text(terminal, String(baudRate).c_str());
  lv_textarea_add_text(terminal, "\n");
}

// Event handler for save button
void event_handler_save(lv_event_t *e) {
  const char *terminalText = lv_textarea_get_text(terminal);
  //const char *fileName = lv_textarea_get_text(fileNameInput);  // دریافت نام فایل
  fileName = "/" + fileName + "..txt";
  File file = SD.open(fileName, FILE_WRITE);
  if (file) {
    file.println(terminalText);  // نوشتن داده‌ها در فایل
    file.close();                // بستن فایل
    Serial.println("Data saved to SD card.");
  } else {
    Serial.println("Failed to open file for writing.");
  }
}

// تابع نمایش کیبورد مجازی
void showKeyboard() {
  kb = lv_keyboard_create(lv_scr_act());
  lv_keyboard_set_mode(kb, LV_KEYBOARD_MODE_USER_4);  // حالت کیبورد متنی
  lv_keyboard_set_textarea(kb, fileNameInput);        // اتصال کیبورد به ورودی نام فایل
  lv_obj_set_size(kb, 480, 140);
  lv_obj_align(kb, LV_ALIGN_BOTTOM_MID, 0, 0);  // Align keyboard at the bottom

  // دکمه تأیید
  btn_confirm = lv_btn_create(lv_scr_act());
  lv_obj_set_size(btn_confirm, 100, 45);
  lv_obj_set_pos(btn_confirm, 10, 120);  // تنظیم موقعیت دکمه تأیید

  ///new////

  lv_obj_add_event_cb(
    btn_confirm, [](lv_event_t *e) {
      lv_obj_del(kb);           // حذف کیبورد
      lv_obj_del(btn_confirm);  // حذف دکمه تأیید
      lv_obj_set_pos(fileNameInput, originalX, originalY);
    },
    LV_EVENT_CLICKED, NULL);  // رویداد کلیک
  lv_obj_t *label_confirm = lv_label_create(btn_confirm);
  lv_label_set_text(label_confirm, "Confirm");
}



void create_SerialMonitor() {

  ///////////////////////////////////////////////////////////////////////////////
  // Create a textarea (terminal) to display text
  terminal = lv_textarea_create(lv_scr_act());
  lv_obj_set_size(terminal, 469, 269);                    // سایز ترمینال
  lv_obj_align(terminal, LV_ALIGN_TOP_MID, 1, 3);         // تنظیم موقعیت ترمینال
  lv_textarea_set_text(terminal, "Awaiting input...\n");  // متن اولیه

  // Create a clear button
  btn_clear = lv_btn_create(lv_scr_act());
  lv_obj_align(btn_clear, LV_ALIGN_BOTTOM_LEFT, 400, -10);  // موقعیت دکمه پاک کردن
  lv_obj_set_size(btn_clear, 70, 35);

  // Label for the clear button
  label_clear = lv_label_create(btn_clear);
  lv_label_set_text(label_clear, "Clear");
  lv_obj_center(label_clear);

  // Assign event handler to the clear button
  lv_obj_add_event_cb(btn_clear, event_handler_clear, LV_EVENT_CLICKED, NULL);

  // Create a color change button
  btn_color = lv_btn_create(lv_scr_act());
  lv_obj_align(btn_color, LV_ALIGN_BOTTOM_LEFT, 339, -10);  // موقعیت دکمه تغییر رنگ
  lv_obj_set_size(btn_color, 50, 35);

  // Label for the color change button
  label_color = lv_label_create(btn_color);
  lv_label_set_text(label_color, "Color");
  lv_obj_center(label_color);

  // Assign event handler to the color change button
  lv_obj_add_event_cb(btn_color, event_handler_color, LV_EVENT_CLICKED, NULL);

  // Create a baud rate change button
  btn_baud = lv_btn_create(lv_scr_act());
  lv_obj_align(btn_baud, LV_ALIGN_BOTTOM_LEFT, 210, -10);  // موقعیت دکمه تغییر baud rate
  lv_obj_set_size(btn_baud, 120, 35);

  // Label for the baud rate button
  label_baud = lv_label_create(btn_baud);
  lv_label_set_text(label_baud, "Change Baud");
  lv_obj_center(label_baud);

  // Assign event handler to the baud rate change button
  lv_obj_add_event_cb(btn_baud, event_handler_baud, LV_EVENT_CLICKED, NULL);

  // Create a save button
  btn_save = lv_btn_create(lv_scr_act());
  lv_obj_align(btn_save, LV_ALIGN_BOTTOM_LEFT, 5, -10);  // موقعیت دکمه ذخیره
  lv_obj_set_size(btn_save, 70, 35);

  // Label for the save button
  label_save = lv_label_create(btn_save);
  lv_label_set_text(label_save, "Save");
  lv_obj_center(label_save);

  // ورودی نام فایل
  fileNameInput = lv_textarea_create(lv_scr_act());
  lv_obj_set_size(fileNameInput, 126, 35);
  originalX = 79;  // ذخیره موقعیت اصلی
  originalY = 275;
  lv_obj_set_pos(fileNameInput, originalX, originalY);  // تنظیم موقعیت ورودی نام فایل
  lv_textarea_set_placeholder_text(fileNameInput, "Enter file name");

  // اضافه کردن رویداد کلیک به فیلد ورودی
  lv_obj_add_event_cb(
    fileNameInput, [](lv_event_t *e) {
      // جابه‌جا کردن فیلد ورودی
      lv_obj_set_pos(fileNameInput, originalX + 100, originalY - 158);  // جابه‌جایی به پایین
      showKeyboard();                                                   // نمایش کیبورد با کلیک بر روی فیلد ورودی
    },
    LV_EVENT_CLICKED, NULL);  // رویداد کلیک

  lv_obj_add_event_cb(btn_save, event_handler_save, LV_EVENT_CLICKED, NULL);
}