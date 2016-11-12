#pragma once
#include <pebble.h>

#define SETTINGS_KEY 1

// Define settings struct
typedef struct ClaySettings {
	GColor backgroundColor;
	GColor circleColor;
	GColor textColor;
} ClaySettings;

void breathe_window_push();