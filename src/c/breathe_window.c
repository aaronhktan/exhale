#include <pebble.h>
#include "math.h"
#include "breathe_window.h"
#include "src/c/data.h"
#include "src/c/graphics.h"
#include "src/c/settings.h"
#include "src/c/localize.h"
#include "src/c/settings_menu.h"
#if !PBL_PLATFORM_APLITE
	#include "src/c/achievement.h"
	#include "src/c/achievement_window.h"
	#include "src/c/achievement_menu.h"
#endif

#define D_START_DELAY 2100
#define D_START_ANIMATION_TIME 5000
#define D_BREATH_HOLD_TIME 1000
#define D_CIRCLE_ANIMATION_DURATION 1500
#define D_CIRCLE_ANIMATION_DELAY 150
#define D_START_ANIMATION_DURATION 2000
#define D_START_ANIMATION_DELAY 0
#define D_END_ANIMATION_DURATION 2000
#define D_END_ANIMATION_DELAY 2500

static Window *s_main_window;
static Layer *s_circle_layer, *s_inside_text_layer, *s_upper_text_layer, *s_lower_text_layer;
static AppTimer *s_animation_completed_timer, *animationTimer, *s_hide_lower_text_layer, *s_click_provider_timer, *s_interrupt_timer, *s_update_hr_timer, *s_main_timer;
static GRect bounds;
static GPoint s_center;
static uint8_t s_radius_final, s_radius = 0;
static int s_min_to_breathe = 1, s_min_breathed_today = 0, s_times_played = 0, s_breath_duration, s_breaths_per_minute, s_current_radius;
static bool s_animating = false, s_main_done, s_achievement_window_pushed = false;
static char s_min_to_breathe_text[3] = "1", s_instruct_text[27], s_min_text[25], s_min_today[25], s_greet_text[27], s_start_time[11], s_end_time[11];
static time_t t;
static time_t s_start_stamp;

// ******************************************************************************************* Layer Update Procedures
// Updates circle
static void canvas_update_proc(Layer *s_drawing_layer, GContext *ctx) {
	graphics_draw_main_circle(ctx, settings_get_circleColor(), s_center, s_radius);
}

// Updates text inside circle, side semicircle, and triangles
static void inside_text_layer_update_proc(Layer *s_inside_text_layer, GContext *ctx) {
	graphics_draw_inner_text(ctx, bounds, s_animating, s_min_to_breathe, settings_get_textColor(), s_min_to_breathe_text, s_instruct_text, s_min_text);
}

// Draws text at top of screen
static void upper_text_layer_update_proc(Layer *s_inside_text_layer, GContext *ctx) {
	graphics_draw_upper_text(ctx, bounds, s_animating, settings_get_displayText(), settings_get_textColor(), s_greet_text);
}

