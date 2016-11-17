#include <pebble.h>
#include "settings.h"
#include "wakeup.h"

ClaySettings settings;

// Sets default settings and then loads custom ones if set
void settings_init() {
	settings.backgroundColor = GColorBlack;
	settings.circleColor = PBL_IF_COLOR_ELSE(GColorJaegerGreen, GColorWhite);
	settings.textColor = GColorWhite;
	settings.vibrationEnabled = true;
	#if PBL_API_EXISTS(health_service_peek_current_value)
		settings.heartRateEnabled = true;
	#else
		settings.heartRateEnabled = false;
	#endif
	settings.reminderHours = 4;
	persist_read_data(SETTINGS_KEY, &settings, sizeof(settings));
}

// Saves settings
void settings_save_settings() {
		persist_write_data(SETTINGS_KEY, &settings, sizeof(settings));
}

// Receives and applies settings from phone
void settings_handle_settings(DictionaryIterator *iter, void *context) {
	Tuple *bg_color_t = dict_find(iter, MESSAGE_KEY_backgroundColor);
	if (bg_color_t) {
		settings.backgroundColor = GColorFromHEX(bg_color_t->value->int32);
		if (bg_color_t->value->int32 == 0x000000) {
			APP_LOG(APP_LOG_LEVEL_DEBUG, "The background color is black.");
			settings.textColor = GColorWhite;
		} else {
			APP_LOG(APP_LOG_LEVEL_DEBUG, "The background color is white.");
			settings.textColor = GColorBlack;
		}
		settings.circleColor = settings.textColor;
	}
	
	Tuple *circle_color_t = dict_find(iter, MESSAGE_KEY_circleColor);
	if (circle_color_t) {
		settings.circleColor = GColorFromHEX(circle_color_t->value->int32);
	}
	
	Tuple *vibration_enabled_t = dict_find(iter, MESSAGE_KEY_vibrationEnabled);
	if (vibration_enabled_t) {
		settings.vibrationEnabled = vibration_enabled_t->value->int32 == 1;
	}
	
	Tuple *heartRate_enabled_t = dict_find(iter, MESSAGE_KEY_heartRateEnabled);
	if (heartRate_enabled_t) {
		settings.heartRateEnabled = heartRate_enabled_t->value->int32 == 1;
	}
	
	Tuple *reminder_hours_t = dict_find(iter, MESSAGE_KEY_reminderHours);
	if (reminder_hours_t) {
		settings.reminderHours = reminder_hours_t->value->int8;
		APP_LOG(APP_LOG_LEVEL_DEBUG, "The reminderHours value is %d.", settings.reminderHours);
		if (settings.reminderHours == 0) {
			wakeup_cancel_all();
		} else if (settings.reminderHours != 0) {
			wakeup_cancel_all();
			wakeup_force_next_schedule(settings.reminderHours, 0);
		}
	}
}

GColor settings_get_backgroundColor() {
	return settings.backgroundColor;
}

GColor settings_get_circleColor() {
	return settings.circleColor;
}

GColor settings_get_textColor() {
	return settings.textColor;
}

bool settings_get_vibrationEnabled() {
	return settings.vibrationEnabled;
}

bool settings_get_heartRateEnabled() {
	return settings.heartRateEnabled;
}

int settings_get_reminderHours() {
	return settings.reminderHours;
}