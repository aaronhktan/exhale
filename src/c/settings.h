#pragma once
#include <pebble.h>

#define SETTINGS_KEY 20

// Define settings struct
typedef struct ClaySettings {
	GColor backgroundColor;
	GColor circleColor;
	GColor textColor;
	bool vibrationEnabled;
	bool heartRateEnabled;
	int reminderHours;
	bool rememberDuration;
} ClaySettings;

void settings_init();
void settings_save_settings();
void settings_handle_settings();

GColor settings_get_backgroundColor();
GColor settings_get_circleColor();
GColor settings_get_textColor();
bool settings_get_vibrationEnabled();
bool settings_get_heartRateEnabled();
int settings_get_reminderHours();
bool settings_get_rememberDuration();