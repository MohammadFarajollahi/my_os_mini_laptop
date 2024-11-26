

void cash_size() {

  size_t freeHeap = heap_caps_get_free_size(MALLOC_CAP_DEFAULT);
  size_t largestFreeBlock = heap_caps_get_largest_free_block(MALLOC_CAP_DEFAULT);
  size_t totalHeap = heap_caps_get_total_size(MALLOC_CAP_DEFAULT);
  size_t usedHeap = totalHeap - freeHeap;

  // بررسی حافظه PSRAM (در صورت وجود)
  size_t freePSRAM = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
  size_t totalPSRAM = heap_caps_get_total_size(MALLOC_CAP_SPIRAM);
  size_t usedPSRAM = totalPSRAM - freePSRAM;

  // چاپ اطلاعات در سریال
  Serial.println("========== Memory Info ==========");
  Serial.printf("Total Heap: %u bytes\n", totalHeap);
  Serial.printf("Used Heap: %u bytes\n", usedHeap);
  Serial.printf("Free Heap: %u bytes\n", freeHeap);
  Serial.printf("Largest Free Block: %u bytes\n", largestFreeBlock);

  if (totalPSRAM > 0) {
    Serial.printf("Total PSRAM: %u bytes\n", totalPSRAM);
    Serial.printf("Used PSRAM: %u bytes\n", usedPSRAM);
    Serial.printf("Free PSRAM: %u bytes\n", freePSRAM);
  } else {
    Serial.println("PSRAM not available");
  }
}


void print_memory_usage() {
  lv_mem_monitor_t mon;
  lv_mem_monitor(&mon);
  Serial.printf("Free: %u bytes, Largest free block: %u bytes\n", mon.free_size, mon.free_biggest_size);
}