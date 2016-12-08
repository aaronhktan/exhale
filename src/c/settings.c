#include <pebble.h>
#include "settings.h"
#include "wakeup.h"
#include "src/c/data.h"

ClaySettings settings;

int settings_version = 1, current_settings_version = 2;

static void migrate_settings_data() {
	switch (settings_version) { // Not useful now, but might become useful when there are more storage versions.
		default: // Storage Version 1
			switch(settings.displayText) { // DisplayText keys changed between Storage Version 1 and 2
				case 0: // In Storage Version 1, this used to mean that a greeting was to be displayed
					settings.displayText = 1; // This has been switched to 1 in Storage Version 2
					break;
				case 1: // This used to mean that steps were to be displayed
					settings.displayText = 2; // This has been switched to 2 in Storage Version 2
					break;
				case 2: // This used to mean that heart rate was to be displayed.
					settings.displayText = 3; // This has been switched to 3 in Storage Version 2
					break;
			}
			switch(settings.vibrationType) {
				case 1: // This used to mean that they chose the "Taps" vibration.
					settings.vibrationType = 2; // This has been switched to 2 in Storage Version 2
				case 2: // This used to mean that they chose the "Vibrate on Inhale and Exhale" option
					settings.vibrationType = 1; // This has been switched to 1 in Storage Version 2
			}
			// App Glance was not a setting in the previous version of storage, so set these before saving.
			settings.appGlanceEnabled = true;
			settings.appGlanceType = 0;
			APP_LOG(APP_LOG_LEVEL_DEBUG, "Settings have been migrated from version 1 to version %d.", current_settings_version);
			settings_save_settings(); // Save these new settings.
	}
}
	
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
	settings.appGlanceEnabled = true;
	settings.appGlanceType = 0;
	persist_read_data(SETTINGS_KEY, &settings, sizeof(settings));
	
	// Check for storage version and migrate as necessary
	if (persist_exists(SETTINGS_VERSION_KEY)) { // This means that the storage version exists.
		settings_version = persist_read_int(SETTINGS_VERSION_KEY); // Get storage version and compare with current; if is different, then migrate.
		APP_LOG(APP_LOG_LEVEL_DEBUG, "The saved settings version is %d.", (int)persist_read_int(SETTINGS_VERSION_KEY));
		if (!settings_version == current_settings_version) {
			APP_LOG(APP_LOG_LEVEL_DEBUG, "The saved settings version and current version do not match; performing migration.");
			migrate_settings_data();
		}
	} else { // This means that the storage version hadn't been saved before (i.e. Storage Version 1)
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Settings key does not exist, and migration has commenced.");
		migrate_settings_data();
	}
}

// Saves settings
void settings_save_settings() {
		persist_write_data(SETTINGS_KEY, &settings, sizeof(settings));
		persist_write_int(SETTINGS_VERSION_KEY, 2);
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

	Tuple *app_glance_enabled_t = dict_find(iter, MESSAGE_KEY_appGlanceEnabled);
	if (app_glance_enabled_t) {
		settings.appGlanceEnabled = app_glance_enabled_t->value->int32 == 1;
	}

	Tuple *app_glance_type_t = dict_find(iter, MESSAGE_KEY_appGlanceType);
	if (app_glance_type_t) {
		settings.appGlanceType = app_glance_type_t->value->int8;
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
	if (settings.heartRateVariation && data_get_current_heart_rate() != 0) {
		int heart_rate = data_get_current_heart_rate();
		if (heart_rate <= 60) {
			return 4;
		} else if (heart_rate > 60 && heart_rate <= 65) {
			return 5;
		} else if (heart_rate > 65 && heart_rate <= 70) {
			return 6;
		} else if (heart_rate > 70 && heart_rate <= 80) {
			return 7;
		} else if (heart_rate > 80 && heart_rate <= 90) {
			return 8;
		} else if (heart_rate > 90 && heart_rate <= 100) {
			return 9;
		} else { // The heart rate is more than 100
			return 10;
		} 
	} else {
		return settings.breathsPerMinute;
	}
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
			return (MILLISECONDS_PER_MINUTE - (2000 * 7)) / 7 / 2;
		} else if (heart_rate > 80 && heart_rate <= 90) {
			return (MILLISECONDS_PER_MINUTE - (2000 * 8)) / 8 / 2;
		} else if (heart_rate > 90 && heart_rate <= 100) {
			return (MILLISECONDS_PER_MINUTE - (2000 * 9)) / 9 / 2;
		} else { // The heart rate is more than 100
			return (MILLISECONDS_PER_MINUTE - (2000 * 10)) / 10 / 2;
		} 
	} else {
		/* Return one minute minus 2 second delay for each breath, divided by the number of breaths to take
				That gives us how many seconds the entire breath should take, so divide by 2 to get inhale/exhale duration */
		return (MILLISECONDS_PER_MINUTE - (2000 * settings.breathsPerMinute)) / settings.breathsPerMinute / 2;
	}
}

bool settings_get_heartRateVariation() {
	return settings.heartRateVariation;
}

int settings_get_version() {
	return settings_version;
}
	
bool settings_get_appGlanceEnabled() {
	return settings.appGlanceEnabled;
}

int settings_get_appGlanceType() {
	return settings.appGlanceType;
}