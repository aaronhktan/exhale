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