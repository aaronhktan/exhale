#include <pebble.h>
#include "src/c/breathe_window.h"
#include "src/c/data.h"
#include "src/c/health.h"

static void init() {
	// Subscribe to health service
	health_init();
	data_init();
	// Show main window
	breathe_window_push();
}

static void deinit() {
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}