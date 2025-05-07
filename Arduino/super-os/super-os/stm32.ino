

void stm32() {

  if (stm32_ready == 0 && stm32_check_timer >= 2) {  //stm32 Check
    stm32_check_timer = 0;
    stm32_serial.println("stm32-check");
    lcd_show2("STM32 Check...");
  }

  if (stm32_ready == 1 && Network_check_timer >= 5 && Network_ready == 0) {  //network Check
    Network_check_timer = 0;
    stm32_serial.println("network-check");
    lcd_show2("network Check...");
  }

  check_uart_stm32();
}


void check_uart_stm32() {
  while (stm32_serial.available() > 0) {
    stm32_string = stm32_serial.readStringUntil('\n');
    stm32_string.trim();
    Serial.println(stm32_string);
   // lcd_show2(stm32_string);
    //******stm32_ready******
    if (stm32_string == "stm32_ready") {
      Serial.println("stm ready");
      lcd_show2("STM32 ready");
      stm32_ready = 1;
      check_stm32_count = 0;
      stm32_check_timer = 0;
    }
    //******Network_ready*******
    if (stm32_string == "network_ready") {
      Serial.println("Network ready");
      lcd_show2("***network Ready***");
      Network_ready = 1;
      check_Network_count = 0;
      Network_check_timer = 0;
      lv_obj_del(label_create[5]);
      free((void *)img_dsc[5].data);  //main free cach
      set_label(5, 80, 80, 265, -20, "/desktop_pic/anten.bmp");
    }
    sub_stm32_string = stm32_string.substring(0, 17);
    if (sub_stm32_string == "network not ready") {
      Serial.println("network not ready");
      lcd_show2("network Not Ready");
      Network_ready = 0;
      check_Network_count = 0;
      Network_check_timer = 0;
      lv_obj_del(label_create[6]);
      free((void *)img_dsc[6].data);  //main free cach
      set_label(6, 80, 80, 265, -20, "/desktop_pic/anten2.bmp");
    }
  }
}