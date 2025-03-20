
static char selected_item[64];  // بافر برای ذخیره آیتم انتخاب شده
lv_obj_t *cb1, *cb2, *cb3;
int check1;
void server() {
  if (change_menu == 1) {
    change_menu = 0;
    Serial.println("Create SerialMonitor...");
    create_server();
    main_timer = 0;
    main_sec = 0;
    batt_sec = 0;
  }
}

void btn_xl(lv_event_t *e) {
  Serial.println("btn xl");
}


void btn_l(lv_event_t *e) {
  Serial.println("btn l");
}

void btn_on1(lv_event_t *e) {
  Serial.println("btn on");
}

void btn_off(lv_event_t *e) {
  Serial.println("btn off");
}

void btn_s1(lv_event_t *e) {
  Serial.println("btn s1");
}

void btn_s2(lv_event_t *e) {
  Serial.println("btn s2");
}

void btn_bt(lv_event_t *e) {
  Serial.println("btn BT");
}

void btn_r(lv_event_t *e) {
  Serial.println("btn R");
}


// تابع handler برای واکنش به تغییر انتخاب
static void dropdown_event_handler(lv_event_t *e) {
  lv_obj_t *ddlist = lv_event_get_target(e);

  // دریافت انتخاب انجام‌شده از Drop-Down
  lv_dropdown_get_selected_str(ddlist, selected_item, sizeof(selected_item));

  // نمایش انتخاب در سریال (برای مثال)
  Serial.print("آیتم انتخاب شده: ");
  Serial.println(selected_item);  // در اینجا از selected_item استفاده می‌شود که نوع آن char[] است
  String ss1 = selected_item;
  // if (ss1 == "250ms") sample_time = 1;
  // if (ss1 == "500ms") sample_time = 2;
  // if (ss1 == "1sec") sample_time = 3;
  // if (ss1 == "5sec") sample_time = 4;
  // if (ss1 == "10sec") sample_time = 5;
  // if (ss1 == "20sec") sample_time = 6;
  // if (sample_time == 1) count_ok = 50;
  // if (sample_time == 2) count_ok = 100;
  // if (sample_time == 3) count_ok = 200;
  // if (sample_time == 4) count_ok = 1000;
  // if (sample_time == 5) count_ok = 2000;
  // if (sample_time == 6) count_ok = 4000;
  Serial.println(ss1);
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

void create_server() {
  //btn_xl
  btn[0] = lv_btn_create(lv_scr_act());
  lv_obj_set_size(btn[0], 76, 41);
  lv_obj_set_pos(btn[0], 399, 272);
  lv_obj_add_event_cb(btn[0], btn_xl, LV_EVENT_CLICKED, NULL);
  label_create[0] = lv_label_create(btn[0]); /*Add a label to the button*/
  lv_label_set_text(label_create[0], "XL");  /*Set the labels text*/
  lv_obj_center(label_create[0]);
  //btn_l
  btn[1] = lv_btn_create(lv_scr_act());
  lv_obj_set_size(btn[1], 76, 41);
  lv_obj_set_pos(btn[1], 399, 219);
  lv_obj_add_event_cb(btn[1], btn_l, LV_EVENT_CLICKED, NULL);
  label_create[1] = lv_label_create(btn[1]); /*Add a label to the button*/
  lv_label_set_text(label_create[1], "L");   /*Set the labels text*/
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
  label_create[4] = lv_label_create(btn[4]); /*Add a label to the button*/
  lv_label_set_text(label_create[4], "S1");  /*Set the labels text*/
  lv_obj_center(label_create[4]);
  //S2
  btn[5] = lv_btn_create(lv_scr_act());
  lv_obj_set_size(btn[5], 76, 41);
  lv_obj_set_pos(btn[5], 399, 5);
  lv_obj_add_event_cb(btn[5], btn_s2, LV_EVENT_CLICKED, NULL);
  label_create[5] = lv_label_create(btn[5]); /*Add a label to the button*/
  lv_label_set_text(label_create[5], "S2");  /*Set the labels text*/
  lv_obj_center(label_create[5]);
  //bt
  btn[6] = lv_btn_create(lv_scr_act());
  lv_obj_set_size(btn[6], 76, 41);
  lv_obj_set_pos(btn[6], 315, 5);
  lv_obj_add_event_cb(btn[6], btn_bt, LV_EVENT_CLICKED, NULL);
  label_create[6] = lv_label_create(btn[6]); /*Add a label to the button*/
  lv_label_set_text(label_create[6], "BT");  /*Set the labels text*/
  lv_obj_center(label_create[6]);
  //r
  btn[7] = lv_btn_create(lv_scr_act());
  lv_obj_set_size(btn[7], 76, 41);
  lv_obj_set_pos(btn[7], 315, 57);
  lv_obj_add_event_cb(btn[7], btn_r, LV_EVENT_CLICKED, NULL);
  label_create[7] = lv_label_create(btn[7]); /*Add a label to the button*/
  lv_label_set_text(label_create[7], "R");   /*Set the labels text*/
  lv_obj_center(label_create[7]);
  //l1
  label_create[8] = lv_label_create(lv_scr_act());      /*Add a label to the button*/
  lv_label_set_text(label_create[8], "Select Number:"); /*Set the labels text*/
  lv_obj_set_pos(label_create[8], 15, 5);
  lv_obj_set_size(label_create[8], LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  //list
  lv_obj_t *scr = lv_scr_act();
  lv_obj_t *ddlist = lv_dropdown_create(scr);  // فقط یک آرگومان
  lv_dropdown_set_options(ddlist, "sim1: 09372425086\nsim2: 09114764806\nsim3: 09379274959\nsim4:\nsim5:\nsim6:");
  lv_obj_set_pos(ddlist, 15, 26);
  lv_obj_set_size(ddlist, 200, LV_SIZE_CONTENT);
  lv_obj_add_event_cb(ddlist, dropdown_event_handler, LV_EVENT_VALUE_CHANGED, NULL);
  //check1
  cb1 = lv_checkbox_create(lv_scr_act());
  lv_checkbox_set_text(cb1, "Auto");
  lv_obj_set_style_text_color(cb1, lv_color_hex(0xFF0000), 0);
  lv_obj_set_pos(cb1, 23, 78);
  lv_obj_add_event_cb(cb1, checkbox_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
  //terminal
  terminal = lv_textarea_create(lv_scr_act());
  lv_obj_set_size(terminal, 386, 207);  // سایز ترمینال
  lv_obj_set_pos(terminal, 7, 109);
  lv_textarea_set_text(terminal, "Awaiting input...\n");  // متن اولیه
}