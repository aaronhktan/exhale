#include <pebble.h>
#include "breathe_window.h"
#include "src/c/data.h"
#include "src/c/graphics.h"

//creates a pointer for main window
static Window *s_main_window;
static Layer *s_circle_layer, *s_inside_text_layer, *s_upper_text_layer, *s_lower_text_layer;
static AppTimer *s_animation_completed_timer, *s_main_animation_ended_timer, *animationTimer[69], *s_show_relax_text_timer, *s_show_inhale_timer, *s_hide_exhale_timer, *s_show_exhale_timer, *s_hide_lower_text_layer, *s_click_provider_timer;
static GRect bounds;
static uint8_t s_radius_final, s_radius = 0;
static int s_min_to_breathe = 1, s_min_breathed_today = 0, s_times_played = 0;
static bool s_animation_completed = false, s_animating = false;
static GPoint s_center;
static char s_min_to_breathe_text[3] = "1", s_instruct_text[8], s_min_text[5], s_min_today[19] = "TODAY: 0 MIN", s_greet_text[19] = "HELLO.", *s_start_time, *s_end_time;
static time_t t;
ClaySettings settings;

// ******************************************************************************************* Settings Procedures
// Loads default settings; if user has set settings, then other settings are loaded
static void load_settings() {
	settings.backgroundColor = GColorBlack;
	settings.circleColor = PBL_IF_COLOR_ELSE(GColorJaegerGreen, GColorWhite);
	settings.textColor = GColorWhite;
	settings.vibrationEnabled = true;
	#if PBL_API_EXISTS(health_service_peek_current_value)
		settings.heartRateEnabled = true;
	#else
		settings.heartRateEnabled = false;
	#endif
	persist_read_data(SETTINGS_KEY, &settings, sizeof(settings));
}

// ******************************************************************************************* Layer Update Procedures
// updates circle
static void canvas_update_proc(Layer *s_drawing_layer, GContext *ctx) {
	graphics_draw_main_circle(ctx, settings.circleColor, s_center, s_radius);
}

// updates text inside circle, side semicircle, and triangles
static void inside_text_layer_update_proc(Layer *s_inside_text_layer, GContext *ctx) {
	graphics_draw_inner_text(ctx, bounds, s_min_to_breathe, settings.textColor, s_min_to_breathe_text, s_instruct_text, s_min_text);
}

// draws text at top of screen
static void upper_text_layer_update_proc(Layer *s_inside_text_layer, GContext *ctx) {
	graphics_draw_upper_text(ctx, bounds, s_animating, settings.heartRateEnabled, settings.textColor, s_greet_text);
}

// draws text at bottom of screen
static void lower_text_layer_update_proc(Layer *s_inside_text_layer, GContext *ctx) {
	graphics_draw_lower_text(ctx, bounds, s_animating, settings.textColor, s_min_today);
}

// ******************************************************************************************* Animation Stuff
// create generic animation
static void create_animation(int duration, int delay, AnimationCurve animationcurve, AnimationImplementation *implementation) {
	// creates animation instance
  Animation *anim = animation_create();
  animation_set_duration(anim, duration);
  animation_set_delay(anim, delay);
  animation_set_curve(anim, animationcurve);
  animation_set_implementation(anim, implementation);
	// schedules animation to start
  animation_schedule(anim);
}

