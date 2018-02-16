#pragma once

#define SETTINGS_KEY 20
#define SETTINGS_VERSION_KEY 21
#define MILLISECONDS_PER_MINUTE 60000

// Define settings struct
typedef struct ClaySettings {
  GColor backgroundColor;
  GColor circleColor;
  GColor textColor;
  bool vibrationEnabled;
  int vibrationType;
  int displayText;
  int reminderHours;
  bool rememberDuration;
  int reminderHoursStart;
  int breathsPerMinute;
  bool heartRateVariation;
  bool appGlanceEnabled;
  int appGlanceType;
  bool achievementsEnabled;
  int bottomTextType;
} ClaySettings;

void settings_init();
void settings_save_settings();
void settings_handle_settings(DictionaryIterator *iter, void *context);

GColor settings_get_backgroundColor();
GColor settings_get_circleColor();
GColor settings_get_textColor();
bool settings_get_vibrationEnabled();
int settings_get_vibrationType();
int settings_get_displayText();
int settings_get_reminderHours();
bool settings_get_rememberDuration();
int settings_get_reminderHoursStart();
int settings_get_breathsPerMinute();
int settings_get_breathDuration();
#if !PBL_PLATFORM_APLITE
bool settings_get_heartRateVariation();
#endif
bool settings_get_appGlanceEnabled();
int settings_get_appGlanceType();
bool settings_get_achievementsEnabled();
int settings_get_bottomTextType();

void settings_set_rememberDuration(bool value);
void settings_set_vibrationEnabled(bool value);
void settings_set_vibrationType(int value);
void settings_set_breathsPerMinute(int value);
#if !PBL_PLATFORM_APLITE
void settings_set_heartRateVariation(bool value);
#endif
void settings_set_displayText(int value);
void settings_set_reminderHours(int value);
void settings_set_reminderHoursStart(int value);
#if !PBL_PLATFORM_APLITE
void settings_set_appGlanceEnabled(bool value);
void settings_set_appGlanceType(int value);
void settings_set_achievementsEnabled(bool value);
void settings_set_bottomTextType(int value);
void settings_send_settings();
#endif