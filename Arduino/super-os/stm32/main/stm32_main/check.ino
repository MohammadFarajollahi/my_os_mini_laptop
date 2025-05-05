

void check_() {

  //****check for ready****
  if (inputData == "stm32-check") {
    Serial1.println("stm32_ready");
    Serial2.println("esp32 check for ready");
  }

  //****go to serial mode****
  if (inputData == "Serial_mode") {
    serialMode = 1;
    Serial1.println("stm32_serial_mode");
    Serial2.println("goto Serial mode");
  }

  //****exit serial mode****
  if (inputData == "exit serial") {
    serialMode = 0;
    Serial1.println("exit serial mode");
    Serial2.println("exit Serial mode");
  }

  //****atcommand mode****
  if (inputData == "atcommand-mode on") {
    Serial1.println("stm32-atcommandmode on");
    Serial2.println("stm32-atcommandmode on");
    AtCommandMode = 1;
  }

  if (inputData == "atcommand-mode off") {
    Serial1.println("stm32-atcommandmode off");
    Serial2.println("stm32-atcommandmode off");
    AtCommandMode = 0;
  }

  if (AtCommandMode == 1) {  //atcommand  for sed to 4G madule
    sub1 = inputData.substring(0, 9);
    if (sub1 == "atcommand") {
      sub1 = inputData.substring(9, 50);
      Serial2.println(sub1);
    }
  }

  //change buad_rate
  sub1 = inputData.substring(0, 8);  ///baudrate
  if (sub1 == "baudrate") {
    Serial2.flush();
    delay(100);
    Serial2.end();
    delay(100);
    if (inputData == "baudrate:9600") {
      Serial1.println("buadrate 9600");
      Serial2.begin(9600);
    }
    if (inputData == "baudrate:115200") {
      Serial1.println("buadrate 115200");
      Serial2.begin(115200);
    }
    if (inputData == "baudrate:19200") {
      Serial1.println("buadrate 19200");
      Serial2.begin(19200);
    }
    if (inputData == "baudrate:38400") {
      Serial1.println("buadrate 38400");
      Serial2.begin(38400);
    }
    if (inputData == "baudrate:57600") {
      Serial1.println("buadrate 57600");
      Serial2.begin(57600);
    }


    /////
  }

  
}