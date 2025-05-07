
void PavanProductTester() {

  if (pavanCheckTimer >= 10 && PavanCountCheck == 0) {
    lcd_show2("Number1 SMS Send...");
    pavanCheckTimer = 0;
    PavanCountCheck = 1;
    ///sms
    StaticJsonDocument<200> jsonDoc;
    jsonDoc["type"] = "sendSMS";                 // نوع دیتا (مثلاً ارسال پیامک)
    jsonDoc["phone"] = NumberTest[PavanSMSNum];  // شماره تلفن
    jsonDoc["message"] = "BT 1111";              // متن پیام
    jsonDoc["extra"] = "urgent";                 // هر دیتای اضافی
    String jsonString;
    serializeJson(jsonDoc, jsonString);
    Serial.println(jsonString);
    lcd_show2(jsonString);
    stm32_serial.println(jsonString);
    ////
  }
}