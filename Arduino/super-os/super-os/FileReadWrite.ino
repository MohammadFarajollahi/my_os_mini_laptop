


void writeSdCard(const char* FileName, const String& FileCommand) {
  SD.remove(FileName);  // حذف فایل قبلی
  File file = SD.open(FileName, FILE_WRITE);
  if (!file) {
    Serial.println("file not found");
    SdCardMessage = "file not found";
    return;
  }
  // file.println(FileCommand);
  size_t written = file.println(FileCommand);
  if (written == 0) {
    Serial.println("⚠️ خطا در نوشتن محتوا در فایل");
    SdCardMessage = "File not save!!!";
  } else {
    Serial.println("✅ نوشتن موفق");
    SdCardMessage = "**File saved**";
  }
  file.close();
  // if (SD.exists(FileName)) {
  //   Serial.println("✅ فایل با موفقیت ذخیره شد.");
  //   SdCardMessage = "File saved";
  // } else {
  //   Serial.println("File Error");
  //   SdCardMessage = "File saved";
  // }
}


void readSdCard(const char* filename) {
  File file = SD.open(filename);
  if (!file) {
    Serial.println("⛔ خطا در باز کردن فایل");
    SdCardMessage = "file not found";
    return;
  }
  fileContent = "";  // پاک‌سازی قبلی
                     // while (file.available()) {
                     //   fileContent += (char)file.read();
                     // }
  fileContent = file.readStringUntil('\n');
  fileContent.trim();
  file.close();
  Serial.println("📄 محتوای فایل:");
  Serial.println(fileContent);
}
