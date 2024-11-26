


//******************************************************************************************
//
//                                          image from sd card

//******************************************************************************************
// تابع بارگذاری تصویر از SD
uint16_t *loadImageFromSD(const char *path) {
  File file = SD.open(path);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return nullptr;
  }

  img_size = file.size();
   uint16_t *img_data = (uint16_t *)malloc(img_size);
  //lv_img_dsc_t *img_data = (lv_img_dsc_t *)malloc(img_size);
  if (img_data == nullptr) {
    Serial.println("Failed to allocate memory for image");
    file.close();
    return nullptr;
  }

  file.read((uint8_t *)img_data, img_size);
  file.close();
  return img_data;
}

//******************************************************************************************
//
//                                          LIST files

//******************************************************************************************
void listDir(fs::FS &fs, const char *dirname, uint8_t levels) {
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if (!root) {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      files[count_files] = file.name();
      ++count_files;
      if (levels) {
        listDir(fs, file.name(), levels - 1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
      files[count_files] = file.name();
      ++count_files;
    }
    file = root.openNextFile();
  }
  file.close();
}