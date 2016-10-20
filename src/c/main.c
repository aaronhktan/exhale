#include <pebble.h>
//creates a pointer for main window
static Window *s_main_window;
static Layer *s_circle_layer, *s_inside_text_layer, *s_upper_text_layer, *s_lower_text_layer;
static AppTimer *s_animation_completed_timer, *s_main_animation_ended_timer, *animationTimer[69], *s_show_relax_text_timer, *s_show_inhale_timer, *s_hide_exhale_timer, *s_show_exhale_timer;
static GPath *s_up_triangle, *s_down_triangle;
static GRect bounds;
static uint8_t s_radius_final, s_radius = 0;
static int s_min_to_breathe = 1, s_times_clicked_select = 0, s_times_played = 0;
static bool s_animation_completed = false;
static GPoint s_center;
static char s_min_to_breathe_text[3] = "1", s_instruct_text[8], s_min_text[5], s_min_today[19] = "BREATHED TODAY: 0", s_greet_text[19] = "HELLO, AARON";
static time_t t;

#if PBL_DISPLAY_HEIGHT == 168
	static const 	GPathInfo UP_PATH_INFO = {
			.num_points = 3,
			.points = (GPoint []) {{72, 39}, {77, 44}, {67, 44}}
		};
	static const GPathInfo DOWN_PATH_INFO = {
			.num_points = 3,
		.points = (GPoint []) {{72, 129}, {67, 124}, {77, 124}}
		};
#elif PBL_DISPLAY_HEIGHT == 180
	static const 	GPathInfo UP_PATH_INFO = {
			.num_points = 3,
			.points = (GPoint []) {{90, 45}, {85, 50}, {95, 50}}
		};
	static const GPathInfo DOWN_PATH_INFO = {
			.num_points = 3,
		.points = (GPoint []) {{90, 135}, {85, 130}, {95, 130}}
		};
#else
	static const GPathInfo UP_PATH_INFO = {
			.num_points = 3,
			.points = (GPoint []) {{100, 59}, {100, 55}, {100, 45}}
		};
	static const GPathInfo DOWN_PATH_INFO = {
			.num_points = 3,
			.points = (GPoint []) {{105, 164}, {95, 164}, {100, 169}}
		};
#endif

// ******************************************************************************************* Layer Update Processes
static void canvas_update_proc(Layer *s_drawing_layer, GContext *ctx) {
	graphics_context_set_stroke_color(ctx, PBL_IF_COLOR_ELSE(GColorVividCerulean, GColorBlack));
	graphics_context_set_stroke_width(ctx, 5);
	graphics_draw_circle(ctx, s_center, s_radius);
}

static void inside_text_layer_update_proc(Layer *s_inside_text_layer, GContext *ctx) {
		
		//draw side circle
		graphics_context_set_fill_color(ctx, GColorBlack);
		graphics_fill_circle(ctx, GPoint(bounds.size.w + 5, bounds.size.h / 2), 10);
		
		// draw triangles
		switch(s_min_to_breathe) {
			case 1 :
				gpath_draw_filled(ctx, s_up_triangle);
				break;
			case 10 :
				gpath_draw_filled(ctx, s_down_triangle);
				break;
			default:
				gpath_draw_filled(ctx, s_up_triangle);
				gpath_draw_filled(ctx, s_down_triangle);
		}
		
		// draw text in circle
		graphics_context_set_text_color(ctx, GColorBlack);
		GSize min_to_breathe_bounds = graphics_text_layout_get_content_size("10", fonts_get_system_font(FONT_KEY_LECO_42_NUMBERS), 
																																		GRect(0, 0, bounds.size.w, bounds.size.h), 
																								GTextOverflowModeWordWrap, GTextAlignmentCenter);
		graphics_draw_text(ctx, s_min_to_breathe_text, fonts_get_system_font(FONT_KEY_LECO_42_NUMBERS), 
											 GRect((bounds.size.w - min_to_breathe_bounds.w) / 2, (bounds.size.h - min_to_breathe_bounds.h) / 2 - 6, min_to_breathe_bounds.w, min_to_breathe_bounds.h), 
											 GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);

		GSize instruct_text_bounds = graphics_text_layout_get_content_size("BREATHE", fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD), 
																																		GRect(0, 0, bounds.size.w, bounds.size.h), 
																								GTextOverflowModeWordWrap, GTextAlignmentCenter);
		graphics_draw_text(ctx, s_instruct_text, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD), 
											 GRect((bounds.size.w - instruct_text_bounds.w) / 2, (bounds.size.h - instruct_text_bounds.h) / 2 - 29, instruct_text_bounds.w, instruct_text_bounds.h), 
											 GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);

		GSize min_text_bounds = graphics_text_layout_get_content_size("MINS", fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD), 
																																		GRect(0, 0, bounds.size.w, bounds.size.h), 
																								GTextOverflowModeWordWrap, GTextAlignmentCenter);
		graphics_draw_text(ctx, s_min_text, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD), 
											 GRect((bounds.size.w - min_text_bounds.w) / 2, (bounds.size.h - min_text_bounds.h) / 2 + 25, min_text_bounds.w, min_text_bounds.h), 
											 GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
}


