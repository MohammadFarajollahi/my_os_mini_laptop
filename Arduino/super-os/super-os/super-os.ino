// //*******sd_card********
// #if defined(ARDUINO_WIO_TERMINAL)
// #include <Seeed_FS.h>
// #include <SD/Seeed_SD.h>
// #else
// #include <SD.h>
// #include <SPIFFS.h>
// #endif
// #ifndef SDCARD_SS_PIN
// #define SDCARD_SS_PIN 2
// #endif
// #ifndef SDCARD_SPI
// #define SDCARD_SPI SPI
// #endif

//*******sd_card********
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#define CS_PIN 2  // پایه CS برای ESP32-S3 (می‌تواند بسته به سیم‌کشی تغییر کند)

//*******lcd_config*******
#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <lvgl.h>
#include "lcd_config.h"
LGFX tft;

int menu_select;
int change_menu;

String files[30];
int count_files;
int btn_count;
//****manual_touch_setting****
int x_t, y_t;  // touch
int dodol;
lv_obj_t *btn[17];
lv_obj_t *img[17];
lv_img_dsc_t img_dsc[20];
lv_obj_t *menu_container;
size_t img_size;

//********************************************setup*****************************************
//********************************************setup*****************************************
//********************************************setup*****************************************
void setup() {
  //*******serial_config******
  Serial.begin(115200);
  lvgl_start();
  Serial.println("Pavan_os start...");
  //SD.begin(SDCARD_SS_PIN, SDCARD_SPI, 25000000);
  // Initialize SD card
  if (!SD.begin(CS_PIN)) {
    Serial.println("Failed to initialize SD card.");
    return;
  }
  Serial.println("SD card initialized.");
  tft.init();
  tft.setRotation(1);
  tft.startWrite();
  menu_select = 1;
  change_menu = 1;
}

void loop() {
  select_menu();
  //lv_timer_handler(); /* let the GUI do its work */
}