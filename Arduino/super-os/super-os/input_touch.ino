
//****lcd_touch_input****
void input_touch() {

  if (tft.getTouch(&x_t, &y_t)) {
    //  tft.fillRoundRect(373, 3, 100, 11, 4, TFT_WHITE);
    // tft.setCursor(375, 5);
    String s1 = "x:" + String(x_t) + "  y:" + String(y_t);
    // tft.setTextFont(1);
    // tft.setTextSize(1);
    // tft.setTextColor(TFT_BLACK, TFT_WHITE);
    // tft.println(s1);
    Serial.print("touch:");
    Serial.println(s1);

  }
}