static void upper_text_layer_update_proc(Layer *s_inside_text_layer, GContext *ctx) {
		// draw text at top of screen
		graphics_context_set_text_color(ctx, GColorBlack);
		GSize greet_text_bounds = graphics_text_layout_get_content_size("HELLO, SAMANTHA", fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD),
																																	 GRect(0, 0, bounds.size.w, bounds.size.h),
																																	 GTextOverflowModeWordWrap, GTextAlignmentCenter);
		
		graphics_draw_text(ctx, s_greet_text, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD), 
											 GRect((bounds.size.w - greet_text_bounds.w) / 2, 5, greet_text_bounds.w, greet_text_bounds.h),
											GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
}

static void lower_text_layer_update_proc(Layer *s_inside_text_layer, GContext *ctx) {
		// draw text at bottom of screen
		graphics_context_set_text_color(ctx, GColorBlack);
		GSize today_text_bounds = graphics_text_layout_get_content_size("BREATHED TODAY: 10", fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD),
																																	 GRect(0, 0, bounds.size.w, bounds.size.h),
																																	 GTextOverflowModeWordWrap, GTextAlignmentCenter);
		
		graphics_draw_text(ctx, s_min_today, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD), 
											 GRect((bounds.size.w - today_text_bounds.w) / 2, bounds.size.h - today_text_bounds.h - 8, today_text_bounds.w, today_text_bounds.h),
											GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
}

// ******************************************************************************************* Animation Stuff
// create generic animation
static void create_animation(int duration, int delay, AnimationCurve animationcurve, AnimationImplementation *implementation) {
	// creates animation instance
  Animation *anim = animation_create();
  animation_set_duration(anim, duration);
  animation_set_delay(anim, delay);
  animation_set_curve(anim, animationcurve);
	// sets what the instance does with implementation passed in to argument
  animation_set_implementation(anim, implementation);
	// schedules animation to start
  animation_schedule(anim);
}

// fires at the end of app start
static void finish_setup_callback(void *context) {
	snprintf(s_instruct_text, 12, "%s", "BREATHE");
	snprintf(s_min_text, 5, "%s", "MIN");
	s_animation_completed = true;
	
	s_up_triangle = gpath_create(&UP_PATH_INFO);
	s_down_triangle = gpath_create(&DOWN_PATH_INFO);
	
	// creates more layers to hold text
	s_inside_text_layer = layer_create(bounds);
	layer_set_update_proc(s_inside_text_layer, inside_text_layer_update_proc);
	
	s_upper_text_layer = layer_create(bounds);
	layer_set_update_proc(s_upper_text_layer, upper_text_layer_update_proc);
	
	s_lower_text_layer = layer_create(bounds);
	layer_set_update_proc(s_lower_text_layer, lower_text_layer_update_proc);
	
	//adds layers to the window
	layer_add_child(window_get_root_layer(s_main_window), s_inside_text_layer);
	layer_add_child(window_get_root_layer(s_main_window), s_upper_text_layer);
	layer_add_child(window_get_root_layer(s_main_window), s_lower_text_layer);
	layer_mark_dirty(s_inside_text_layer);
	layer_mark_dirty(s_upper_text_layer);
	layer_mark_dirty(s_lower_text_layer);
}

// ******************************************************************************************* Animation Sequence Creator
// animation while breathing
static void radius_contract_update(Animation *anim, AnimationProgress dist_normalized) {
	s_radius = s_radius_final - dist_normalized * s_radius_final / ANIMATION_NORMALIZED_MAX;
	layer_mark_dirty(s_circle_layer);
}

