#if !PBL_PLATFORM_APLITE
#include <pebble.h>
#include "achievement_menu.h"
#include "achievement.h"
#include "settings.h"
#include "src/c/localize.h"
#include "src/c/achievement_window.h"
#include "src/c/data.h"

static Window *s_achievement_menu_window;
static MenuLayer *s_achievement_layer;
static GBitmap *s_achievement_complete, *s_achievement_incomplete;

// 2 sections - one for achievements, one for stats; 12 achievemenets and 2 stats
#define NUM_MENU_SECTIONS 2
#define NUM_ACHIEVEMENT_MENU_ITEMS 12
#define NUM_STATS_MENU_ITEMS 2


static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
	return NUM_MENU_SECTIONS;
}

static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
	switch (section_index) {
	case 0:
		return NUM_ACHIEVEMENT_MENU_ITEMS;
	case 1:
		return NUM_STATS_MENU_ITEMS;
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
		menu_cell_basic_header_draw(ctx, cell_layer, localize_get_achievements_section_title());
		break;
	case 1:
		menu_cell_basic_header_draw(ctx, cell_layer, localize_get_stats_section_title());
		break;
	}
}

static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
	// Determine which section we're going to draw in
	switch (cell_index->section) {
		case 0: // These are the achievements
			// Use the row to specify which item we'll draw
			switch (cell_index->row) {
				case 0: // One week streak
					if (achievement_get_one_week_streak().complete == 1) { // User has completed the achievement, draw completed icon
						menu_cell_basic_draw(ctx, cell_layer, localize_get_one_week_streak_name(), localize_get_one_week_streak_description(), s_achievement_complete);
					} else { // User hasn't compelted the achievement, draw incomplete icon
						menu_cell_basic_draw(ctx, cell_layer, localize_get_locked_title(), localize_get_locked_description(), s_achievement_incomplete);
					}
				break;
				case 1: // One month streak
					if (achievement_get_one_month_streak().complete == 1) {
						menu_cell_basic_draw(ctx, cell_layer, localize_get_one_month_streak_name(), localize_get_one_month_streak_description(), s_achievement_complete);
					} else {
						menu_cell_basic_draw(ctx, cell_layer, localize_get_locked_title(), localize_get_locked_description(), s_achievement_incomplete);
					}
				break;
				case 2: // One year streak
					if (achievement_get_one_year_streak().complete == 1) {
						menu_cell_basic_draw(ctx, cell_layer, localize_get_one_year_streak_name(), localize_get_one_year_streak_description(), s_achievement_complete);
					} else {
						menu_cell_basic_draw(ctx, cell_layer, localize_get_locked_title(), localize_get_locked_description(), s_achievement_incomplete);
					}
				break;
				case 3: // Five minutes total in one day
					if (achievement_get_five_minutes_day().complete == 1) {
						char five_minutes_day_description[100];
						snprintf(five_minutes_day_description, sizeof(five_minutes_day_description), localize_get_minutes_day_description(), 5);
						menu_cell_basic_draw(ctx, cell_layer, localize_get_five_minutes_day_name(), five_minutes_day_description, s_achievement_complete);
					} else {
						menu_cell_basic_draw(ctx, cell_layer, localize_get_locked_title(), localize_get_locked_description(), s_achievement_incomplete);
					}
				break;
				case 4: // Ten minutes total in one day
					if (achievement_get_ten_minutes_day().complete == 1) {
						char ten_minutes_day_description[100];
						snprintf(ten_minutes_day_description, sizeof(ten_minutes_day_description), localize_get_minutes_day_description(), 10);
						menu_cell_basic_draw(ctx, cell_layer, localize_get_ten_minutes_day_name(), ten_minutes_day_description, s_achievement_complete);
					} else {
						menu_cell_basic_draw(ctx, cell_layer, localize_get_locked_title(), localize_get_locked_description(), s_achievement_incomplete);
					}
				break;
				case 5: // Thirty minutes total in one day
					if (achievement_get_thirty_minutes_day().complete == 1) {
						char thirty_minutes_day_description[100];
						snprintf(thirty_minutes_day_description, sizeof(thirty_minutes_day_description), localize_get_minutes_day_description(), 30);
						menu_cell_basic_draw(ctx, cell_layer, localize_get_thirty_minutes_day_name(), thirty_minutes_day_description, s_achievement_complete);
					} else {
						menu_cell_basic_draw(ctx, cell_layer, localize_get_locked_title(), localize_get_locked_description(), s_achievement_incomplete);
					}
				break;
				case 6: // One hour total in one day
					if (achievement_get_one_hour_day().complete == 1) {
						menu_cell_basic_draw(ctx, cell_layer, localize_get_one_hour_day_name(), localize_get_one_hour_day_description(), s_achievement_complete);
					} else {
						menu_cell_basic_draw(ctx, cell_layer, localize_get_locked_title(), localize_get_locked_description(), s_achievement_incomplete);
					}
				break;
				case 7: // Five minutes breathed during one session
					if (achievement_get_five_minutes_session().complete == 1) {
						char five_minutes_session_description[100];
						snprintf(five_minutes_session_description, sizeof(five_minutes_session_description), localize_get_minutes_session_description(), 5);
						menu_cell_basic_draw(ctx, cell_layer, localize_get_five_minutes_session_name(), five_minutes_session_description, s_achievement_complete);
					} else {
						menu_cell_basic_draw(ctx, cell_layer, localize_get_locked_title(), localize_get_locked_description(), s_achievement_incomplete);
					}
				break;
				case 8: // Eight minutes breathed during one session
					if (achievement_get_eight_minutes_session().complete == 1) {
						char eight_minutes_session_description[100];
						snprintf(eight_minutes_session_description, sizeof(eight_minutes_session_description), localize_get_minutes_session_description(), 8);
						menu_cell_basic_draw(ctx, cell_layer, localize_get_eight_minutes_session_name(), eight_minutes_session_description, s_achievement_complete);
					} else {
						menu_cell_basic_draw(ctx, cell_layer, localize_get_locked_title(), localize_get_locked_description(), s_achievement_incomplete);
					}
				break;
				case 9: // Ten minutes breathed during one session
					if (achievement_get_ten_minutes_session().complete == 1) {
						char ten_minutes_session_description[100];
						snprintf(ten_minutes_session_description, sizeof(ten_minutes_session_description), localize_get_minutes_session_description(), 10);
						menu_cell_basic_draw(ctx, cell_layer, localize_get_ten_minutes_session_name(), ten_minutes_session_description, s_achievement_complete);
					} else {
						menu_cell_basic_draw(ctx, cell_layer, localize_get_locked_title(), localize_get_locked_description(), s_achievement_incomplete);
					}
				break;
				case 10: // User has changed settings for the first time
					if (achievement_get_changed_settings().complete == 1) {
						menu_cell_basic_draw(ctx, cell_layer, localize_get_changed_settings_name(), localize_get_changed_settings_description(), s_achievement_complete);
					} else {
						menu_cell_basic_draw(ctx, cell_layer, localize_get_locked_title(), localize_get_locked_description(), s_achievement_incomplete);
					}
				break;
				case 11: // User has completed all the achievements
					if (achievement_get_completionist().complete == 1) {
						menu_cell_basic_draw(ctx, cell_layer, localize_get_completionist_name(), localize_get_completionist_description(), s_achievement_complete);
					} else {
						menu_cell_basic_draw(ctx, cell_layer, localize_get_locked_title(), localize_get_locked_description(), s_achievement_incomplete);
					}
				break;
			}
			break;
		case 1: // These are the stats
			switch (cell_index->row) {
				case 0: ; // First, the total number of minutes breathed
					char total_breathed_description[100];
					snprintf(total_breathed_description, sizeof(total_breathed_description), localize_get_total_breathed_description(), data_get_total_minutes_breathed());
					menu_cell_basic_draw(ctx, cell_layer, localize_get_total_breathed_name(), total_breathed_description, NULL);
					break;
				case 1: ; // Then, the longest streak
					char longest_streak_description[100];
					snprintf(longest_streak_description, sizeof(longest_streak_description), localize_get_longest_streak_description(data_get_longest_streak()), data_get_longest_streak());
					menu_cell_basic_draw(ctx, cell_layer, localize_get_longest_streak_name(), longest_streak_description, NULL);
					break;
			}
	}
}

