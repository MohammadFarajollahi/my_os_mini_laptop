

void jasonCheck() {
  String jsonString = inputData;

  // تبدیل رشته JSON به یک آبجکت
  StaticJsonDocument<200> jsonDoc;
  DeserializationError error = deserializeJson(jsonDoc, jsonString);

  // if (error) {
  //   Serial2.println("خطا در پارس کردن JSON!");
  //   return;
  // }

  // استخراج داده‌ها از JSON
  String type = jsonDoc["type"];
  String phone = jsonDoc["phone"];
  String message = jsonDoc["message"];
  String extra = jsonDoc["extra"];

  // نمایش در سریال مانیتور
  Serial2.println("type: " + type);
  Serial2.println("number: " + phone);
  Serial2.println("messafe: " + message);
  Serial2.println("extra: " + extra);

  if(type == "sendSMS"){
    // PhoneNumber = phone;
    // TextMessage = message;
    sendSMS(phone , message);
  }
}