void timer_(void) {
  iwdg_feed();
  digitalWrite(led, !digitalRead(led));

  if (GsmReady == 0) {
    ++GsmReadyTimer;
    digitalWrite(gsmLed, !digitalRead(gsmLed));
  }

  if (GsmReady == 1){
    digitalWrite(gsmLed, 1);
    if(messageSendCheck == 1)++messageSendTimer;
  }
}