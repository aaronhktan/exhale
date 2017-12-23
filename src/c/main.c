#include <pebble.h>
#include "src/c/breathe_window.h"
#include "src/c/reminder_window.h"
#include "src/c/settings.h"
#include "src/c/wakeup.h"
#include "src/c/localize.h"
#include "src/c/data.h"
#if !PBL_PLATFORM_APLITE
#include "src/c/health.h"
#include "src/c/appglance.h"
#include "src/c/achievement.h"
// 	#include "src/c/achievement_menu.h"
// 	#include "src/c/achievement_window.h"
#include "src/c/new_version_window.h"
#endif

static void inbox_received_handler(DictionaryIterator *iter, void *context) {
	#if !PBL_PLATFORM_APLITE
	// Check if this is a request to send the settings on watch to the phone
	Tuple *request_settings_t = dict_find(iter, MESSAGE_KEY_requestSettings);
	Tuple *request_achievements_t = dict_find(iter, MESSAGE_KEY_requestAchievements);
	Tuple *achievements_t = dict_find(iter, 0);
	if (request_settings_t) {
		settings_send_settings(); // If yes, then send the settings
	} else if (request_achievements_t) {
		achievement_send_achievements(); // If yes, send the achievements
	} else if (achievements_t) {
		if (persist_exists(ACHIEVEMENT_OFFLINE_KEY)) {
			if (persist_read_bool(ACHIEVEMENT_OFFLINE_KEY) == false) { // This means that achievements were earned while offline
				achievement_send_achievements(); // So send the achievements
			} else {
				achievement_handle_achievements(iter, context); // Otherwise, the version on the phone is the best version
			}
		} else {
			achievement_handle_achievements(iter, context);
		}
	} else {
	#endif
	// Otherwise, save settings received from phone, and refresh screen
	settings_handle_settings(iter, context);
	breathe_window_redraw_window();
	#if !PBL_PLATFORM_APLITE
	}
	#endif
}

static void init() {
	APP_LOG(APP_LOG_LEVEL_INFO, "You are running version 2.52 of the Breathe app.");
	// Open AppMessage connection
	app_message_register_inbox_received(inbox_received_handler);
	app_message_open(256, 256);
	
	settings_init(); // Subscribe to settings service
	#if PBL_HEALTH
	if (settings_get_displayText() == 2 || settings_get_displayText() == 3) {
		health_init(); // Subscribe to health service if health API is available
		data_init(); // Subscribe to data service
	}
	#endif
	#if !PBL_PLATFORM_APLITE
	achievement_init(); // Subscribe to the achievement service
	#endif
	wakeup_service_subscribe(wakeup_handler); // Subscribe to Wakeup Service
	
	if(launch_reason() == APP_LAUNCH_WAKEUP) { // The app was started by a wakeup event.
		// Pushes the reminder window stack
		reminder_window_push();
		// If the user still has reminders enabled, schedule next wakeup
		if (settings_get_reminderHours() != 0) {
			wakeup_schedule_next_wakeup(settings_get_reminderHours(), 0, settings_get_reminderHoursStart());
		}
	} else {
// 		APP_LOG(APP_LOG_LEVEL_DEBUG, "Heap free is %d.", (int)heap_bytes_free());
// 				reminder_window_push(); // For testing
// 		The app was started by the user; push the standard breathe window
		if (settings_get_rememberDuration() && data_read_last_duration_data() != 0) { // Set the minutes to breathe to the same as last one, unless the number is zero (meaning they haven't breathed yet)
			breathe_window_push(data_read_last_duration_data());
		} else {
			breathe_window_push(1);
		}
		
		// Cancel any snoozed reminders because the user has decided to open the app and presumably doesn't need to be reminded again.
		if (persist_exists(SNOOZE_WAKEUP)) {
			if (wakeup_query(persist_read_int(SNOOZE_WAKEUP), NULL)) { // Returns true if the wakeup at this ID is still scheduled
				// Canceled a snooze timer!
// 				APP_LOG(APP_LOG_LEVEL_DEBUG, "Cancelling a timer at %d.", (int)persist_read_int(SNOOZE_WAKEUP));
				wakeup_cancel(persist_read_int(SNOOZE_WAKEUP));
			}
		}
		// Schedule next normal wakeup
		if (settings_get_reminderHours() != 0) {
			wakeup_schedule_next_wakeup(settings_get_reminderHours(), 0, settings_get_reminderHoursStart());
		}
		
		// Push the new version alert if the user has not previously seen it.
		#if !PBL_PLATFORM_APLITE
// 			char description[100];
// 			snprintf(description, sizeof(description), localize_get_minutes_session_description(), 10);
// 			achievement_window_push(localize_get_thirty_minutes_day_name(), description); // For testing
		if (!persist_exists(SEEN_NEW_VERSION_KEY) || !persist_exists(SEEN_NEW_VERSION_NUMBER_KEY)){
			new_version_window_push(true);
		} else if ((persist_read_bool(SEEN_NEW_VERSION_KEY) == false) || persist_read_int(SEEN_NEW_VERSION_NUMBER_KEY) != 24) {
// 						data_set_streak_date_persist_data();		
			new_version_window_push(false);
		} else {
// 			APP_LOG(APP_LOG_LEVEL_DEBUG, "The user has already seen the new version page!");
		}
		#endif
// 		achievement_menu_window_push(); // For testing
	}
}

static void deinit() {
	vibes_cancel(); // Cancel all scheduled vibrations, just in case
	#if PBL_API_EXISTS(health_service_set_heart_rate_sample_period)
	health_service_set_heart_rate_sample_period(0); // Reset heart rate sample period to default as to not waste too much battery
	#endif

	// Only load appglance if platform is not Aplite because Aplite does not support AppGlances
	#if !PBL_PLATFORM_APLITE
	if (settings_get_appGlanceEnabled()) { // Check if app glance is enabled
		char app_glance_text[79];
		switch(settings_get_appGlanceType()) { 
			case 0: // Show last session time on app glance
				snprintf(app_glance_text, sizeof(app_glance_text), localize_get_app_glance_text(settings_get_appGlanceType(), data_read_last_duration_data()), data_read_last_duration_data());
				break;
			case 1: // Show total daily time on app glance
				snprintf(app_glance_text, sizeof(app_glance_text), localize_get_app_glance_text(settings_get_appGlanceType(), data_read_breathe_persist_data()), data_read_breathe_persist_data());
				break;
			#if !PBL_PLATFORM_APLITE
			case 2: // Show streak on app glance
				snprintf(app_glance_text, sizeof(app_glance_text), localize_get_app_glance_text(settings_get_appGlanceType(), data_get_streak_length()), data_get_streak_length());
			#endif
		}
		app_glance_reload(appglance_update_app_glance, app_glance_text); // Reload app glance
	} else {
		app_glance_reload(appglance_update_app_glance, NULL); // Clear app glance completely
	}
	#endif
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}