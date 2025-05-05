



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
  if (GsmReady == 1) {
    if (messageSendCheck == 1 && messageSendTimer >= 10) {
      Serial1.println("sms not send");
      messageSendTimer = 0;
      messageSendCheck = 0;
    }
  }
}


