#include <pebble.h>
#include "achievement_window.h"
#include "localize.h"

#define DELTA 50

#ifdef PBL_PLATFORM_EMERY
	#define FONT_KEY FONT_KEY_GOTHIC_18_BOLD
	#define LARGE_FONT_KEY FONT_KEY_GOTHIC_24_BOLD
#elif PBL_PLATFORM_CHALK
	#define FONT_KEY FONT_KEY_GOTHIC_14
	#define LARGE_FONT_KEY FONT_KEY_GOTHIC_18_BOLD
#else
	#define FONT_KEY FONT_KEY_GOTHIC_14_BOLD
	#define LARGE_FONT_KEY FONT_KEY_GOTHIC_18_BOLD
#endif

#if !PBL_PLATFORM_APLITE
static Window *s_achievement_window;
static Layer *s_canvas_layer;
static TextLayer *s_announce_text_layer, *s_title_layer, *s_description_layer;
static GDrawCommandSequence *s_command_seq;
static GBitmap *s_achievement_bitmap;
static BitmapLayer *s_bitmap_layer;
static GColor random_color, text_color;
static AppTimer *s_timer;
static char *s_achievement_name, *s_achievement_description;
static bool s_draw_complete;
static int s_index = 0;

// Finds and displays the next frame in PDC
static void next_frame_handler(void *context) {
	APP_LOG(APP_LOG_LEVEL_DEBUG, "The next frame was loaded.");
	layer_mark_dirty(s_canvas_layer);
	s_timer = app_timer_register(DELTA, next_frame_handler, NULL);
}

// Method to update the PDC layer
static void canvas_update_proc(Layer *layer, GContext *ctx) {
	GRect bounds = layer_get_bounds(layer);
	GSize seq_bounds = gdraw_command_sequence_get_bounds_size(s_command_seq);

	GDrawCommandFrame *frame = gdraw_command_sequence_get_frame_by_index(s_command_seq, s_index); // Grabs frame from PDC

	if (frame) { // A next frame was found
		gdraw_command_frame_draw(ctx, s_command_seq, frame, GPoint(
			(bounds.size.w - seq_bounds.w) / 2,
			(bounds.size.h - seq_bounds.h) / 2
		));
	}

	// Advance to the next frame, stopping when done
	int num_frames = gdraw_command_sequence_get_num_frames(s_command_seq);
	s_index++;
	APP_LOG(APP_LOG_LEVEL_DEBUG, "The number of frames in the sequence is %d and the played number of frames is %d.", num_frames, s_index);
	if (s_index >= num_frames) {
		--s_index;
		app_timer_cancel(s_timer);
		s_draw_complete = true;
	}
}

// Allow exiting the window only after the PDC is done animating; this prevents a crash.
static void back_click_handler(ClickRecognizerRef recognizer, void *context) {
	if (s_draw_complete) {
		window_stack_pop(true);
	}
}

static void click_config_provider(void *context) {
	window_single_click_subscribe(BUTTON_ID_BACK, back_click_handler);
}

