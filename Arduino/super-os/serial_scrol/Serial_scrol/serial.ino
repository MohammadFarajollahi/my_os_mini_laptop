String sub;
String stm32_string;
void serial_input(){
while (Serial.available() > 0) {
    stm32_string = Serial.readStringUntil('\n');
    stm32_string.trim();
    Serial.println(stm32_string);
    //const char *text_=stm32_string.c_str();
   // lv_list_add_text(panel, text_);
    
    
    //sub = stm32_string.substring(0,11);  
    
  }
}