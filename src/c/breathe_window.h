#pragma once
#include <pebble.h>

#define SETTINGS_KEY 3

// Define settings struct
typedef struct ClaySettings {
	GColor backgroundColor;
	GColor circleColor;
	GColor textColor;
	bool vibrationEnabled;
} ClaySettings;

void breathe_window_push();