static void achievement_window_load(Window *window) {
	// Information about screen
	GRect bounds = layer_get_bounds(window_get_root_layer(window));
	
	// Layer for text with achievement description
	s_description_layer = text_layer_create(GRect(bounds.size.w / 12, PBL_IF_RECT_ELSE(bounds.size.h * 11 / 16, bounds.size.h * 9 / 32), bounds.size.w * 5 / 6, bounds.size.h / 3));
	text_layer_set_text(s_description_layer, s_achievement_description);
	// Set text location a little bit lower if the text isn't that big
	#if PBL_RECT
	if (text_layer_get_content_size(s_description_layer).h < bounds.size.h / 4) {
		text_layer_destroy(s_description_layer);
		s_description_layer = text_layer_create(GRect(bounds.size.w / 12, bounds.size.h * 3 / 4, bounds.size.w * 5 / 6, bounds.size.h / 3));
	}
	#endif
	text_layer_set_text(s_description_layer, s_achievement_description);
	text_layer_set_font(s_description_layer, fonts_get_system_font(FONT_KEY));
	text_layer_set_background_color(s_description_layer, GColorClear);
	text_layer_set_text_color(s_description_layer, text_color);
	text_layer_set_text_alignment(s_description_layer, GTextAlignmentCenter);
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_description_layer));

	// Layer for PDC
	#if PBL_RECT
	if (text_layer_get_content_size(s_description_layer).h < bounds.size.h / 4) {
			s_canvas_layer = layer_create(GRect(0, -bounds.size.h / 24, bounds.size.w, bounds.size.h));
	} else {
	#endif
		s_canvas_layer = layer_create(GRect(0, PBL_IF_RECT_ELSE(-bounds.size.h / 12, bounds.size.h / 4), bounds.size.w, bounds.size.h));
	#if PBL_RECT
	}
	#endif
	layer_set_update_proc(s_canvas_layer, canvas_update_proc);
	layer_add_child(window_get_root_layer(window), s_canvas_layer);
	
	// Layer for top text
	s_announce_text_layer = text_layer_create(GRect(0, PBL_IF_RECT_ELSE(0, 15), bounds.size.w, bounds.size.h / 6));
	text_layer_set_font(s_announce_text_layer, fonts_get_system_font(LARGE_FONT_KEY));
	text_layer_set_background_color(s_announce_text_layer, GColorClear);
	text_layer_set_text_color(s_announce_text_layer, text_color);
	text_layer_set_text_alignment(s_announce_text_layer, GTextAlignmentCenter);
	text_layer_set_text(s_announce_text_layer, localize_get_achievement_text());
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_announce_text_layer));
	
	// Layer for text with achievement name
	#if PBL_RECT
	if (text_layer_get_content_size(s_description_layer).h < bounds.size.h / 4) {
		s_title_layer = text_layer_create(GRect(0, bounds.size.h * 5 / 8, bounds.size.w, bounds.size.h / 6));
	} else {
	#endif
		s_title_layer = text_layer_create(GRect(0, PBL_IF_RECT_ELSE(bounds.size.h * 9 / 16, bounds.size.h * 3 / 16), bounds.size.w, bounds.size.h / 6));
	#if PBL_RECT
	}
	#endif
	text_layer_set_font(s_title_layer, fonts_get_system_font(LARGE_FONT_KEY));
	text_layer_set_background_color(s_title_layer, GColorClear);
	text_layer_set_text_color(s_title_layer, text_color);
	text_layer_set_text_alignment(s_title_layer, GTextAlignmentCenter);
	text_layer_set_text(s_title_layer, s_achievement_name);
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_title_layer));
	
	// Miscellaneous
	window_set_background_color(s_achievement_window, PBL_IF_COLOR_ELSE(random_color, GColorWhite));
	vibes_double_pulse();
	if (s_command_seq) {
		s_timer = app_timer_register(DELTA, next_frame_handler, NULL);
	} else {
		s_draw_complete = true;
		s_bitmap_layer = bitmap_layer_create(GRect(0, 0, bounds.size.w, PBL_IF_RECT_ELSE(bounds.size.h * 3 / 4, bounds.size.h * 7 / 8)));
		layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_bitmap_layer));
		bitmap_layer_set_compositing_mode(s_bitmap_layer, GCompOpSet);
		bitmap_layer_set_bitmap(s_bitmap_layer, s_achievement_bitmap);
		#if PBL_ROUND
			bitmap_layer_set_alignment(s_bitmap_layer, GAlignBottom);
		#endif
	}
}

// DESTROY ALL THE THINGS (hopefully)
static void achievement_window_unload(Window *window) {
	if (s_command_seq) {
		gdraw_command_sequence_destroy(s_command_seq);
	} else {
		bitmap_layer_destroy(s_bitmap_layer);
		gbitmap_destroy(s_achievement_bitmap);
	}
	layer_destroy(s_canvas_layer);
	text_layer_destroy(s_announce_text_layer);
	text_layer_destroy(s_title_layer);
	text_layer_destroy(s_description_layer);
	s_index = 0;
	s_draw_complete = false;
	window_destroy(s_achievement_window);
}

// Method to open and display this window
void achievement_window_push(char *achievement_name, char *achievement_description) {
	s_achievement_name = achievement_name;
	s_achievement_description = achievement_description;
	
	// Create sequence from PDC
	s_command_seq = gdraw_command_sequence_create_with_resource(RESOURCE_ID_ACHIEVEMENT_SEQUENCE);
	
	if (s_command_seq) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "The command sequence was successfully created.");
	} else { // Not enough memory to allocate for PDC; fall back to displaying static PNG
		APP_LOG(APP_LOG_LEVEL_DEBUG, "The command sequence was not successfully created.");
		s_achievement_bitmap = gbitmap_create_with_resource(RESOURCE_ID_ACHIEVEMENT_COMPLETE_BITMAP);
	}
	
	#if PBL_COLOR
		random_color = (GColor){ .a = 3, .r = rand() % 4, .g = rand() % 4, .b = rand() % 4 }; // Random color. Cool.
		while (random_color.r == 0 && random_color.g == 0 && random_color.b == 0) {
			APP_LOG(APP_LOG_LEVEL_DEBUG, "The random colour generated was black! Oh no!");
			random_color = (GColor){ .a = 3, .r = rand() % 4, .g = rand() % 4, .b = rand() % 4 }; // To make sure that the background color is not black.
		}
		text_color = gcolor_legible_over(random_color);
	#else
		random_color = GColorWhite;
		text_color = GColorBlack;
	#endif
	
	s_achievement_window = window_create();
	
	window_set_window_handlers(s_achievement_window, (WindowHandlers) {
		.load = achievement_window_load, 
		.unload = achievement_window_unload
	});
	
	window_stack_push(s_achievement_window, true);

	window_set_click_config_provider(s_achievement_window, click_config_provider);
}
#endif