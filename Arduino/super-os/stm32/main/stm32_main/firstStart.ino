

//*****************firstStart**************
void firstStart() {
  if (GsmReadyCount == 0 && GsmReadyTimer >= 10) {
    Serial2.println("Check GSM");
    GsmReadyCount = 1;
    GsmReadyTimer = 0;
    Serial3.println("AT");
    delay(50);
    Serial3.println("AT");
    delay(50);
    Serial3.println("AT");
  }
  if (GsmReadyCount == 1 && GsmReadyTimer >= 5) {
    Serial2.println("GSM off");
    digitalWrite(GSMPower, 1);
    delay(2000);
    digitalWrite(GSMPower, 0);
    delay(3000);
    GsmReadyCount = 0;
    GsmReadyTimer = 0;
  }

  if (GsmReadyCount == 2) {
    Serial3.println("AT+CREG?");
    GsmReadyCount = 3;
    GsmReadyTimer = 0;
  }

  if (GsmReadyCount == 3 && GsmReadyTimer >= 4) {
    Serial3.println("AT+CREG?");
    GsmReadyCount = 3;
    GsmReadyTimer = 0;
    Serial2.println("GSM Not Connect");
    ++gsmError;
  }

  if (gsmError >= 4) {
    gsmError = 0;
    Serial2.println("Reset Gsm modem");
    Serial2.println("GSM off");
    digitalWrite(GSMPower, 1);
    delay(2000);
    digitalWrite(GSMPower, 0);
    delay(2000);
    GsmReadyCount = 0;
    GsmReadyTimer = 0;
  }

  if (GsmReadyCount == 4) {
    gsmError = 0;
    Serial2.println("Connected To network");
    GsmReadyCount = 5;
    GsmReadyTimer = 0;
    Serial3.println("AT+COPS?");
  }


  if (GsmReadyCount == 5 && GsmReadyTimer >= 2) {
    Serial2.println("Sim Model Error");
    GsmReadyCount = 5;
    GsmReadyTimer = 0;
    Serial3.println("AT+COPS?");
    gsmError++;
  }

  if (GsmReadyCount == 6) {
    Serial2.println("SimCard OK");
    GsmReadyCount = 7;
    GsmReadyTimer = 0;
    gsmError = 0;
  }


  if (GsmReadyCount == 7) {
    Serial2.println("GSM ON");
    Serial2.println("Config GSM Madule");
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
    Serial3.println("AT+CMGD=1,4");
    GsmReady = 1;
    GsmReadyCount = 8;
    smsReceived = 0;
    GsmReadyTimer = 0;
    Serial2.println("GSM READY && START");
  }
}