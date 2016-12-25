#include <pebble.h>
#include "achievement_window.h"
#include "localize.h"

#define DELTA 50

#ifdef PBL_PLATFORM_EMERY
	#define FONT_KEY FONT_KEY_GOTHIC_18_BOLD
#elif PBL_PLATFORM_CHALK
	#define FONT_KEY FONT_KEY_GOTHIC_14_BOLD
#else
	#define FONT_KEY FONT_KEY_GOTHIC_14_BOLD
#endif

static Window *s_achievement_window;
static Layer *s_canvas_layer;
static TextLayer *s_text_layer;
static GDrawCommandSequence *s_command_seq;
GColor random_color, text_color;
static AppTimer *s_timer;

static int s_index = 0;

// Finds and displays the next frame in PDC
static void next_frame_handler(void *context) {
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

	// Advance to the next frame, wrapping if neccessary
	int num_frames = gdraw_command_sequence_get_num_frames(s_command_seq);
	s_index++;
	if (s_index == num_frames) {
		--s_index;
		app_timer_cancel(s_timer);
	}
	
// 	graphics_context_set_fill_color(ctx, GColorBlack);
// 	#ifdef PBL_PLATFORM_EMERY
// 		graphics_fill_circle(ctx, GPoint(bounds.size.w + 7, bounds.size.h / 2), 12);
// 	#elif PBL_PLATFORM_CHALK
// 		graphics_fill_circle(ctx, GPoint(bounds.size.w + 1, bounds.size.h / 2), 12);
// 	#else
// 		graphics_fill_circle(ctx, GPoint(bounds.size.w + 5, bounds.size.h / 2), 10);
// 	#endif
}

static void achievement_window_load(Window *window) {
	// Information about screen
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_bounds(window_layer);

	// Layer for PDC
	s_canvas_layer = layer_create(bounds);
	layer_set_update_proc(s_canvas_layer, canvas_update_proc);
	
	layer_add_child(window_layer, s_canvas_layer);
	
	window_set_background_color(s_achievement_window, PBL_IF_COLOR_ELSE(random_color, GColorWhite));
	
	// Layer for text
	s_text_layer = text_layer_create(GRect(0, 0, bounds.size.w, bounds.size.h / 6));
	text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY));
	text_layer_set_background_color(s_text_layer, random_color);
	text_layer_set_text_alignment(s_text_layer, GTextAlignmentCenter);
	text_layer_set_text(s_text_layer, localize_get_achievement_text());
	
	layer_add_child(window_layer, text_layer_get_layer(s_text_layer));
	
	vibes_double_pulse();
}

// DESTROY ALL THE THINGS (hopefully)
static void achievement_window_unload(Window *window) {
	layer_destroy(s_canvas_layer);
	layer_destroy(text_layer_get_layer(s_text_layer));
	gdraw_command_sequence_destroy(s_command_seq);
	window_destroy(s_achievement_window);
}

// Method to open and display this window
void achievement_window_push() {
	// Create sequence from PDC
	s_command_seq = gdraw_command_sequence_create_with_resource(RESOURCE_ID_ACHIEVEMENT_SEQUENCE);
	
	#if PBL_COLOR
		random_color = (GColor){ .a = 3, .r = rand() % 4, .g = rand() % 4, .b = rand() % 4 }; // Random color. Cool.
		while (random_color.r == 0 && random_color.g == 0 && random_color.b == 0) {
			APP_LOG(APP_LOG_LEVEL_DEBUG, "The random colour generated was black! Oh no!");
			random_color = (GColor){ .a = 3, .r = rand() % 4, .g = rand() % 4, .b = rand() % 4 }; // To make sure that the background color is not black.
		}
	#else
		random_color = GColorWhite;
	#endif

	
	s_achievement_window = window_create();
	
	window_set_window_handlers(s_achievement_window, (WindowHandlers) {
		.load = achievement_window_load, 
		.unload = achievement_window_unload
	});
	
	window_stack_push(s_achievement_window, true);
	
	s_timer = app_timer_register(DELTA, next_frame_handler, NULL);
}