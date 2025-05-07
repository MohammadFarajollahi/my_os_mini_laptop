
void gsm() {
  while (Serial3.available() > 0) {
    GsmData = Serial3.readStringUntil('\n');
    GsmData.trim();
    Serial2.println(GsmData);
    Gsm_check();
    GsmData = "";
  }


  if (smsReceived == 1 && GsmReady == 1) {
    smsReceived = 0;  // ریست کردن فلگ
    readSMS();        // تابع خواندن پیامک
    Serial2.println("sms");
  }


  if (GsmReady == 0) firstStart();
  //***********************GSM Ready***********************
  if (GsmReady == 1) {
    if (messageSendCheck == 1 && messageSendTimer >= 10) {
      Serial1.println("sms not send");
      messageSendTimer = 0;
      messageSendCheck = 0;
    }


    ////////sms Check/////////
    if (smsCheck == 1 && SMScheckTimer >= 2) {
      SMScheckTimer = 0;
      ++smsCheckCount;
      Serial3.println("AT+CMGL=\"ALL\"");
      Serial2.println("Read SMS again...");

      if (smsCheckCount >= 3) {
        smsCheckCount = 0;
        smsCheck = 0;
        SMScheckTimer = 0;
        Serial2.println("No SMS");
        Serial2.println("Dellet All Message");
        Serial3.println("AT+CMGD=1,4");
        delay(1000);
        Serial3.println("AT+CSMP=17,167,0,0");
        delay(500);
        Serial3.println("AT+CSCS=\"GSM\"");
        delay(500);
      }
    }
  }
}
