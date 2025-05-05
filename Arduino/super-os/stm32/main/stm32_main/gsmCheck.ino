

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

  sub1 = GsmData.substring(0, 5);  ///+CMGS: 112
  if (sub1 == "+CMGS") {
    messageSendCheck = 0;
    messageSendTimer = 0;
    Serial1.println("sms send");
    Serial2.println("**sms send**");
  }
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
    // Serial2.println("GSM ERROR");
    Serial1.println("gsm error");
    // digitalWrite(GSMPower, 1);
    // delay(2000);
    // digitalWrite(GSMPower, 0);
    // delay(2000);
    // GsmReadyCount = 0;
    // GsmReadyTimer = 0;
    // GsmReady = 0;
  }
}