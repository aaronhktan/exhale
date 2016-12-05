#pragma once
#include <pebble.h>

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
} ClaySettings;

void settings_init();
void settings_save_settings();
void settings_handle_settings();

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
bool settings_get_heartRateVariation();