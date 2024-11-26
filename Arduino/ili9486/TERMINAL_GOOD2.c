//*******sd_card********
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#define CS_PIN 2  // پایه CS برای ESP32-S3 (می‌تواند بسته به سیم‌کشی تغییر کند)


#include <lvgl.h>
#define LGFX_USE_V1
#include <LovyanGFX.hpp>
class LGFX : public lgfx::LGFX_Device {
  //lgfx::Panel_GC9A01      _panel_instance;
  //lgfx::Panel_GDEW0154M09 _panel_instance;
  //lgfx::Panel_HX8357B     _panel_instance;
  //lgfx::Panel_HX8357D     _panel_instance;
  //lgfx::Panel_ILI9163     _panel_instance;
  // lgfx::Panel_ILI9341     _panel_instance;
  //lgfx::Panel_ILI9342     _panel_instance;
  //lgfx::Panel_ILI9481     _panel_instance;
  //lgfx::Panel_ILI9486 _panel_instance;
  lgfx::Panel_ILI9488 _panel_instance;
  //lgfx::Panel_IT8951      _panel_instance;
  //lgfx::Panel_RA8875      _panel_instance;
  //lgfx::Panel_SH110x      _panel_instance; // SH1106, SH1107
  //lgfx::Panel_SSD1306     _panel_instance;
  //lgfx::Panel_SSD1327     _panel_instance;
  //lgfx::Panel_SSD1331     _panel_instance;
  //lgfx::Panel_SSD1351     _panel_instance; // SSD1351, SSD1357
  //lgfx::Panel_SSD1963     _panel_instance;
  //lgfx::Panel_ST7735      _panel_instance;
  //lgfx::Panel_ST7735S     _panel_instance;
  //lgfx::Panel_ST7789      _panel_instance;
  //lgfx::Panel_ST7796      _panel_instance;
  lgfx::Bus_Parallel16 _bus_instance;
  lgfx::Light_PWM _light_instance;
  // touch driver
  //lgfx::Touch_CST816S          _touch_instance;
  // lgfx::Touch_FT5x06           _touch_instance; // FT5206, FT5306, FT5406, FT6206, FT6236, FT6336, FT6436
  //lgfx::Touch_GSL1680E_800x480 _touch_instance; // GSL_1680E, 1688E, 2681B, 2682B
  //lgfx::Touch_GSL1680F_800x480 _touch_instance;
  //lgfx::Touch_GSL1680F_480x272 _touch_instance;
  //lgfx::Touch_GSLx680_320x320  _touch_instance;
  //lgfx::Touch_GT911            _touch_instance;
  //lgfx::Touch_STMPE610         _touch_instance;
  //lgfx::Touch_TT21xxx          _touch_instance; // TT21100
  lgfx::Touch_XPT2046 _touch_instance;
public:

  LGFX(void) {
    {
      auto cfg = _bus_instance.config();  // バス設定用の構造体を取得します。
      cfg.port = GPIO_NUM_1;              // 使用するI2Sポートを選択 (I2S_NUM_0 or I2S_NUM_1) (ESP32のI2S LCDモードを使用します)
      cfg.freq_write = 20000000;          // 送信クロック (最大20MHz, 80MHzを整数で割った値に丸められます)
      cfg.pin_wr = 6;                     // WR
      cfg.pin_rd = 7;                     // RD
      cfg.pin_rs = 15;                    // RS(D/C)
      cfg.pin_d0 = 16;                    // D0
      cfg.pin_d1 = 8;                     // D1
      cfg.pin_d2 = 18;                    // D2
      cfg.pin_d3 = 9;                     // D3
      cfg.pin_d4 = 3;                     // D4
      cfg.pin_d5 = 17;                    // D5
      cfg.pin_d6 = 10;                    // D6
      cfg.pin_d7 = 14;                    // D7
      cfg.pin_d8 = 21;                    // D8
      cfg.pin_d9 = 47;                    // D9
      cfg.pin_d10 = 48;                   // D10
      cfg.pin_d11 = 35;                   // D11
      cfg.pin_d12 = 36;                   // D12
      cfg.pin_d13 = 37;                   // D13
      cfg.pin_d14 = 38;                   // D14
      cfg.pin_d15 = 1;                    // D15
      _bus_instance.config(cfg);
      _panel_instance.setBus(&_bus_instance);
    }

    {  // 表示パネル制御の設定を行います。
      auto cfg = _panel_instance.config();
      cfg.pin_cs = 5;     // CS
      cfg.pin_rst = -1;   // RST
      cfg.pin_busy = -1;  // BUSY
      cfg.panel_width = 320;
      cfg.panel_height = 480;
      cfg.offset_x = 0;
      cfg.offset_y = 0;
      cfg.offset_rotation = 0;
      cfg.dummy_read_pixel = 8;
      cfg.dummy_read_bits = 1;
      cfg.readable = true;
      cfg.invert = false;
      cfg.rgb_order = false;
      cfg.dlen_16bit = true;  // 16bit
      cfg.bus_shared = true;  // SDカードとバスを共有している場合 trueに設定(drawJpgFile等でバス制御を行います)
      cfg.memory_width = 320;
      cfg.memory_height = 480;
      _panel_instance.config(cfg);
    }

    {
      auto cfg = _touch_instance.config();
      cfg.x_min = 0;
      cfg.x_max = 319;
      cfg.y_min = 0;
      cfg.y_max = 479;
      cfg.pin_int = -1;
      cfg.bus_shared = true;
      cfg.offset_rotation = 0;
      cfg.spi_host = SPI2_HOST;
      cfg.freq = 1000000;
      cfg.pin_sclk = 12;  // SCLK
      cfg.pin_mosi = 11;  // MOSI
      cfg.pin_miso = 13;  // MISO
      cfg.pin_cs = 4;     //   CS
      _touch_instance.config(cfg);
      _panel_instance.setTouch(&_touch_instance);
    }
    setPanel(&_panel_instance);
  }
};
LGFX tft;
/*Change to your screen resolution*/
static const uint16_t screenWidth = 480;
static const uint16_t screenHeight = 320;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * screenHeight / 10];





