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
      cfg.port = GPIO_NUM_1;              // 使用するI2Sポートを選択 (I2S_NUM_0 or I2S_NUM_1) (ESP32のI2S tftモードを使用します)
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



////////////////////////////////
//static LGFX lcd;
static LGFX_Sprite canvas(&tft);        // オフスクリーン描画用バッファ
static LGFX_Sprite clockbase(&canvas);  // 時計の文字盤パーツ
static LGFX_Sprite needle1(&canvas);    // 長針・短針パーツ
static LGFX_Sprite shadow1(&canvas);    // 長針・短針の影パーツ
static LGFX_Sprite needle2(&canvas);    // 秒針パーツ
static LGFX_Sprite shadow2(&canvas);    // 秒針の影パーツ

static constexpr uint64_t oneday = 86400000;  // 1日 = 1000msec x 60sec x 60min x 24hour = 86400000
static uint64_t count = rand() % oneday;      // 現在時刻 (ミリ秒カウンタ)
static int32_t width = 239;                   // 時計の縦横画像サイズ
static int32_t halfwidth = width >> 1;        // 時計盤の中心座標
static auto transpalette = 0;                 // 透過色パレット番号
static float zoom;                            // 表示倍率

#ifdef min
#undef min
#endif


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
  ////////////////////////////////////////////////


  zoom = (float)(std::min(tft.width(), tft.height())) / width;  // 表示が画面にフィットするよう倍率を調整

  tft.setPivot(tft.width() >> 1, tft.height() >> 1);  // 時計描画時の中心を画面中心に合わせる

  canvas.setColorDepth(lgfx::palette_4bit);  // 各部品を４ビットパレットモードで準備する
  clockbase.setColorDepth(lgfx::palette_4bit);
  needle1.setColorDepth(lgfx::palette_4bit);
  shadow1.setColorDepth(lgfx::palette_4bit);
  needle2.setColorDepth(lgfx::palette_4bit);
  shadow2.setColorDepth(lgfx::palette_4bit);
  // パレットの初期色はグレースケールのグラデーションとなっており、
  // 0番が黒(0,0,0)、15番が白(255,255,255)
  // 1番～14番は黒から白へ段階的に明るさが変化している
  //
  // パレットを使う場合、描画関数は色の代わりに0～15のパレット番号を指定する

  canvas.createSprite(width, width);  // メモリ確保
  clockbase.createSprite(width, width);
  needle1.createSprite(9, 119);
  shadow1.createSprite(9, 119);
  needle2.createSprite(3, 119);
  shadow2.createSprite(3, 119);

  canvas.fillScreen(transpalette);  // 透過色で背景を塗り潰す (create直後は0埋めされているので省略可能)
  clockbase.fillScreen(transpalette);
  needle1.fillScreen(transpalette);
  shadow1.fillScreen(transpalette);

  clockbase.setTextFont(4);  // フォント種類を変更(時計盤の文字用)
  clockbase.setTextDatum(lgfx::middle_center);
  clockbase.fillCircle(halfwidth, halfwidth, halfwidth, 6);  // 時計盤の背景の円を塗る
  clockbase.drawCircle(halfwidth, halfwidth, halfwidth - 1, 15);
  for (int i = 1; i <= 60; ++i) {
    float rad = i * 6 * -0.0174532925;  // 時計盤外周の目盛り座標を求める
    float cosy = -cos(rad) * (halfwidth * 10 / 11);
    float sinx = -sin(rad) * (halfwidth * 10 / 11);
    bool flg = 0 == (i % 5);                                                           // ５目盛り毎フラグ
    clockbase.fillCircle(halfwidth + sinx + 1, halfwidth + cosy + 1, flg * 3 + 1, 4);  // 目盛りを描画
    clockbase.fillCircle(halfwidth + sinx, halfwidth + cosy, flg * 3 + 1, 12);
    if (flg) {  // 文字描画
      cosy = -cos(rad) * (halfwidth * 10 / 13);
      sinx = -sin(rad) * (halfwidth * 10 / 13);
      clockbase.setTextColor(1);
      clockbase.drawNumber(i / 5, halfwidth + sinx + 1, halfwidth + cosy + 4);
      clockbase.setTextColor(15);
      clockbase.drawNumber(i / 5, halfwidth + sinx, halfwidth + cosy + 3);
    }
  }
  clockbase.setTextFont(7);

  needle1.setPivot(4, 100);  // 針パーツの回転中心座標を設定する
  shadow1.setPivot(4, 100);
  needle2.setPivot(1, 100);
  shadow2.setPivot(1, 100);

  for (int i = 6; i >= 0; --i) {  // 針パーツの画像を作成する
    needle1.fillTriangle(4, -16 - (i << 1), 8, needle1.height() - (i << 1), 0, needle1.height() - (i << 1), 15 - i);
    shadow1.fillTriangle(4, -16 - (i << 1), 8, shadow1.height() - (i << 1), 0, shadow1.height() - (i << 1), 1 + i);
  }
  for (int i = 0; i < 7; ++i) {
    needle1.fillTriangle(4, 16 + (i << 1), 8, needle1.height() + 32 + (i << 1), 0, needle1.height() + 32 + (i << 1), 15 - i);
    shadow1.fillTriangle(4, 16 + (i << 1), 8, shadow1.height() + 32 + (i << 1), 0, shadow1.height() + 32 + (i << 1), 1 + i);
  }
  needle1.fillTriangle(4, 32, 8, needle1.height() + 64, 0, needle1.height() + 64, 0);
  shadow1.fillTriangle(4, 32, 8, shadow1.height() + 64, 0, shadow1.height() + 64, 0);
  needle1.fillRect(0, 117, 9, 2, 15);
  shadow1.fillRect(0, 117, 9, 2, 1);
  needle1.drawFastHLine(1, 117, 7, 12);
  shadow1.drawFastHLine(1, 117, 7, 4);

  needle1.fillCircle(4, 100, 4, 15);
  shadow1.fillCircle(4, 100, 4, 1);
  needle1.drawCircle(4, 100, 4, 14);

  needle2.fillScreen(9);
  shadow2.fillScreen(3);
  needle2.drawFastVLine(1, 0, 119, 8);
  shadow2.drawFastVLine(1, 0, 119, 1);
  needle2.fillRect(0, 99, 3, 3, 8);

  tft.startWrite();
}

