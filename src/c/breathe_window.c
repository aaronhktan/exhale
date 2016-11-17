#include <pebble.h>
#include "breathe_window.h"
#include "src/c/data.h"
#include "src/c/graphics.h"
#include "src/c/settings.h"
#include "localize.h"

static Window *s_main_window;
static Layer *s_circle_layer, *s_inside_text_layer, *s_upper_text_layer, *s_lower_text_layer;
static AppTimer *s_animation_completed_timer, *s_main_animation_ended_timer, *animationTimer[69], *s_show_relax_text_timer, *s_show_inhale_timer, *s_hide_exhale_timer, *s_show_exhale_timer, *s_hide_lower_text_layer, *s_click_provider_timer;
static GRect bounds;
static uint8_t s_radius_final, s_radius = 0;
static int s_min_to_breathe = 1, s_min_breathed_today = 0, s_times_played = 0;
static bool s_animation_completed = false, s_animating = false;
static GPoint s_center;
static char s_min_to_breathe_text[3] = "1", s_instruct_text[9], s_min_text[5], s_min_today[25], s_greet_text[27], *s_start_time, *s_end_time;
static time_t t;

// ******************************************************************************************* Layer Update Procedures
// Updates circle
static void canvas_update_proc(Layer *s_drawing_layer, GContext *ctx) {
	graphics_draw_main_circle(ctx, settings_get_circleColor(), s_center, s_radius);
}

// Updates text inside circle, side semicircle, and triangles
static void inside_text_layer_update_proc(Layer *s_inside_text_layer, GContext *ctx) {
	graphics_draw_inner_text(ctx, bounds, s_min_to_breathe, settings_get_textColor(), s_min_to_breathe_text, s_instruct_text, s_min_text);
}

// Draws text at top of screen
static void upper_text_layer_update_proc(Layer *s_inside_text_layer, GContext *ctx) {
	graphics_draw_upper_text(ctx, bounds, s_animating, settings_get_heartRateEnabled(), settings_get_textColor(), s_greet_text);
}

// Draws text at bottom of screen
static void lower_text_layer_update_proc(Layer *s_inside_text_layer, GContext *ctx) {
	graphics_draw_lower_text(ctx, bounds, s_animating, settings_get_textColor(), s_min_today);
}

// ******************************************************************************************* Animation Stuff
// Create generic animation
static void create_animation(int duration, int delay, AnimationCurve animationcurve, AnimationImplementation *implementation) {
	// Creates animation instance
  Animation *anim = animation_create();
  animation_set_duration(anim, duration);
  animation_set_delay(anim, delay);
  animation_set_curve(anim, animationcurve);
  animation_set_implementation(anim, implementation);
	// Schedules animation to start
  animation_schedule(anim);
}

// Fires at the end of app start
static void finish_setup_callback(void *context) {
	// Sets string depending on watch language
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		snprintf(s_instruct_text, sizeof(s_instruct_text), "%s", "RESPIRER");
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		snprintf(s_instruct_text, sizeof(s_instruct_text), "%s", "RESPIRAR");
	} else {
		snprintf(s_instruct_text, sizeof(s_instruct_text), "%s", "BREATHE");
	}
	
	// Animation is completed
	s_animation_completed = true;
	
	// Draw triangles
	graphics_create_triangles_gpath();
	
	// Creates more layers to hold text
	s_inside_text_layer = layer_create(bounds);
	layer_set_update_proc(s_inside_text_layer, inside_text_layer_update_proc);
	
	s_upper_text_layer = layer_create(bounds);
	layer_set_update_proc(s_upper_text_layer, upper_text_layer_update_proc);
	
	s_lower_text_layer = layer_create(bounds);
	layer_set_update_proc(s_lower_text_layer, lower_text_layer_update_proc);
	
	// Adds layers to the window
	layer_add_child(window_get_root_layer(s_main_window), s_inside_text_layer);
	layer_add_child(window_get_root_layer(s_main_window), s_upper_text_layer);
	layer_add_child(window_get_root_layer(s_main_window), s_lower_text_layer);
	layer_mark_dirty(s_inside_text_layer);
	layer_mark_dirty(s_upper_text_layer);
	layer_mark_dirty(s_lower_text_layer);
}

