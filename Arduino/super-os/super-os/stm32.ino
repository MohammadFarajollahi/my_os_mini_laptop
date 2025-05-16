

void stm32() {

  if (stm32_ready == 0 && stm32_check_timer >= 2) {  //stm32 Check
    stm32_check_timer = 0;
    stm32_serial.println("stm32-check");
    lcd_show2("STM32 Check...");
  }

  if (stm32_ready == 1 && Network_check_timer >= 20 && Network_ready == 0) {  //network Check
    Network_check_timer = 0;
    stm32_serial.println("network-check");
    lcd_show2("network Check...");
  }

  check_uart_stm32();
}


void check_uart_stm32() {
  while (stm32_serial.available() > 0) {
    stm32_string = stm32_serial.readStringUntil('\n');
    stm32_string.trim();
    Serial.println(stm32_string);
    ///lcd_show2(stm32_string);
    jasonCheck();
    //******stm32_ready******
    if (stm32_string == "stm32_ready") {
      Serial.println("stm ready");
      lcd_show2("STM32 ready");
      stm32_ready = 1;
      check_stm32_count = 0;
      stm32_check_timer = 0;
    }

    //****sms Check*****
    if (stm32_string == "sms send" && PavanCountCheck == 1) {
      chekSMScount = 2;
      cheksmsTimer = 0;
      Serial.println("sms Send --->> level2");
    }
    if (stm32_string == "sms not send" && PavanCountCheck == 1) {
      chekSMScount = 1;
      cheksmsTimer = 20;
      Serial.println("sms Not Send --->> level1");
    }
    //******Network_ready*******
    if (stm32_string == "network_ready") {
      Serial.println("Network ready");
      lcd_show2("***network Ready***");
      Network_ready = 1;
      check_Network_count = 0;
      Network_check_timer = 0;
      lv_obj_del(label_create[5]);
      free((void *)img_dsc[5].data);  //main free cach
      set_label(5, 80, 80, 265, -20, "/desktop_pic/anten.bmp");
    }
    sub_stm32_string = stm32_string.substring(0, 17);
    if (sub_stm32_string == "network not ready") {
      Serial.println("network not ready");
      lcd_show2("network Not Ready");
      Network_ready = 0;
      check_Network_count = 0;
      Network_check_timer = 0;
      lv_obj_del(label_create[6]);
      free((void *)img_dsc[6].data);  //main free cach
      set_label(6, 80, 80, 265, -20, "/desktop_pic/anten2.bmp");
    }
  }
}




void jasonCheck() {
  String jsonString = stm32_string;

  // تبدیل رشته JSON به یک آبجکت
  StaticJsonDocument<200> jsonDoc;
  DeserializationError error = deserializeJson(jsonDoc, jsonString);

  // if (error) {
  //   Serial2.println("خطا در پارس کردن JSON!");
  //   return;
  // }

  // استخراج داده‌ها از JSON
  String type = jsonDoc["type"];
  String phone = jsonDoc["phone"];
  String message = jsonDoc["message"];
  String extra = jsonDoc["extra"];

  // نمایش در سریال مانیتور
  Serial.println("type: " + type);
  Serial.println("number: " + phone);
  Serial.println("message: " + message);
  Serial.println("extra: " + extra);



  if (type == "ReciveSMS") {
    if (chekSMScount == 2) {
      if (phone == NumberTest[PavanSMSNum].substring(0, 13)) chekSMScount = 3;
    }


    //***Server Check***
    if (phone == "+989114764806" || phone == "+989372425086") {
      Serial.print("Message Frome:");
      Serial.println("Mohammad hamrah aval");
      Serial.print("Message:");
      Serial.println(message);
      delay(3000);

      StaticJsonDocument<200> jsonDoc;
      jsonDoc["type"] = "sendSMS";     // نوع دیتا (مثلاً ارسال پیامک)
      jsonDoc["phone"] = phone;        // شماره تلفن
      jsonDoc["message"] = "Im OK";  // متن پیام
      jsonDoc["extra"] = "urgent";     // هر دیتای اضافی
      String jsonString;
      serializeJson(jsonDoc, jsonString);
      Serial.println(jsonString);
      stm32_serial.println(jsonString);
    }
  }
}