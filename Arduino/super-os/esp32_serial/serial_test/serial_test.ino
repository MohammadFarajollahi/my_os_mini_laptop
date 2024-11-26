

HardwareSerial stm32_serial(1);
String stm32_string , sub_stm32_string;
void setup() {
  stm32_serial.begin(115200,SERIAL_8N1 ,42,45);
  Serial.begin(115200);
  Serial.println("Start");
  delay(1000);
  stm32_serial.println("check_stm32");
}

void loop() {
   while (stm32_serial.available() > 0) {
    stm32_string = stm32_serial.readStringUntil('\n');
    stm32_string.trim();
    Serial.println(stm32_string);
    sub_stm32_string = stm32_string.substring(0,11);  
    if (stm32_string == "stm32_ready") {
     Serial.println("stm ready");
    }
  }

}
