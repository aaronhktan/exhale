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

void graphics_draw_upper_text(GContext *ctx, GRect bounds, bool is_animating, bool heart_rate, GColor textColor, char *greet_text) {
	#if defined(PBL_HEALTH)
		const char *steps_buffer = data_get_current_steps_buffer();
	#endif
	
	graphics_context_set_text_color(ctx, (is_animating) ? textColor : PBL_IF_COLOR_ELSE(GColorDarkGray, textColor));
	GSize greet_text_bounds = graphics_text_layout_get_content_size("10,000 STEPS TODAY OMG", fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD),
																																		GRect(0, 0, bounds.size.w, bounds.size.h),
																																		GTextOverflowModeWordWrap, GTextAlignmentCenter);
	if (is_animating) {
		graphics_draw_text(ctx, greet_text, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD), 
											 GRect((bounds.size.w - greet_text_bounds.w) / 2, 5, greet_text_bounds.w, greet_text_bounds.h),
											 GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
	} else {
		if (heart_rate && data_get_current_heart_rate() > 0) {
			const char *heart_rate_buffer = data_get_current_heart_rate_buffer();
			graphics_draw_text(ctx, heart_rate_buffer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD), 
											 GRect((bounds.size.w - greet_text_bounds.w) / 2, 5, greet_text_bounds.w, greet_text_bounds.h),
											 GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
		} else {
			graphics_draw_text(ctx, PBL_IF_HEALTH_ELSE(steps_buffer, greet_text), fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD), 
											 GRect((bounds.size.w - greet_text_bounds.w) / 2, 5, greet_text_bounds.w, greet_text_bounds.h),
											 GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
		}
	}
}

void graphics_draw_lower_text(GContext *ctx, GRect bounds, bool is_animating, GColor textColor, char *min_today) {
	graphics_context_set_text_color(ctx, (is_animating) ? textColor : PBL_IF_COLOR_ELSE(GColorDarkGray, textColor));
	GSize today_text_bounds = graphics_text_layout_get_content_size("TODAY: 10 MINUTES", fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD),
																																	GRect(0, 0, bounds.size.w, bounds.size.h),
																																	GTextOverflowModeWordWrap, GTextAlignmentCenter);

	graphics_draw_text(ctx, min_today, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD), 
										 GRect((bounds.size.w - today_text_bounds.w) / 2, bounds.size.h - today_text_bounds.h - 8, today_text_bounds.w, today_text_bounds.h),
										 GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
}

void graphics_draw_inner_text(GContext *ctx, GRect bounds, int min_to_breathe, GColor textColor, char *min_to_breathe_text, char *instruct_text, char *min_text) {
	//draw side circle
	graphics_context_set_fill_color(ctx, textColor);
	graphics_fill_circle(ctx, GPoint(bounds.size.w + 5, bounds.size.h / 2), 10);

	// draw triangles
	switch(min_to_breathe) {
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
	graphics_context_set_text_color(ctx, textColor);
	GSize min_to_breathe_bounds = graphics_text_layout_get_content_size("10", fonts_get_system_font(FONT_KEY_LECO_42_NUMBERS), 
																																			GRect(0, 0, bounds.size.w, bounds.size.h), 
																																			GTextOverflowModeWordWrap, GTextAlignmentCenter);
	graphics_draw_text(ctx, min_to_breathe_text, fonts_get_system_font(FONT_KEY_LECO_42_NUMBERS), 
										 GRect((bounds.size.w - min_to_breathe_bounds.w) / 2, (bounds.size.h - min_to_breathe_bounds.h) / 2 - 6, min_to_breathe_bounds.w, min_to_breathe_bounds.h), 
										 GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);

	GSize instruct_text_bounds = graphics_text_layout_get_content_size("BREATHE", fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD), 
																																		 GRect(0, 0, bounds.size.w, bounds.size.h), 
																																		 GTextOverflowModeWordWrap, GTextAlignmentCenter);
	graphics_draw_text(ctx, instruct_text, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD), 
										 GRect((bounds.size.w - instruct_text_bounds.w) / 2, (bounds.size.h - instruct_text_bounds.h) / 2 - 29, instruct_text_bounds.w, instruct_text_bounds.h), 
										 GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);

	GSize min_text_bounds = graphics_text_layout_get_content_size("MINS", fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD), 
																																GRect(0, 0, bounds.size.w, bounds.size.h), 
																																GTextOverflowModeWordWrap, GTextAlignmentCenter);
	graphics_draw_text(ctx, min_text, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD), 
										 GRect((bounds.size.w - min_text_bounds.w) / 2, (bounds.size.h - min_text_bounds.h) / 2 + 25, min_text_bounds.w, min_text_bounds.h), 
										 GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
}

void graphics_draw_main_circle(GContext *ctx, GColor circleColor, GPoint center, int radius) {
	graphics_context_set_stroke_color(ctx, circleColor);
	graphics_context_set_stroke_width(ctx, 5);
	graphics_draw_circle(ctx, center, radius);
}

void graphics_create_triangles_gpath() {
	s_up_triangle = gpath_create(&UP_PATH_INFO);
	s_down_triangle = gpath_create(&DOWN_PATH_INFO);
}