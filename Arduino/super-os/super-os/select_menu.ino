
void select_menu() {
  if (menu_select == "desktop") home_screen();
  if (menu_select == "mainMenu") main_menu();
  if (menu_select == "terminal") Terminal_();
  if (menu_select == "SerialMonitor") SerialMonitor();
  if (menu_select == "server") server();
  if (menu_select == "MusicPlayer") MusicPlayer();
  if (menu_select == "ClockSetting") ClockSetting();
}