// Draws text at bottom of screen
static void lower_text_layer_update_proc(Layer *s_inside_text_layer, GContext *ctx) {
	graphics_draw_lower_text(ctx, bounds, s_animating, settings_get_bottomTextType(), settings_get_textColor(), s_min_today);
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

// Creates and adds layers to window at the end of the starting animation
static void finish_setup_callback(void *context) {
	// Sets string depending on watch language
	snprintf(s_instruct_text, sizeof(s_instruct_text), "%s", localize_get_breathe_text());
	
	// Animation is completed
	s_animating = false;
	
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

// ******************************************************************************************* Other stuff
// Changes text in circle if singular or plural
static void set_min_text(int minutes, void *string) {
	if (minutes == 1) {
			snprintf(string, 5, "%s", "MIN");
		} else {
			snprintf(string, 5, "%s", "MINS");
		}
}

// ******************************************************************************************* Animation Sequence Creator
// Animation to contract circle by percentage of animation completed
static void radius_contract_update(Animation *anim, AnimationProgress dist_normalized) {
	s_radius = s_radius_final - dist_normalized * s_radius_final / ANIMATION_NORMALIZED_MAX; // s_radius_final is the radius when the circle is completely expanded
	layer_mark_dirty(s_circle_layer);
}

// Animation to expand the circle by percentage of animation completed
static void radius_expand_update(Animation *anim, AnimationProgress dist_normalized) {
	s_radius = dist_normalized * s_radius_final / ANIMATION_NORMALIZED_MAX;
	layer_mark_dirty(s_circle_layer);
}

// Animation to expand the circle from current size when user interrups breathing session
static void interrupt_expand_update(Animation *anim, AnimationProgress dist_normalized) {
	s_radius = s_current_radius + (s_radius_final - s_current_radius) * dist_normalized / ANIMATION_NORMALIZED_MAX; // Adds to current radius
	layer_mark_dirty(s_circle_layer);
}

// ******************************************************************************************* Animations! (Start to end from bottom to top)
// Hides lower text layer after user is done breathing (i.e. hide the "well done" text)
static void hide_lower_text_callback() {
	layer_set_hidden(s_lower_text_layer, true);
}

// Resets layers once breathing is completely done
static void animation_end_callback(void *data) {
	s_breaths_per_minute = settings_get_breathsPerMinute(); // In case the user changed settings while the they were breathing
	s_breath_duration = settings_get_breathDuration();
	s_animating = false;
	s_main_done = true;
	
	snprintf(s_greet_text, sizeof(s_greet_text), "%s", localize_get_greet_text());
	
	// If the user breathes during passage from one day to another (i.e. 12AM) then set number of breaths to 0
	snprintf(s_end_time, sizeof(s_end_time), "%s", data_get_date_today());
	
	int complete = (int)data; // This tells us whether the user interrupted their session by pressing back

	if (strcmp(s_start_time, s_end_time) == 0 && complete == 0) { // The date is the same and the user did not interrupt their session
		// Add number of minutes breathed
		s_min_breathed_today += s_min_to_breathe;
		
		// Achievements
		#if !PBL_PLATFORM_APLITE
			data_calculate_streak_length();
			// Check whether it's the longest streak and save if it is
			if (data_get_longest_streak() < data_get_streak_length()) {
				data_set_longest_streak(data_get_streak_length());
			}
		
			// Add the minutes breathed to the total number of minutes breathed
			data_set_total_minutes_breathed(data_get_total_minutes_breathed() + s_min_to_breathe);
		
			// String to hold the description
			char * description = "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ!@";
		
			// Show achievements for 5, 8, and 10 minutes breathed in this session if achievements are enabled
			switch (s_min_to_breathe) {
				case 5:
					if (achievement_get_five_minutes_session().complete == 0) {
						achievement_set_five_minutes_session(data_get_date_today(), 1);
						if (settings_get_achievementsEnabled() && !s_achievement_window_pushed) {
							s_achievement_window_pushed = true;
							snprintf(description, 37, localize_get_minutes_session_description(), 5);
							achievement_window_push(localize_get_five_minutes_session_name(), description);
						}
						if (achievement_get_all_complete()) {
							achievement_set_completionist(data_get_date_today(), 1);
						}
					}
					break;
				case 8:
					if (achievement_get_eight_minutes_session().complete == 0) {
						achievement_set_eight_minutes_session(data_get_date_today(), 1);
						if (settings_get_achievementsEnabled() && !s_achievement_window_pushed) {
							s_achievement_window_pushed = true;
							snprintf(description, 37, localize_get_minutes_session_description(), 8);
							achievement_window_push(localize_get_eight_minutes_session_name(), description);
						}
						if (achievement_get_all_complete()) {
							achievement_set_completionist(data_get_date_today(), 1);
						}
					}
					break;
				case 10:
					if (achievement_get_ten_minutes_session().complete == 0) {
						achievement_set_ten_minutes_session(data_get_date_today(), 1);
						if (settings_get_achievementsEnabled() && !s_achievement_window_pushed) {
							s_achievement_window_pushed = true;
							snprintf(description, 38, localize_get_minutes_session_description(), 10);
							achievement_window_push(localize_get_ten_minutes_session_name(), description);
						}
						if (achievement_get_all_complete()) {
							achievement_set_completionist(data_get_date_today(), 1);
						}
					}
					break;
			}
		
			// Show achievements for 5, 10, 30 and 50 minutes breathed in one day if achievements are enabled
			int achievement_case = 0;
			if (s_min_breathed_today >= 60) {
				achievement_case = 60;
			} else if (s_min_breathed_today < 60 && s_min_breathed_today >= 30) {
				achievement_case = 30;
			} else if (s_min_breathed_today < 30 && s_min_breathed_today >= 10) {
				achievement_case = 10;
			} else if (s_min_breathed_today < 10 && s_min_breathed_today >= 5) {
				achievement_case = 5;
			}
			switch (achievement_case) {
				case 10: // 10 is first because we have to check for 10 min in one day and 5 in one day
					if (achievement_get_ten_minutes_day().complete == 0) {
						achievement_set_ten_minutes_day(data_get_date_today(), 1);
						if (settings_get_achievementsEnabled() && !s_achievement_window_pushed) {
							s_achievement_window_pushed = true;
							snprintf(description, 34, localize_get_minutes_day_description(), 10);							
							achievement_window_push(localize_get_ten_minutes_day_name(), description);
						}
						if (achievement_get_all_complete()) {
							achievement_set_completionist(data_get_date_today(), 1);
						}
					}
				case 5:
					if (achievement_get_five_minutes_day().complete == 0) {
						achievement_set_five_minutes_day(data_get_date_today(), 1);
						if (settings_get_achievementsEnabled() && !s_achievement_window_pushed) {
							s_achievement_window_pushed = true;
							snprintf(description, 33, localize_get_minutes_day_description(), 5);							
							achievement_window_push(localize_get_five_minutes_day_name(), description);
						}
						if (achievement_get_all_complete()) {
							achievement_set_completionist(data_get_date_today(), 1);
						}
					}
					break;
				case 30:
					if (achievement_get_thirty_minutes_day().complete == 0) {
						achievement_set_thirty_minutes_day(data_get_date_today(), 1);
						if (settings_get_achievementsEnabled() && !s_achievement_window_pushed) {
							s_achievement_window_pushed = true;
							snprintf(description, 34, localize_get_minutes_day_description(), 30);
							achievement_window_push(localize_get_thirty_minutes_day_name(), description);
						}
						if (achievement_get_all_complete()) {
							achievement_set_completionist(data_get_date_today(), 1);
						}
					}
					break;
				case 60:
					if (achievement_get_one_hour_day().complete == 0) {
						achievement_set_one_hour_day(data_get_date_today(), 1);
						if (settings_get_achievementsEnabled() && !s_achievement_window_pushed) {
							s_achievement_window_pushed = true;
							achievement_window_push(localize_get_one_hour_day_name(), localize_get_one_hour_day_description());
						}
						if (achievement_get_all_complete()) {
							achievement_set_completionist(data_get_date_today(), 1);
						}
					}
					break;
				default:
					break;
			}
		
			// Show achievements for one week, one month, and one year streak if achievements are enabled
			switch(data_get_streak_length()) {
				case 7:
					if (achievement_get_one_week_streak().complete == 0) {
						achievement_set_one_week_streak(data_get_date_today(), 1);
						if (settings_get_achievementsEnabled() && !s_achievement_window_pushed) {
							s_achievement_window_pushed = true;
							achievement_window_push(localize_get_one_week_streak_name(), localize_get_one_week_streak_description());
						}
						if (achievement_get_all_complete()) {
							achievement_set_completionist(data_get_date_today(), 1);
						}
					}
					break;
				case 30:
					if (achievement_get_one_month_streak().complete == 0) {
						achievement_set_one_month_streak(data_get_date_today(), 1);
						if (settings_get_achievementsEnabled() && !s_achievement_window_pushed) {
							s_achievement_window_pushed = true;
							achievement_window_push(localize_get_one_month_streak_name(), localize_get_one_month_streak_description());
						}
						if (achievement_get_all_complete()) {
							achievement_set_completionist(data_get_date_today(), 1);
						}
					}
					break;
				case 365:
					if (achievement_get_one_year_streak().complete == 0) {
						achievement_set_one_year_streak(data_get_date_today(), 1);
						if (settings_get_achievementsEnabled() && !s_achievement_window_pushed) {
							s_achievement_window_pushed = true;
							achievement_window_push(localize_get_one_year_streak_name(), localize_get_one_year_streak_description());
						}
						if (achievement_get_all_complete()) {
							achievement_set_completionist(data_get_date_today(), 1);
						}
					}
					break;
			}
		
		achievement_send_achievements();
		#endif
		
	} else if (complete == 1) { // The user interrupted their session, so only add what was breathed before aborting
		s_min_breathed_today += floor((time(NULL) - s_start_stamp) / 60);
	} else { // Not on the same day, so set number to zero
		s_min_breathed_today = 0;
	}
	
	// Display minutes breathed today
	snprintf(s_min_today, sizeof(s_min_today), localize_get_min_breathed_today_text(), s_min_breathed_today);
	
	// Persist the number of minutes breathed in total today
	data_write_breathe_persist_data(s_min_breathed_today);
	data_write_date_persist_data();
	
	if (complete != 1) {
		// Persist the duration of minutes if user didn't interrupt their session
		data_write_last_duration_data(s_min_to_breathe);
	}
	
	// Sets different number of digits for one digit or two digits
	if (s_min_to_breathe == 10) {
		snprintf(s_min_to_breathe_text, 3, "%d", s_min_to_breathe);
	} else {
		snprintf(s_min_to_breathe_text, 2, "%d", s_min_to_breathe);
	}
	
	// Shows all the layers because breathing is done
	#ifdef PBL_ROUND
		set_min_text(s_min_to_breathe, s_min_text);
		snprintf(s_instruct_text, sizeof(s_instruct_text), "%s", localize_get_breathe_text());
	#endif
	layer_set_hidden(s_inside_text_layer, false);
	layer_set_hidden(s_upper_text_layer, false);
	layer_set_hidden(s_lower_text_layer, false);
	
	#if defined(PBL_HEALTH)
	// Restore the HR sample period
	if (settings_get_heartRateVariation()) {
		data_set_heart_rate_period(0);
	}
	#endif
}

// Last out animation
static void main_animation_end(void *data) {
	int animation_delay; // No delay
	int animation_duration;
	AnimationCurve animation_curve;
	static AnimationImplementation s_main_animation_end = {
		.update = radius_expand_update
	};
	
	int complete = (int)data; // Coerce data into int
	
	if (complete == 0) {
		// Sets duration of animation
		animation_duration = D_END_ANIMATION_DURATION;
		animation_delay = D_END_ANIMATION_DELAY;
		animation_curve = AnimationCurveEaseInOut;
		
		if (settings_get_vibrationEnabled()) {
			vibes_double_pulse(); // Vibration to signal that the thing is ended
		}

		// Localized strings
		snprintf(s_min_today, sizeof(s_min_today), "%s", localize_get_well_done_text());
				
		// Show the "Well done text" and then hides it after 2 seconds
		layer_set_hidden(s_lower_text_layer, false);
		s_hide_lower_text_layer = app_timer_register(animation_duration, hide_lower_text_callback, NULL);
		
		// Next call animation_end_callback to get back to the main menu.
		s_main_timer = app_timer_register(animation_duration + animation_delay, animation_end_callback, NULL);
	} else {
		s_main_animation_end.update = interrupt_expand_update; // Changes the update procedure
		animation_delay = 0;
		animation_duration = 500; // Makes the duration longer
		animation_curve = AnimationCurveEaseOut; // Feels faster than other curves
		layer_set_hidden(s_lower_text_layer, true); // Hides any visible text layers
		layer_set_hidden(s_upper_text_layer, true);
	}

	create_animation(animation_duration, animation_delay, animation_curve, &s_main_animation_end);
}

// Sets up and schedules the main animation of circle contract and expand
static void main_animation() {
		// Hides text layers
		#ifdef PBL_ROUND
			layer_set_hidden(s_inside_text_layer, true);
		#endif

		// Circle expands for 3 seconds and delays for 1 second
		Animation *circle_expand = animation_create();
		animation_set_duration(circle_expand, s_breath_duration);
		animation_set_curve(circle_expand, AnimationCurveEaseInOut);
		animation_set_delay(circle_expand, D_BREATH_HOLD_TIME);
		static AnimationImplementation s_expand_impl = {
			.update = radius_expand_update
		};
		animation_set_implementation(circle_expand, &s_expand_impl);

		// Circle contracts for 3 seconds and delays for 1 second
		Animation *circle_contract = animation_create();
		animation_set_duration(circle_contract, s_breath_duration);
		animation_set_delay(circle_contract, D_BREATH_HOLD_TIME);
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
			int segment_length = 0; // Because it is impossible to find the length of a dynamically allocated array
			switch(settings_get_vibrationType()) {
				case 0: ; // This is vibrations only on inhale
					// Vibrations! (play for 0, rest for 1500, play for 25, rest for 25, etc.)
					static uint32_t *segments;
					switch(s_breaths_per_minute) {
						case 4: ; // 15000 milliseconds long, with an empty statement after a label before a declaration
							static const uint32_t four_segments[49] = {0, 2500, 25, 50, 25, 50, 25, 65, 25, 65, 25, 75, 25, 75, 25, 80, 25, 80, 25, 100, 25, 100, 25, 150, 25, 150, 25, 175, 25, 175, 25, 225, 25, 225, 25, 275, 25, 275, 25, 375, 25, 375, 25, 450, 25, 450, 25, 500, 25};
							segment_length = ARRAY_LENGTH(four_segments); // Get size of array
							segments = (uint32_t *) calloc(segment_length, sizeof(uint32_t)); // Allocate memory equal to size of array to segments and return pointer to segments array
							memcpy(segments, four_segments, sizeof(four_segments)); // Copy each value of array into segments array
							break;
						case 5: ; // 12000 milliseconds long
							static const uint32_t five_segments[44] = {0, 2000, 25, 50, 25, 50, 25, 65, 25, 65, 25, 75, 25, 75, 25, 80, 25, 80, 25, 100, 25, 100, 25, 150, 25, 150, 25, 175, 25, 175, 25, 225, 25, 225, 25, 275, 25, 275, 25, 375, 25, 375, 25};
							segment_length = ARRAY_LENGTH(four_segments);
							segments = (uint32_t *) calloc(segment_length, sizeof(uint32_t));
							memcpy(segments, five_segments, sizeof(five_segments));
							break;
						case 6:
						case 7: ; // 8000 milliseconds long
							static const uint32_t seven_segments[29] = {0, 1500, 25, 25, 25, 25, 25, 25, 25, 25, 25, 50, 25, 75, 25, 125, 25, 125, 25, 125, 25, 125, 25, 200, 25, 325, 25, 550, 25};
							segment_length = ARRAY_LENGTH(four_segments);
							segments = (uint32_t *) calloc(segment_length, sizeof(uint32_t));
							memcpy(segments, seven_segments, sizeof(seven_segments));
							break;
						case 8: // 7000 milliseconds long
						case 9: ;
							static const uint32_t nine_segments[27] = {0, 1500, 25, 25, 25, 25, 25, 25, 25, 50, 25, 75, 25, 100, 25, 125, 25, 150, 25, 200, 25, 250, 25, 300, 25, 350, 25};
							segment_length = ARRAY_LENGTH(four_segments);
							segments = (uint32_t *) calloc(segment_length, sizeof(uint32_t));
							memcpy(segments, nine_segments, sizeof(nine_segments));
							break;
						default: ; // 6000 milliseconds long
							static const uint32_t ten_segments[29] = {0, 1100, 25, 25, 25, 25, 25, 25, 25, 50, 25, 75, 25, 100, 25, 100, 25, 125, 25, 125, 25, 150, 25, 250, 25, 275, 25, 300, 25};
							segment_length = ARRAY_LENGTH(four_segments);
							segments = (uint32_t *) calloc(segment_length, sizeof(uint32_t));
							memcpy(segments, ten_segments, sizeof(ten_segments));
							break;
					}
					VibePattern vibes = {
						.durations = segments,
						.num_segments = segment_length
					};
					vibes_enqueue_custom_pattern(vibes);
					free(segments);
					break;
				case 1: ; // This is vibrations on inhale and exhale
					static uint32_t *segments_both;
					switch(s_breaths_per_minute) {
						case 4: ; // 15000 milliseconds long, with an empty statement after a label before a declaration
							static const uint32_t four_segments[96] = {0, 2500, 25, 50, 25, 50, 25, 65, 25, 65, 25, 75, 25, 75, 25, 80, 25, 80, 25, 100, 25, 100, 25, 150, 25, 150, 25, 175, 25, 175, 25, 225, 25, 225, 25, 275, 25, 275, 25, 375, 25, 450, 25, 450, 25, 500, 25, 2500, 25, 50, 25, 50, 25, 65, 25, 65, 25, 75, 25, 75, 25, 80, 25, 80, 25, 100, 25, 100, 25, 150, 25, 150, 25, 175, 25, 175, 25, 225, 25, 225, 25, 275, 25, 275, 25, 375, 25, 375, 25, 450, 25, 450, 25, 500, 25};
							segment_length = ARRAY_LENGTH(four_segments);
							segments_both = (uint32_t *) calloc(segment_length, sizeof(uint32_t));
							memcpy(segments_both, four_segments, sizeof(four_segments));
							break;
						case 5: ; // 12000 milliseconds long
							static const uint32_t five_segments[86] = {0, 2000, 25, 50, 25, 50, 25, 65, 25, 65, 25, 75, 25, 75, 25, 80, 25, 80, 25, 100, 25, 100, 25, 150, 25, 150, 25, 175, 25, 175, 25, 225, 25, 225, 25, 275, 25, 275, 25, 375, 25, 375, 25, 2200, 25, 50, 25, 50, 25, 65, 25, 65, 25, 75, 25, 75, 25, 80, 25, 80, 25, 100, 25, 100, 25, 150, 25, 150, 25, 175, 25, 175, 25, 225, 25, 225, 25, 275, 25, 275, 25, 275, 25};
							segment_length = ARRAY_LENGTH(five_segments);
							segments_both = (uint32_t *) calloc(segment_length, sizeof(uint32_t));
							memcpy(segments_both, five_segments, sizeof(five_segments));
							break;
						case 6: ; // 8000 milliseconds long
							static const uint32_t six_segments[60] = {0, 1500, 25, 25, 25, 25, 25, 25, 25, 25, 25, 50, 25, 75, 25, 125, 25, 125, 25, 125, 25, 125, 25, 200, 25, 325, 25, 550, 25, 2600, 25, 25, 25, 25, 25, 25, 25, 25, 25, 50, 25, 75, 25, 125, 25, 125, 25, 125, 25, 125, 25, 200, 25, 325, 25, 550, 25};
							segment_length = ARRAY_LENGTH(six_segments);
							segments_both = (uint32_t *) calloc(segment_length, sizeof(uint32_t));
							memcpy(segments_both, six_segments, sizeof(six_segments));
							break;
						case 7: ; // 8000 milliseconds long
							static const uint32_t seven_segments[58] = {0, 1500, 25, 25, 25, 25, 25, 25, 25, 25, 25, 50, 25, 75, 25, 125, 25, 125, 25, 125, 25, 125, 25, 200, 25, 325, 25, 550, 25, 1800, 25, 25, 25, 25, 25, 25, 25, 25, 25, 50, 25, 75, 25, 125, 25, 125, 25, 125, 25, 125, 25, 200, 25, 325, 25, 550, 25};
							segment_length = ARRAY_LENGTH(seven_segments);
							segments_both = (uint32_t *) calloc(segment_length, sizeof(uint32_t));
							memcpy(segments_both, seven_segments, sizeof(seven_segments));
							break;
						case 8: ; // 7000 milliseconds long
							static const uint32_t eight_segments[54] = {0, 1500, 25, 25, 25, 25, 25, 25, 25, 50, 25, 75, 25, 100, 25, 125, 25, 150, 25, 200, 25, 250, 25, 300, 25, 350, 25, 1700, 25, 25, 25, 25, 25, 25, 25, 50, 25, 75, 25, 100, 25, 125, 25, 150, 25, 200, 25, 250, 25, 300, 25};
							segment_length = ARRAY_LENGTH(eight_segments);
							segments_both = (uint32_t *) calloc(segment_length, sizeof(uint32_t));
							memcpy(segments_both, eight_segments, sizeof(eight_segments));
							break;
						case 9: ;
							static const uint32_t nine_segments[54] = {0, 1500, 25, 25, 25, 25, 25, 25, 25, 50, 25, 75, 25, 100, 25, 125, 25, 150, 25, 200, 25, 250, 25, 300, 25, 350, 25, 1200, 25, 25, 25, 25, 25, 25, 25, 50, 25, 75, 25, 100, 25, 125, 25, 150, 25, 200, 25, 250, 25, 300, 25};
							segment_length = ARRAY_LENGTH(nine_segments);
							segments_both = (uint32_t *) calloc(segment_length, sizeof(uint32_t));
							memcpy(segments_both, nine_segments, sizeof(nine_segments));
							break;
						default: ; // 6000 milliseconds long
							static const uint32_t ten_segments[58] = {0, 1000, 25, 25, 25, 25, 25, 25, 25, 50, 25, 75, 25, 100, 25, 100, 25, 125, 25, 125, 25, 150, 25, 250, 25, 275, 25, 300, 25, 1050, 25, 25, 25, 25, 25, 25, 25, 50, 25, 75, 25, 100, 25, 100, 25, 125, 25, 125, 25, 150, 25, 250, 25, 275, 25};
							segment_length = ARRAY_LENGTH(ten_segments);
							segments_both = (uint32_t *) calloc(segment_length, sizeof(uint32_t));
							memcpy(segments_both, ten_segments, sizeof(ten_segments));
							break;
					}
					VibePattern vibes_both = {
						.durations = segments_both,
						.num_segments = segment_length,
					};
					vibes_enqueue_custom_pattern(vibes_both);
					free(segments_both);
					break;
				default: ; // This is simple vibrations (double tap)
					int vib_on, vib_off; // Declare variables for length of vibration on and rest time between vibrations			
					vib_on = 40 + s_breath_duration / 1000;
					vib_off = s_breath_duration / 10 * 2;
					const uint32_t segments_simple[] = {0, 1000, vib_on, vib_off, vib_on, s_breath_duration + 1000 - (vib_off + vib_on * 2), vib_on, vib_off, vib_on};
					VibePattern vibes_simple = {
						.durations = segments_simple,
						.num_segments = ARRAY_LENGTH(segments_simple),
					};
					vibes_enqueue_custom_pattern(vibes_simple);
					break;
			}
		}
}

// Schedules next animation if the number of times played is less than the number of minutes
static void main_animation_callback () {
	#if defined(PBL_HEALTH)
	// Update the breathDuration if in variable HR mode, but only if it's higher. (Which means that the breaths are slowing down and people are relaxing! Wow!)
	if (settings_get_heartRateVariation() && data_get_current_heart_rate() != 0) {
		int new_duration = settings_get_breathDuration();
		if (new_duration > s_breath_duration) {
			APP_LOG(APP_LOG_LEVEL_DEBUG, "Old duration %d, new %d", s_breath_duration, new_duration);
			s_breath_duration = new_duration;
		}
	} else {
		s_breath_duration = settings_get_breathDuration(); // Check for changes in the breath duration; it may have been changed in the settings
	}
	#else
		s_breath_duration = settings_get_breathDuration(); // Check for changes in the breath duration; it may have been changed in the settings
	#endif
	s_breaths_per_minute = settings_get_breathsPerMinute();
	
	// If we are animating and main timer isn't done yet
	if (s_animating && !s_main_done) {
		animationTimer = app_timer_register(2 * s_breath_duration + 2 * D_BREATH_HOLD_TIME, main_animation_callback, NULL); // Run this method again to schedule the next breath
		if (!layer_get_hidden(s_upper_text_layer) || !layer_get_hidden(s_lower_text_layer)) { // The text layers aren't hidden; this keep if HRV is enabled
			#if !PBL_PLATFORM_APLITE
			if (settings_get_heartRateVariation() && s_times_played > 1) {
				layer_set_hidden(s_upper_text_layer, false); // For the HR text
			} else { 
			#endif
				layer_set_hidden(s_upper_text_layer, true); // HRV isn't enabled, so hide the text layers
				layer_set_hidden(s_lower_text_layer, true);
			#if !PBL_PLATFORM_APLITE
			}
			#endif
		}
		main_animation();
	} else { // This means that the main animation is complete
		animationTimer = app_timer_register(500, main_animation_end, (void*)0);
	}
}

#if defined(PBL_HEALTH)
// Update HeartRate in the top slot during a session
static void heart_rate_update_callback(void *context) {
	if (s_animating) {
		data_update_heart_rate_buffer(); // Update heart rate
		snprintf(s_greet_text, sizeof(s_greet_text), "%s", data_get_current_heart_rate_buffer()); // Show heart rate upon redraw of s_upper_text_layer
		layer_set_hidden(s_upper_text_layer, false);
		
		// Re-schedule the timer
		s_update_hr_timer = app_timer_register(s_breath_duration, heart_rate_update_callback, NULL);
	}	
}
#endif

// Called when time to breathe has elapsed
static void main_done_callback(void *context) {
	s_main_done = true;
}

// Shows instructions to exhale; first hides the top text and then shows the bottom text
static void first_breath_out_callback(void *context) {
	// Intro done, start timer for when to stop breathing session 
	s_main_timer = app_timer_register(s_min_to_breathe * MILLISECONDS_PER_MINUTE - 6000 - D_BREATH_HOLD_TIME - s_breath_duration, main_done_callback, NULL);
	
	snprintf(s_min_today, sizeof(s_min_today), "%s", localize_get_exhale_text());
	layer_set_hidden(s_upper_text_layer, true);
	layer_set_hidden(s_lower_text_layer, false);
	
	#if defined(PBL_HEALTH)
	// Start HR update timer which shows the heart rate in the upper text field after the first breath
	if (settings_get_heartRateVariation()) {
		s_update_hr_timer = app_timer_register(s_breath_duration * 2, heart_rate_update_callback, NULL);
	}
	#endif
}

// Shows instructions to inhale
static void first_breath_in_callback(void *context) {
	// Next up, first_breath_out_callback
	s_main_timer = app_timer_register(s_breath_duration, first_breath_out_callback, NULL);
	
	snprintf(s_greet_text, sizeof(s_greet_text), "%s", localize_get_inhale_text());
	layer_set_hidden(s_upper_text_layer, false);
	#ifdef PBL_ROUND
		layer_set_hidden(s_inside_text_layer, true);
	#endif
}

// Start animation show text
static void animation_start_callback(void *context) {
	// Next up, first_breath_in_callback
	s_main_timer = app_timer_register(D_START_ANIMATION_TIME, first_breath_in_callback, NULL);
	
	// Generates a random number between 0 and 8 and 0 and 3 inclusive
	int random_number = rand() % 9;
	int random_number_2 = rand() % 4;
	
	#ifdef PBL_RECT
		// Shows the text at top and bottom
		snprintf(s_greet_text, sizeof(s_greet_text), "%s", localize_get_top_text(random_number));
		snprintf(s_min_today, sizeof(s_min_today), "%s", localize_get_bottom_text(random_number_2));
		layer_set_hidden(s_upper_text_layer, false);
		layer_set_hidden(s_lower_text_layer, false);
	#else
		// Shows the text near the center of the screen
		snprintf(s_instruct_text, sizeof(s_instruct_text), "%s", localize_get_top_text(random_number));
		snprintf(s_min_text, sizeof(s_min_text), "%s", localize_get_bottom_text(random_number_2));
		layer_set_hidden(s_inside_text_layer, false);
	#endif
}

// First contract animation after pressing select button
static void main_animation_start() {
	static AnimationImplementation s_main_animation_start = {
		.update = radius_contract_update
	};
	create_animation(D_START_ANIMATION_DURATION, D_START_ANIMATION_DELAY, AnimationCurveEaseInOut, &s_main_animation_start);
}

// Start up animation
static void circle_animation_setup() {
	static AnimationImplementation s_circle_impl = {
		.update = radius_expand_update // Start up animation expands
	};
	create_animation(D_CIRCLE_ANIMATION_DURATION, D_CIRCLE_ANIMATION_DELAY, AnimationCurveEaseInOut, &s_circle_impl);
	s_animating = true;
}

// ******************************************************************************************* Click Handlers

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
	// Increments number, displays number.
	if ((!s_animating) && (s_min_to_breathe < 10)) {
		s_min_to_breathe += 1;
		set_min_text(s_min_to_breathe, s_min_text);
		if (s_min_to_breathe == 10) {
			snprintf(s_min_to_breathe_text, 3, "%d", s_min_to_breathe);
		} else {
			snprintf(s_min_to_breathe_text, 2, "%d", s_min_to_breathe);
		}
		layer_mark_dirty(s_inside_text_layer);
	}
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
	// Decrements number, displays number.
	if ((!s_animating) && (s_min_to_breathe > 1)) {
		s_min_to_breathe -= 1;
		set_min_text(s_min_to_breathe, s_min_text);
		snprintf(s_min_to_breathe_text, 2, "%d", s_min_to_breathe);
		layer_mark_dirty(s_inside_text_layer);
	}
}

