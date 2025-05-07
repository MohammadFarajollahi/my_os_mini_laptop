#include <EEPROM.h>
#include <libmaple/iwdg.h>
#include <ArduinoJson.h>
//****eeprom****
const int addressEEPROM_min = 0;  // Specify the address restrictions you want to use.
const int addressEEPROM_max = 300;
//*****timer****
void timer_(void);
#define LED_RATE 1000000


String inputData;
int input_data;
int serialMode;
String sub1;
int AtCommandMode;

//port_configs
int led = PC13;

//EC200A LTE 4G CAT4
int GSMPower = PB2;
int gsmLed = PC14;
int GsmReady;
String GsmData;
int GsmReadyCount;
int GsmReadyTimer;
int p2p = 1;
int smsReceived = 0;
int gsmError;
int simModel;

int smsCheck;
int SMScheckTimer;
int smsCheckCount;
String InputNumber;
String inputMessage;

String MessNum;

#define RI_PIN PA4  // پایه RI ماژول متصل به PA0
void smsInterrupt() {
 // if (GsmReady == 1) smsReceived = 1;
}
int messageSendCheck;
int messageSendTimer;
void setup() {
  iwdg_init(IWDG_PRE_256, 2000);
  Serial1.begin(115200);
  Serial2.begin(9600);
  Serial3.begin(115200);
  Serial2.println("STM32 start...");
  pinMode(led, OUTPUT);
  pinMode(gsmLed, OUTPUT);
  pinMode(GSMPower, OUTPUT);
  pinMode(RI_PIN, INPUT_PULLUP);

  //******timer*******
  Timer2.setMode(TIMER_CH1, TIMER_OUTPUTCOMPARE);
  Timer2.setPeriod(LED_RATE);
  Timer2.setCompare(TIMER_CH1, 1);
  Timer2.attachInterrupt(TIMER_CH1, timer_);
  serialMode = 1;
  // تنظیم پایه RI به عنوان ورودی
  attachInterrupt(digitalPinToInterrupt(RI_PIN), smsInterrupt, FALLING);  // تنظیم وقفه روی لبه نزولی
}

void loop() {
  stm32_uart1();
  stm32_uart2();
  gsm();
}
