/*Using LVGL with Arduino requires some extra steps:
 *Be sure to read the docs here: https://docs.lvgl.io/master/get-started/platforms/arduino.html  */

//*******sd_card********
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#define CS_PIN 2  // پایه CS برای ESP32-S3 (می‌تواند بسته به سیم‌کشی تغییر کند)


#include <lvgl.h>
#define LGFX_USE_V1
#include <LovyanGFX.hpp>
class LGFX : public lgfx::LGFX_Device {
  lgfx::Panel_ILI9488 _panel_instance;
  lgfx::Bus_Parallel16 _bus_instance;
  lgfx::Light_PWM _light_instance;
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

////////////////////////////////////////////////////////

// تابع بارگذاری تصویر از SD
uint16_t* loadImageFromSD(const char *path) {
    File file = SD.open(path);
    if (!file) {
        Serial.println("Failed to open file for reading");
        return nullptr;
    }

    size_t img_size = file.size();
    uint16_t *img_data = (uint16_t *)malloc(img_size);
    if (img_data == nullptr) {
        Serial.println("Failed to allocate memory for image");
        file.close();
        return nullptr;
    }

    file.read((uint8_t *)img_data, img_size);
    file.close();
    return img_data;
}

// تابع برای نمایش متن در سریال مانیتور
void button_event_handler(lv_event_t *e) {
    Serial.println("Button Pressed!");  // پیام در سریال مانیتور
}


void create_button_with_image() {
    // ساخت دکمه
    lv_obj_t *btn = lv_btn_create(lv_scr_act());
    lv_obj_set_size(btn, 100, 100);  // اندازه دکمه
    lv_obj_add_event_cb(btn, button_event_handler, LV_EVENT_CLICKED, NULL);  // تنظیم تابع رویداد برای دکمه

    // ایجاد تصویر درون دکمه
    lv_obj_t *img = lv_img_create(btn);

    // بارگذاری تصویر از SD با LovyanGFX
    uint16_t *img_data = loadImageFromSD("/pic.bmp");  // فرض: مسیر تصویر

    if (img_data != nullptr) {
        // ساختن تصویر برای LVGL
        static lv_img_dsc_t img_dsc;
        img_dsc.data = (const uint8_t *)img_data;
        img_dsc.header.cf = LV_IMG_CF_TRUE_COLOR;  // فرمت تصویر
        img_dsc.header.w = 80;  // عرض تصویر
        img_dsc.header.h = 80;  // ارتفاع تصویر

        // تنظیم تصویر در LVGL
        lv_img_set_src(img, &img_dsc);
    } else {
        Serial.println("Failed to load image");
    }
}

void setup() {
  Serial.begin(115200); /* prepare for possible serial debug */
  if (!SD.begin(CS_PIN)) {
    Serial.println("Failed to initialize SD card.");
    return;
  }

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

  /////////////////////////////////////////////

    create_button_with_image();
}

void loop() {
  lv_timer_handler(); /* let the GUI do its work */
}