// Set what happens when an item is selected
static void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
	if (cell_index->section == 0) {
		switch (cell_index->row) {
			case 0:
				if (achievement_get_one_week_streak().complete) {
					achievement_window_push(localize_get_one_week_streak_name(), localize_get_one_week_streak_description());
				}
				break;
			case 1:
				if (achievement_get_one_month_streak().complete) {				
					achievement_window_push(localize_get_one_month_streak_name(), localize_get_one_month_streak_description());
				}
				break;
			case 2:
				if (achievement_get_one_year_streak().complete) {					
					achievement_window_push(localize_get_one_year_streak_name(), localize_get_one_year_streak_description());
				}
				break;
			case 3: ;
				if (achievement_get_five_minutes_day().complete) {
					char * five_minutes_day_description = "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ";
					snprintf(five_minutes_day_description, 33, localize_get_minutes_day_description(), 5);
					achievement_window_push(localize_get_five_minutes_day_name(), five_minutes_day_description);
				}
				break;
			case 4: ;
				if (achievement_get_ten_minutes_day().complete) {
					char * ten_minutes_day_description = "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ!";
					snprintf(ten_minutes_day_description, 34, localize_get_minutes_day_description(), 10);
					achievement_window_push(localize_get_ten_minutes_day_name(), ten_minutes_day_description);
				}
				break;
			case 5: ;
				if (achievement_get_thirty_minutes_day().complete) {
					char * thirty_minutes_day_description = "1234567890ABCDEFGHIJKLMNOPQRSTUVWXY";;
					snprintf(thirty_minutes_day_description, 34, localize_get_minutes_day_description(), 30);
					achievement_window_push(localize_get_thirty_minutes_day_name(), thirty_minutes_day_description);
				}
				break;
			case 6:
				if (achievement_get_one_hour_day().complete) {
					achievement_window_push(localize_get_one_hour_day_name(), localize_get_one_hour_day_description());
				}
				break;
			case 7: ;
				if (achievement_get_five_minutes_session().complete) {
					char * five_minutes_session_description = "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ!";
					snprintf(five_minutes_session_description, 37, localize_get_minutes_session_description(), 5);
					achievement_window_push(localize_get_five_minutes_session_name(), five_minutes_session_description);
				}
				break;
			case 8: ;
				if (achievement_get_eight_minutes_session().complete) {
					char * eight_minutes_session_description = "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ!";
					snprintf(eight_minutes_session_description, 37, localize_get_minutes_session_description(), 8);
					APP_LOG(APP_LOG_LEVEL_DEBUG, eight_minutes_session_description);
					achievement_window_push(localize_get_eight_minutes_session_name(), eight_minutes_session_description);
				}
				break;
			case 9: ;
				if (achievement_get_ten_minutes_session().complete) {
					char * ten_minutes_session_description = "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ!@";
					snprintf(ten_minutes_session_description, 38, localize_get_minutes_session_description(), 10);
					achievement_window_push(localize_get_ten_minutes_session_name(), ten_minutes_session_description);
				}
				break;
			case 10:
				if (achievement_get_changed_settings().complete) {
					achievement_window_push(localize_get_changed_settings_name(), localize_get_changed_settings_description());
				}
				break;
			case 11:
				if (achievement_get_completionist().complete) {
					achievement_window_push(localize_get_completionist_name(), localize_get_completionist_description());
				}
				break;
		}
	}
}

