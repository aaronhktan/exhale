#include <pebble.h>
#include "appglance.h"
#include "src/c/settings.h"

void appglance_update_app_glance(AppGlanceReloadSession *session, size_t limit, void *context) {
	// This should never happen, but developers should always ensure
	// they’re not adding more app glance slices than the limit
	if (limit < 1) return;

	// Cast the context object to a string
	const char *message = (char *)context;

	// Create the AppGlanceSlice
	// When layout.icon is not set, the app's default icon is used
	const AppGlanceSlice entry = (AppGlanceSlice) {
	.layout = {
		.icon = APP_GLANCE_SLICE_DEFAULT_ICON,
		.subtitle_template_string = message
	},
	
	.expiration_time = (settings_get_appGlanceType() == 0) ? APP_GLANCE_SLICE_NO_EXPIRATION : (time_start_of_today() + SECONDS_PER_DAY)
		
	};

	// Add the slice, and check the result
	const AppGlanceResult result = app_glance_add_slice(session, entry);
	if (result != APP_GLANCE_RESULT_SUCCESS)
		APP_LOG(APP_LOG_LEVEL_ERROR, "AppGlance Error: %d", result);
}