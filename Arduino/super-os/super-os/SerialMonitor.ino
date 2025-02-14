
//////////////////////////////////////////////////////////////////////////////////////////
lv_obj_t *terminal; /* Object for textarea (terminal) */
lv_obj_t *btn_clear;
lv_obj_t *btn_atcommand;
lv_obj_t *label_atcommand; /* Object for clear button */
lv_obj_t *btn_color;       /* Object for color change button */
lv_obj_t *btn_baud;        /* Object for baud rate change button */
lv_obj_t *btn_save;        /* Object for save to SD button */
lv_obj_t *label_clear;     /* Object for clear button label */
lv_obj_t *label_color;     /* Object for color button label */
lv_obj_t *label_baud;      /* Object for baud rate button label */
lv_obj_t *label_save;

lv_obj_t *BtnSend;
;
lv_obj_t *LabelSend;
;

static lv_obj_t *my_switch;  // ذخیره داده‌های ورودی
static lv_obj_t *label_command;
static lv_obj_t *parent;
/* Object for save button label */
lv_color_t textColor = lv_color_hex(0xFFFFFF);            // Default color (white)
int baudRate = 9600;                                      // Default baud rate
int colorIndex = 0;                                       // Index to track current color
int baudRates[] = { 9600, 19200, 38400, 57600, 115200 };  // Array of baud rates
int baudIndex = 0;                                        // Index to track current baud rate
String logData = "";

lv_obj_t *fileNameInput;          // ورودی نام فایل
lv_obj_t *btn_keyboard;           // دکمه کیبورد
lv_obj_t *kb;                     // کیبورد مجازی
String fileName;                  // متغیر ذخیره نام فایل
lv_obj_t *btn_confirm;            // دکمه تأیید
lv_coord_t originalX, originalY;  // ذخیره موقعیت اصلی فیلد ورودی

void SerialMonitor() {
  if (change_menu == 1) {
    change_menu = 0;
    Serial.println("Create SerialMonitor...");
    create_SerialMonitor();
    main_timer = 0;
    main_sec = 0;
    batt_sec = 0;
    Serial_mode_count = 1;  // start timer to set serial mode
    Serial_mode_timer = 0;
    AtCommandMode = 0;
    AtCommandMode_count = 0;
    AtCommandMode_timer = 0;
  }
  //cash_size();
  while (stm32_serial.available() > 0) {
    stm32_string = stm32_serial.readStringUntil('\n');
    stm32_string.trim();
    Serial.println(stm32_string);
    stm32_serial.flush();
    Serial.flush();
    if (serial_mode == 1) {
      lcd_show(stm32_string);
    }
    //***stm32_serial-mode***
    if (stm32_string == "stm32_serial_mode") {
      Serial.println("stm in Serial mode");
      serial_mode = 1;
      Serial_mode_count = 0;
      Serial_mode_timer = 0;
      String input = "STM32 Ready: Baud rate-->9600";  // Read input until newline
      lv_textarea_add_text(terminal, input.c_str());
      lv_textarea_add_text(terminal, "\n");  // Add new line
    }

    if (stm32_string == "stm32-atcommandmode on") {
      Serial.println("STM3232 in At-Command mode");
      lcd_show("STM32 in AtCommand mode");
      AtCommandMode = 1;
      AtCommandMode_count = 0;
      AtCommandMode_timer = 0;
    }

    if (stm32_string == "stm32-atcommandmode off") {
      Serial.println("STM32 exit At-Command mode");
      lcd_show("STM32 exit At-Command mode");
      AtCommandMode = 0;
      AtCommandMode_count = 0;
      AtCommandMode_timer = 0;
    }
  }


  if (AtCommandMode_timer >= 2) {  //at command mode check timer
    AtCommandMode_timer = 0;
    lcd_show("STM32 in AtCommand mode:not response Send again!!!");
    if (AtCommandMode == 0) {
      AtCommandMode_count = 1;
      lcd_show("At command mode!!!");
      stm32_serial.println("atcommand-mode on");
    } else {
      lcd_show("EXIT At command mode!!!");
      AtCommandMode_count = 1;
      stm32_serial.println("atcommand-mode off");
    }
  }

  if (serial_mode == 0 && Serial_mode_count == 1 && Serial_mode_timer >= 1) {  //serial mode timer
    Serial_mode_timer = 0;
    stm32_serial.println("Serial_mode");
    Serial.println("request stm32 goto serial mode");
  }

  // Check if data is available on Serial
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');  // Read input until newline
    lv_textarea_add_text(terminal, input.c_str());
    lv_textarea_add_text(terminal, "\n");  // Add new line
    if(AtCommandMode == 1){
      stm32_serial.println(input);
    }
  }



  // خواندن نام فایل از ورودی
  const char *text = lv_textarea_get_text(fileNameInput);
  if (text[0] != '\0') {
    fileName = text;  // ذخیره نام فایل در متغیر
  }
}

// lv_obj_add_state(my_switch, LV_STATE_CHECKED);  // روشن
// lv_obj_clear_state(my_switch, LV_STATE_CHECKED);  // خاموش
// تابع رویداد برای مدیریت تغییر وضعیت سوییچ
static void switch_event_handler(lv_event_t *e) {
  lv_obj_t *obj = lv_event_get_target(e);

  if (lv_obj_has_state(obj, LV_STATE_CHECKED)) {
    lcd_show("At command mode!!!");
    AtCommandMode_count = 1;
    stm32_serial.println("atcommand-mode on");
  } else {
    lcd_show("EXIT At command mode!!!");
    AtCommandMode_count = 1;
    stm32_serial.println("atcommand-mode off");
  }
}

