#include <pebble.h>
#include "wakeup.h"

static int launch_number = 0;
static time_t future_timestamp;
WakeupId id;

void wakeup_force_next_schedule(int hours, int wakeup_id) {
	
	persist_delete(PERSIST_WAKEUP);
	id = -1;
	APP_LOG(APP_LOG_LEVEL_DEBUG, "The wakeup module was opened");
	
	time_t current_t = time(NULL);
	const struct tm *current_time = localtime(&current_t);
	int current_hour = current_time->tm_hour;

	// Checks for # of times launched today
	for (int i = 0; i <= 12 / hours; i++) {
		
		time_t t_to_check = time_start_of_today() + 8 * SECONDS_PER_HOUR + i * hours * SECONDS_PER_HOUR;
		const struct tm *time_check = localtime(&t_to_check);
		int check_hour = time_check->tm_hour;
		
		APP_LOG(APP_LOG_LEVEL_DEBUG, "The current hour is %d", current_hour);
		APP_LOG(APP_LOG_LEVEL_DEBUG, "The hour to check is %d", check_hour);
		
		if (current_hour >= check_hour) {
			launch_number = i + 1;
			APP_LOG(APP_LOG_LEVEL_DEBUG, "The app's launch number after looping is %d", launch_number);
		} else {
			break;
		}
	}
	if (launch_number >= 12 / hours) {
		launch_number = 0;
		APP_LOG(APP_LOG_LEVEL_DEBUG, "The app's launch number was reset to: %d", launch_number);
	}

	if (launch_number != 0) {
		future_timestamp = clock_to_timestamp(TODAY, 8 + hours * launch_number, 0);
	} else {
		future_timestamp = time_start_of_today() + (24 * SECONDS_PER_HOUR + 8 * SECONDS_PER_HOUR);
	}

	const int reason_for_launch = 1;

	const bool notify_if_missed = true;

	id = wakeup_schedule(future_timestamp, reason_for_launch, notify_if_missed);

	APP_LOG(APP_LOG_LEVEL_DEBUG, "The app's launch_number is %d", launch_number);

	// Check the scheduling was successful
	if(id >= 0) {
		struct tm *future_time = localtime(&future_timestamp);
		static char future_time_string[9];
		strftime(future_time_string, sizeof(future_time_string), "%H:%M:%S", future_time);
		APP_LOG(APP_LOG_LEVEL_DEBUG, "The app is scheduled to wake up at %s.", future_time_string);
		// Persist the ID so that a future launch can query it
		persist_write_int(PERSIST_WAKEUP, id);
	}
}

void wakeup_schedule_next_wakeup(int hours, int wakeup_id) {
	if (persist_exists(PERSIST_WAKEUP)) {
    id = persist_read_int(PERSIST_WAKEUP);
    // query if event is still valid, otherwise delete
    if (wakeup_query(id, NULL)) {
      APP_LOG(APP_LOG_LEVEL_DEBUG, "There is already a wakeup scheduled!");
    } else {
      wakeup_force_next_schedule(hours, wakeup_id);
		}
	}
}