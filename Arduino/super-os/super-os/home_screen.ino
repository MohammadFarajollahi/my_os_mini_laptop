


void home_screen() {
  input_touch();
  if (change_menu == 1) {
    change_menu = 0;
    Serial.println("main menu start ...");
    tft.fillScreen(TFT_WHITE);
    tft.drawJpgFile(SD, "/background/back1.jpg", 0, 0);
  }

  if (x_t > 10 && x_t < 90 && y_t > 230 && y_t < 300) {
    x_t = 0;
    y_t = 0;
    change_menu = 1;
    menu_select = 2;
  }

  if (x_t > 10 && x_t < 90 && y_t > 10 && y_t < 90) {
    x_t = 0;
    y_t = 0;
    cash_size();
    print_memory_usage();
    delay(500);
  }
  /////
}