void lcd_show(String input_char) {
  String input = input_char;  // Read input until newline
  lv_textarea_add_text(terminal, input.c_str());
  lv_textarea_add_text(terminal, "\n");  // Add new line
}

void btn_exit_serial(lv_event_t *e) {
  String Serial_baud = "baudrate:" + String(9600);
  stm32_serial.println(Serial_baud);
  delay(500);
  stm32_serial.println("exit serial");
  delay(500);
  stm32_serial.println("atcommand-mode off");
  delay(500);
  Serial.println("Exit");
  count_files = 0;
  btn_count = 0;
  lv_obj_clean(lv_scr_act());
  menu_select = "mainMenu";
  change_menu = 1;
  serial_mode = 0;
  Serial_mode_timer = 0;
  Serial_mode_count = 0;
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
  // stm32_serial.flush();  // اطمینان از ارسال همه داده‌ها
  // delay(100);
  // stm32_serial.end();
  // delay(100);  // یک تأخیر کوتاه برای پایداری
  // stm32_serial.begin(baudRate, SERIAL_8N1, 42, 45);
  String Serial_baud = "baudrate:" + String(baudRate);
  stm32_serial.println(Serial_baud);

  // Show the updated baud rate on terminal
  lv_textarea_add_text(terminal, "Baud rate changed to: ");
  lv_textarea_add_text(terminal, String(baudRate).c_str());
  lv_textarea_add_text(terminal, "\n");
}

void event_handler_send(lv_event_t *e) {
  String send_str = "atcommand" + fileName;
  stm32_serial.println(send_str);
}

// Event handler for save button
void event_handler_save(lv_event_t *e) {
  lcd_show("Data saved...");
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
  lv_obj_set_size(btn_confirm, 100, 35);
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
  label_clear = lv_label_create(btn_clear);
  lv_label_set_text(label_clear, "Clear");
  lv_obj_center(label_clear);
  lv_obj_add_event_cb(btn_clear, event_handler_clear, LV_EVENT_CLICKED, NULL);

  // Create a color change button
  btn_color = lv_btn_create(lv_scr_act());
  lv_obj_align(btn_color, LV_ALIGN_BOTTOM_LEFT, 339, -10);  // موقعیت دکمه تغییر رنگ
  lv_obj_set_size(btn_color, 50, 35);
  label_color = lv_label_create(btn_color);
  lv_label_set_text(label_color, "Color");
  lv_obj_center(label_color);
  lv_obj_add_event_cb(btn_color, event_handler_color, LV_EVENT_CLICKED, NULL);

  // Create a baud rate change button
  btn_baud = lv_btn_create(lv_scr_act());
  lv_obj_align(btn_baud, LV_ALIGN_BOTTOM_LEFT, 210, -10);  // موقعیت دکمه تغییر baud rate
  lv_obj_set_size(btn_baud, 120, 35);
  label_baud = lv_label_create(btn_baud);
  lv_label_set_text(label_baud, "Change Baud");
  lv_obj_center(label_baud);
  lv_obj_add_event_cb(btn_baud, event_handler_baud, LV_EVENT_CLICKED, NULL);


  ////////////send/////////////
  // Create a save button
  BtnSend = lv_btn_create(lv_scr_act());
  lv_obj_align(BtnSend, LV_ALIGN_TOP_LEFT, 410, 220);  // موقعیت دکمه ارسال
  lv_obj_set_size(BtnSend, 60, 30);
  LabelSend = lv_label_create(BtnSend);
  lv_label_set_text(LabelSend, "Sned");
  lv_obj_center(LabelSend);
  lv_obj_add_event_cb(BtnSend, event_handler_send, LV_EVENT_CLICKED, NULL);

  //*****exit button*****
  btn[0] = lv_btn_create(lv_scr_act());
  lv_obj_set_size(btn[0], 70, 30);
  lv_obj_align(btn[0], LV_ALIGN_TOP_LEFT, 400, 10);
  lv_obj_add_event_cb(btn[0], btn_exit_serial, LV_EVENT_CLICKED, NULL);
  lv_obj_t *label = lv_label_create(btn[0]); /*Add a label to the button*/
  lv_label_set_text(label, "Exit");          /*Set the labels text*/
  lv_obj_center(label);

  // ****Create a atcommand button****
  if (parent == NULL) parent = lv_scr_act();                                           // اگر والد مشخص نبود، صفحه اصلی را بگیر
  my_switch = lv_switch_create(parent);                                                // ایجاد سوییچ
  lv_obj_align(my_switch, LV_ALIGN_TOP_LEFT, 410, 58);                                 // قرار دادن در مرکز صفحه
  lv_obj_add_event_cb(my_switch, switch_event_handler, LV_EVENT_VALUE_CHANGED, NULL);  // رویداد تغییر وضعیت
  label_command = lv_label_create(parent);
  lv_label_set_text(label_command, "AtCmnd");                                 // تنظیم متن
  lv_obj_align_to(label_command, my_switch, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);  // قرار دادن زیر سوییچ


  // Create a save button
  btn_save = lv_btn_create(lv_scr_act());
  lv_obj_align(btn_save, LV_ALIGN_BOTTOM_LEFT, 5, -10);  // موقعیت دکمه ذخیره
  lv_obj_set_size(btn_save, 70, 35);
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