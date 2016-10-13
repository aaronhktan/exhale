#include <pebble.h>
//creates a pointer for main window
static Window *s_main_window;
static Layer *s_drawing_layer;
static AppTimer *s_animation_completed_timer;
static GPath *s_up_triangle, *s_down_triangle;
static GRect bounds;
static const 	GPathInfo UP_PATH_INFO = {
		.num_points = 3,
		.points = (GPoint []) {{72, 36}, {77, 41}, {67, 41}}
	};
static const GPathInfo DOWN_PATH_INFO = {
		.num_points = 3,
	.points = (GPoint []) {{72, 131}, {67, 126}, {77, 126}}
	};
static uint8_t s_radius_final, s_radius = 30;
static int s_minutes_breathed = 1;
static bool s_animating = false, s_animation_completed = false;
static GPoint s_center;
static char s_greeting_text[2] = "1", s_instruct_text[8], s_min_text[5];


static void canvas_update_proc(Layer *s_drawing_layer, GContext *ctx) {
	graphics_context_set_text_color(ctx, GColorBlack);
	GSize greet_text_bounds = graphics_text_layout_get_content_size("8", fonts_get_system_font(FONT_KEY_LECO_42_NUMBERS), 
																																	GRect(0, 0, bounds.size.w, bounds.size.h), 
																							GTextOverflowModeWordWrap, GTextAlignmentCenter);
	graphics_draw_text(ctx, s_greeting_text, fonts_get_system_font(FONT_KEY_LECO_42_NUMBERS), 
                     GRect((bounds.size.w - greet_text_bounds.w) / 2, (bounds.size.h - greet_text_bounds.h) / 2 - 6, greet_text_bounds.w, greet_text_bounds.h), 
										 GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
	
	GSize instruct_text_bounds = graphics_text_layout_get_content_size("BREATHE", fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD), 
																																	GRect(0, 0, bounds.size.w, bounds.size.h), 
																							GTextOverflowModeWordWrap, GTextAlignmentCenter);
	graphics_draw_text(ctx, s_instruct_text, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD), 
                     GRect((bounds.size.w - instruct_text_bounds.w) / 2, (bounds.size.h - instruct_text_bounds.h) / 2 - 30, instruct_text_bounds.w, instruct_text_bounds.h), 
										 GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
	
	GSize min_text_bounds = graphics_text_layout_get_content_size("MINS", fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD), 
																																	GRect(0, 0, bounds.size.w, bounds.size.h), 
																							GTextOverflowModeWordWrap, GTextAlignmentCenter);
	graphics_draw_text(ctx, s_min_text, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD), 
                     GRect((bounds.size.w - min_text_bounds.w) / 2, (bounds.size.h - min_text_bounds.h) / 2 + 26, min_text_bounds.w, min_text_bounds.h), 
										 GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
	
	graphics_context_set_stroke_color(ctx, GColorVividCerulean);
	graphics_context_set_stroke_width(ctx, 5);
	graphics_draw_circle(ctx, s_center, s_radius);
	
	if (s_animation_completed) {
		
		//draw side circle
		graphics_context_set_fill_color(ctx, GColorBlack);
		graphics_fill_circle(ctx, GPoint(bounds.size.w + 5, bounds.size.h / 2), 10);
		
		// draw triangles
		gpath_draw_filled(ctx, s_up_triangle);
		gpath_draw_filled(ctx, s_down_triangle);
	}
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
	s_radius = dist_normalized * s_radius_final / ANIMATION_NORMALIZED_MAX + 30;
	layer_mark_dirty(s_drawing_layer);
}

static void circle_animation_setup() {
	// set the update method for animation to radius_update
	static AnimationImplementation s_circle_impl = {
		.update = radius_update
	};
	// starts animation
	circle_animate(1500, 250, &s_circle_impl);
}

static void finish_setup_callback(void *context) {
	snprintf(s_instruct_text, 12, "%s", "BREATHE");
	snprintf(s_min_text, 5, "%s", "MIN");
	s_animation_completed = true;
	
	s_up_triangle = gpath_create(&UP_PATH_INFO);
	s_down_triangle = gpath_create(&DOWN_PATH_INFO);
	
	layer_mark_dirty(s_drawing_layer);
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
	s_minutes_breathed += 1;
	if (s_minutes_breathed == 1) {
		snprintf(s_min_text, 5, "%s", "MIN");
	}
	else {
		snprintf(s_min_text, 5, "%s", "MINS");
	}
	snprintf(s_greeting_text, 2, "%d", s_minutes_breathed);
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Breathing minutes is now %d", s_minutes_breathed);
	layer_mark_dirty(s_drawing_layer);
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
	s_minutes_breathed -= 1;
	if (s_minutes_breathed == 1) {
		snprintf(s_min_text, 5, "%s", "MIN");
	}
	else {
		snprintf(s_min_text, 5, "%s", "MINS");
	}
	snprintf(s_greeting_text, 2, "%d", s_minutes_breathed);
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Breathing minutes is now %d", s_minutes_breathed);
	layer_mark_dirty(s_drawing_layer);
}

static void click_config_provider(void *context) {
  ButtonId id_up = BUTTON_ID_UP;  // The Up button
	ButtonId id_down = BUTTON_ID_DOWN; // The Down button
  window_single_click_subscribe(id_up, up_click_handler);
	window_single_click_subscribe(id_down, down_click_handler);
}

//function for when window loads
static void main_window_load(Window *window){
	//sets bounds of drawing layer
	bounds = layer_get_bounds(window_get_root_layer(s_main_window));
	
	//creates drawing layer and sets update process
	s_drawing_layer = layer_create(bounds);
	layer_set_update_proc(s_drawing_layer, canvas_update_proc);
	
	//adds drawing layer to the window
	layer_add_child(window_get_root_layer(s_main_window), s_drawing_layer);
	layer_mark_dirty(s_drawing_layer);
	
	s_center = grect_center_point(&bounds); // sets the center of the circle to be the center of the screen
	s_radius_final = 30; // sets the final radius of the circle
	circle_animation_setup(); // starts the circle animation
}

//function for when window is destroyed
static void main_window_unload(Window *window) {
	
}

static void init() {
	
	//create main window and assign to pointer
	s_main_window = window_create();
	
	//set funcions to handle window
	window_set_window_handlers(s_main_window, (WindowHandlers) {
		.load = main_window_load,
		.unload = main_window_unload
	});
	
	window_set_click_config_provider(s_main_window, click_config_provider);
	
	const int delay_ms = 1750;
	s_animation_completed_timer = app_timer_register(delay_ms, finish_setup_callback, NULL);
	
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