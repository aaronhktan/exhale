#include <pebble.h>
#include "src/c/settings_menu.h"
#include "src/c/localize.h"
#include "src/c/settings.h"
#include "src/c/new_version_window.h"

static Window *s_settings_window;
static MenuLayer *s_settings_layer;

#if !PBL_PLATFORM_APLITE
// 1 section with 9 settings to change
#if PBL_PLATFORM_DIORITE || PBL_PLATFORM_EMERY
#define NUM_IN_APP_MENU_ITEMS 4
#elif !PBL_PLATFORM_APLITE
#define NUM_IN_APP_MENU_ITEMS 3
#endif
#define NUM_MENU_SECTIONS 6
#define NUM_REMINDERS_MENU_ITEMS 2
#define NUM_ACHIEVEMENTS_MENU_ITEMS 2
#define NUM_HEALTH_MENU_ITEMS 1
#define NUM_APP_GLANCE_MENU_ITEMS 1
#define NUM_ABOUT_MENU_ITEMS 2

static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
	return NUM_MENU_SECTIONS;
}

static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
	switch (section_index) {
		case 0:
			return NUM_IN_APP_MENU_ITEMS;
		case 1:
			return NUM_REMINDERS_MENU_ITEMS;
		case 2:
			return NUM_APP_GLANCE_MENU_ITEMS;
		case 3:
			return NUM_HEALTH_MENU_ITEMS;
		case 4:
			return NUM_ACHIEVEMENTS_MENU_ITEMS;
		case 5:
			return NUM_ABOUT_MENU_ITEMS;
		default:
			return 0;
  }
}

static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
	return MENU_CELL_BASIC_HEADER_HEIGHT;
}

static void menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
	// Determine which section we're working with
	switch (section_index) {
	case 0:
		// Draw title text in the section header
		menu_cell_basic_header_draw(ctx, cell_layer, localize_get_in_app_section_title());
		break;
	case 1:
		menu_cell_basic_header_draw(ctx, cell_layer, localize_get_reminders_section_title());
		break;
	case 2:
		menu_cell_basic_header_draw(ctx, cell_layer, localize_get_app_glance_section_title());
		break;
	case 3:
		menu_cell_basic_header_draw(ctx, cell_layer, localize_get_health_section_title());
		break;
	case 4:
		menu_cell_basic_header_draw(ctx, cell_layer, localize_get_achievements_section_title());
		break;
	case 5:
		menu_cell_basic_header_draw(ctx, cell_layer, localize_get_about_section_title());
		break;
	}
}