// animation while breathing
static void radius_expand_update(Animation *anim, AnimationProgress dist_normalized) {
	s_radius = dist_normalized * s_radius_final / ANIMATION_NORMALIZED_MAX;
	layer_mark_dirty(s_circle_layer);
}

// start up animation
static void circle_animation_setup() {
	// set the update method for animation to radius_update
	static AnimationImplementation s_circle_impl = {
		.update = radius_expand_update
	};
	// starts animation
	create_animation(1500, 250, AnimationCurveEaseInOut, &s_circle_impl);
}

// first in animation
static void main_animation_start() {
	static AnimationImplementation s_main_animation_start = {
		.update = radius_contract_update
	};
	
	create_animation(2000, 0, AnimationCurveEaseInOut, &s_main_animation_start);
}


// last out animation
static void main_animation_end() {
	static AnimationImplementation s_main_animation_end = {
		.update = radius_expand_update
	};
	
	create_animation(2000, 0, AnimationCurveEaseInOut, &s_main_animation_end);
}

// sets up and schedules circle contract and expand
static void main_animation() {
	layer_set_hidden(s_upper_text_layer, true);
	layer_set_hidden(s_lower_text_layer, true);
	static AnimationImplementation s_contract_impl = {
		.update = radius_contract_update
	};
	static AnimationImplementation s_expand_impl = {
		.update = radius_expand_update
	};
	
	Animation *circle_expand = animation_create();
  animation_set_duration(circle_expand, 3000);
  animation_set_curve(circle_expand, AnimationCurveEaseInOut);
	animation_set_delay(circle_expand, 1000);
	// sets what the instance does with implementation passed in to argument
  animation_set_implementation(circle_expand, &s_expand_impl);
	
	Animation *circle_contract = animation_create();
	animation_set_duration(circle_contract, 3000);
	animation_set_delay(circle_contract, 1000);
	animation_set_curve(circle_contract, AnimationCurveEaseInOut);
	// sets what the instance does with implementation passed in to argument
  animation_set_implementation(circle_contract, &s_contract_impl);
	
	Animation *circle_animation_sequence = animation_sequence_create(circle_expand, circle_contract, NULL);
	animation_schedule(circle_animation_sequence);
	s_times_played++;
}

// schedules next animation
static void main_animation_callback () {
	if (s_times_played < 7 * s_min_to_breathe) {
		animationTimer[s_times_played] = app_timer_register(8000, main_animation_callback, NULL);
		main_animation();
	}
}

// shows instructions to inhale
static void first_breath_in_callback(void *context) {
	snprintf(s_greet_text, 18, "%s", "NOW INHALE...");
	layer_set_hidden(s_upper_text_layer, false);
}

// shows instructions to exhale
static void first_breath_out_callback(void *context) {
	snprintf(s_min_today, 12, "%s", "AND EXHALE.");
	layer_set_hidden(s_lower_text_layer, false);
}

static void first_breath_out_hide_callback(void *context) {
	layer_set_hidden(s_lower_text_layer, true);
}

// start animation show text
static void animation_start_callback(void *context) {
	const char* strings[10] = {"TAKE A MOMENT;", "BE STILL;", "CLEAR YOUR MIND;", "DON'T TALK;", "EMPTY YOUR THOUGHTS;", "BE CALM;", "THINK NOTHING;", "RELAX;", "CHILL FOR A SEC;", "SPACE OUT;",};
	APP_LOG(APP_LOG_LEVEL_DEBUG, "The string is %s", &*strings[rand() % 10]);
	snprintf(s_greet_text, 20, "%s", &*strings[rand() % 10]);
	const char* bottom_text[4] = {"BREATHE.", "EXHALE.", "CONCENTRATE.", "FOCUS."};
	snprintf(s_min_today, 19, "%s", &*bottom_text[rand() % 4]);
	layer_set_hidden(s_upper_text_layer, false);
	layer_set_hidden(s_lower_text_layer, false);
}

// end animation show text
static void animation_end_callback(void *context) {
	s_animation_completed = true;
	snprintf(s_greet_text, 19, "%s", "HELLO, AARON");
	snprintf(s_min_today, 19, "BREATHED TODAY: %d", s_times_clicked_select);
	layer_set_hidden(s_inside_text_layer, false);
	layer_set_hidden(s_upper_text_layer, false);
	layer_set_hidden(s_lower_text_layer, false);
}


