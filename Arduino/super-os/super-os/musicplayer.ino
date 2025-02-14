
lv_obj_t *play_btn, *stop_btn, *next_btn, *prev_btn, *seekbar;

const char *song_list[] = { "/music1.mp3", "/music2.mp3", "/music3.mp3" };
int song_index = 0;
bool isPlaying = false;
int totalDuration = 0;

void MusicPlayer() {

  if (change_menu == 1) {
    change_menu = 0;
    Serial.println("Music player");

    lv_obj_t *screen = lv_scr_act();

    time_label = lv_label_create(screen);
    lv_label_set_text(time_label, "00:00 / 00:00");
    lv_obj_align(time_label, LV_ALIGN_TOP_MID, 0, 10);

    play_btn = lv_btn_create(screen);
    lv_obj_align(play_btn, LV_ALIGN_CENTER, -40, 50);
    lv_obj_add_event_cb(play_btn, play_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_t *play_label = lv_label_create(play_btn);
    lv_label_set_text(play_label, "Play");

    stop_btn = lv_btn_create(screen);
    lv_obj_align(stop_btn, LV_ALIGN_CENTER, 40, 50);
    lv_obj_add_event_cb(stop_btn, stop_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_t *stop_label = lv_label_create(stop_btn);
    lv_label_set_text(stop_label, "Stop");

    next_btn = lv_btn_create(screen);
    lv_obj_align(next_btn, LV_ALIGN_CENTER, 80, 50);
    lv_obj_add_event_cb(next_btn, next_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_t *next_label = lv_label_create(next_btn);
    lv_label_set_text(next_label, "Next");

    prev_btn = lv_btn_create(screen);
    lv_obj_align(prev_btn, LV_ALIGN_CENTER, -80, 50);
    lv_obj_add_event_cb(prev_btn, prev_event_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_t *prev_label = lv_label_create(prev_btn);
    lv_label_set_text(prev_label, "Prev");

    lv_obj_t *forward_btn = lv_btn_create(screen);
    lv_obj_align(forward_btn, LV_ALIGN_CENTER, 50, 100);
    lv_obj_add_event_cb(forward_btn, seek_forward, LV_EVENT_CLICKED, NULL);
    lv_obj_t *fwd_label = lv_label_create(forward_btn);
    lv_label_set_text(fwd_label, "+5s");

    lv_obj_t *backward_btn = lv_btn_create(screen);
    lv_obj_align(backward_btn, LV_ALIGN_CENTER, -50, 100);
    lv_obj_add_event_cb(backward_btn, seek_backward, LV_EVENT_CLICKED, NULL);
    lv_obj_t *bwd_label = lv_label_create(backward_btn);
    lv_label_set_text(bwd_label, "-5s");

    seekbar = lv_slider_create(screen);
    lv_obj_align(seekbar, LV_ALIGN_BOTTOM_MID, 0, -20);
    lv_slider_set_range(seekbar, 0, 100);

    audio.setPinout(BCK, LCK, DIN);
    audio.setVolume(21);
  }

  audio.loop();
  update_time_label();
}


void update_time_label() {
  int currentTime = audio.getAudioCurrentTime() / 1000;
  int totalTime = totalDuration / 1000;

  char time_str[32];
  snprintf(time_str, sizeof(time_str), "%02d:%02d / %02d:%02d",
           currentTime / 60, currentTime % 60, totalTime / 60, totalTime % 60);
  lv_label_set_text(time_label, time_str);

  if (totalTime > 0) {
    lv_slider_set_value(seekbar, (currentTime * 100) / totalTime, LV_ANIM_ON);
  }
}

int getMp3Duration(const char *filename) {
  File file = SD.open(filename);
  if (!file) return 0;

  file.seek(0);
  uint8_t header[10];
  file.read(header, 10);

  int bitrate = 128;  // مقدار پیش‌فرض
  if (header[6] == 'X' && header[7] == 'i' && header[8] == 'n') {
    bitrate = header[9];  // خواندن بیت‌ریت دقیق از هدر MP3
  }

  int fileSize = file.size();
  file.close();

  return (fileSize * 8) / (bitrate * 1000);
}

void play_music(const char *filename) {
  Serial.printf("Playing: %s\n", filename);
  audio.stopSong();
  audio.connecttoFS(SD, filename);
  totalDuration = getMp3Duration(filename) * 1000;
  isPlaying = true;
}

void play_event_handler(lv_event_t *e) {
  if (!isPlaying) {
    play_music(song_list[song_index]);
  } else {
    audio.pauseResume();
    isPlaying = !isPlaying;
  }
}

void stop_event_handler(lv_event_t *e) {
  audio.stopSong();
  isPlaying = false;
}

void next_event_handler(lv_event_t *e) {
  song_index = (song_index + 1) % 3;
  play_music(song_list[song_index]);
}

void prev_event_handler(lv_event_t *e) {
  song_index = (song_index - 1 + 3) % 3;
  play_music(song_list[song_index]);
}

void seek_forward(lv_event_t *e) {
  int currentPos = audio.getAudioCurrentTime();
  audio.setAudioPlayPosition((currentPos + 5000) / 1000);
}

void seek_backward(lv_event_t *e) {
  int currentPos = audio.getAudioCurrentTime();
  int newPos = max(0, (currentPos - 5000) / 1000);
  audio.setAudioPlayPosition(newPos);
}