// ******************************************************************************************* Animation Sequence Creator
// Animation while breathing
static void radius_contract_update(Animation *anim, AnimationProgress dist_normalized) {
	s_radius = s_radius_final - dist_normalized * s_radius_final / ANIMATION_NORMALIZED_MAX;
	layer_mark_dirty(s_circle_layer);
}

// Animation while breathing
static void radius_expand_update(Animation *anim, AnimationProgress dist_normalized) {
	s_radius = dist_normalized * s_radius_final / ANIMATION_NORMALIZED_MAX;
	layer_mark_dirty(s_circle_layer);
}

// Start up animation
static void circle_animation_setup() {
	// Set the update method for animation to radius_update
	static AnimationImplementation s_circle_impl = {
		.update = radius_expand_update
	};
	// Starts animation: duration 1500, delay 250, curve, implementation
	create_animation(1500, 250, AnimationCurveEaseInOut, &s_circle_impl);
}

// First in animation after pressing select button
static void main_animation_start() {
	static AnimationImplementation s_main_animation_start = {
		.update = radius_contract_update
	};
	// Starts animation: duration 2000, delay 0, curve, implementation
	create_animation(2000, 0, AnimationCurveEaseInOut, &s_main_animation_start);
}

// Hides lower text
static void hide_lower_text_callback() {
	layer_set_hidden(s_lower_text_layer, true);
}

// Last out animation
static void main_animation_end() {
	static AnimationImplementation s_main_animation_end = {
		.update = radius_expand_update
	};
	
	create_animation(2000, 2000, AnimationCurveEaseInOut, &s_main_animation_end);
	
	// Vibration to signal that the thing is ended
	vibes_double_pulse();
	
	// Localized strings
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		snprintf(s_min_today, sizeof(s_min_today), "Bien fait.");
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		snprintf(s_min_today, sizeof(s_min_today), "Bien hecho.");
	} else {
		snprintf(s_min_today, sizeof(s_min_today), "Well done.");
	}
	
	// Show the "Well done text" and then hides it after 2 seconds
	layer_set_hidden(s_lower_text_layer, false);
	s_hide_lower_text_layer = app_timer_register(2000, hide_lower_text_callback, NULL);
}

// Sets up and schedules circle contract and expand
static void main_animation() {
	// Hides text layers
	layer_set_hidden(s_upper_text_layer, true);
	layer_set_hidden(s_lower_text_layer, true);
	
	// Circle expands for 3 seconds and delays for 1 second
	Animation *circle_expand = animation_create();
  animation_set_duration(circle_expand, 3000);
  animation_set_curve(circle_expand, AnimationCurveEaseInOut);
	animation_set_delay(circle_expand, 1000);
	static AnimationImplementation s_expand_impl = {
		.update = radius_expand_update
	};
  animation_set_implementation(circle_expand, &s_expand_impl);
	
	// Circle contracts for 3 seconds and delays for 1 second
	Animation *circle_contract = animation_create();
	animation_set_duration(circle_contract, 3000);
	animation_set_delay(circle_contract, 1000);
	animation_set_curve(circle_contract, AnimationCurveEaseInOut);
	static AnimationImplementation s_contract_impl = {
		.update = radius_contract_update
	};
  animation_set_implementation(circle_contract, &s_contract_impl);
	
	// Plays expand and contract as a sequence (delay for 1 second, expand for 3, delay for 1, contract for 3)
	Animation *circle_animation_sequence = animation_sequence_create(circle_expand, circle_contract, NULL);
	animation_schedule(circle_animation_sequence);
	s_times_played++; // Used to keep track to see how many should be played to fill time
	
	if (settings_get_vibrationEnabled()) {
		// Vibrations! (play for 0, rest for 1500, play for 25, rest for 25, etc.)
		static const uint32_t const segments[] = {0, 1500, 25, 25, 25, 25, 25, 25, 25, 25, 25, 50, 25, 75, 25, 125, 25, 125, 25, 125, 25, 125, 25, 200, 25, 325, 25, 550, 25, 4000};
		VibePattern vibes = {
			.durations = segments,
			.num_segments = ARRAY_LENGTH(segments),
		};
		vibes_enqueue_custom_pattern(vibes);
	}
}