void loop() {
  lv_timer_handler();  // LVGL task handler
  delay(5);
  static uint32_t p_milli = 0;
  uint32_t milli = lgfx::millis() % 1000;
  if (p_milli > milli) count += 1000 + (milli - p_milli);
  else count += (milli - p_milli);
  p_milli = milli;

  int32_t tmp = (count % 1000) >> 3;
  canvas.setPaletteColor(8, 255 - (tmp >> 1), 255 - (tmp >> 1), 200 - tmp);  // 秒針の描画色を変化させる
                                                                             //count += 60000;
  if (count > oneday) { count -= oneday; }
  drawClock(count);
}

//////////////////////////

void update7Seg(int32_t hour, int32_t min) {  // 時計盤のデジタル表示部の描画
  int x = clockbase.getPivotX() - 69;
  int y = clockbase.getPivotY();
  clockbase.setCursor(x, y);
  clockbase.setTextColor(5);  // 消去色で 88:88 を描画する
  clockbase.print("88:88");
  clockbase.setCursor(x, y);
  clockbase.setTextColor(12);  // 表示色で時:分を描画する
  clockbase.printf("%02d:%02d", hour, min);
}

void drawDot(int pos, int palette) {
  bool flg = 0 == (pos % 5);            // ５目盛り毎フラグ
  float rad = pos * 6 * -0.0174532925;  // 時計盤外周の目盛り座標を求める
  float cosy = -cos(rad) * (halfwidth * 10 / 11);
  float sinx = -sin(rad) * (halfwidth * 10 / 11);
  canvas.fillCircle(halfwidth + sinx, halfwidth + cosy, flg * 3 + 1, palette);
}

void drawClock(uint64_t time) {  // 時計の描画
  static int32_t p_min = -1;
  int32_t sec = time / 1000;
  int32_t min = sec / 60;
  if (p_min != min) {  // 分の値が変化していれば時計盤のデジタル表示部分を更新
    p_min = min;
    update7Seg(min / 60, min % 60);
  }
  clockbase.pushSprite(0, 0);  // 描画用バッファに時計盤の画像を上書き

  drawDot(sec % 60, 14);
  drawDot(min % 60, 15);
  drawDot(((min / 60) * 5) % 60, 15);

  float fhour = (float)time / 120000;   // 短針の角度
  float fmin = (float)time / 10000;     // 長針の角度
  float fsec = (float)time * 6 / 1000;  // 秒針の角度
  int px = canvas.getPivotX();
  int py = canvas.getPivotY();
  shadow1.pushRotateZoom(px + 2, py + 2, fhour, 1.0, 0.7, transpalette);  // 針の影を右下方向にずらして描画する
  shadow1.pushRotateZoom(px + 3, py + 3, fmin, 1.0, 1.0, transpalette);
  shadow2.pushRotateZoom(px + 4, py + 4, fsec, 1.0, 1.0, transpalette);
  needle1.pushRotateZoom(fhour, 1.0, 0.7, transpalette);  // 針を描画する
  needle1.pushRotateZoom(fmin, 1.0, 1.0, transpalette);
  needle2.pushRotateZoom(fsec, 1.0, 1.0, transpalette);

  canvas.pushRotateZoom(0, zoom, zoom, transpalette);  // 完了した時計盤をtftに描画する
  tft.display();
}

