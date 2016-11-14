#include <pebble.h>
#include "health.h"

static void health_handler(HealthEventType event, void *context) {
	if (event == HealthEventHeartRateUpdate) {
		data_set_current_heart_rate((int)health_service_peek_current_value(HealthMetricHeartRateBPM));
		data_update_heart_rate_buffer();
	} else if (event == HealthEventMovementUpdate) {
		data_set_current_steps((int)health_service_sum_today(HealthMetricStepCount));
  	data_update_steps_buffer();
	}
}

void health_init() {
  health_service_events_subscribe(health_handler, NULL);
	#if PBL_API_EXISTS(health_service_set_heart_rate_sample_period)
  	health_service_set_heart_rate_sample_period(0);
	#endif
}