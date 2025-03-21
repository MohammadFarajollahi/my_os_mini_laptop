lv_obj_t *ddlist;
String selected_number = "";    // متغیر برای ذخیره شماره انتخاب‌شده
static char selected_item[64];  // بافر برای ذخیره آیتم انتخاب شده
lv_obj_t *cb1, *cb2, *cb3;
int check1;
lv_obj_t *modal;
lv_obj_t *ta;
lv_obj_t *list;
#define FILE_PATH "/phone_numbers.txt"

void server() {
  if (change_menu == 1) {
    change_menu = 0;
    Serial.println("Create SerialMonitor...");
    create_server();
    main_timer = 0;
    main_sec = 0;
    batt_sec = 0;
  }

  while (stm32_serial.available() > 0) {
    stm32_string = stm32_serial.readStringUntil('\n');
    stm32_string.trim();
    Serial.println(stm32_string);
    stm32_serial.flush();
    lcd_show2(stm32_string);
  }
}

void lcd_show2(String input_char) {
  String input = input_char;  // Read input until newline
  lv_textarea_add_text(terminal, input.c_str());
  lv_textarea_add_text(terminal, "\n");  // Add new line
}

void btn_xl(lv_event_t *e) {
  Serial.println("btn xl");
  StaticJsonDocument<200> jsonDoc;
  jsonDoc["type"] = "sendSMS";         // نوع دیتا (مثلاً ارسال پیامک)
  jsonDoc["phone"] = selected_number;  // شماره تلفن
  jsonDoc["message"] = "XL 1111";      // متن پیام
  jsonDoc["extra"] = "urgent";         // هر دیتای اضافی
  String jsonString;
  serializeJson(jsonDoc, jsonString);
  Serial.println(jsonString);
  stm32_serial.println(jsonString);
}

void btn_exit2(lv_event_t *e) {
  Serial.println("Exit");
  count_files = 0;
  btn_count = 0;
  lv_obj_clean(lv_scr_act());
  menu_select = "mainMenu";
  change_menu = 1;
}

void btn_l(lv_event_t *e) {
  Serial.println("btn l");
  StaticJsonDocument<200> jsonDoc;
  jsonDoc["type"] = "sendSMS";         // نوع دیتا (مثلاً ارسال پیامک)
  jsonDoc["phone"] = selected_number;  // شماره تلفن
  jsonDoc["message"] = "L 1111";       // متن پیام
  jsonDoc["extra"] = "urgent";         // هر دیتای اضافی
  String jsonString;
  serializeJson(jsonDoc, jsonString);
  Serial.println(jsonString);
  stm32_serial.println(jsonString);
}

void btn_on1(lv_event_t *e) {
  Serial.println("btn on");
  StaticJsonDocument<200> jsonDoc;
  jsonDoc["type"] = "sendSMS";         // نوع دیتا (مثلاً ارسال پیامک)
  jsonDoc["phone"] = selected_number;  // شماره تلفن
  jsonDoc["message"] = "On1 1111";     // متن پیام
  jsonDoc["extra"] = "urgent";         // هر دیتای اضافی
  String jsonString;
  serializeJson(jsonDoc, jsonString);
  Serial.println(jsonString);
  stm32_serial.println(jsonString);
}

void btn_off(lv_event_t *e) {
  Serial.println("btn off");
  StaticJsonDocument<200> jsonDoc;
  jsonDoc["type"] = "sendSMS";         // نوع دیتا (مثلاً ارسال پیامک)
  jsonDoc["phone"] = selected_number;  // شماره تلفن
  jsonDoc["message"] = "OF 1111";      // متن پیام
  jsonDoc["extra"] = "urgent";         // هر دیتای اضافی
  String jsonString;
  serializeJson(jsonDoc, jsonString);
  Serial.println(jsonString);
  stm32_serial.println(jsonString);
}

void btn_s1(lv_event_t *e) {
  Serial.println("btn s1");
  StaticJsonDocument<200> jsonDoc;
  jsonDoc["type"] = "sendSMS";         // نوع دیتا (مثلاً ارسال پیامک)
  jsonDoc["phone"] = selected_number;  // شماره تلفن
  jsonDoc["message"] = "S1 1111";      // متن پیام
  jsonDoc["extra"] = "urgent";         // هر دیتای اضافی
  String jsonString;
  serializeJson(jsonDoc, jsonString);
  Serial.println(jsonString);
  stm32_serial.println(jsonString);
}