static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
	// Determine which section we're going to draw in
	switch (cell_index->section) {
		case 0: // In-App Section
			// Use the row to specify which item we'll draw
			switch (cell_index->row) {
				case 0: // Remember last duration
						if (settings_get_rememberDuration()) {
							menu_cell_basic_draw(ctx, cell_layer, localize_get_remember_duration_row_title(), localize_get_enabled_text(), NULL);
						} else {
							menu_cell_basic_draw(ctx, cell_layer, localize_get_remember_duration_row_title(), localize_get_disabled_text(), NULL);
						}
				break;
				case 1: // Vibration Type
					if (settings_get_vibrationEnabled()) {
						switch (settings_get_vibrationType()) {
							case 0: // Shakes on inhale only
								menu_cell_basic_draw(ctx, cell_layer, localize_get_vibrations_row_title(), localize_get_vibration_shakes_inhale_type_text(), NULL);
								break;
							case 1: // Shakes for both
								menu_cell_basic_draw(ctx, cell_layer, localize_get_vibrations_row_title(), localize_get_vibration_shakes_type_text(), NULL);
								break;
							case 2: // Taps
								menu_cell_basic_draw(ctx, cell_layer, localize_get_vibrations_row_title(), localize_get_vibration_taps_type_text(), NULL);
								break;
						}
					} else { // Vibrations are not enabled
						menu_cell_basic_draw(ctx, cell_layer, localize_get_vibrations_row_title(), localize_get_disabled_text(), NULL);
					}
				break;
				case 2: ; // Breaths per minute
					char breaths_per_minute[3];
					snprintf(breaths_per_minute, sizeof(breaths_per_minute), "%d", settings_get_breathsPerMinute());
					menu_cell_basic_draw(ctx, cell_layer, localize_get_breaths_per_minute_row_title(), breaths_per_minute, NULL);
				break;
				case 3: // Heart Rate Variation or Greeting depending on whether is Diorite or Aplite
					#if PBL_PLATFORM_DIORITE || PBL_PLATFORM_EMERY
						if (settings_get_heartRateVariation()) {
							menu_cell_basic_draw(ctx, cell_layer, localize_get_heart_rate_variation_row_title(), localize_get_enabled_text(), NULL);
						} else {
							menu_cell_basic_draw(ctx, cell_layer, localize_get_heart_rate_variation_row_title(), localize_get_disabled_text(), NULL);
						}
					#endif
				break;
			}
			break;
		case 1: // Reminders
			switch (cell_index->row) {
					case 0: ;// Reminder Frequency
							if (settings_get_reminderHours() != 0) {
								char frequency_text[30];
								snprintf(frequency_text, sizeof(frequency_text), localize_get_reminder_frequency_text(settings_get_reminderHours()), settings_get_reminderHours());
								menu_cell_basic_draw(ctx, cell_layer, localize_get_reminder_frequency_row_title(), frequency_text, NULL);
							} else {
								menu_cell_basic_draw(ctx, cell_layer, localize_get_reminder_frequency_row_title(), localize_get_reminder_frequency_text(settings_get_reminderHours()), NULL);
							}
						break;
					case 1: ; // Reminder Start Time
						char start_text[30];
						snprintf(start_text, sizeof(start_text), localize_get_reminder_frequency_start_text(), settings_get_reminderHoursStart());
						menu_cell_basic_draw(ctx, cell_layer, localize_get_reminder_start_row_title(), start_text, NULL);
						break;
			}
			break;
		case 2: // App Glance
			switch (cell_index->row) {	
				case 0: // App Glance Type
						if (settings_get_appGlanceEnabled()) {
							switch (settings_get_appGlanceType()) {
								case 0: // Last session
									menu_cell_basic_draw(ctx, cell_layer, localize_get_app_glance_row_title(), localize_get_app_glance_last_session_text(), NULL);
									break;
								case 1: // Current Daily Total
									menu_cell_basic_draw(ctx, cell_layer, localize_get_app_glance_row_title(), localize_get_app_glance_daily_total_text(), NULL);
									break;
								case 2: // Streak
									menu_cell_basic_draw(ctx, cell_layer, localize_get_app_glance_row_title(), localize_get_bottom_text_streak_type_text(), NULL);
									break;
							}
						} else {
							menu_cell_basic_draw(ctx, cell_layer, localize_get_app_glance_row_title(), localize_get_disabled_text(), NULL);
						}
				break;
			}
			break;
		case 3: // Health
			switch (cell_index->row) {
				case 0: // Top Text Display
				switch (settings_get_displayText()) {
					case 0:
					menu_cell_basic_draw(ctx, cell_layer, localize_get_top_text_row_title(), localize_get_disabled_text(), NULL);
					break;
					case 1:
					menu_cell_basic_draw(ctx, cell_layer, localize_get_top_text_row_title(), localize_get_top_text_greeting_type_text(), NULL);
					break;
					case 2:
					menu_cell_basic_draw(ctx, cell_layer, localize_get_top_text_row_title(), localize_get_top_text_steps_type_text(), NULL);
					break;
					case 3: // Only display heart rate text if is Diorite
					#if PBL_PLATFORM_DIORITE || PBL_PLATFORM_EMERY
					menu_cell_basic_draw(ctx, cell_layer, localize_get_top_text_row_title(), localize_get_top_text_heart_rate_type_text(), NULL);
					#else
					menu_cell_basic_draw(ctx, cell_layer, localize_get_top_text_row_title(), localize_get_top_text_steps_type_text(), NULL);
					#endif
					break;
				}
				break;
			}
			break;
		case 4: // This is the Achievement Section
			switch (cell_index->row) {	
				case 0: // Enable or disable achievements
						if (settings_get_achievementsEnabled()) {
							menu_cell_basic_draw(ctx, cell_layer, localize_get_achievement_row_title(), localize_get_enabled_text(), NULL);
						} else {
							menu_cell_basic_draw(ctx, cell_layer, localize_get_achievement_row_title(), localize_get_disabled_text(), NULL);
						}
				break;
				case 1: // Bottom Text Type
					switch(settings_get_bottomTextType()) {
						case 0: // Total Today
							menu_cell_basic_draw(ctx, cell_layer, localize_get_bottom_text_row_title(), localize_get_bottom_text_total_type_text(), NULL);
							break;
						case 1: // Streak
							menu_cell_basic_draw(ctx, cell_layer, localize_get_bottom_text_row_title(), localize_get_bottom_text_streak_type_text(), NULL);
							break;
					}
				break;
			}
			break;
		case 5: // This is the about section
			switch (cell_index->row) {
				case 0: // This is the version number
					menu_cell_basic_draw(ctx, cell_layer, localize_get_version_row_title(), "v2.4, 2017-07-01", NULL);
					break;
				case 1: // This is the credits
					menu_cell_basic_draw(ctx, cell_layer, localize_get_credits_row_title(), "cheeseisdisgusting", NULL);
			}
			break;
	}
}

