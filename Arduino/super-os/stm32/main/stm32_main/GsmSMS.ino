
//***************************//
void readSMS() {
  Serial3.println("AT+CMGL=\"ALL\"");
  smsCheck = 1;
}

//******sendSMS*****
void sendSMS(String PhoneNumber, String TextMessage) {
  for (int i = 0; i <= 10; i++) {
    digitalWrite(gsmLed, !digitalRead(gsmLed));
    delay(50);
  }
  String n = "AT+CMGS=\"" + PhoneNumber + "\"";
  Serial3.println(n);
  Serial2.println(n);
  delay(300);
  Serial3.print(TextMessage);
  delay(200);
  Serial3.write(26);
  delay(200);
  Serial3.write(26);
  delay(200);
  Serial3.write(26);
  delay(200);
  Serial2.println("send sms");
  Serial2.println("send sms");
  messageSendCheck = 1;
}