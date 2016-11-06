#include <pebble.h>
#include "src/c/breathe_window.h"

static void init() {
	breathe_window_push();
}

static void deinit() {
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}