#if !PBL_PLATFORM_APLITE
static void long_down_click_handler(ClickRecognizerRef recognizer, void *context) {
	if ((!s_animating) && settings_get_achievementsEnabled() == true) {
		achievement_menu_window_push();
	}
}
// #endif

static void long_up_click_handler(ClickRecognizerRef recognizer, void *context) {
	if (!s_animating) {
		settings_menu_window_push();
	}
}
#endif

static void back_click_handler(ClickRecognizerRef recognizer, void *context) {
	if (s_animating) {
		// Prevents any further animations and vibrations
		animation_unschedule_all();
		vibes_cancel();
		
		// Destroys all currently running timers
		if (s_animation_completed_timer != NULL) {
			app_timer_cancel(s_animation_completed_timer);
		}
		if (animationTimer != NULL) {
			app_timer_cancel(animationTimer);
		}
		if (s_update_hr_timer != NULL) {
			app_timer_cancel(s_update_hr_timer);
		}
		if (s_main_timer != NULL) {
			app_timer_cancel(s_main_timer);
		}
		
		// Start expand animation
		s_current_radius = s_radius;
		main_animation_end((void*)1); // Tells animation that user interrupted session
		s_interrupt_timer = app_timer_register(525, animation_end_callback, (void*)1);
	} else {
		window_stack_pop_all(true);
	}
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
	// Only does things if things are not animating
	if (!s_animating) {
		s_times_played = 0; // Keep track of how many times animation has run (show heart rate text after once)
		s_animating = true;
		s_main_done = false;
		s_breaths_per_minute = settings_get_breathsPerMinute();
		s_breath_duration = settings_get_breathDuration();
		
		#if defined(PBL_HEALTH)
		// Kick the HR to high gear (update every one second!)
		if (settings_get_heartRateVariation() && data_get_current_heart_rate() != 0) {
			data_set_heart_rate_period(1);
		}
		#endif
		
		// Hides all text layers
		layer_set_hidden(s_inside_text_layer, true);
		layer_set_hidden(s_upper_text_layer, true);
		layer_set_hidden(s_lower_text_layer, true);
		
		// Gets timestamp of start time as the epoch time to calculate duration if user interrupts session
		s_start_stamp = time(NULL) + 6;
		
		// Starts the first circle contraction
		main_animation_start();

		// Starts the main timer
		s_main_timer = app_timer_register(D_START_DELAY, animation_start_callback, NULL);
		
		// First animationTimer, which will schedule the next time the circle expands or contracts
		animationTimer = app_timer_register(6000, main_animation_callback, NULL); 
		
		// Gets today's date to compare with the end date after breathing is finished
		snprintf(s_start_time, sizeof(s_start_time), "%s", data_get_date_today());
	}
}

