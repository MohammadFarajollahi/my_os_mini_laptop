

void stm32() {

  if (stm32_ready == 0 && stm32_check_timer >= 2) {
    stm32_check_timer = 0;
    stm32_serial.println("stm32-check");
  }

  check_uart_stm32();
}


void check_uart_stm32() {
  while (stm32_serial.available() > 0) {
    stm32_string = stm32_serial.readStringUntil('\n');
    stm32_string.trim();
    Serial.println(stm32_string);
    //***stm32_ready***
    sub_stm32_string = stm32_string.substring(0, 11);
    if (stm32_string == "stm32_ready") {
      Serial.println("stm ready");
      stm32_ready = 1;
      check_stm32_count = 0;
      stm32_check_timer = 0;
    }

  }
}