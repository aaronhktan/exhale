#include <pebble.h>
#include "data.h"

static int s_current_steps;
static char s_current_steps_buffer[19];

void data_update_steps_buffer() {
  int thousands = s_current_steps / 1000;
  int hundreds = s_current_steps % 1000;
  if(thousands > 0) {
    snprintf(s_current_steps_buffer, sizeof(s_current_steps_buffer), "%d,%03d STEPS TODAY", thousands, hundreds);
  } else {
    snprintf(s_current_steps_buffer, sizeof(s_current_steps_buffer), "%d STEPS TODAY", hundreds);
  }
}

static void load_health_data_handler() {
	s_current_steps = health_service_sum_today(HealthMetricStepCount);
	data_update_steps_buffer();
}

void data_init() {
	app_timer_register(500, load_health_data_handler, NULL);
}

int data_get_current_steps() {
	return s_current_steps;
}

void data_set_current_steps(int value) {
	s_current_steps = value;
}

char* data_get_current_steps_buffer() {
  return s_current_steps_buffer;
}

char *data_get_date_today() {
	time_t temp = time(NULL);
	struct tm *tick_time = localtime(&temp);
	static char date_buffer[11];
	strftime(date_buffer, sizeof(date_buffer), "%F", tick_time);
	return date_buffer;
} 

void data_write_breathe_persist_data(int min_breathed_today){
	persist_write_int(MIN_BREATHED_TODAY_KEY, min_breathed_today);
	APP_LOG(APP_LOG_LEVEL_DEBUG, "The minutes breathed today are: %dd", min_breathed_today);
}

void data_write_date_persist_data() {
	static char *date_stored;
	date_stored = data_get_date_today();
	persist_write_string(DATE_STORED_KEY, date_stored);
}

int data_read_breathe_persist_data() {
	int min_breathed_today = 0;
	time_t temp = time(NULL);
	struct tm *tick_time = localtime(&temp);
	static char *date_stored, date_today[11], date_buffer[11];
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