// Schedules next animation if the number of times played is less than 7 times the number of minutes (seven breaths per minute)
static void main_animation_callback () {
	if (s_times_played < 7 * s_min_to_breathe) {
		animationTimer[s_times_played] = app_timer_register(8000, main_animation_callback, NULL);
		main_animation();
	}
}

// Shows instructions to inhale
static void first_breath_in_callback(void *context) {
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		snprintf(s_greet_text, sizeof(s_greet_text), "INHALEZ...");
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		snprintf(s_greet_text, sizeof(s_greet_text), "AHORA INHALA...");
	} else {
		snprintf(s_greet_text, sizeof(s_greet_text), "NOW INHALE...");
	}
	layer_set_hidden(s_upper_text_layer, false);
}

// Shows instructions to exhale; first hides the top text and then shows the bottom text
static void first_breath_out_callback(void *context) {
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		snprintf(s_min_today, sizeof(s_min_today), "...ET EXHALEZ.");
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		snprintf(s_min_today, sizeof(s_min_today), "...Y EXHALA.");
	} else {
		snprintf(s_min_today, sizeof(s_min_today), "AND EXHALE.");
	}
	layer_set_hidden(s_upper_text_layer, true);
	layer_set_hidden(s_lower_text_layer, false);
}

// Hides bottom text
static void first_breath_out_hide_callback(void *context) {
	layer_set_hidden(s_lower_text_layer, true);
}

// Start animation show text
static void animation_start_callback(void *context) {
	// Sets strings as English, change if watch is set to another language
	char *strings[9] = {"TAKE A MOMENT;", "BE STILL;", "CLEAR YOUR MIND;", "EMPTY YOUR THOUGHTS;", "BE CALM;", "THINK NOTHING;", "RELAX;", "CHILL FOR A SEC;", "SPACE OUT;"};
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		char *french_strings[9] = {"PRENEZ UN MOMENT;", "NE BOUGEZ PAS;", "VIDEZ VOTRE ESPRIT;", "NE PENSEZ À RIEN;", "SOYEZ CALME;", "CONCENTREZ;", "RELAXEZ;", "NE VOUS INQUIETEZ PAS;", "DONNEZ-VOUS DE L'ESPACE;"};
		for (int i = 0; i <= 8; i++) {
			strings[i] = french_strings[i];
		}
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		char *spanish_strings[9] = {"TOMA UN DESCANSO;", "NO TE MUEVAS;", "ACLARA TU MENTE;", "NO PIENSA A NADA;", "SÉ CALMO;", "CONCÉNTRATE;", "RELÁJATE;", "NO TE PREOCUPES;", "TOMA UN RATO;"};
		for (int i = 0; i <= 8; i++) {
			strings[i] = spanish_strings[i];
		}
	}
	
	// Generates a random number between 0 and 8 and sets string to that index of the array of strings
	int random_number = rand() % 9;
	APP_LOG(APP_LOG_LEVEL_DEBUG, "The random_number is %d", random_number);
	APP_LOG(APP_LOG_LEVEL_DEBUG, "The string is %s", &*strings[random_number]);
	snprintf(s_greet_text, sizeof(s_greet_text), "%s", &*strings[random_number]);
	
	// Same thing as above but for bottom text
	char* bottom_text[4] = {"BREATHE.", "EXHALE.", "CONCENTRATE.", "FOCUS."};
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		char* french_bottom_text[4] = {"RESPIREZ.", "EXHALEZ.", "RESPIREZ.", "EXHALEZ."};
		for (int i = 0; i <= 3; i++) {
			bottom_text[i] = french_bottom_text[i];
		}
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		char* spanish_bottom_text[4] = {"RESPIRA.", "EXHALA.", "RESPIRA.", "EXHALA."};
		for (int i = 0; i <= 8; i++) {
			bottom_text[i] = spanish_bottom_text[i];
		}
	}
	
	int random_number_2 = rand() % 4;
	APP_LOG(APP_LOG_LEVEL_DEBUG, "The random_number #2 is %d", random_number_2);
	APP_LOG(APP_LOG_LEVEL_DEBUG, "The string is %s", &*bottom_text[random_number_2]);
	snprintf(s_min_today, sizeof(s_min_today), "%s", &*bottom_text[random_number_2]);
	
	// Shows the text at top and bottom
	layer_set_hidden(s_upper_text_layer, false);
	layer_set_hidden(s_lower_text_layer, false);
}