void btn_s2(lv_event_t *e) {
  Serial.println("btn s2");
  StaticJsonDocument<200> jsonDoc;
  jsonDoc["type"] = "sendSMS";         // نوع دیتا (مثلاً ارسال پیامک)
  jsonDoc["phone"] = selected_number;  // شماره تلفن
  jsonDoc["message"] = "S2 1111";      // متن پیام
  jsonDoc["extra"] = "urgent";         // هر دیتای اضافی
  String jsonString;
  serializeJson(jsonDoc, jsonString);
  Serial.println(jsonString);
  stm32_serial.println(jsonString);
}

void btn_bt(lv_event_t *e) {
  Serial.println("btn BT");
  StaticJsonDocument<200> jsonDoc;
  jsonDoc["type"] = "sendSMS";         // نوع دیتا (مثلاً ارسال پیامک)
  jsonDoc["phone"] = selected_number;  // شماره تلفن
  jsonDoc["message"] = "BT 1111";      // متن پیام
  jsonDoc["extra"] = "urgent";         // هر دیتای اضافی
  String jsonString;
  serializeJson(jsonDoc, jsonString);
  Serial.println(jsonString);
  stm32_serial.println(jsonString);
}

void btn_r(lv_event_t *e) {
  Serial.println("btn R");
  String NumerSend;
  if (selected_number.startsWith("0")) {
    NumerSend = "+98" + selected_number.substring(1);
  }

  StaticJsonDocument<200> jsonDoc;
  jsonDoc["type"] = "sendSMS";                               // نوع دیتا (مثلاً ارسال پیامک)
  jsonDoc["phone"] = selected_number;                        // شماره تلفن
  jsonDoc["message"] = "R 1111 1111 12345678 " + NumerSend;  // متن پیام
  jsonDoc["extra"] = "urgent";                               // هر دیتای اضافی
  String jsonString;
  serializeJson(jsonDoc, jsonString);
  Serial.println(jsonString);
  stm32_serial.println(jsonString);
}

// تابعی برای جدا کردن فقط شماره از فرمت "numberX:09123456789"
String extract_phone_number(String line) {
  int index = line.indexOf(":");
  if (index != -1) {
    return line.substring(index + 1);  // فقط شماره تلفن را برمی‌گرداند
  }
  return "";
}
// تابع handler برای واکنش به تغییر انتخاب
static void dropdown_event_handler(lv_event_t *e) {
  lv_obj_t *dropdown = lv_event_get_target(e);
  char selected_text[32];
  lv_dropdown_get_selected_str(dropdown, selected_text, sizeof(selected_text));
  selected_number = extract_phone_number(String(selected_text));
  Serial.print("Selected Number: ");
  Serial.println(selected_number);
}

// تابع تغییر وضعیت چک باکس
void checkbox_event_cb(lv_event_t *e) {
  if (check1 == 0) {
    Serial.println("auto on");
    check1 = 0;
  } else {
    Serial.println("auto off");
    check1 = 1;
  }
}

/////////////////////////////////////////add number///////////////////////////


// تابع ذخیره شماره تلفن در SD Card
void save_to_sd(const char *phone_number) {

  File file = SD.open("/phone_numbers.txt", FILE_READ);
  int count = 0;

  // شمارش تعداد شماره‌های موجود
  if (file) {
    while (file.available()) {
      String line = file.readStringUntil('\n');
      if (line.startsWith("number")) {
        count++;
      }
    }
    file.close();
  }

  // ذخیره شماره جدید با فرمت numberX
  file = SD.open("/phone_numbers.txt", FILE_APPEND);
  if (file) {
    file.print("number");
    file.print(count + 1);  // شماره جدید
    file.print(":");
    file.println(phone_number);
    file.close();
    Serial.println("Phone number added!");
  } else {
    Serial.println("Failed to open file!");
  }
}

// تابعی برای بستن پنجره مودال
void close_modal(lv_event_t *e) {
  lv_obj_del(modal);
  lv_obj_del(ddlist);
  ddlist = lv_dropdown_create(lv_scr_act());  // فقط یک آرگومان
  File file = SD.open("/phone_numbers.txt", FILE_READ);
  int count = 0;
  file = SD.open("/phone_numbers.txt", FILE_READ);
  if (!file) {
    Serial.println("Failed to open file!");
    return;
  }
  String dropdown_options = "";
  bool first_line = true;
  while (file.available()) {
    String line = file.readStringUntil('\n');
    if (line.startsWith("number")) {
      if (!first_line) dropdown_options += "\n";  // بین گزینه‌ها فقط `\n` اضافه کن
      dropdown_options += line;
      dropdown_options.trim();  // در صورت نیاز حذف فاصله‌های اضافی از کل رشته نهایی
      first_line = false;
    }
  }
  file.close();
  lv_dropdown_set_options(ddlist, dropdown_options.c_str());
  // lv_dropdown_set_options(ddlist, "sim1: 09372425086\nsim2: 09114764806\nsim3: 09379274959\nsim4:\nsim5:\nsim6:\n");
  lv_obj_set_pos(ddlist, 15, 26);
  lv_obj_set_size(ddlist, 200, LV_SIZE_CONTENT);
  lv_obj_add_event_cb(ddlist, dropdown_event_handler, LV_EVENT_VALUE_CHANGED, NULL);

  modal = NULL;
}

