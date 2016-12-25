#include <pebble.h>
#include "achievement_menu.h"
#include "achievement.h"
#include "settings.h"

static Window *s_achievement_window;
static MenuLayer *s_achievement_layer;
static GBitmap *s_achievement_complete, *s_achievement_incomplete;

#define NUM_MENU_SECTIONS 1
#define NUM_MENU_ITEMS 12


static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
	return NUM_MENU_SECTIONS;
}

static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
	return NUM_MENU_ITEMS;
}

static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}

static void menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
  menu_cell_basic_header_draw(ctx, cell_layer, "Achievements");
}

static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
	// Use the row to specify which item we'll draw
	switch (cell_index->row) {
		case 0:
			if (achievement_get_one_week_streak().complete == 1) { // User has completed the achievement, draw completed icon
				menu_cell_basic_draw(ctx, cell_layer, "7 Days Alive", "One week streak!", s_achievement_complete);
			} else { // User hasn't compelted the achievement, draw incomplete icon
				menu_cell_basic_draw(ctx, cell_layer, "Locked", "Keep breathing!", s_achievement_incomplete);
			}
			break;
		case 1:
			if (achievement_get_one_month_streak().complete == 1) {
				menu_cell_basic_draw(ctx, cell_layer, "1 Month", "One month streak!", s_achievement_complete);
			} else {
				menu_cell_basic_draw(ctx, cell_layer, "Locked", "Keep breathing!", s_achievement_incomplete);
			}
			break;
		case 2:
			if (achievement_get_one_year_streak().complete == 1) {
				menu_cell_basic_draw(ctx, cell_layer, "1 Year", "One year streak!", s_achievement_complete);
			} else {
				menu_cell_basic_draw(ctx, cell_layer, "Locked", "Keep breathing!", s_achievement_incomplete);
			}
			break;
		case 3:
			if (achievement_get_five_minutes_day().complete == 1) {
				menu_cell_basic_draw(ctx, cell_layer, "5 Minutes", "5 minutes in 1 day", s_achievement_complete);
			} else {
				menu_cell_basic_draw(ctx, cell_layer, "Locked", "Keep breathing!", s_achievement_incomplete);
			}
			break;
		case 4:
			if (achievement_get_ten_minutes_day().complete == 1) {
				menu_cell_basic_draw(ctx, cell_layer, "10 Minutes", "10 minutes in 1 day", s_achievement_complete);
			} else {
				menu_cell_basic_draw(ctx, cell_layer, "Locked", "Keep breathing!", s_achievement_incomplete);
			}
			break;
		case 5:
			if (achievement_get_thirty_minutes_day().complete == 1) {
				menu_cell_basic_draw(ctx, cell_layer, "30 Minutes", "30 minutes in 1 day", s_achievement_complete);
			} else {
				menu_cell_basic_draw(ctx, cell_layer, "Locked", "Keep breathing!", s_achievement_incomplete);
			}
			break;
		case 6:
			if (achievement_get_one_hour_day().complete == 1) {
				menu_cell_basic_draw(ctx, cell_layer, "1 hour", "1 hour in 1 day", s_achievement_complete);
			} else {
				menu_cell_basic_draw(ctx, cell_layer, "Locked", "Keep breathing!", s_achievement_incomplete);
			}
			break;
		case 7:
			if (achievement_get_five_minutes_session().complete == 1) {
				menu_cell_basic_draw(ctx, cell_layer, "5 Minutes", "5 minutes in 1 session", s_achievement_complete);
			} else {
				menu_cell_basic_draw(ctx, cell_layer, "Locked", "Keep breathing!", s_achievement_incomplete);
			}
			break;
		case 8:
			if (achievement_get_eight_minutes_session().complete == 1) {
				menu_cell_basic_draw(ctx, cell_layer, "8 Minutes", "8 minutes in 1 session", s_achievement_complete);
			} else {
				menu_cell_basic_draw(ctx, cell_layer, "Locked", "Keep breathing!", s_achievement_incomplete);
			}
			break;
		case 9:
			if (achievement_get_thirty_minutes_day().complete == 1) {
				menu_cell_basic_draw(ctx, cell_layer, "30 Minutes", "30 minutes in 1 day", s_achievement_complete);
			} else {
				menu_cell_basic_draw(ctx, cell_layer, "Locked", "Keep breathing!", s_achievement_incomplete);
			}
			break;
		case 10:
			if (achievement_get_changed_settings().complete == 1) {
				menu_cell_basic_draw(ctx, cell_layer, "Customizer", "Changed settings", s_achievement_complete);
			} else {
				menu_cell_basic_draw(ctx, cell_layer, "Locked", "Keep breathing!", s_achievement_incomplete);
			}
		break;
		case 11:
			if (achievement_get_completionist().complete == 1) {
				menu_cell_basic_draw(ctx, cell_layer, "Completionist", "Gotten all achievements", s_achievement_complete);
			} else {
				menu_cell_basic_draw(ctx, cell_layer, "Locked", "Keep breathing!", s_achievement_incomplete);
			}
		break;
	}
}

void achievement_window_load(Window *window) {
	// Load the achievement icons
	s_achievement_complete = gbitmap_create_with_resource(RESOURCE_ID_ACHIEVEMENT_COMPLETE_ICON);
	s_achievement_incomplete = gbitmap_create_with_resource(RESOURCE_ID_ACHIEVEMENT_INCOMPLETE_ICON);
	
	// Load the MenuLayer
	Layer *window_layer = window_get_root_layer(s_achievement_window);
	GRect bounds = layer_get_frame(window_layer);
	
	// Create the menu layer
  s_achievement_layer = menu_layer_create(bounds);
	menu_layer_set_normal_colors(s_achievement_layer, settings_get_backgroundColor(), gcolor_legible_over(settings_get_backgroundColor()));
	menu_layer_set_highlight_colors(s_achievement_layer, gcolor_legible_over(settings_get_backgroundColor()), settings_get_backgroundColor());
  menu_layer_set_callbacks(s_achievement_layer, NULL, (MenuLayerCallbacks){
    .get_num_sections = menu_get_num_sections_callback,
    .get_num_rows = menu_get_num_rows_callback,
		.draw_header = menu_draw_header_callback,
    .get_header_height = menu_get_header_height_callback,
    .draw_row = menu_draw_row_callback
  });
	
	// Bind the menu layer's click config provider to the window for interactivity
	menu_layer_set_click_config_onto_window(s_achievement_layer, window);

	layer_add_child(window_layer, menu_layer_get_layer(s_achievement_layer));
}

void achievement_window_unload() {
	menu_layer_destroy(s_achievement_layer);
	gbitmap_destroy(s_achievement_complete);
	gbitmap_destroy(s_achievement_incomplete);
}

void achievement_menu_window_push() {
	s_achievement_window = window_create();
	window_set_window_handlers(s_achievement_window, (WindowHandlers) {
		.load = achievement_window_load,
		.unload = achievement_window_unload,
	});
	window_stack_push(s_achievement_window, true);
}