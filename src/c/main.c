#include <pebble.h>
#include "src/c/breathe_window.h"
#include "src/c/reminder_window.h"
#include "src/c/data.h"
#include "src/c/health.h"
#include "src/c/settings.h"
#include "src/c/wakeup.h"
#include "src/c/localize.h"

static void init() {
	APP_LOG(APP_LOG_LEVEL_DEBUG, "You are running the most recent version of this app.");
	#if PBL_HEALTH
		health_init(); // Subscribe to health service if health API is available
	#endif
	data_init(); // Subscribe to data service
	settings_init(); // Subscribe to settings service
	if(launch_reason() == APP_LAUNCH_WAKEUP) { // The app was started by a wakeup event.
    WakeupId id = 0;
    int32_t reason = 0;

    // Get details and handle the event appropriately
    wakeup_get_launch_event(&id, &reason);
		
		// Pushes the reminder window stack
		reminder_window_push();
		
		// If the user still has reminders enabled, schedule next wakeup
		if (settings_get_reminderHours() != 0) {
			wakeup_schedule_next_wakeup(settings_get_reminderHours(), reason);
		}
  } else {
		// The app was started by the user; push the standard breathe window
		breathe_window_push(1);
		// Schedule next wakeup, just in case
		if (settings_get_reminderHours() != 0) {
			wakeup_schedule_next_wakeup(settings_get_reminderHours(), 0);
		}
	}
}

static void deinit() {
	vibes_cancel(); // Cancel all scheduled vibrations, just in case
	#if PBL_API_EXISTS(health_service_set_heart_rate_sample_period)
  	health_service_set_heart_rate_sample_period(0); // Reset heart rate sample period to default as to not waste too much battery
	#endif
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}