// تابع ذخیره شماره تلفن و بستن پنجره
void save_phone_number(lv_event_t *e) {
  const char *phone_number = lv_textarea_get_text(ta);
  save_to_sd(phone_number);
  close_modal(NULL);
}
// تابع ایجاد صفحه مودال
void show_modal(lv_event_t *e) {
  // ساخت پنجره مودال
  modal = lv_obj_create(lv_scr_act());
  lv_obj_set_size(modal, 350, 270);
  lv_obj_align(modal, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_style_bg_color(modal, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
  lv_obj_set_style_border_width(modal, 2, LV_PART_MAIN);

  // تکست باکس برای شماره تلفن
  ta = lv_textarea_create(modal);
  lv_obj_set_pos(ta, 0, 0);
  lv_obj_set_size(ta, 300, 40);
  lv_textarea_set_placeholder_text(ta, "Enter phone number");

  // صفحه کلید مجازی
  kb = lv_keyboard_create(modal);
  lv_keyboard_set_mode(kb, LV_KEYBOARD_MODE_NUMBER);
  lv_keyboard_set_textarea(kb, ta);
  lv_obj_align(kb, LV_ALIGN_TOP_LEFT, 0, 48);
  lv_obj_set_size(kb, 300, 160);

  // دکمه ذخیره
  lv_obj_t *btn_save = lv_btn_create(modal);
  lv_obj_set_pos(btn_save, 29, 215);
  lv_obj_set_size(btn_save, 118, 30);
  lv_obj_t *label_save = lv_label_create(btn_save);
  lv_label_set_text(label_save, "Save");
  lv_obj_center(label_save);
  lv_obj_add_event_cb(btn_save, save_phone_number, LV_EVENT_CLICKED, NULL);

  // دکمه کنسل
  lv_obj_t *btn_cancel = lv_btn_create(modal);
  lv_obj_set_pos(btn_cancel, 189, 215);
  lv_obj_set_size(btn_cancel, 118, 30);
  lv_obj_t *label_cancel = lv_label_create(btn_cancel);
  lv_label_set_text(label_cancel, "Cancel");
  lv_obj_center(label_cancel);
  lv_obj_add_event_cb(btn_cancel, close_modal, LV_EVENT_CLICKED, NULL);
}

//******************************Edit****************************

void delete_number(lv_event_t *e) {
  lv_obj_t *btn = lv_event_get_target(e);
  const char *number = (const char *)lv_event_get_user_data(e);
  Serial.printf("Deleting: %s\n", number);

  // بازنویسی فایل بدون این شماره
  File file = SD.open("/phone_numbers.txt", "r");
  String content = "";
  while (file.available()) {
    String line = file.readStringUntil('\n');
    line.trim();
    if (line != number) {
      content += line + "\n";
    }
  }
  file.close();

  file = SD.open("/phone_numbers.txt", "w");
  file.print(content);
  file.close();

  lv_obj_del(btn->parent);
}


void load_numbers(lv_event_t *e) {
  modal = lv_obj_create(lv_scr_act());
  lv_obj_set_size(modal, 400, 250);
  lv_obj_center(modal);
  lv_obj_set_style_bg_color(modal, lv_color_hex(0xFFFFFF), LV_PART_MAIN);

  list = lv_list_create(modal);
  lv_obj_set_size(list, 380, 180);
  lv_obj_align(list, LV_ALIGN_TOP_MID, 0, 10);

  File file = SD.open("/phone_numbers.txt", "r");
  while (file.available()) {
    String number = file.readStringUntil('\n');
    number.trim();
    lv_obj_t *list_btn = lv_list_add_btn(list, NULL, number.c_str());
    lv_obj_t *del_btn = lv_btn_create(list_btn);
    lv_obj_set_size(del_btn, 40, 20);
    lv_obj_align(del_btn, LV_ALIGN_RIGHT_MID, -10, 0);
    lv_obj_add_event_cb(del_btn, delete_number, LV_EVENT_CLICKED, strdup(number.c_str()));
    lv_obj_t *labelll = lv_label_create(del_btn);
    lv_label_set_text(labelll, "Delete");
  }
  file.close();

  lv_obj_t *close_btn = lv_btn_create(modal);
  lv_obj_align(close_btn, LV_ALIGN_BOTTOM_MID, 0, -10);
  lv_obj_add_event_cb(close_btn, close_modal, LV_EVENT_CLICKED, NULL);
  lv_obj_t *close_label = lv_label_create(close_btn);
  lv_label_set_text(close_label, "Close");
}

//******************************************creat panel**************************************
//******************************************creat panel**************************************
//******************************************creat panel**************************************
void create_server() {
  //btn_xl
  btn[0] = lv_btn_create(lv_scr_act());
  lv_obj_set_size(btn[0], 76, 41);
  lv_obj_set_pos(btn[0], 399, 272);
  lv_obj_add_event_cb(btn[0], btn_xl, LV_EVENT_CLICKED, NULL);
  label_create[0] = lv_label_create(btn[0]);      /*Add a label to the button*/
  lv_label_set_text(label_create[0], "Loc test"); /*Set the labels text*/
  lv_obj_center(label_create[0]);
  //btn_l
  btn[1] = lv_btn_create(lv_scr_act());
  lv_obj_set_size(btn[1], 76, 41);
  lv_obj_set_pos(btn[1], 399, 219);
  lv_obj_add_event_cb(btn[1], btn_l, LV_EVENT_CLICKED, NULL);
  label_create[1] = lv_label_create(btn[1]);      /*Add a label to the button*/
  lv_label_set_text(label_create[1], "Location"); /*Set the labels text*/
  lv_obj_center(label_create[1]);
  //btn_on
  btn[2] = lv_btn_create(lv_scr_act());
  lv_obj_set_size(btn[2], 76, 41);
  lv_obj_set_pos(btn[2], 399, 165);
  lv_obj_add_event_cb(btn[2], btn_on1, LV_EVENT_CLICKED, NULL);
  label_create[2] = lv_label_create(btn[2]); /*Add a label to the button*/
  lv_label_set_text(label_create[2], "ON");  /*Set the labels text*/
  lv_obj_center(label_create[2]);
  //of
  btn[3] = lv_btn_create(lv_scr_act());
  lv_obj_set_size(btn[3], 76, 41);
  lv_obj_set_pos(btn[3], 399, 111);
  lv_obj_add_event_cb(btn[3], btn_off, LV_EVENT_CLICKED, NULL);
  label_create[3] = lv_label_create(btn[3]); /*Add a label to the button*/
  lv_label_set_text(label_create[3], "OFF"); /*Set the labels text*/
  lv_obj_center(label_create[3]);
  //S1
  btn[4] = lv_btn_create(lv_scr_act());
  lv_obj_set_size(btn[4], 76, 41);
  lv_obj_set_pos(btn[4], 399, 57);
  lv_obj_add_event_cb(btn[4], btn_s1, LV_EVENT_CLICKED, NULL);
  label_create[4] = lv_label_create(btn[4]);    /*Add a label to the button*/
  lv_label_set_text(label_create[4], "Sec On"); /*Set the labels text*/
  lv_obj_center(label_create[4]);
  //S2
  btn[5] = lv_btn_create(lv_scr_act());
  lv_obj_set_size(btn[5], 76, 41);
  lv_obj_set_pos(btn[5], 399, 5);
  lv_obj_add_event_cb(btn[5], btn_exit2, LV_EVENT_CLICKED, NULL);
  label_create[5] = lv_label_create(btn[5]);  /*Add a label to the button*/
  lv_label_set_text(label_create[5], "Exit"); /*Set the labels text*/
  lv_obj_center(label_create[5]);
  //bt
  btn[6] = lv_btn_create(lv_scr_act());
  lv_obj_set_size(btn[6], 76, 41);
  lv_obj_set_pos(btn[6], 315, 5);
  lv_obj_add_event_cb(btn[6], btn_bt, LV_EVENT_CLICKED, NULL);
  label_create[6] = lv_label_create(btn[6]);   /*Add a label to the button*/
  lv_label_set_text(label_create[6], "Anten"); /*Set the labels text*/
  lv_obj_center(label_create[6]);
  //s2
  btn[7] = lv_btn_create(lv_scr_act());
  lv_obj_set_size(btn[7], 76, 41);
  lv_obj_set_pos(btn[7], 315, 57);
  lv_obj_add_event_cb(btn[7], btn_s2, LV_EVENT_CLICKED, NULL);
  label_create[7] = lv_label_create(btn[7]);     /*Add a label to the button*/
  lv_label_set_text(label_create[7], "Sec Off"); /*Set the labels text*/
  lv_obj_center(label_create[7]);
  //R
  btn[8] = lv_btn_create(lv_scr_act());
  lv_obj_set_size(btn[8], 76, 41);
  lv_obj_set_pos(btn[8], 234, 5);
  lv_obj_add_event_cb(btn[8], btn_r, LV_EVENT_CLICKED, NULL);
  label_create[8] = lv_label_create(btn[8]);      /*Add a label to the button*/
  lv_label_set_text(label_create[8], "Register"); /*Set the labels text*/
  lv_obj_center(label_create[8]);
  //btn_add_number
  btn[9] = lv_btn_create(lv_scr_act());
  lv_obj_set_size(btn[9], 150, 40);
  lv_obj_set_pos(btn[9], 160, 340);
  lv_obj_add_event_cb(btn[9], show_modal, LV_EVENT_CLICKED, NULL);
  label_create[10] = lv_label_create(btn[9]);        /*Add a label to the button*/
  lv_label_set_text(label_create[10], "Add Number"); /*Set the labels text*/
  lv_obj_center(label_create[10]);

  //btn_editNumber
  btn[10] = lv_btn_create(lv_scr_act());
  lv_obj_set_pos(btn[10], 1, 340);
  lv_obj_set_size(btn[10], 150, 40);
  lv_obj_add_event_cb(btn[10], load_numbers, LV_EVENT_CLICKED, NULL);
  label_create[11] = lv_label_create(btn[10]);          /*Add a label to the button*/
  lv_label_set_text(label_create[11], "Delete Number"); /*Set the labels text*/
  lv_obj_center(label_create[11]);
  Serial.println("button10 creat");
  //labelDown
  label_create[12] = lv_label_create(lv_scr_act());                      /*Add a label to the button*/
  lv_label_set_text(label_create[12], "***PAVAN PRODUCT CONTROLLER***"); /*Set the labels text*/
  lv_obj_set_pos(label_create[12], 100, 500);
  lv_obj_set_size(label_create[12], LV_SIZE_CONTENT, LV_SIZE_CONTENT);

  //select number
  label_create[9] = lv_label_create(lv_scr_act());      /*Add a label to the button*/
  lv_label_set_text(label_create[9], "Select Number:"); /*Set the labels text*/
  lv_obj_set_pos(label_create[9], 15, 5);
  lv_obj_set_size(label_create[9], LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  //list
  // lv_obj_t *scr = lv_scr_act();
  ddlist = lv_dropdown_create(lv_scr_act());  // فقط یک آرگومان
  File file = SD.open("/phone_numbers.txt", FILE_READ);
  int count = 0;
  file = SD.open("/phone_numbers.txt", FILE_READ);
  if (!file) {
    Serial.println("Failed to open file!");
    return;
  }
  String dropdown_options = "";
  bool first_line = true;
  while (file.available()) {
    String line = file.readStringUntil('\n');
    if (line.startsWith("number")) {
      if (!first_line) dropdown_options += "\n";  // بین گزینه‌ها فقط `\n` اضافه کن
      dropdown_options += line;
      dropdown_options.trim();  // در صورت نیاز حذف فاصله‌های اضافی از کل رشته نهایی
      first_line = false;
    }
  }
  file.close();
  lv_dropdown_set_options(ddlist, dropdown_options.c_str());
  // lv_dropdown_set_options(ddlist, "sim1: 09372425086\nsim2: 09114764806\nsim3: 09379274959\nsim4:\nsim5:\nsim6:\n");
  lv_obj_set_pos(ddlist, 15, 26);
  lv_obj_set_size(ddlist, 200, LV_SIZE_CONTENT);
  lv_obj_add_event_cb(ddlist, dropdown_event_handler, LV_EVENT_VALUE_CHANGED, NULL);
  //check1
  cb1 = lv_checkbox_create(lv_scr_act());
  lv_checkbox_set_text(cb1, "Auto");
  lv_obj_set_style_text_color(cb1, lv_color_hex(0xFF0000), 0);
  lv_obj_set_pos(cb1, 23, 78);
  lv_obj_add_event_cb(cb1, checkbox_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
  //terminalp
  terminal = lv_textarea_create(lv_scr_act());
  lv_obj_set_size(terminal, 386, 207);  // سایز ترمینال
  lv_obj_set_pos(terminal, 7, 109);
  lv_textarea_set_text(terminal, "Awaiting input...\n");  // متن اولیه
}