static void click_config_provider(void *context) {
	ButtonId id_up = BUTTON_ID_UP;  // The Up button
	ButtonId id_down = BUTTON_ID_DOWN; // The Down button
	ButtonId id_select = BUTTON_ID_SELECT; // The Select button
	ButtonId id_back = BUTTON_ID_BACK;
	window_single_click_subscribe(id_up, up_click_handler);
	window_single_click_subscribe(id_down, down_click_handler);
	window_single_click_subscribe(id_back, back_click_handler);
	window_single_click_subscribe(id_select, select_click_handler);
	#if !PBL_PLATFORM_APLITE
	window_long_click_subscribe(id_up, 500, long_up_click_handler, NULL);
	// #if !PBL_PLATFORM_APLITE
	if (settings_get_achievementsEnabled()) {
		window_long_click_subscribe(id_down, 500, long_down_click_handler, NULL); // Click for 500ms before firing
	}
	#endif
}

// ******************************************************************************************* Main App Functions
static void inbox_received_handler(DictionaryIterator *iter, void *context) {
	#if !PBL_PLATFORM_APLITE
	// Check if this is a request to send the settings on watch to the phone
	Tuple *request_settings_t = dict_find(iter, MESSAGE_KEY_requestSettings);
	Tuple *achievements_t = dict_find(iter, 0);
	if (request_settings_t) {
		settings_send_settings(); // If yes, then send the settings
	} else if (achievements_t) {
		achievement_handle_achievements(iter, context);
	} else {
	#endif
		// Otherwise, save settings received from phone, and refresh screen
		settings_handle_settings(iter, context);
		window_set_background_color(s_main_window, settings_get_backgroundColor());
		layer_mark_dirty(s_circle_layer);
		layer_mark_dirty(s_inside_text_layer);
		layer_mark_dirty(s_upper_text_layer);
	#if !PBL_PLATFORM_APLITE
	}
	#endif
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
		app_timer_cancel(animationTimer);
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
	
	// Load text
	s_min_breathed_today = data_read_breathe_persist_data(); // Read the number of minutes breathed today from persistent memory
	snprintf(s_min_today, sizeof(s_min_today), localize_get_min_breathed_today_text(), s_min_breathed_today);
	s_min_to_breathe = min;
	snprintf(s_min_to_breathe_text, sizeof(s_min_to_breathe_text), "%d", s_min_to_breathe);
	set_min_text(s_min_to_breathe, s_min_text); // Sets the text to "min" or "mins" depending on singular or plural
	snprintf(s_greet_text, sizeof(s_greet_text), "%s", localize_get_greet_text());
	
	s_main_window = window_create();
	window_set_background_color(s_main_window, settings_get_backgroundColor());
	window_set_window_handlers(s_main_window, (WindowHandlers) {
		.load = main_window_load,
		.unload = main_window_unload
	});
	
	s_click_provider_timer = app_timer_register(2000, set_click_config_providers, NULL);
	
	// Starts timer to show text at end of start animation
	const int delay_ms = 1800;
	s_animation_completed_timer = app_timer_register(delay_ms, finish_setup_callback, NULL);
	
	// Show window on the watch, with animated = true
	window_stack_push(s_main_window, true);
}