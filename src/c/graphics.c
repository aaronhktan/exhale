#include <pebble.h>
#include "graphics.h"

static GPath *s_up_triangle, *s_down_triangle;

// Depending on display size, change location of up and down arrows
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
			.points = (GPoint []) {{105, 65}, {95, 65}, {100, 60}}
		};
	static const GPathInfo DOWN_PATH_INFO = {
			.num_points = 3,
			.points = (GPoint []) {{105, 164}, {95, 164}, {100, 169}}
		};
#endif

// Method for updating the upper text layer
void graphics_draw_upper_text(GContext *ctx, GRect bounds, bool is_animating, int display_text, GColor textColor, char *greet_text) {
	#if defined(PBL_HEALTH)
		const char *steps_buffer = data_get_current_steps_buffer(); // Pebble Health exists; fetch the number of steps walked today
	#endif
	
	graphics_context_set_text_color(ctx, (is_animating) ? textColor : PBL_IF_COLOR_ELSE(GColorDarkGray, textColor)); // Set text color to dark gray on main menu, but white for other sections
	GSize greet_text_bounds = graphics_text_layout_get_content_size("THAT IS A LOT OF STEPS TO TAKE IN JUST ONE DAY, INNIT?", fonts_get_system_font(FONT_KEY),
																																		GRect(0, 0, bounds.size.w, bounds.size.h),
																																		GTextOverflowModeWordWrap, GTextAlignmentCenter); // Random string for size purposes
	if (is_animating) { // If animating, shows the normal text
		graphics_draw_text(ctx, greet_text, fonts_get_system_font(FONT_KEY), 
											 GRect((bounds.size.w - greet_text_bounds.w) / 2, PBL_IF_RECT_ELSE(5, 15), greet_text_bounds.w, greet_text_bounds.h),
											 GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
	} else {
		#if defined(PBL_HEALTH)
		if (display_text == 3 && data_get_current_heart_rate() > 0) { // If heart rate monitor is enabled in configuration and is available, show heart rate
			const char *heart_rate_buffer = data_get_current_heart_rate_buffer();
			graphics_draw_text(ctx, heart_rate_buffer, fonts_get_system_font(FONT_KEY), 
											 GRect((bounds.size.w - greet_text_bounds.w) / 2, PBL_IF_RECT_ELSE(5, 15), greet_text_bounds.w, greet_text_bounds.h),
											 GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
		} else { // Otherwise, show step counts if Pebble Health, and string if not.
			switch(display_text) {
				case 0:
					// No text
					break;
				case 1:
					graphics_draw_text(ctx, greet_text, fonts_get_system_font(FONT_KEY), 
											 GRect((bounds.size.w - greet_text_bounds.w) / 2, PBL_IF_RECT_ELSE(5, 15), greet_text_bounds.w, greet_text_bounds.h),
											 GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
					break;
				case 2:
				case 3:
					graphics_draw_text(ctx, PBL_IF_HEALTH_ELSE(steps_buffer, greet_text), fonts_get_system_font(FONT_KEY), 
											 GRect((bounds.size.w - greet_text_bounds.w) / 2, PBL_IF_RECT_ELSE(5, 15), greet_text_bounds.w, greet_text_bounds.h),
											 GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
					break;
			}
		}
	#else
		switch(display_text) {
				case 0:
					// No text
					break;
				case 1:
					graphics_draw_text(ctx, greet_text, fonts_get_system_font(FONT_KEY), 
											 GRect((bounds.size.w - greet_text_bounds.w) / 2, PBL_IF_RECT_ELSE(5, 15), greet_text_bounds.w, greet_text_bounds.h),
											 GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
					break;
				case 2:
				case 3:
					graphics_draw_text(ctx, PBL_IF_HEALTH_ELSE(steps_buffer, greet_text), fonts_get_system_font(FONT_KEY), 
											 GRect((bounds.size.w - greet_text_bounds.w) / 2, PBL_IF_RECT_ELSE(5, 15), greet_text_bounds.w, greet_text_bounds.h),
											 GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
					break;
		}
	#endif
	}
}

// Method for updating the lower text layer
void graphics_draw_lower_text(GContext *ctx, GRect bounds, bool is_animating, GColor textColor, char *min_today) {
	graphics_context_set_text_color(ctx, (is_animating) ? textColor : PBL_IF_COLOR_ELSE(GColorDarkGray, textColor)); // Like above, sets text color to dark gray on main menu, but white for other sections
	GSize today_text_bounds = graphics_text_layout_get_content_size("TODAY: 10,000 MINUTES", fonts_get_system_font(FONT_KEY),
																																	GRect(0, 0, bounds.size.w, bounds.size.h),
																																	GTextOverflowModeWordWrap, GTextAlignmentCenter);

	graphics_draw_text(ctx, min_today, fonts_get_system_font(FONT_KEY), 
										 GRect((bounds.size.w - today_text_bounds.w) / 2, bounds.size.h - today_text_bounds.h - PBL_IF_RECT_ELSE(8, 20), today_text_bounds.w, today_text_bounds.h),
										 GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
}

// Method for updating the inner text, triangles, and the semicircle on the right center of the screen
void graphics_draw_inner_text(GContext *ctx, GRect bounds, bool is_animating, int min_to_breathe, GColor textColor, char *min_to_breathe_text, char *instruct_text, char *min_text) {
	// Draw side circle
	graphics_context_set_fill_color(ctx, textColor);
	#ifdef PBL_PLATFORM_EMERY
		graphics_fill_circle(ctx, GPoint(bounds.size.w + 7, bounds.size.h / 2), 12);
	#elif PBL_PLATFORM_CHALK
		if (!is_animating) {
			graphics_fill_circle(ctx, GPoint(bounds.size.w + 1, bounds.size.h / 2), 12);
		}
	#else
		graphics_fill_circle(ctx, GPoint(bounds.size.w + 5, bounds.size.h / 2), 10);
	#endif

	if (!is_animating) {
		// Draw triangles
		switch(min_to_breathe) {
			case 1 :
			gpath_draw_filled(ctx, s_up_triangle); // Only draw the upper triangle because user cannot set a time lower than 1
			break;
			case 10 :
			gpath_draw_filled(ctx, s_down_triangle); // Only draw the lower triangle because the user cannot set a time higher than 10
			break;
			default:
			gpath_draw_filled(ctx, s_up_triangle);
			gpath_draw_filled(ctx, s_down_triangle);
		}
	}

	// Draw text in circle
	graphics_context_set_text_color(ctx, textColor);
	if (!is_animating) {
		GSize min_to_breathe_bounds = graphics_text_layout_get_content_size("10", fonts_get_system_font(FONT_KEY_LECO_42_NUMBERS), 
																																				GRect(0, 0, bounds.size.w, bounds.size.h), 
																																				GTextOverflowModeWordWrap, GTextAlignmentCenter);
		graphics_draw_text(ctx, min_to_breathe_text, fonts_get_system_font(FONT_KEY_LECO_42_NUMBERS), 
											 GRect((bounds.size.w - min_to_breathe_bounds.w) / 2, (bounds.size.h - min_to_breathe_bounds.h) / 2 - 6, min_to_breathe_bounds.w, min_to_breathe_bounds.h), 
											 GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
	}
	
	GSize instruct_text_bounds = graphics_text_layout_get_content_size(PBL_IF_ROUND_ELSE("DONNEZ_VOUS DE L'ESPACE;", "RESPIRAR"), fonts_get_system_font(FONT_KEY), 
																																		 GRect(0, 0, bounds.size.w, bounds.size.h), 
																																		 GTextOverflowModeWordWrap, GTextAlignmentCenter);
	graphics_draw_text(ctx, instruct_text, fonts_get_system_font(FONT_KEY), 
										 GRect((bounds.size.w - instruct_text_bounds.w) / 2, (bounds.size.h - instruct_text_bounds.h) / 2 - 29, instruct_text_bounds.w, instruct_text_bounds.h), 
										 GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);

	GSize min_text_bounds = graphics_text_layout_get_content_size("CONCENTRATE.", fonts_get_system_font(FONT_KEY), 
																																GRect(0, 0, bounds.size.w, bounds.size.h), 
																																GTextOverflowModeWordWrap, GTextAlignmentCenter);
	graphics_draw_text(ctx, min_text, fonts_get_system_font(FONT_KEY), 
										 GRect((bounds.size.w - min_text_bounds.w) / 2, (bounds.size.h - min_text_bounds.h) / 2 + 25, min_text_bounds.w, min_text_bounds.h), 
										 GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
}

// Method to update the main circle in the UI
void graphics_draw_main_circle(GContext *ctx, GColor circleColor, GPoint center, int radius) {
	graphics_context_set_stroke_color(ctx, circleColor);
	graphics_context_set_stroke_width(ctx, 5);
	graphics_draw_circle(ctx, center, radius);
}

// Creates paths from the coordinates given in preprocessor directives
void graphics_create_triangles_gpath() {
	s_up_triangle = gpath_create(&UP_PATH_INFO);
	s_down_triangle = gpath_create(&DOWN_PATH_INFO);
}