/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  //tft.pushColors((uint16_t *)&color_p->full, w * h, true);
  tft.writePixels((lgfx::rgb565_t *)&color_p->full, w * h);
  tft.endWrite();

  lv_disp_flush_ready(disp_drv);
}

/*Read the touchpad*/
void my_touchpad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data) {
  uint16_t touchX, touchY;

  bool touched = tft.getTouch(&touchX, &touchY);

  if (!touched) {
    data->state = LV_INDEV_STATE_REL;
  } else {
    data->state = LV_INDEV_STATE_PR;

    /*Set the coordinates*/
    data->point.x = touchX;
    data->point.y = touchY;

    // Serial.print("Data x ");
    // Serial.println(touchX);

    // Serial.print("Data y ");
    // Serial.println(touchY);
  }
}

//////////////////////////////////////////////////////////////////////////////////////////
lv_obj_t *terminal;        /* Object for textarea (terminal) */
lv_obj_t *btn_clear;       /* Object for clear button */
lv_obj_t *btn_color;       /* Object for color change button */
lv_obj_t *btn_baud;        /* Object for baud rate change button */
lv_obj_t *btn_save;        /* Object for save to SD button */
lv_obj_t *label_clear;     /* Object for clear button label */
lv_obj_t *label_color;     /* Object for color button label */
lv_obj_t *label_baud;      /* Object for baud rate button label */
lv_obj_t *label_save;      /* Object for save button label */
lv_color_t textColor = lv_color_hex(0xFFFFFF);  // Default color (white)
int baudRate = 9600;       // Default baud rate
int colorIndex = 0;        // Index to track current color
int baudRates[] = {9600, 19200, 38400, 57600, 115200};  // Array of baud rates
int baudIndex = 0;         // Index to track current baud rate
String logData = "";       // ذخیره داده‌های ورودی
// تن
// Event handler for clear button
void event_handler_clear(lv_event_t *e) {
  lv_textarea_set_text(terminal, "");  // Clear terminal
}

// Event handler for color button
void event_handler_color(lv_event_t *e) {
  // Define an array of colors
  lv_color_t colors[] = {lv_color_hex(0xFFFFFF), lv_color_hex(0xFF0000), lv_color_hex(0x00FF00), lv_color_hex(0x0000FF)};
  colorIndex = (colorIndex + 1) % 4;  // Cycle through colors
  textColor = colors[colorIndex];     // Update text color
  lv_obj_set_style_text_color(terminal, textColor, LV_PART_MAIN);  // Apply new color to terminal
}

// Event handler for baud rate button
void event_handler_baud(lv_event_t *e) {
  // Cycle through baud rates
  baudIndex = (baudIndex + 1) % 5;
  baudRate = baudRates[baudIndex];
  Serial.end();            // Stop current Serial communication
  Serial.begin(baudRate);  // Start with new baud rate

  // Show the updated baud rate on terminal
  lv_textarea_add_text(terminal, "Baud rate changed to: ");
  lv_textarea_add_text(terminal, String(baudRate).c_str());
  lv_textarea_add_text(terminal, "\n");
}