void achievement_window_load(Window *window) {
	// Load the achievement icons
	s_achievement_complete = gbitmap_create_with_resource(RESOURCE_ID_ACHIEVEMENT_COMPLETE_ICON);
	s_achievement_incomplete = gbitmap_create_with_resource(RESOURCE_ID_ACHIEVEMENT_INCOMPLETE_ICON);
	
	// Load the MenuLayer
	Layer *window_layer = window_get_root_layer(s_achievement_menu_window);
	GRect bounds = layer_get_frame(window_layer);
	
	// Create the menu layer
	s_achievement_layer = menu_layer_create(bounds);
	menu_layer_set_normal_colors(s_achievement_layer, settings_get_backgroundColor(), gcolor_legible_over(settings_get_backgroundColor()));
	menu_layer_set_highlight_colors(s_achievement_layer, settings_get_circleColor(), gcolor_legible_over(settings_get_circleColor()));
	menu_layer_set_callbacks(s_achievement_layer, NULL, (MenuLayerCallbacks){
		.get_num_sections = menu_get_num_sections_callback,
		.get_num_rows = menu_get_num_rows_callback,
		.draw_header = menu_draw_header_callback,
		.get_header_height = menu_get_header_height_callback,
		.draw_row = menu_draw_row_callback,
		.select_click = menu_select_callback,
	});
	
	// Bind the menu layer's click config provider to the window for interactivity
	menu_layer_set_click_config_onto_window(s_achievement_layer, window);

	layer_add_child(window_layer, menu_layer_get_layer(s_achievement_layer));
}

void achievement_window_unload() {
	menu_layer_destroy(s_achievement_layer);
	gbitmap_destroy(s_achievement_complete);
	gbitmap_destroy(s_achievement_incomplete);
	window_destroy(s_achievement_menu_window);
	APP_LOG(APP_LOG_LEVEL_DEBUG, "The number of bytes free is %d.", (int)heap_bytes_free());
}

void achievement_menu_window_push() {
	s_achievement_menu_window = window_create();
	window_set_window_handlers(s_achievement_menu_window, (WindowHandlers) {
		.load = achievement_window_load,
		.unload = achievement_window_unload,
	});
	window_stack_push(s_achievement_menu_window, true);
}
#endif