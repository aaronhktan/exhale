#include <pebble.h>
#include "settings.h"
#include "wakeup.h"
#include "src/c/data.h"

ClaySettings settings;

// Sets default settings and then loads custom ones if set
void settings_init() {
	settings.backgroundColor = GColorBlack;
	settings.circleColor = PBL_IF_COLOR_ELSE(GColorJaegerGreen, GColorWhite);
	settings.textColor = GColorWhite;
	settings.vibrationEnabled = true;
	settings.vibrationType = 0;
	settings.rememberDuration = false;
	#if PBL_API_EXISTS(health_service_peek_current_value)
		settings.displayText = 3;
	#else
		settings.displayText = 2;
	#endif
	settings.reminderHours = 4;
	settings.reminderHoursStart = 8;
	settings.breathsPerMinute = 7;
	settings.heartRateVariation = false;
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
		settings.circleColor = settings.textColor; // Sets the circle color to the same as text color for BW platforms
	}
	
	Tuple *circle_color_t = dict_find(iter, MESSAGE_KEY_circleColor);
	if (circle_color_t) {
		settings.circleColor = GColorFromHEX(circle_color_t->value->int32);
	}
	
	Tuple *vibration_enabled_t = dict_find(iter, MESSAGE_KEY_vibrationEnabled);
	if (vibration_enabled_t) {
		settings.vibrationEnabled = vibration_enabled_t->value->int32 == 1;
	}
	
	Tuple *vibration_type_t = dict_find(iter, MESSAGE_KEY_vibrationType);
	if (vibration_type_t) {
		settings.vibrationType = vibration_type_t->value->int8;
	}
	
	Tuple *displayText_t = dict_find(iter, MESSAGE_KEY_displayText);
	if (displayText_t) {
		settings.displayText = displayText_t->value->int8;
		APP_LOG(APP_LOG_LEVEL_DEBUG, "The displayText value is %d.", settings.displayText);
	}
	
	Tuple *reminder_hours_start_t = dict_find(iter, MESSAGE_KEY_reminderHoursStart);
	if (reminder_hours_start_t) {
		settings.reminderHoursStart = reminder_hours_start_t->value->int32;
	}
	
	Tuple *reminder_hours_t = dict_find(iter, MESSAGE_KEY_reminderHours);
	if (reminder_hours_t) {
		settings.reminderHours = reminder_hours_t->value->int8;
		APP_LOG(APP_LOG_LEVEL_DEBUG, "The reminderHours value is %d.", settings.reminderHours);
		if (settings.reminderHours == 0) {
			wakeup_cancel_all();
		} else if (settings.reminderHours != 0) {
			wakeup_cancel_all();
			wakeup_force_next_schedule(settings.reminderHours, 0, settings.reminderHoursStart);
		}
	}
	
	Tuple *remember_duration_t = dict_find(iter, MESSAGE_KEY_rememberDuration);
	if (remember_duration_t) {
		settings.rememberDuration = remember_duration_t->value->int32 == 1;
	}
	
	Tuple *breaths_per_minute_t = dict_find(iter, MESSAGE_KEY_breathsPerMinute);
	if (breaths_per_minute_t) {
		settings.breathsPerMinute = breaths_per_minute_t->value->int32;
	}
	
	Tuple *heart_rate_variation_t = dict_find(iter, MESSAGE_KEY_heartRateVariation);
	if (heart_rate_variation_t) {
		settings.heartRateVariation = heart_rate_variation_t->value->int32 == 1;
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

int settings_get_vibrationType() {
	return settings.vibrationType;
}

int settings_get_displayText() {
	return settings.displayText;
}

int settings_get_reminderHours() {
	return settings.reminderHours;
}

bool settings_get_rememberDuration() {
	return settings.rememberDuration;
}

int settings_get_reminderHoursStart(){
	return settings.reminderHoursStart;
}

int settings_get_breathsPerMinute() {
	return settings.breathsPerMinute;
}

int settings_get_breathDuration() {
	if (settings.heartRateVariation && data_get_current_heart_rate() != 0) { // This means that the user has enabled heart rate variation and the heart rate monitor isn't returning a zero.
		int heart_rate = data_get_current_heart_rate();
		if (heart_rate <= 60) {
			return (MILLISECONDS_PER_MINUTE - (2000 * 4)) / 4 / 2;
		} else if (heart_rate > 60 && heart_rate <= 65) {
			return (MILLISECONDS_PER_MINUTE - (2000 * 5)) / 5 / 2;
		} else if (heart_rate > 65 && heart_rate <= 70) {
			return (MILLISECONDS_PER_MINUTE - (2000 * 6)) / 6 / 2;
		} else if (heart_rate > 70 && heart_rate <= 80) {
			return (MILLISECONDS_PER_MINUTE - 4000  - (2000 * 7)) / 7 / 2;
		} else if (heart_rate > 80 && heart_rate <= 90) {
			return (MILLISECONDS_PER_MINUTE - 4000  - (2000 * 8)) / 8 / 2;
		} else if (heart_rate > 90 && heart_rate <= 100) {
			return (MILLISECONDS_PER_MINUTE + 3000 - (2000 * 9)) / 9 / 2;
		} else { // The heart rate is more than 100
			return (MILLISECONDS_PER_MINUTE - (2000 * 10)) / 10 / 2;
		} 
	} else {
		switch(settings.breathsPerMinute) {
			case 4:
			case 5:
			case 6:
			case 10:
				/* Return one minute minus 2 second delay for each breath, divided by the number of breaths to take
				That gives us how many seconds the entire breath should take, so divide by 2 to get inhale/exhale duration */
				APP_LOG(APP_LOG_LEVEL_DEBUG, "The duration of breaths is %d.", (MILLISECONDS_PER_MINUTE - (2000 * settings.breathsPerMinute)) / settings.breathsPerMinute / 2);
				return (MILLISECONDS_PER_MINUTE - (2000 * settings.breathsPerMinute)) / settings.breathsPerMinute / 2;
				break;
			case 7:
			case 8:
				// Like above, but taking removing 4 seconds because otherwise a decimal would happen. Don't want that happening!
				return (MILLISECONDS_PER_MINUTE - 4000 - (2000 * settings.breathsPerMinute)) / settings.breathsPerMinute / 2;
				break;
			default: // 9 breaths per minute
				return (MILLISECONDS_PER_MINUTE + 3000 - (2000 * settings.breathsPerMinute)) / settings.breathsPerMinute / 2;
				break;
		}
	}
}