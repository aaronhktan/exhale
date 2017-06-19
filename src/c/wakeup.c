#include <pebble.h>
#include "wakeup.h"
#include "src/c/settings.h"
#include "src/c/data.h"

static int launch_number = 0;
static time_t future_timestamp;
WakeupId id;

void wakeup_force_next_schedule(int hours, int wakeup_id, int startHours) {
	
	// Deletes any previously scheduled wakeup
	persist_delete(PERSIST_WAKEUP);
	id = -1;
	
	// Gets current time, formats to struct tm, and extracts hours from struct tm
	time_t current_t = time(NULL);
	const struct tm *current_time = localtime(&current_t);
	int current_hour = current_time->tm_hour;

	// Checks for # of times launched today
	for (int i = 0; i <= 12 / hours; i++) {
		// Start a time set by user, and add iterator # of hours to check whether it's past that time
		time_t t_to_check = time_start_of_today() + startHours * SECONDS_PER_HOUR + i * hours * SECONDS_PER_HOUR;
		const struct tm *time_check = localtime(&t_to_check);
		int check_hour = time_check->tm_hour; // Like before, formatted to struct tm, and hours are extracted
		
		if (current_hour >= check_hour) { // Compares the current time to the time if the app were launched i times since 12 o'clock AM
			launch_number = i + 1; // This means that the current time is past that time, so increment number of times "launched"
		} else {
			break;
		}
	}
	
	if (launch_number > 12 / hours) { // This means that it's past 8 o'clock PM, therefore the next time it should launch is 8 o'clock AM tomorrow
		launch_number = 0;
	}

	if (launch_number != 0) { // Sets timestamp to the next time it should launch (day, hours, minutes)
		future_timestamp = clock_to_timestamp(TODAY, startHours + hours * launch_number, 0);
	} else { // Sets the timestamp to tomorrow at the time that's set in the app
		future_timestamp = time_start_of_today() + (SECONDS_PER_DAY + startHours * SECONDS_PER_HOUR);
	}

	const int reason_for_launch = 1;

	const bool notify_if_missed = true; // Notifies user if their Pebble is turned off or something

	id = wakeup_schedule(future_timestamp, reason_for_launch, notify_if_missed);

	// Check the scheduling was successful
	if(id >= 0) {
		struct tm *future_time = localtime(&future_timestamp);
		static char future_time_string[9];
		strftime(future_time_string, sizeof(future_time_string), "%H:%M:%S", future_time);
		data_write_wakeup_time_data(future_time_string);
		APP_LOG(APP_LOG_LEVEL_DEBUG, "The app is scheduled to wake up at %s.", future_time_string);
		// Persist the ID so that a future launch can query it
		persist_write_int(PERSIST_WAKEUP, id);
	}
}

// Check if there is a wakeup scheduled at the time, if so, do not schedule a new one.
void wakeup_schedule_next_wakeup(int hours, int wakeup_id, int startHours) {
	if (persist_exists(PERSIST_WAKEUP)) {
    id = persist_read_int(PERSIST_WAKEUP);
    // query if event is still valid, otherwise delete
    if (wakeup_query(id, NULL)) {
      APP_LOG(APP_LOG_LEVEL_DEBUG, "There is already a wakeup scheduled at %s!", data_read_wakeup_time_data());
    } else {
      wakeup_force_next_schedule(hours, wakeup_id, startHours);
		}
	} else {
		wakeup_force_next_schedule(hours, wakeup_id, startHours);
	}
}

void wakeup_handler(WakeupId id, int32_t reason) {
  // A wakeup event has occurred while the app was already open
	wakeup_schedule_next_wakeup(settings_get_reminderHours(), reason, settings_get_reminderHoursStart());
}