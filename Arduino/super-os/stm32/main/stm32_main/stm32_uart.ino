String inputData2;
int input_data2;


//uart1 STM32<---->ESP32
//uart2 STM32<---->TTL
void stm32_uart1() {
  while (Serial1.available() > 0) {
    inputData = Serial1.readStringUntil('\n');
    inputData.trim();
    Serial2.println(inputData);
    check_();
    jasonCheck();
    inputData = "";
    input_data = 0;
  }
}

void stm32_uart2() {
  while (Serial2.available() > 0) {
    inputData2 = Serial2.readStringUntil('\n');
    inputData2.trim();

    if (p2p == 1) {
      Serial3.println(inputData2);
    }
    if (inputData2 == "p2p on" || inputData2 == "P2P ON") {
      Serial2.println("p2p turned on");
      p2p = 1;
    }

    Serial2.println(inputData2);
    if (serialMode == 1) Serial1.println(inputData2);
    inputData2 = "";
    input_data2 = 0;
  }
}