// End animation show text
static void animation_end_callback(void *context) {
	s_animation_completed = true;
	s_animating = false;
	
	// Add number of minutes breathed and persist
	s_min_breathed_today += s_min_to_breathe;
	data_write_breathe_persist_data(s_min_breathed_today);
	data_write_date_persist_data();
	
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		snprintf(s_greet_text, sizeof(s_greet_text), "%s", "ALLO!");
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		snprintf(s_greet_text, sizeof(s_greet_text), "%s", "¡HOLA!");
	} else {
		snprintf(s_greet_text, sizeof(s_greet_text), "%s", "HELLO.");
	}
	
	// If the user breathes during passage from one day to another (i.e. 12AM) then set number of breaths to 0
	s_end_time = data_get_date_today();
	APP_LOG(APP_LOG_LEVEL_DEBUG, "The date started is %s", s_start_time);
	APP_LOG(APP_LOG_LEVEL_DEBUG, "The date started is %s", s_end_time);
	if (strcmp(s_start_time, s_end_time) == 0) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "The date started and ended are the same");
		if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			snprintf(s_min_today, sizeof(s_min_today), "AUJ: %d MIN", s_min_breathed_today);
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			snprintf(s_min_today, sizeof(s_min_today), "HOY: %d MIN", s_min_breathed_today);
		} else {
			snprintf(s_min_today, sizeof(s_min_today), "TODAY: %d MIN", s_min_breathed_today);
		}
	} else {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "The date started and ended are not the same");
		if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			snprintf(s_min_today, sizeof(s_min_today), "AUJ: 0 MIN");
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			snprintf(s_min_today, sizeof(s_min_today), "HOY: 0 MIN");
		} else {
			snprintf(s_min_today, sizeof(s_min_today), "TODAY: 0 MIN");
		}
	}
	
	// Sets different number of digits for one digit or two digits
	if (s_min_to_breathe == 10) {
			snprintf(s_min_to_breathe_text, 3, "%dd", s_min_to_breathe);
		} else {
			snprintf(s_min_to_breathe_text, 2, "%d", s_min_to_breathe);
		}
	
	// Shows all the layers because breathing is done
	layer_set_hidden(s_inside_text_layer, false);
	layer_set_hidden(s_upper_text_layer, false);
	layer_set_hidden(s_lower_text_layer, false);
}

// Changes text in circle if singular or plural
static void set_min_text(int minutes, void *string) {
	if (minutes == 1) {
			snprintf(string, 5, "%s", "MIN");
		} else {
			snprintf(string, 5, "%s", "MINS");
		}
}

