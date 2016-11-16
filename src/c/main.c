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
	// Subscribe to health service
	#if PBL_HEALTH
		health_init();
	#endif
	data_init();
	settings_init();
	if(launch_reason() == APP_LAUNCH_WAKEUP) {
    // The app was started by a wakeup event.
    WakeupId id = 0;
    int32_t reason = 0;

    // Get details and handle the event appropriately
    wakeup_get_launch_event(&id, &reason);
		
		reminder_window_push();
		
		if (settings_get_reminderHours() != 0) {
			wakeup_schedule_next_wakeup(settings_get_reminderHours(), reason);
		}
  } else {
		// The app was started by the user
		breathe_window_push(1);
		if (settings_get_reminderHours() != 0) {
			wakeup_schedule_next_wakeup(settings_get_reminderHours(), 0);
		}
	}
}

static void deinit() {
	#if PBL_API_EXISTS(health_service_set_heart_rate_sample_period)
  	health_service_set_heart_rate_sample_period(0);
	#endif
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}