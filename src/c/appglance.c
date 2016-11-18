#include <pebble.h>
#include "appglance.h"

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
    // TODO: Change to APP_GLANCE_SLICE_NO_EXPIRATION in SDK 4-dp2
    .expiration_time = APP_GLANCE_SLICE_NO_EXPIRATION
  };

  // Add the slice, and check the result
  const AppGlanceResult result = app_glance_add_slice(session, entry);
  if (result != APP_GLANCE_RESULT_SUCCESS) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "AppGlance Error: %d", result);
  }
}