// Event handler for save button
void event_handler_save(lv_event_t *e) {
  // File file = SD.open("/terminal_log.txt", FILE_WRITE);  // Open file on SD card for writing
  // if (file) {
  //   file.println(terminalText);  // Write the terminal text to the file
  //   file.close();                // Close the file
  //   lv_textarea_add_text(terminal, "Data saved to SD card.\n");
  // } else {
  //   lv_textarea_add_text(terminal, "Error opening file for writing.\n");
  // }
const char *terminalText = lv_textarea_get_text(terminal);
    File file = SD.open("/terminal_log.txt", FILE_WRITE);
  if (file) {
    file.println(terminalText);  // نوشتن داده‌ها در فایل
    file.close();           // بستن فایل
    Serial.println("Data saved to SD card.");
  } else {
    Serial.println("Failed to open file for writing.");
  }
}


/////////////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(115200); /* prepare for possible serial debug */

 // Initialize SD card
  if (!SD.begin(CS_PIN)) {
    Serial.println("Failed to initialize SD card.");
    return;
  }
  Serial.println("SD card initialized.");

  String LVGL_Arduino = "Hello Arduino! ";
  LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

  Serial.println(LVGL_Arduino);
  Serial.println("I am LVGL_Arduino");


  tft.begin();        /* TFT init */
  tft.setRotation(1); /* Landscape orientation, flipped */
  uint16_t calData[] = { 239, 203, 175, 3880, 3777, 210, 3870, 3885 };
  tft.setTouchCalibrate(calData);
  lv_init();
  lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * screenHeight / 10);

  /*Initialize the display*/
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  /*Change the following line to your display resolution*/
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  /*Initialize the (dummy) input device driver*/
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);




  ///////////////////////////////////////////////////////////////////////////////
 // Create a textarea (terminal) to display text
  terminal = lv_textarea_create(lv_scr_act());
  lv_obj_set_size(terminal, 300, 140);  // سایز ترمینال
  lv_obj_align(terminal, LV_ALIGN_TOP_MID, 0, 0);  // تنظیم موقعیت ترمینال
  lv_textarea_set_text(terminal, "Awaiting input...\n");  // متن اولیه

  // Create a clear button
  btn_clear = lv_btn_create(lv_scr_act());
  lv_obj_align(btn_clear, LV_ALIGN_BOTTOM_LEFT, 400, -10);  // موقعیت دکمه پاک کردن
  lv_obj_set_size(btn_clear, 70, 40);

  // Label for the clear button
  label_clear = lv_label_create(btn_clear);
  lv_label_set_text(label_clear, "Clear");
  lv_obj_center(label_clear);

  // Assign event handler to the clear button
  lv_obj_add_event_cb(btn_clear, event_handler_clear, LV_EVENT_CLICKED, NULL);

  // Create a color change button
  btn_color = lv_btn_create(lv_scr_act());
  lv_obj_align(btn_color, LV_ALIGN_BOTTOM_LEFT, 300, -10);  // موقعیت دکمه تغییر رنگ
  lv_obj_set_size(btn_color, 50, 40);

  // Label for the color change button
  label_color = lv_label_create(btn_color);
  lv_label_set_text(label_color, "Color");
  lv_obj_center(label_color);

  // Assign event handler to the color change button
  lv_obj_add_event_cb(btn_color, event_handler_color, LV_EVENT_CLICKED, NULL);

  // Create a baud rate change button
  btn_baud = lv_btn_create(lv_scr_act());
  lv_obj_align(btn_baud, LV_ALIGN_BOTTOM_LEFT, 150, -10);  // موقعیت دکمه تغییر baud rate
  lv_obj_set_size(btn_baud, 100, 40);

  // Label for the baud rate button
  label_baud = lv_label_create(btn_baud);
  lv_label_set_text(label_baud, "Change Baud");
  lv_obj_center(label_baud);

  // Assign event handler to the baud rate change button
  lv_obj_add_event_cb(btn_baud, event_handler_baud, LV_EVENT_CLICKED, NULL);

    // Create a save button
  btn_save = lv_btn_create(lv_scr_act());
  lv_obj_align(btn_save, LV_ALIGN_BOTTOM_LEFT, 0, -10);  // موقعیت دکمه ذخیره
  lv_obj_set_size(btn_save, 70, 40);

  // Label for the save button
  label_save = lv_label_create(btn_save);
  lv_label_set_text(label_save, "Save");
  lv_obj_center(label_save);



  // Assign event handler to the save button
  lv_obj_add_event_cb(btn_save, event_handler_save, LV_EVENT_CLICKED, NULL);
}

void loop() {
  lv_timer_handler(); // LVGL task handler
  delay(5);

  // Check if data is available on Serial
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');  // Read input until newline

    // Update the terminal with the new text
    lv_textarea_add_text(terminal, input.c_str());
    lv_textarea_add_text(terminal, "\n");  // Add new line
  }
}
