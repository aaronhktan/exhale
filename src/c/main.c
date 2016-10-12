#include <pebble.h>
//creates a pointer for main window
static Window *s_main_window;
static Layer *s_drawing_layer;
static TextLayer *s_greet_layer;
static ActionBarLayer *s_main_actionbar;
static GBitmap *s_icon_plus;
static uint8_t s_radius_final, s_radius;
static int s_minutes_breathed = 0;
static bool s_animating = false;
static GPoint s_center;
static char s_greeting_text[14];

static void canvas_update_proc(Layer *s_drawing_layer, GContext *ctx) {
	graphics_context_set_stroke_color(ctx, GColorVividCerulean);
	graphics_context_set_stroke_width(ctx, 5);
	graphics_draw_circle(ctx, s_center, s_radius);
}

static void circle_animation_started(Animation *anim, void *context) {
	s_animating = true;
}

static void circle_animation_stopped(Animation *anim, bool stopped, void *context) {
	s_animating = false;
}
	
static void circle_animate(int duration, int delay, AnimationImplementation *implementation) {
	// creates animation instance
  Animation *anim = animation_create();
  animation_set_duration(anim, duration);
  animation_set_delay(anim, delay);
  animation_set_curve(anim, AnimationCurveEaseInOut);
	// sets what the instance does with implementation passed in to argument
  animation_set_implementation(anim, implementation);
	animation_set_handlers(anim, (AnimationHandlers) {
		.started = circle_animation_started,
		.stopped = circle_animation_stopped
	}, NULL);
	// schedules animation to start
  animation_schedule(anim);
}

static void radius_update(Animation *anim, AnimationProgress dist_normalized) {
	s_radius = dist_normalized * s_radius_final / ANIMATION_NORMALIZED_MAX;
	layer_mark_dirty(s_drawing_layer);
}

static void circle_animation_setup() {
	// set the update method for animation to radius_update
	static AnimationImplementation s_circle_impl = {
		.update = radius_update
	};
	// starts animation
	circle_animate(2000, 500, &s_circle_impl);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
	s_minutes_breathed += 1;
	snprintf(s_greeting_text, 14, "30 DAY AVG: %d", s_minutes_breathed);
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Breathing minutes is now %d", s_minutes_breathed);
	text_layer_set_text(s_greet_layer, s_greeting_text);
	layer_mark_dirty(s_drawing_layer);
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, (ClickHandler) select_click_handler);
}

//function for when window loads
static void main_window_load(Window *window){
	//sets bounds of drawing layer
	GRect bounds = layer_get_bounds(window_get_root_layer(s_main_window));
	
	//creates drawing layer and sets update process
	s_drawing_layer = layer_create(bounds);
	layer_set_update_proc(s_drawing_layer, canvas_update_proc);
	
	//adds drawing layer to the window
	layer_add_child(window_get_root_layer(s_main_window), s_drawing_layer);
	layer_mark_dirty(s_drawing_layer);
	
	s_center = GPoint((bounds.size.w - 30) / 2, (bounds.size.h - 30) / 2); // sets the center of the circle to be the center of the screen
	s_radius_final = bounds.size.w / 3; // sets the final radius of the circle
	circle_animation_setup(); // starts the circle animation
	
	s_main_actionbar = action_bar_layer_create();
	action_bar_layer_set_background_color(s_main_actionbar, GColorVividCerulean);
	action_bar_layer_set_icon(s_main_actionbar, BUTTON_ID_SELECT, s_icon_plus);
	action_bar_layer_add_to_window(s_main_actionbar, s_main_window);
	action_bar_layer_set_click_config_provider(s_main_actionbar,
																						 click_config_provider);
	
	s_greet_layer = text_layer_create(GRect(5, bounds.size.h - 25, bounds.size.w - 40, 20));
	text_layer_set_background_color(s_greet_layer, GColorVividCerulean);
	text_layer_set_text_color(s_greet_layer, GColorWhite);
	text_layer_set_text(s_greet_layer, "30 DAY AVG: 0");
	text_layer_set_text_alignment(s_greet_layer, GTextAlignmentCenter);
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_greet_layer));
}

//function for when window is destroyed
static void main_window_unload(Window *window) {
	
}

static void init() {
	
	s_icon_plus = gbitmap_create_with_resource(RESOURCE_ID_S_ICON_PLUS);
	
	//create main window and assign to pointer
	s_main_window = window_create();
	
	//set funcions to handle window
	window_set_window_handlers(s_main_window, (WindowHandlers) {
		.load = main_window_load,
		.unload = main_window_unload
	});
	
	//show window on the watch, with animated = true
	window_stack_push(s_main_window, true);
}

static void deinit() {
	window_destroy(s_main_window);
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}