// ******************************************************************************************* Click Handlers
static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
	// Increments number, displays number.
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
	// Decrements number, displays number.
	if ((s_animation_completed) && (s_min_to_breathe > 1)) {
		s_min_to_breathe -= 1;
		set_min_text(s_min_to_breathe, s_min_text);
		snprintf(s_min_to_breathe_text, 2, "%d", s_min_to_breathe);
		layer_mark_dirty(s_inside_text_layer);
	}
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
	// Only does things if things are not animating
	if (!s_animating) {
		s_animating = true;
		s_times_played = 0;
		s_animation_completed = false;
		
		// Hides all text layers
		layer_set_hidden(s_inside_text_layer, true);
		layer_set_hidden(s_upper_text_layer, true);
		layer_set_hidden(s_lower_text_layer, true);
		
		// Starts the first circle contraction
		main_animation_start();
	
		// Shows encouragement text at 2.1 seconds in
		s_show_relax_text_timer = app_timer_register(2100, animation_start_callback, NULL);
		// Shows the instruction to inhale at 7.1 seconds in
		s_show_inhale_timer = app_timer_register(7100, first_breath_in_callback, NULL);
		/* Shows the instruction to exhale at 10.6 seconds in (i.e. 3.5 seconds later, which is 2.5 duration + 1 delay)
		Also hides the first instruction */
		s_show_exhale_timer = app_timer_register(10600, first_breath_out_callback, NULL);
		// Hides the instruction to exhale
		s_hide_exhale_timer = app_timer_register(14100, first_breath_out_hide_callback, NULL);
		
		// First animationTimer, which will schedule the next time the circle expands or contracts
		animationTimer[0] = app_timer_register(6000, main_animation_callback, NULL); 
	
		// Schedules the last out animation (circle expand) after min * duration of 7 breaths + duration of first circle contraction
		s_main_animation_ended_timer = app_timer_register(s_min_to_breathe * 56000 + 7000, main_animation_end, NULL);
		// Schedules the "hide everything and revert to main menu" callback
		int s_animation_completed_delay = s_min_to_breathe * 56000 + 11000;
		s_animation_completed_timer = app_timer_register(s_animation_completed_delay, animation_end_callback, NULL);
		
		// Gets today's date to compare with the end date after breathing is finished
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
static void inbox_received_handler(DictionaryIterator *iter, void *context) {
	// When settings happen, save settings, and refresh screen
	settings_handle_settings();
	settings_save_settings();
	window_set_background_color(s_main_window, settings_get_backgroundColor());
	layer_mark_dirty(s_circle_layer);
	layer_mark_dirty(s_inside_text_layer);
	layer_mark_dirty(s_upper_text_layer);
}


static void main_window_load(Window *window){
	// Sets bounds of drawing layer
	bounds = layer_get_bounds(window_get_root_layer(s_main_window));
	
	// Creates drawing layer and sets update process
	s_circle_layer = layer_create(bounds);
	layer_set_update_proc(s_circle_layer, canvas_update_proc);
	
	// Adds drawing layer to the window
	layer_add_child(window_get_root_layer(s_main_window), s_circle_layer);
	layer_mark_dirty(s_circle_layer);
	
	s_center = grect_center_point(&bounds); // Sets the center of the circle to be the center of the screen
	s_radius_final = bounds.size.w / PBL_IF_RECT_ELSE(2.6, 3.3); // Sets the final radius of the circle
	circle_animation_setup(); // Starts the circle animation
}

// Function for when window is destroyed
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

// Sets click_providers, see click providers section
static void set_click_config_providers() {
	window_set_click_config_provider(s_main_window, click_config_provider);
}

void breathe_window_push(int min) {
	// Starts random number generator
	srand((unsigned) time(&t));
	
	// Open AppMessage connection
	app_message_register_inbox_received(inbox_received_handler);
	app_message_open(128, 128);
	
	// Load settings
	s_min_breathed_today = data_read_breathe_persist_data();
	
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			snprintf(s_min_today, sizeof(s_min_today), "AUJ: %d MIN", s_min_breathed_today);
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			snprintf(s_min_today, sizeof(s_min_today), "HOY: %d MIN", s_min_breathed_today);
		} else {
			snprintf(s_min_today, sizeof(s_min_today), "TODAY: %d MIN", s_min_breathed_today);
	}
	
	// Create main window and assign to pointer
	s_main_window = window_create();
	window_set_background_color(s_main_window, settings_get_backgroundColor());
	
	// Set funcions to handle window
	window_set_window_handlers(s_main_window, (WindowHandlers) {
		.load = main_window_load,
		.unload = main_window_unload
	});
	
	s_click_provider_timer = app_timer_register(2000, set_click_config_providers, NULL);
	
	// Starts timer to show text at end of start animation
	const int delay_ms = 1800;
	s_animation_completed_timer = app_timer_register(delay_ms, finish_setup_callback, NULL);
	
	s_min_to_breathe = min;
	snprintf(s_min_to_breathe_text, sizeof(s_min_to_breathe_text), "%d", s_min_to_breathe);
	set_min_text(s_min_to_breathe, s_min_text);
	
	// Show window on the watch, with animated = true
	window_stack_push(s_main_window, true);
}