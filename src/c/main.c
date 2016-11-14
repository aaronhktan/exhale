#include <pebble.h>
#include "src/c/breathe_window.h"
#include "src/c/data.h"
#include "src/c/health.h"

static void init() {
	APP_LOG(APP_LOG_LEVEL_DEBUG, "You are running the most recent version of this app.");
	// Subscribe to health service
	#if PBL_HEALTH
		health_init();
	#endif
	data_init();
	// Show main window
	breathe_window_push();
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