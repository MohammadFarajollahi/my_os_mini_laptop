
void PavanProductTester() {
  stm32();

  if (pavanCheckTimer >= smsChekTime && PavanCountCheck == 0) {
    lcd_show2("Number1 SMS Send...");
    Serial.println("Number1 SMS Send --->> level1");
    pavanCheckTimer = 0;
    PavanCountCheck = 1;
    ///sms
    StaticJsonDocument<200> jsonDoc;
    jsonDoc["type"] = "sendSMS";  // نوع دیتا (مثلاً ارسال پیامک)
    //jsonDoc["phone"] = NumberTest[PavanSMSNum];  // شماره تلفن
    jsonDoc["phone"] = NumberTest[PavanSMSNum].substring(0, 13);  ///+989372425086
    jsonDoc["message"] = "BT 1111";                               // متن پیام
    jsonDoc["extra"] = "urgent";                                  // هر دیتای اضافی
    String jsonString;
    serializeJson(jsonDoc, jsonString);
    Serial.println(jsonString);
    lcd_show2(jsonString);
    stm32_serial.println(jsonString);
    chekSMScount = 1;
    cheksmsTimer = 0;
    ////
  }


  if (chekSMScount == 3) {  //****sms done****
    StorageServer();
    lcd_show2("Next Number");
    PavanCountCheck = 0;
    pavanCheckTimer = smsChekTime - 30;
    cheksmsTimer = 0;
    chekSMScount = 0;
    reciveError = 0;
    ++PavanSMSNum;
    if (PavanSMSNum >= FoundNumbers) {
      PavanSMSNum = 0;
      lcd_show2("First Number...");
      pavanCheckTimer = 0;
    }
  }
  //*****error******
  if (chekSMScount == 1 && cheksmsTimer >= 30) {
    lcd_show2("sms not Send");
    chekSMScount = 0;
    cheksmsTimer = 0;
    pavanCheckTimer = smsChekTime - 30;
    PavanCountCheck = 0;
    ++reciveError;
  }

  if (chekSMScount == 2 && cheksmsTimer >= 30) {
    lcd_show2("sms not Recive!!!");
    chekSMScount = 0;
    cheksmsTimer = 0;
    pavanCheckTimer = 280;
    PavanCountCheck = 0;
    ++reciveError;
  }

  if (reciveError >= 3) {
    StorageServer();
    lcd_show2("sms not Recive from:");
    lcd_show2(NumberTest[PavanSMSNum]);
    lcd_show2("Next Number");
    PavanCountCheck = 0;
    pavanCheckTimer = smsChekTime - 30;
    cheksmsTimer = 0;
    chekSMScount = 0;
    reciveError = 0;
    ++PavanSMSNum;
    if (PavanSMSNum >= FoundNumbers) {
      PavanSMSNum = 0;
      lcd_show2("First Number...");
      pavanCheckTimer = 0;
    }
  }
}


void StorageServer() {
  //**********************************storage*****************************
  FileServer = "/server_History/" + NumberTest[PavanSMSNum].substring(0, 13) + ".txt";
  readSdCard(FileServer.c_str());
  String jsonString = fileContent;
  StaticJsonDocument<200> jsonDoc;
  DeserializationError error = deserializeJson(jsonDoc, jsonString);
  String servertime = jsonDoc["servertime"];
  String numberTest = jsonDoc["numberTest"];
  String sendTest = jsonDoc["sendTest"];
  String sucsses = jsonDoc["sucsses"];
  if (numberTest == NumberTest[PavanSMSNum].substring(0, 13)) {
    Serial.println("Save history...");
    lcd_show2("Save history...");
    int SENDTest = sendTest.toInt() + 1;
    int Sucsses;
    if (reciveError < 3) Sucsses = sucsses.toInt() + 1;
    if (reciveError >= 3) Sucsses = sucsses.toInt();
    String TEXTS = "D:" + String(date_Year) + "/" + String(date_month) + "/" + String(date_day) + " T:" + String(clock_hour) + ":" + String(clock_minute) + ":" + String(clock_second);
    StaticJsonDocument<200> jsonDoc;
    jsonDoc["servertime"] = TEXTS;
    jsonDoc["numberTest"] = NumberTest[PavanSMSNum].substring(0, 13);  ///+989372425086
    jsonDoc["sendTest"] = String(SENDTest);                            // متن پیام
    jsonDoc["sucsses"] = String(Sucsses);                              // هر دیتای اضافی
    String jsonString;
    serializeJson(jsonDoc, jsonString);
    Serial.println(jsonString);
    writeSdCard(FileServer.c_str(), jsonString);
  }
  if (numberTest != NumberTest[PavanSMSNum].substring(0, 13)) {
    Serial.println("First sd save...");
    lcd_show2("First SD Save...");
    String TEXTS = "D:" + String(date_Year) + "/" + String(date_month) + "/" + String(date_day) + " T:" + String(clock_hour) + ":" + String(clock_minute) + ":" + String(clock_second);
    StaticJsonDocument<200> jsonDoc;
    jsonDoc["servertime"] = TEXTS;
    jsonDoc["numberTest"] = NumberTest[PavanSMSNum].substring(0, 13);  ///+989372425086
    jsonDoc["sendTest"] = "1";                                         // متن پیام
    jsonDoc["sucsses"] = "1";                                          // هر دیتای اضافی
    String jsonString;
    serializeJson(jsonDoc, jsonString);
    Serial.println(jsonString);
    writeSdCard(FileServer.c_str(), jsonString);
  }
  /////////////////////////////////////////////////////////////
}