// fires at the end of app start
static void finish_setup_callback(void *context) {
	snprintf(s_instruct_text, 12, "%s", "BREATHE");
	snprintf(s_min_text, 5, "%s", "MIN");
	s_animation_completed = true;
	
	graphics_create_triangles_gpath();
	
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

static void hide_lower_text_callback() {
	layer_set_hidden(s_lower_text_layer, true);
}

// last out animation
static void main_animation_end() {
	static AnimationImplementation s_main_animation_end = {
		.update = radius_expand_update
	};
	
	create_animation(2000, 2000, AnimationCurveEaseInOut, &s_main_animation_end);
	
	vibes_double_pulse();
	snprintf(s_min_today, 17, "%s", "Well done.");
	layer_set_hidden(s_lower_text_layer, false);
	
	s_hide_lower_text_layer = app_timer_register(2000, hide_lower_text_callback, NULL);
}

// sets up and schedules circle contract and expand
static void main_animation() {
	// hides text layers
	layer_set_hidden(s_upper_text_layer, true);
	layer_set_hidden(s_lower_text_layer, true);
	
	// circle expands
	Animation *circle_expand = animation_create();
  animation_set_duration(circle_expand, 3000);
  animation_set_curve(circle_expand, AnimationCurveEaseInOut);
	animation_set_delay(circle_expand, 1000);
	// sets what the instance does with implementation passed in to argument
	static AnimationImplementation s_expand_impl = {
		.update = radius_expand_update
	};
  animation_set_implementation(circle_expand, &s_expand_impl);
	
	// circle contracts
	Animation *circle_contract = animation_create();
	animation_set_duration(circle_contract, 3000);
	animation_set_delay(circle_contract, 1000);
	animation_set_curve(circle_contract, AnimationCurveEaseInOut);
	// sets what the instance does with implementation passed in to argument
	static AnimationImplementation s_contract_impl = {
		.update = radius_contract_update
	};
  animation_set_implementation(circle_contract, &s_contract_impl);
	
	// plays expand and contract as a sequence
	Animation *circle_animation_sequence = animation_sequence_create(circle_expand, circle_contract, NULL);
	animation_schedule(circle_animation_sequence);
	s_times_played++;
	
	if (settings.vibrationEnabled) {
		// vibrations!
		static const uint32_t const segments[] = {0, 5500, 25, 25, 25, 25, 25, 25, 25, 25, 25, 50, 25, 75, 25, 125, 25, 125, 25, 125, 25, 125, 25, 200, 25, 325, 25, 550, 25};
		VibePattern vibes = {
			.durations = segments,
			.num_segments = ARRAY_LENGTH(segments),
		};
		vibes_enqueue_custom_pattern(vibes);
	}
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
	const char* strings[10] = {"TAKE A MOMENT;", "BE STILL;", "CLEAR YOUR MIND;", "EMPTY YOUR THOUGHTS;", "BE CALM;", "THINK NOTHING;", "RELAX;", "CHILL FOR A SEC;", "SPACE OUT;",};
	APP_LOG(APP_LOG_LEVEL_DEBUG, "The string is %s", &*strings[rand() % 9]);
	snprintf(s_greet_text, 21, "%s", &*strings[rand() % 9]);
	const char* bottom_text[4] = {"BREATHE.", "EXHALE.", "CONCENTRATE.", "FOCUS."};
	snprintf(s_min_today, 19, "%s", &*bottom_text[rand() % 4]);
	layer_set_hidden(s_upper_text_layer, false);
	layer_set_hidden(s_lower_text_layer, false);
}

// end animation show text
static void animation_end_callback(void *context) {
	s_animation_completed = true;
	
	s_min_breathed_today += s_min_to_breathe;
	data_write_breathe_persist_data(s_min_breathed_today);
	data_write_date_persist_data();
	
	snprintf(s_greet_text, 19, "%s", "HELLO.");
	s_end_time = data_get_date_today();
	if (strcmp(s_start_time, s_end_time) == 0) {
		snprintf(s_min_today, 19, "TODAY: %d MIN", s_min_breathed_today);
	} else {
		snprintf(s_min_today, 19, "TODAY: 0 MIN");
	}
	if (s_min_to_breathe == 10) {
			snprintf(s_min_to_breathe_text, 3, "%dd", s_min_to_breathe);
		} else {
			snprintf(s_min_to_breathe_text, 2, "%d", s_min_to_breathe);
		}
	layer_set_hidden(s_inside_text_layer, false);
	layer_set_hidden(s_upper_text_layer, false);
	layer_set_hidden(s_lower_text_layer, false);
	s_animating = false;
}

static void set_min_text(int minutes, void *string) {
	if (minutes == 1) {
			snprintf(string, 5, "%s", "MIN");
		} else {
			snprintf(string, 5, "%s", "MINS");
		}
}

// ******************************************************************************************* Click Handlers
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
	if (!s_animating) {
		s_animating = true;
		s_times_played = 0;
		s_animation_completed = false;
		layer_set_hidden(s_inside_text_layer, true);
		layer_set_hidden(s_upper_text_layer, true);
		layer_set_hidden(s_lower_text_layer, true);

		main_animation_start();

		s_show_relax_text_timer = app_timer_register(2100, animation_start_callback, NULL);
		s_show_inhale_timer = app_timer_register(10600, first_breath_in_callback, NULL);
		s_show_exhale_timer = app_timer_register(14100, first_breath_out_callback, NULL);
		s_hide_exhale_timer = app_timer_register(18000, first_breath_out_hide_callback, NULL);

		animationTimer[0] = app_timer_register(6000, main_animation_callback, NULL); 

		s_main_animation_ended_timer = app_timer_register(s_min_to_breathe * 56000 + 7000, main_animation_end, NULL);
		int s_animation_completed_delay = s_min_to_breathe * 56000 + 11000;
		s_animation_completed_timer = app_timer_register(s_animation_completed_delay, animation_end_callback, NULL);
		
		s_start_time = data_get_date_today();
	}
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

// Save settings and reload visual aspects
static void save_settings() {
	persist_write_data(SETTINGS_KEY, &settings, sizeof(settings));
	window_set_background_color(s_main_window, settings.backgroundColor);
	layer_mark_dirty(s_circle_layer);
	layer_mark_dirty(s_inside_text_layer);
	layer_mark_dirty(s_upper_text_layer);
}

// Receive settings from phone
static void inbox_received_handler(DictionaryIterator *iter, void *context) {
	Tuple *bg_color_t = dict_find(iter, MESSAGE_KEY_backgroundColor);
	if (bg_color_t) {
		settings.backgroundColor = GColorFromHEX(bg_color_t->value->int32);
		if (bg_color_t->value->int32 == 0x000000) {
			APP_LOG(APP_LOG_LEVEL_DEBUG, "The background color is black.");
			settings.textColor = GColorWhite;
		} else {
			APP_LOG(APP_LOG_LEVEL_DEBUG, "The background color is white.");
			settings.textColor = GColorBlack;
		}
		settings.circleColor = settings.textColor;
	}
	
	Tuple *circle_color_t = dict_find(iter, MESSAGE_KEY_circleColor);
	if (circle_color_t) {
		settings.circleColor = GColorFromHEX(circle_color_t->value->int32);
	}
	
	Tuple *vibration_enabled_t = dict_find(iter, MESSAGE_KEY_vibrationEnabled);
	if (vibration_enabled_t) {
		settings.vibrationEnabled = vibration_enabled_t->value->int32 == 1;
	}
	
	Tuple *heartRate_enabled_t = dict_find(iter, MESSAGE_KEY_heartRateEnabled);
	if (heartRate_enabled_t) {
		settings.heartRateEnabled = heartRate_enabled_t->value->int32 == 1;
	}
	
	save_settings();
}


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
	layer_destroy(s_upper_text_layer);
	layer_destroy(s_lower_text_layer);
	layer_destroy(s_inside_text_layer);
	vibes_cancel();
	if (s_times_played != 0) {
		app_timer_cancel(animationTimer[s_times_played]);
	}
	window_destroy(s_main_window);
}

static void set_click_config_providers() {
	window_set_click_config_provider(s_main_window, click_config_provider);
}

void breathe_window_push() {
	//starts random number generator
	srand((unsigned) time(&t));
	
	// Open AppMessage connection
	app_message_register_inbox_received(inbox_received_handler);
	app_message_open(128, 128);
	
	// Load settings
	load_settings();
	s_min_breathed_today = data_read_breathe_persist_data();
	snprintf(s_min_today, 19, "TODAY: %d MIN", s_min_breathed_today);
	
	//create main window and assign to pointer
	s_main_window = window_create();
	window_set_background_color(s_main_window, settings.backgroundColor);
	
	//set funcions to handle window
	window_set_window_handlers(s_main_window, (WindowHandlers) {
		.load = main_window_load,
		.unload = main_window_unload
	});
	
	s_click_provider_timer = app_timer_register(2000, set_click_config_providers, NULL);
	
	// starts timer to show text at end of start animation
	const int delay_ms = 1800;
	s_animation_completed_timer = app_timer_register(delay_ms, finish_setup_callback, NULL);
	
	//show window on the watch, with animated = true
	window_stack_push(s_main_window, true);
}