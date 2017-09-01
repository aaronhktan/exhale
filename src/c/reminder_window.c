#include <pebble.h>
#include "reminder_window.h"
#include "src/c/breathe_window.h"
#include "localize.h"

#define DELTA 33
#ifdef PBL_PLATFORM_EMERY
#define FONT_KEY FONT_KEY_GOTHIC_28_BOLD
#elif PBL_PLATFORM_CHALK
#define FONT_KEY FONT_KEY_GOTHIC_18_BOLD
#else
#define FONT_KEY FONT_KEY_GOTHIC_24_BOLD
#endif

static Window *s_reminder_window;
static ActionMenu *s_action_menu;
static ActionMenuLevel *s_root_level, *s_snooze_level;
static int s_min_to_breathe;
static Layer *s_canvas_layer;
static TextLayer *s_text_layer;
static AppTimer *s_close_timer, *s_next_frame_timer;
#if !PBL_PLATFORM_APLITE
static GDrawCommandSequence *s_command_seq;
#else
static GBitmap *s_remind_bitmap;
static BitmapLayer *s_bitmap_layer;
#endif
static GColor random_color, text_color;
static WakeupId id;

static int s_index = 0;

// Closes app
static void close_app() {
	window_stack_pop_all(true);
}

// Finds and displays the next frame in PDC
static void next_frame_handler(void *context) {
	layer_mark_dirty(s_canvas_layer);
	s_next_frame_timer = app_timer_register(DELTA, next_frame_handler, NULL);
}

// Method to update the PDC layer
static void canvas_update_proc(Layer *layer, GContext *ctx) {
	GRect bounds = layer_get_bounds(layer);
	
	#if !PBL_PLATFORM_APLITE
	GSize seq_bounds = gdraw_command_sequence_get_bounds_size(s_command_seq);

	GDrawCommandFrame *frame = gdraw_command_sequence_get_frame_by_index(s_command_seq, s_index); // Grabs frame from PDC

	if (frame) { // A next frame was found
		gdraw_command_frame_draw(ctx, s_command_seq, frame, GPoint(
			(bounds.size.w - seq_bounds.w) / 2,
			(bounds.size.h - seq_bounds.h) / 2
		));
	}

	// Advance to the next frame, wrapping if neccessary
	int num_frames = gdraw_command_sequence_get_num_frames(s_command_seq);
	s_index++;
	if (s_index == num_frames) {
		s_index = 0;
	}
	#else
		
	#endif
	
	#if PBL_COLOR
	graphics_context_set_fill_color(ctx, gcolor_legible_over(random_color));
	#else
	graphics_context_set_fill_color(ctx, GColorBlack);
	#endif

	#ifdef PBL_PLATFORM_EMERY
	graphics_fill_circle(ctx, GPoint(bounds.size.w + 7, bounds.size.h / 2), 12);
	#elif PBL_PLATFORM_CHALK
	graphics_fill_circle(ctx, GPoint(bounds.size.w + 1, bounds.size.h / 2), 12);
	#else
	graphics_fill_circle(ctx, GPoint(bounds.size.w + 5, bounds.size.h / 2), 10);
	#endif
}

static void action_performed_callback(ActionMenu *action_menu, const ActionMenuItem *action, void *context) {
	// Some amount of minutes was selected; find which one and pass to breathe_window to start
	s_min_to_breathe = (int)action_menu_item_get_action_data(action);
	app_timer_cancel(s_close_timer);
	breathe_window_push(s_min_to_breathe);
	window_stack_remove(s_reminder_window, false);
}

static void snooze_performed_callback(ActionMenu *action_menu, const ActionMenuItem *action, void *context) {
	// Some amount of minutes to snooze was selected; find out how much and set a new wakeup
	time_t future_timestamp = time(NULL) + ((int)action_menu_item_get_action_data(action) * SECONDS_PER_MINUTE);
	
	// Schedule the wakeup
	id = wakeup_schedule(future_timestamp, 0, true);
	
	// Log when the wakeup is set
	char future_time_string[9];
	struct tm *future_time = localtime(&future_timestamp);
	strftime(future_time_string, sizeof(future_time_string), "%H:%M:%S", future_time);
	APP_LOG(APP_LOG_LEVEL_DEBUG, "The app is scheduled to wake up at %s.", future_time_string);
	
	// Persist the id so future can query it
	persist_write_int(SNOOZE_WAKEUP, id);
	APP_LOG(APP_LOG_LEVEL_DEBUG, "The wakeup id is %d.", (int)(persist_read_int(SNOOZE_WAKEUP)));
	
	// Quit the app
	window_stack_pop_all(true);
}