// Set what happens when an item is selected
static void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
	switch (cell_index->section) {
		case 0: // In-app section
			switch (cell_index->row) {
				case 0: // Remember last duration
					if (settings_get_rememberDuration()) {
						settings_set_rememberDuration(false);
					} else {
						settings_set_rememberDuration(true);
					}
					APP_LOG(APP_LOG_LEVEL_DEBUG, "The remember duration setting was changed.");
				break;
				case 1: // Vibration Type
					if (settings_get_vibrationEnabled()) {
						switch (settings_get_vibrationType()) {
							case 2: // Taps; move to disable vibrations
								settings_set_vibrationEnabled(false);
							break;
							default:
								settings_set_vibrationType(settings_get_vibrationType() + 1);
							break;
						}
					} else { // Vibration is set to false; move to shakes on inhale only
						settings_set_vibrationEnabled(true);
						settings_set_vibrationType(0);
					}
				break;
				case 2: // Breaths per minute
					switch (settings_get_breathsPerMinute()) {
						case 10:
							settings_set_breathsPerMinute(2);
						break;
						default:
							settings_set_breathsPerMinute(settings_get_breathsPerMinute() + 1);
						break;
					}
				break;
				case 3: // Heart Rate Variation for Diorite
				#if PBL_PLATFORM_DIORITE || PBL_PLATFORM_EMERY
					if (settings_get_heartRateVariation()) {
						settings_set_heartRateVariation(false);
					} else {
						settings_set_heartRateVariation(true);
					}
				#endif
				break;
			}
			break;
		case 1: // This is the reminders section
			switch (cell_index->row) {
					case 0: // Reminder Frequency
						switch (settings_get_reminderHours()) {
							case 4:
								settings_set_reminderHours(settings_get_reminderHours() + 2);
								break;
							case 6:
								settings_set_reminderHours(0);
								break;
							default:
								settings_set_reminderHours(settings_get_reminderHours() + 1);
						}
						break;
					case 1: ; // Reminder Start Time
						switch (settings_get_reminderHoursStart()) {
							case 10:
								settings_set_reminderHoursStart(6);
								break;
							default:
								settings_set_reminderHoursStart(settings_get_reminderHoursStart() + 1);
								break;
						}
						break;
			}
			break;
		case 2: // This is the App Glance Section
			switch (cell_index->row) {	
					case 0: // App Glance Type
						if (settings_get_appGlanceEnabled()) {
							switch (settings_get_appGlanceType()) {
								case 2: // Streak
									settings_set_appGlanceEnabled(false);
									break;
								default: // Anything else
									settings_set_appGlanceType(settings_get_appGlanceType() + 1);
									break;
							}
						} else {
							settings_set_appGlanceEnabled(true);
							settings_set_appGlanceType(0);
						}
				break;
			}
			break;
		case 3: // Health
			switch (cell_index->row) {
				case 0: // Top Text Display
				switch (settings_get_displayText()) {
					case 3:
					settings_set_displayText(0);
					break;
					#if !PBL_PLATFORM_DIORITE && !PBL_PLATFORM_EMERY
					case 2:
					settings_set_displayText(0);
					break;
					#endif
					default:
					settings_set_displayText(settings_get_displayText() + 1);
				}
			}
			break;
		case 4: // This is the Achievement Section
			switch (cell_index->row) {	
				case 0: // Enable or disable achievements
						if (settings_get_achievementsEnabled()) {
							settings_set_achievementsEnabled(false);
						} else {
							settings_set_achievementsEnabled(true);
						}
				break;
				case 1: // Bottom Text Type
					switch(settings_get_bottomTextType()) {
						case 0: // Total Today
							settings_set_bottomTextType(1);
							break;
						case 1: // Streak
							settings_set_bottomTextType(0);
							break;
					}
				break;
			}
		break;
		case 5: // This is the About Section
			switch (cell_index->row) {
				case 0: // See information about new version
					new_version_window_push(false);
				break;
			}
		break;
	} 
	layer_mark_dirty(menu_layer_get_layer(s_settings_layer));
}

void settings_window_load(Window *window) {
	// Load the MenuLayer
	Layer *window_layer = window_get_root_layer(s_settings_window);
	GRect bounds = layer_get_frame(window_layer);
	
	// Create the menu layer
	s_settings_layer = menu_layer_create(bounds);
	menu_layer_set_normal_colors(s_settings_layer, settings_get_backgroundColor(), gcolor_legible_over(settings_get_backgroundColor()));
	menu_layer_set_highlight_colors(s_settings_layer, settings_get_circleColor(), gcolor_legible_over(settings_get_circleColor()));
	menu_layer_set_callbacks(s_settings_layer, NULL, (MenuLayerCallbacks){
		.get_num_sections = menu_get_num_sections_callback,
		.get_num_rows = menu_get_num_rows_callback,
		.draw_header = menu_draw_header_callback,
		.get_header_height = menu_get_header_height_callback,
		.draw_row = menu_draw_row_callback,
		.select_click = menu_select_callback,
	});
	
	// Bind the menu layer's click config provider to the window for interactivity
	menu_layer_set_click_config_onto_window(s_settings_layer, window);

	layer_add_child(window_layer, menu_layer_get_layer(s_settings_layer));
}

void settings_window_unload() {
	window_destroy(s_settings_window);
	menu_layer_destroy(s_settings_layer);
}

void settings_menu_window_push() {
	s_settings_window = window_create();
	window_set_window_handlers(s_settings_window, (WindowHandlers) {
		.load = settings_window_load,
		.unload = settings_window_unload,
	});
	window_stack_push(s_settings_window, true);
}
#endif