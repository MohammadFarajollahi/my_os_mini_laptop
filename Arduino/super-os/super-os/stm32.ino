

void stm32() {

  if (stm32_ready == 0 && stm32_check_timer >= 2) {  //stm32 Check
    stm32_check_timer = 0;
    stm32_serial.println("stm32-check");
    if (menu_select == "desktop") lcd_show2("STM32 Check...");
  }

  if (stm32_ready == 1 && Network_check_timer >= 20 && Network_ready == 0) {  //network Check
    Network_check_timer = 0;
    stm32_serial.println("network-check");
    if (menu_select == "desktop") lcd_show2("network Check...");
  }

  //************Connettion Chek************
  if (PavanCountCheck == 0 && stm32checkTimer >= 85 && stm32checkCount == 0) {
    stm32_serial.println("checkStm32");
    stm32checkTimer = 0;
    stm32checkCount = 1;
  }

  if (PavanCountCheck == 0 && stm32checkTimer >= 3 && stm32checkCount == 1) {
    stm32_serial.println("checkStm32 Again");
    stm32checkTimer = 80;
    stm32checkCount = 0;
    ++checkError;
    if (checkError > 3) ESP.restart();  // ریست نرم‌افزاری
    if (menu_select == "desktop") lcd_show2("Stm32 not Response Chek Again....");
  }

  check_uart_stm32();
}


void check_uart_stm32() {
  while (stm32_serial.available() > 0) {
    stm32_string = stm32_serial.readStringUntil('\n');
    stm32_string.trim();
    Serial.println(stm32_string);
    if (menu_select == "server" && lcdShow == 1) lcd_show2(stm32_string);
    if (menu_select == "SerialMonitor") lcd_show(stm32_string);
    //lcd_show2(stm32_string);
    jasonCheck();
    //******stm32_ready******
    if (stm32_string == "stm32_ready") {
      Serial.println("stm ready");
      if (menu_select == "desktop") lcd_show2("STM32 ready");
      stm32_ready = 1;
      check_stm32_count = 0;
      stm32_check_timer = 0;
    }
    if (stm32_string == "stm32 ok") {
      Serial.println("stm OK");
      //if (menu_select == "desktop") lcd_show2("STM32 OK");
      stm32checkTimer = 0;
      stm32checkCount = 0;
      checkError = 0;
    }

    //************************sms Check*********************
    if (stm32_string == "sms send" && PavanCountCheck == 1) {
      chekSMScount = 2;
      cheksmsTimer = 0;
      SendError = 0;
      Serial.println("sms Send --->> level2");
      lcd_show2("***SMS SEND***");
    }
    if (stm32_string == "wait for Send..." && chekSMScount == 1) {
      lcd_show2("Response ok");
    }
    /////
    if (stm32_string == "sms not send" && PavanCountCheck == 1) {
      chekSMScount = 1;
      cheksmsTimer = 20;
      SendError = 1;
      Serial.println("sms Not Send --->> level1");
    }
    if (stm32_string == "SMS ERROR" && PavanCountCheck == 1) {
      chekSMScount = 1;
      cheksmsTimer = 20;
      SendError = 1;
      Serial.println("sms Not Send --->> level1");
    }

    //*************************************************************network_ready*************************************************************************
    //*************************************************************network_ready*************************************************************************
    //*************************************************************network_ready*************************************************************************
    if (stm32_string == "network_ready") {
      Serial.println("Network ready");
      if (menu_select == "desktop") lcd_show2("***network Ready***");
      Network_ready = 1;
      check_Network_count = 0;
      Network_check_timer = 0;
      if (menu_select == "desktop") {
        lv_obj_del(label_create[5]);
        free((void *)img_dsc[5].data);  //main free cach
        set_label(5, 80, 80, 265, -20, "/desktop_pic/anten.bmp");
      }
    }
    sub_stm32_string = stm32_string.substring(0, 17);
    if (sub_stm32_string == "network not ready") {
      Serial.println("network not ready");
      if (menu_select == "desktop") lcd_show2("network Not Ready");
      Network_ready = 0;
      check_Network_count = 0;
      Network_check_timer = 0;
      if (menu_select == "desktop") {
        lv_obj_del(label_create[6]);
        free((void *)img_dsc[6].data);  //main free cach
        set_label(6, 80, 80, 265, -20, "/desktop_pic/anten2.bmp");
      }
    }

    //*************************************************************clock*************************************************************************
    //*************************************************************clock*************************************************************************
    //*************************************************************clock*************************************************************************
    if (stm32_string.startsWith("clockRead:")) {
      String input = "";
      input = stm32_string.substring(String("clockRead:").length());

      int values[7];
      int idx = 0;
      char *token = strtok((char *)input.c_str(), ",");
      while (token != NULL && idx < 7) {
        values[idx++] = atoi(token);
        token = strtok(NULL, ",");
      }

      if (idx == 7) {
        date_Year = values[0];
        date_month = values[1];
        date_day = values[2];
        clock_hour = values[3];
        clock_minute = values[4];
        clock_second = values[5];
        weekday = values[6];
        Day_ = weekDays[weekday];

        Serial.print("⏰ ساعت: ");
        print2Digit(clock_hour);
        Serial.print(":");
        print2Digit(clock_minute);
        Serial.print(":");
        print2Digit(clock_second);
        Serial.println();

        Serial.print("📅 تاریخ شمسی: ");
        Serial.print(date_Year);
        Serial.print("/");
        print2Digit(date_month);
        Serial.print("/");
        print2Digit(date_day);
        Serial.println();

        Serial.print("📌 روز هفته: ");
        Serial.println(weekDays[weekday]);
        requestClock = 0;
        requestClockTimer = 0;
      }
    }


    ////
  }
}

void print2Digit(int num) {
  if (num < 10) Serial.print("0");
  Serial.print(num);
}


void jasonCheck() {
  String jsonString = stm32_string;
  StaticJsonDocument<200> jsonDoc;
  DeserializationError error = deserializeJson(jsonDoc, jsonString);
  String type = jsonDoc["type"];
  String phone = jsonDoc["phone"];
  String message = jsonDoc["message"];
  String extra = jsonDoc["extra"];
  // Serial.println("type: " + type);
  // Serial.println("number: " + phone);
  // Serial.println("message: " + message);
  // Serial.println("extra: " + extra);

  if (type == "ReciveSMS") {
    lcd_show2("SMS:" + phone);
    lcd_show2("Message:" + message);

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
      //***Server check***
      if (message == "check") {
        StaticJsonDocument<200> jsonDoc;
        jsonDoc["type"] = "sendSMS";   // نوع دیتا (مثلاً ارسال پیامک)
        jsonDoc["phone"] = phone;      // شماره تلفن
        jsonDoc["message"] = "Im OK";  // متن پیام
        jsonDoc["extra"] = "urgent";   // هر دیتای اضافی
        String jsonString;
        serializeJson(jsonDoc, jsonString);
        Serial.println(jsonString);
        stm32_serial.println(jsonString);
      }
    }
  }

  //
}