static void init_action_menu() {
	// Create the root level
	s_root_level = action_menu_level_create(4);

	// Set up the actions for this level, using action context to pass types
	static char menu_text[4][16];
	for (int i = 1; i < 4; i++) {
		static char actionmenu_entry_text[16];
		snprintf(actionmenu_entry_text, sizeof(actionmenu_entry_text), localize_get_reminder_action_menu_text(), i); 
		strcpy(menu_text[i], actionmenu_entry_text);
		action_menu_level_add_action(s_root_level, menu_text[i], action_performed_callback, (void *)i);
	}
	
	// Create level for snoozing and add as child to root level
	s_snooze_level = action_menu_level_create(5);
	action_menu_level_add_child(s_root_level, s_snooze_level, "Snooze");
	
	// Add options to the snooze level
	static char snooze_text[6][22];
	for (int i = 1; i <= 5; i++) { // This is used for the index of the array
		static char snooze_entry_text[22];
		snprintf(snooze_entry_text, sizeof(snooze_entry_text), localize_get_snooze_text(), i * 10);
		strcpy(snooze_text[i], snooze_entry_text);
		action_menu_level_add_action(s_snooze_level, snooze_text[i], snooze_performed_callback, (void *)(i * 10));
	}
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
	// ActionMenu
	ActionMenuConfig config = (ActionMenuConfig) {
		.root_level = s_root_level,
		.colors = {
			.background = random_color,
			.foreground = GColorBlack,
		},
		.align = ActionMenuAlignTop
	};
	
	// Show the ActionMenu
	s_action_menu = action_menu_open(&config);
	
	// Cancel the automatic close timer because the user interacted with their watch
	app_timer_cancel(s_close_timer);
}

static void click_config_provider(void *context) {
	ButtonId id_select = BUTTON_ID_SELECT; // The Select button
	window_single_click_subscribe(id_select, select_click_handler);
}

static void reminder_window_load(Window *window) {
	// Information about screen
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_bounds(window_layer);

	// Layer for text
	s_text_layer = text_layer_create(GRect(0, bounds.size.w - PBL_IF_RECT_ELSE(16, 50), bounds.size.w, bounds.size.h / 2));
	text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY));
	text_layer_set_background_color(s_text_layer, random_color);
	text_layer_set_text_color(s_text_layer, text_color);
	text_layer_set_text_alignment(s_text_layer, GTextAlignmentCenter);
	
	text_layer_set_text(s_text_layer, localize_get_reminder_text());
	
	layer_add_child(window_layer, text_layer_get_layer(s_text_layer));
	
	// Layer for PDC
	s_canvas_layer = layer_create(bounds);
	layer_set_update_proc(s_canvas_layer, canvas_update_proc);
	
	layer_add_child(window_layer, s_canvas_layer);
	
	#if !PBL_PLATFORM_APLITE
	// Create sequence from PDC
	s_command_seq = gdraw_command_sequence_create_with_resource(RESOURCE_ID_ALARM_SEQUENCE);
	s_next_frame_timer = app_timer_register(DELTA, next_frame_handler, NULL);
	#else
	// Load static image because not enough memory for PDC
	s_remind_bitmap = gbitmap_create_with_resource(RESOURCE_ID_ALARM_BITMAP);
	s_bitmap_layer = bitmap_layer_create(bounds);
	layer_add_child(s_canvas_layer, bitmap_layer_get_layer(s_bitmap_layer));
	bitmap_layer_set_bitmap(s_bitmap_layer, s_remind_bitmap);
	bitmap_layer_set_compositing_mode(s_bitmap_layer, GCompOpSet);
	#endif
	
	init_action_menu();
	
	// Start timer to close the app after 30 seconds if the user doesn't respond as to not waste battery
	s_close_timer = app_timer_register(30000, close_app, NULL);
		
	// Only vibrate when the watch isn't in Quiet Time
	if (!quiet_time_is_active()){
		vibes_double_pulse();
	}
}

// DESTROY ALL THE THINGS (hopefully)
static void reminder_window_unload(Window *window) {
	app_timer_cancel(s_next_frame_timer);
	app_timer_cancel(s_close_timer);
	text_layer_destroy(s_text_layer);
	#if !PBL_PLATFORM_APLITE
	gdraw_command_sequence_destroy(s_command_seq);
	#else
	bitmap_layer_destroy(s_bitmap_layer);
	gbitmap_destroy(s_remind_bitmap);
	#endif
	layer_destroy(s_canvas_layer);
	action_menu_hierarchy_destroy(s_root_level, NULL, NULL);
	window_destroy(s_reminder_window);
	s_reminder_window = NULL;
}

// Method to open and display this window
void reminder_window_push() {
	#if PBL_COLOR
	random_color = (GColor){ .a = 3, .r = rand() % 4, .g = rand() % 4, .b = rand() % 4 }; // Random color. Cool.
	while (random_color.r == 0 && random_color.g == 0 && random_color.b == 0) {
		random_color = (GColor){ .a = 3, .r = rand() % 4, .g = rand() % 4, .b = rand() % 4 }; // To make sure that the background color is not black.
	}
	text_color = gcolor_legible_over(random_color);
	#else
	random_color = GColorWhite;
	text_color = GColorBlack;
	#endif
	
	s_reminder_window = window_create();
	window_set_window_handlers(s_reminder_window, (WindowHandlers) {
		.load = reminder_window_load, 
		.unload = reminder_window_unload,
	});
	window_set_click_config_provider(s_reminder_window, click_config_provider);
	window_set_background_color(s_reminder_window, PBL_IF_COLOR_ELSE(random_color, GColorWhite));
	window_stack_push(s_reminder_window, true);
	
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Heap free is %d after launching the reminder window.", (int)heap_bytes_free());
}