// ******************************************************************************************* Click Handlers
static void set_min_text(int minutes, void *string) {
	if (minutes == 1) {
			snprintf(string, 5, "%s", "MIN");
		} else {
			snprintf(string, 5, "%s", "MINS");
		}
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
	if ((s_animation_completed) && (s_min_to_breathe < 10)) {
		s_min_to_breathe += 1;
		set_min_text(s_min_to_breathe, s_min_text);
		if (s_min_to_breathe == 10) {
			snprintf(s_min_to_breathe_text, 3, "%dd", s_min_to_breathe);
		} else {
			snprintf(s_min_to_breathe_text, 2, "%d", s_min_to_breathe);
		}
		layer_mark_dirty(s_inside_text_layer);
	}
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
	if ((s_animation_completed) && (s_min_to_breathe > 1)) {
		s_min_to_breathe -= 1;
		set_min_text(s_min_to_breathe, s_min_text);
		snprintf(s_min_to_breathe_text, 2, "%d", s_min_to_breathe);
		layer_mark_dirty(s_inside_text_layer);
	}
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
	s_times_played = 0;
	int s_animation_completed_delay = s_min_to_breathe * 56000 + 10000;
	s_animation_completed_timer = app_timer_register(s_animation_completed_delay, animation_end_callback, NULL);
	s_times_clicked_select += 1;
	s_animation_completed = false;
	layer_set_hidden(s_inside_text_layer, true);
	layer_set_hidden(s_upper_text_layer, true);
	layer_set_hidden(s_lower_text_layer, true);
	if (s_times_clicked_select > 9) {
		snprintf(s_min_today, 19, "BREATHED TODAY: %dd", s_times_clicked_select);
	} else {
		snprintf(s_min_today, 19, "BREATHED TODAY: %d", s_times_clicked_select);
	}
	
	main_animation_start();
	
	s_show_relax_text_timer = app_timer_register(2100, animation_start_callback, NULL);
	s_show_inhale_timer = app_timer_register(10600, first_breath_in_callback, NULL);
	s_show_exhale_timer = app_timer_register(14100, first_breath_out_callback, NULL);
	s_hide_exhale_timer = app_timer_register(18000, first_breath_out_hide_callback, NULL);
	
	animationTimer[0] = app_timer_register(6000, main_animation_callback, NULL); 
	
	s_main_animation_ended_timer = app_timer_register(s_min_to_breathe * 56000 + 8000, main_animation_end, NULL);
}

static void click_config_provider(void *context) {
  ButtonId id_up = BUTTON_ID_UP;  // The Up button
	ButtonId id_down = BUTTON_ID_DOWN; // The Down button
	ButtonId id_select = BUTTON_ID_SELECT; // The Select button
  window_single_click_subscribe(id_up, up_click_handler);
	window_single_click_subscribe(id_down, down_click_handler);
	window_single_click_subscribe(id_select, select_click_handler);
}

// ******************************************************************************************* Main App Functions
static void main_window_load(Window *window){
	//sets bounds of drawing layer
	bounds = layer_get_bounds(window_get_root_layer(s_main_window));
	
	//creates drawing layer and sets update process
	s_circle_layer = layer_create(bounds);
	layer_set_update_proc(s_circle_layer, canvas_update_proc);
	
	//adds drawing layer to the window
	layer_add_child(window_get_root_layer(s_main_window), s_circle_layer);
	layer_mark_dirty(s_circle_layer);
	
	s_center = grect_center_point(&bounds); // sets the center of the circle to be the center of the screen
	s_radius_final = bounds.size.w / PBL_IF_RECT_ELSE(2.6, 3.3); // sets the final radius of the circle
	circle_animation_setup(); // starts the circle animation
}

//function for when window is destroyed
static void main_window_unload(Window *window) {
	layer_destroy(s_circle_layer);
	window_destroy(s_main_window);
}

static void init() {
	
	//starts random number generator
	srand((unsigned) time(&t));
	
	//create main window and assign to pointer
	s_main_window = window_create();
	
	//set funcions to handle window
	window_set_window_handlers(s_main_window, (WindowHandlers) {
		.load = main_window_load,
		.unload = main_window_unload
	});
	
	window_set_click_config_provider(s_main_window, click_config_provider);
	
	// starts timer to show text at end of start animation
	const int delay_ms = 1800;
	s_animation_completed_timer = app_timer_register(delay_ms, finish_setup_callback, NULL);
	
	//show window on the watch, with animated = true
	window_stack_push(s_main_window, true);
}

static void deinit() {
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}