

void Gsm_check() {

  if (GsmReady == 0) {
    if (GsmData == "OK" && GsmReadyCount == 1) GsmReadyCount = 2;
    if (GsmData == "+CREG: 0,1" && GsmReadyCount == 3) GsmReadyCount = 4;
    sub1 = GsmData.substring(12, 18);  ///+COPS: 0,0,"IR-TCI",7
    if (GsmReadyCount == 5) {
      if (sub1 == "IR-TCI" || sub1 == "IR-MCI") {  //
        simModel = 2;
        Serial2.println("Sim Model:MCI");
        GsmReadyCount = 6;
      }
    }
  }

  //******READ SMS*****
  sub1 = GsmData.substring(0, 5);  ///+CMTI: "ME",0
  if (sub1 == "+CMTI") {
    readSMS();
  }

  sub1 = GsmData.substring(0, 5);  ///+CMGL: 0,"REC UNREAD","+989114764806",,"25/05/07,01:52:03+14"
  if (sub1 == "+CMGL") {
    sub1 = GsmData.substring(10, 20);  ///+CMGL: 0,"REC UNREAD","+989114764806",,"25/05/07,01:52:03+14"
    if (sub1 == "REC UNREAD") {
      InputNumber = GsmData.substring(23, 36);
      MessNum = GsmData.substring(7, 8);
      inputMessage = Serial3.readStringUntil('\n');
      Serial2.print("Message Number:");
      Serial2.println(MessNum);
      Serial2.print("Input Number:");
      Serial2.println(InputNumber);
      Serial2.print("Messsage:");
      Serial2.println(inputMessage);
      ///////////////////////////////////
      StaticJsonDocument<200> jsonDoc;
      jsonDoc["type"] = "ReciveSMS";      // نوع دیتا (مثلاً ارسال پیامک)
      jsonDoc["phone"] = InputNumber;     // شماره تلفن
      jsonDoc["message"] = inputMessage;  // متن پیام
      jsonDoc["extra"] = "urgent";        // هر دیتای اضافی
      String jsonString;
      serializeJson(jsonDoc, jsonString);
      Serial2.println(jsonString);
      Serial1.println(jsonString);
      //////////////////////////////////////
      smsCheckCount = 0;
      smsCheck = 0;
      SMScheckTimer = 0;
      // Serial3.println("AT+CMGD=1,4");
      String s = "AT+CMGD=" + MessNum;
      Serial3.println(s);
      delay(1000);
      Serial3.println("AT+CSMP=17,167,0,0");
      delay(500);
      Serial3.println("AT+CSCS=\"GSM\"");
      delay(500);
      SMScheckTimer = 0;
      smsCheck = 1;
    }
    sub1 = GsmData.substring(10, 18);  ///+CMGL: 0,"REC READ","+989114764806",,"25/05/07,01:52:03+14"
    if (sub1 == "REC READ") {
      InputNumber = GsmData.substring(21, 34);
      MessNum = GsmData.substring(7, 8);
      inputMessage = Serial3.readStringUntil('\n');
      Serial2.print("Message Number:");
      Serial2.println(MessNum);
      Serial2.print("Input Number:");
      Serial2.println(InputNumber);
      Serial2.print("Messsage:");
      Serial2.println(inputMessage);
      ///////////////////////////////////
      StaticJsonDocument<200> jsonDoc;
      jsonDoc["type"] = "ReciveSMS";      // نوع دیتا (مثلاً ارسال پیامک)
      jsonDoc["phone"] = InputNumber;     // شماره تلفن
      jsonDoc["message"] = inputMessage;  // متن پیام
      jsonDoc["extra"] = "urgent";        // هر دیتای اضافی
      String jsonString;
      serializeJson(jsonDoc, jsonString);
      Serial2.println(jsonString);
      Serial1.println(jsonString);
      //////////////////////////////////////
      smsCheckCount = 0;
      smsCheck = 0;
      SMScheckTimer = 0;
      // Serial3.println("AT+CMGD=1,4");
      String s = "AT+CMGD=" + MessNum;
      Serial3.println(s);
      delay(1000);
      Serial3.println("AT+CSMP=17,167,0,0");
      delay(500);
      Serial3.println("AT+CSCS=\"GSM\"");
      delay(500);
      SMScheckTimer = 0;
      smsCheck = 1;
    }
  }

  //******SMS SEND********
  sub1 = GsmData.substring(0, 5);  ///+CMGS: 112
  if (sub1 == "+CMGS") {
    messageSendCheck = 0;
    messageSendTimer = 0;
    Serial1.println("sms send");
    Serial2.println("**sms send**");
  }
  //*******SMS ERROR********
  if (GsmData == "+CMS ERROR: 304") {
    Serial2.println("sms storage full");
    Serial3.println("AT+CMGD=1,4");
    delay(500);
    Serial3.println("AT+CMGF=1");
    delay(100);
    Serial3.println("AT+CSMP=17,167,0,0");
    delay(100);
    Serial3.println("AT+CSCS=\"GSM\"");
    delay(100);
    Serial3.println("ATE0");
    delay(100);
    Serial3.println("AT&W");
    delay(100);
    Serial1.println("gsm error");
  }
}