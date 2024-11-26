/*Using LVGL with Arduino requires some extra steps:
 *Be sure to read the docs here: https://docs.lvgl.io/master/get-started/platforms/arduino.html  */

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


lv_obj_t * kb;  // شیء کیبورد
lv_obj_t * ta;  // شیء Text Area


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

void event_handler(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);

    if(code == LV_EVENT_VALUE_CHANGED) {
        // وقتی که کاربر کلیدی را در کیبورد فشار می‌دهد
        const char * txt = lv_textarea_get_text(ta);
        Serial.println(txt);  // متن ورودی را در سریال چاپ می‌کند
    }

    if(code == LV_EVENT_ALL || code == LV_EVENT_CANCEL) {
        // بستن کیبورد در صورت اعمال یا لغو
        lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
    }
}

void setup() {
  Serial.begin(115200); /* prepare for possible serial debug */

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

 // ایجاد Text Area
    ta = lv_textarea_create(lv_scr_act());  // ایجاد یک Text Area در صفحه اصلی
    lv_obj_set_size(ta, 200, 50);  // تنظیم اندازه Text Area
    lv_obj_align(ta, LV_ALIGN_TOP_MID, 0, 20);  // تنظیم محل Text Area
    lv_obj_add_event_cb(ta, event_handler, LV_EVENT_ALL, NULL);  // افزودن callback برای تغییرات ورودی

    // ایجاد کیبورد
    kb = lv_keyboard_create(lv_scr_act());  // ایجاد کیبورد
    lv_keyboard_set_textarea(kb, ta);  // کیبورد را به Text Area متصل کنید

}

void loop() {
  lv_timer_handler(); /* let the GUI do its work */

}

