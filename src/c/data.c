#include <pebble.h>
#include "data.h"
#include "localize.h"

static int s_current_steps;
HealthValue s_current_heart_rate;
static char s_current_steps_buffer[19], s_current_heart_rate_buffer[8];

// Updates how many steps there are
void data_update_steps_buffer() {
  int thousands = s_current_steps / 1000;
  int hundreds = s_current_steps % 1000;
  // Puts the number of steps in a string
	if(thousands > 0) { // There's a thousands digit!
		if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			snprintf(s_current_steps_buffer, sizeof(s_current_steps_buffer), "%d,%03d PAS AUJ", thousands, hundreds);
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			snprintf(s_current_steps_buffer, sizeof(s_current_steps_buffer), "%d,%03d PASOS HOY", thousands, hundreds);
		} else {
			snprintf(s_current_steps_buffer, sizeof(s_current_steps_buffer), "%d,%03d STEPS TODAY", thousands, hundreds);
		}
  } else { // There isn't a thousands digit!
		if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			snprintf(s_current_steps_buffer, sizeof(s_current_steps_buffer), "%d PAS AUJ", hundreds);
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			snprintf(s_current_steps_buffer, sizeof(s_current_steps_buffer), "%d PASOS HOY", hundreds);
		} else {
			snprintf(s_current_steps_buffer, sizeof(s_current_steps_buffer), "%d STEPS TODAY", hundreds);
		}
  }
}

// Updates the current heart rate
void data_update_heart_rate_buffer() {
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
				snprintf(s_current_heart_rate_buffer, sizeof(s_current_heart_rate_buffer), "%lu BPM", (uint32_t) s_current_heart_rate);
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		snprintf(s_current_heart_rate_buffer, sizeof(s_current_heart_rate_buffer), "%lu LPM", (uint32_t) s_current_heart_rate);
	} else {
		snprintf(s_current_heart_rate_buffer, sizeof(s_current_heart_rate_buffer), "%lu BPM", (uint32_t) s_current_heart_rate);
	}
}

// Handles update of step count
static void load_health_data_handler() {
	s_current_steps = health_service_sum_today(HealthMetricStepCount);
	data_update_steps_buffer();
	#if PBL_API_EXISTS(health_service_peek_current_value)
		s_current_heart_rate = health_service_peek_current_value(HealthMetricHeartRateBPM);
		data_update_heart_rate_buffer();
	#endif
}

// Initiates after a delay so user doesn't see a delay
void data_init() {
	app_timer_register(500, load_health_data_handler, NULL);
}

int data_get_current_steps() {
	return s_current_steps;
}

void data_set_current_steps(int value) {
	s_current_steps = value;
}

int data_get_current_heart_rate() {
	return s_current_heart_rate;
}

void data_set_current_heart_rate(int value) {
	s_current_heart_rate = value;
}

char* data_get_current_steps_buffer() {
  return s_current_steps_buffer;
}

char* data_get_current_heart_rate_buffer() {
	return s_current_heart_rate_buffer;
}

// Gets today's date
char *data_get_date_today() {
	time_t temp = time(NULL);
	struct tm *tick_time = localtime(&temp);
	static char date_buffer[11];
	strftime(date_buffer, sizeof(date_buffer), "%F", tick_time);
	return date_buffer;
} 

// Writes the number of minutes breathed today to persistent storage
void data_write_breathe_persist_data(int min_breathed_today){
	persist_write_int(MIN_BREATHED_TODAY_KEY, min_breathed_today);
	APP_LOG(APP_LOG_LEVEL_DEBUG, "The minutes breathed today are: %d", min_breathed_today);
}

// Stores the date associated with the minutes of breathing done
void data_write_date_persist_data() {
	static char *date_stored;
	date_stored = data_get_date_today();
	persist_write_string(DATE_STORED_KEY, date_stored);
}

// Reads the number of minutes breathed today from persistent storage
int data_read_breathe_persist_data() {
	int min_breathed_today = 0;
	time_t temp = time(NULL);
	struct tm *tick_time = localtime(&temp);
	static char date_today[11], date_buffer[11];
	strftime(date_today, sizeof(date_today), "%F", tick_time);
	if (persist_exists(MIN_BREATHED_TODAY_KEY) && persist_exists(DATE_STORED_KEY)) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Data exists.");
		persist_read_string(DATE_STORED_KEY, date_buffer, sizeof(date_buffer));
			if (strcmp(date_buffer, date_today) == 0) {
				min_breathed_today = persist_read_int(MIN_BREATHED_TODAY_KEY);
				APP_LOG(APP_LOG_LEVEL_DEBUG, "Data for today is: %d", min_breathed_today);
				return min_breathed_today;
			} else {
				APP_LOG(APP_LOG_LEVEL_DEBUG, "Data exists but not for today.");
				data_write_date_persist_data();
				min_breathed_today = 0;
				data_write_breathe_persist_data(min_breathed_today);
				return min_breathed_today;
			}
	} else {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Data does not exist yet, have created.");
		data_write_date_persist_data();
		min_breathed_today = 0;
		data_write_breathe_persist_data(min_breathed_today);
  	return min_breathed_today;
	}
	return 0;
}