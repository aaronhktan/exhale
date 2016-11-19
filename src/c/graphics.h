#pragma once
#include <pebble.h>
#include "src/c/data.h"

#ifdef PBL_PLATFORM_EMERY
	#define FONT_KEY FONT_KEY_GOTHIC_18_BOLD
#else
	#define FONT_KEY FONT_KEY_GOTHIC_14_BOLD
#endif

void graphics_draw_upper_text(GContext *ctx, GRect bounds, bool is_animating, bool heart_rate, GColor textColor, char *);

void graphics_draw_lower_text(GContext *ctx, GRect bounds, bool is_animating, GColor textColor, char *);

void graphics_draw_inner_text(GContext *ctx, GRect bounds, bool is_animating, int min_to_breathe, GColor textColor, char *, char *, char *);

void graphics_draw_main_circle(GContext *ctx, GColor circleColor, GPoint center, int radius);

void graphics_create_triangles_gpath();