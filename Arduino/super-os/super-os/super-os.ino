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
#include <ArduinoJson.h>

//*******sd_card********
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#define CS_PIN 2  // پایه CS برای ESP32-S3 (می‌تواند بسته به سیم‌کشی تغییر کند)
// #include <Audio.h>
// #define BCK 39  // پین BCK ماژول PCM5102A
// #define DIN 41  // پین DATA ماژول PCM5102A
// #define LCK 40  // پین LCK ماژول PCM5102A
// Audio audio;
//*******lcd_config*******
#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <lvgl.h>
#include "lcd_config.h"
LGFX tft;

String menu_select;
int change_menu;

String files[30];
int count_files;
int btn_count;
//****manual_touch_setting****
int x_t, y_t;            // touch
lv_obj_t *battery_bg;    // برای بدنه باتری
lv_obj_t *battery_fill;  // برای نمایش مقدار شارژ

//برای کارت حافظه
lv_obj_t *btn[17];
lv_obj_t *img[17];
lv_img_dsc_t img_dsc[20];
lv_obj_t *menu_container;
size_t img_size;
lv_style_t style1;
lv_style_t style2;
lv_style_t style3;
lv_style_t style4;
lv_style_t styleTerminal;
lv_obj_t *label1;
lv_obj_t *screen;
lv_obj_t *clock_label;
lv_style_t clock_style;
lv_obj_t *label_create[20];
lv_obj_t *labelCheck[50];
// تعریف ویجت‌های ترمینال
lv_obj_t *terminal_output;
lv_obj_t *command_input;
lv_obj_t *keyboard;
lv_obj_t *terminal; /* Object for textarea (terminal) */
int charj;
//TIMER
int main_sec;
int batt_sec;
int main_timer;
int clock_second, clock_minute, clock_hour;
int date_day, date_month, date_Year;
hw_timer_t *timer = NULL;       // اشاره‌گر به تایمر
volatile uint32_t seconds = 0;  // شمارنده ثانیه
// متغیرهای دما
float temp_sensor1, temp_sensor2, temp_sensor3;
int refreshScreenDesk;
int desktopScreen;
//stm32_uart
HardwareSerial stm32_serial(1);
String stm32_string, sub_stm32_string;
int check_stm32, check_stm32_count, stm32_ready;
int stm32_check_timer;

//serial mode
int serial_mode;
int Serial_mode_count;
int Serial_mode_timer;

int AtCommandMode;
int AtCommandMode_count;
int AtCommandMode_timer;
int numberCount = 0;
int readyNumber;

//Server var//
lv_obj_t *ddlist;
String selected_number = "";    // متغیر برای ذخیره شماره انتخاب‌شده
static char selected_item[64];  // بافر برای ذخیره آیتم انتخاب شده
lv_obj_t *cb1, *cb2, *cb3;
int check1;
lv_obj_t *modal;
lv_obj_t *ta;
lv_obj_t *list;
#define FILE_PATH "/phone_numbers.txt"
String Checknumbers[50];
//Auto Setting
#define MAX_NUMBERS 50
#define FILE_NAME "/numbers.txt"

typedef struct {
  String number;
  bool checked;
} PhoneNumber;
PhoneNumber phoneNumbers[MAX_NUMBERS];
uint8_t phoneCount = 0;
lv_obj_t *checkBoxes[MAX_NUMBERS];
lv_obj_t *main_screen;
// ایجاد دکمه برای افزودن شماره
lv_obj_t *add_btn;
lv_obj_t *label_add;
String ServerNumbers[50];
String NumberTest[50];
String SdCardMessage;
String fileContent = "";
int AutoServerChek;
int autoCheckCount;
int SerialState;
int pavanCheckTimer;
int smsChekTime = 1800;  // 1800

int Network_ready;
int check_Network_count;
int Network_check_timer = 20;
int FoundNumbers;

int PavanSendSMStimer;
int PavanSendSmSCheck;
int PavanSMSNum;
int PavanCountCheck;
int chekSMScount;
int cheksmsTimer;
int reciveError;
int SendError;
int lcdShow;
int TerminalCount;
String FileServer;
int stm32checkTimer;
int stm32checkCount;
int checkError;

//*****rtcTime*****
int requestClock;
int requestClockCount;
int requestClockTimer = 500;
String weekDays[] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
int weekday;
String Day_;

// تابع وقفه تایمر
void IRAM_ATTR onTimer() {
  ++clock_second;
  if (clock_second > 59) {
    clock_second = 0;
    ++clock_minute;
    if (clock_minute > 59) {
      clock_minute = 0;
      ++clock_hour;
      if (clock_hour > 23) {
        clock_hour = 0;
      }
    }
  }

  if (main_timer == 1) {
    ++batt_sec;
    ++main_sec;
    ++refreshScreenDesk;
    if (requestClock == 1) ++requestClockCount;
  }



  ///stm32timer
  if (stm32_ready == 0) ++stm32_check_timer;
  if (stm32_ready == 1 && Network_ready == 0) ++Network_check_timer;
  if (Serial_mode_count == 1) ++Serial_mode_timer;
  if (AtCommandMode_count == 1) ++AtCommandMode_timer;
  if (AutoServerChek == 1 && Network_ready == 1) ++pavanCheckTimer;
  if (chekSMScount >= 1) ++cheksmsTimer;
  if (PavanCountCheck == 0) ++stm32checkTimer;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//********************************************setup*****************************************
//********************************************setup*****************************************
//********************************************setup*****************************************
void setup() {
  //*******serial_config******
  Serial.begin(115200);

  stm32_serial.begin(115200, SERIAL_8N1, 42, 45);

  //TIMER
  //TIMER
  timer = timerBegin(0, 80, true);              // تایمر 0، تقسیم‌کننده 80 (1 میکروثانیه)
  timerAttachInterrupt(timer, &onTimer, true);  // اتصال وقفه
  timerAlarmWrite(timer, 1000000, true);        // وقفه هر 1,000,000 میکروثانیه (1 ثانیه)
  timerAlarmEnable(timer);
  lvgl_start();
  Serial.println("Pavan_os start...");
  //SD.begin(SDCARD_SS_PIN, SDCARD_SPI, 25000000);
  // Initialize SD card
  if (!SD.begin(CS_PIN)) {
    Serial.println("Failed to initialize SD card.");
    ESP.restart();  // ریست نرم‌افزاری
  }
  Serial.println("SD card initialized.");
  tft.init();
  tft.setRotation(3);
  tft.startWrite();
  menu_select = "desktop";
  change_menu = 1;

  charj = 100;
  //*************************read data from sd card******************
  //***auto Setting***
}

void loop() {
  // ++charj;
  // if (charj > 100) charj = 0;
  stm32();
  AutoPrograms();
  select_menu();
  lv_timer_handler(); /* let the GUI do its work */
  delay(1);
}
