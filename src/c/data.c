#include <pebble.h>
#include "data.h"
#include "localize.h"

#if defined(PBL_HEALTH)
static int s_current_steps;
HealthValue s_current_heart_rate;
static char s_current_steps_buffer[22], s_current_heart_rate_buffer[8];

// Updates how many steps there are
void data_update_steps_buffer() {
	int thousands = s_current_steps / 1000;
	int hundreds = s_current_steps % 1000;
	// Puts the number of steps in a string
	if(thousands >= 10) { // There's a ten thousands digit!
			snprintf(s_current_steps_buffer, sizeof(s_current_steps_buffer), localize_get_steps_today_text(thousands), thousands, hundreds);
	} else if (thousands > 0) { // There's just a thousands digit
			snprintf(s_current_steps_buffer, sizeof(s_current_steps_buffer), localize_get_steps_today_text(thousands), thousands, hundreds);
	} else { // There isn't a thousands digit!
			snprintf(s_current_steps_buffer, sizeof(s_current_steps_buffer), localize_get_steps_today_text(thousands), hundreds);
	}
}

// Updates the current heart rate
void data_update_heart_rate_buffer() {
	snprintf(s_current_heart_rate_buffer, sizeof(s_current_heart_rate_buffer), localize_get_heart_rate_text(), (uint32_t) s_current_heart_rate);
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

// Calculates a rolling average
void data_set_current_heart_rate(int value) {
	int alpha = 64; // lower alpha -> slower changes
	s_current_heart_rate = (alpha * value + (256 - alpha) * s_current_heart_rate )/ 256;
	//APP_LOG(APP_LOG_LEVEL_DEBUG, "Raw BPM: %d, avg BPM: %d", value, (int)s_current_heart_rate);
}

// Sets heart rate sample period to get fresh data
void data_set_heart_rate_period(int val){
	#if PBL_API_EXISTS(health_service_set_heart_rate_sample_period)
		health_service_set_heart_rate_sample_period(val);
	#endif
}

char* data_get_current_steps_buffer() {
  return s_current_steps_buffer;
}

char* data_get_current_heart_rate_buffer() {
	return s_current_heart_rate_buffer;
}
#endif

// Gets today's date
char *data_get_date_today() {
	time_t temp = time(NULL);
	struct tm *tick_time = localtime(&temp);
	static char date_buffer[11];
	strftime(date_buffer, sizeof(date_buffer), "%F", tick_time);
	APP_LOG(APP_LOG_LEVEL_DEBUG, "The date today is %s", date_buffer);
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

void data_write_last_duration_data(int last_duration) {
	persist_write_int(LAST_DURATION_KEY, last_duration);
}

// Get last duration from persistent storage
int data_read_last_duration_data() {
	int last_duration = 0;
	if (persist_exists(LAST_DURATION_KEY)) {
		last_duration = persist_read_int(LAST_DURATION_KEY);
		APP_LOG(APP_LOG_LEVEL_DEBUG, "The last duration was %d.", last_duration);
	} else {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "User has not breathed before; created data.");
		data_write_last_duration_data(last_duration);
	}
	return last_duration;
}

// Store Wakeup id
void data_write_wakeup_time_data(char* wakeup_time) {
	persist_write_string(WAKEUP_TIME_KEY, wakeup_time);
}

// Wakeup time as a string
char * data_read_wakeup_time_data() {
	static char wakeup_time[9];
	if (persist_exists(WAKEUP_TIME_KEY)) {
		persist_read_string(WAKEUP_TIME_KEY, wakeup_time, sizeof(wakeup_time));
	} else {
		snprintf(wakeup_time, sizeof(wakeup_time), "oh no");
	}
	return wakeup_time;
}