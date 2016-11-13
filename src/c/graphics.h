#pragma once
#include <pebble.h>
#include "src/c/data.h"

void graphics_draw_upper_text(GContext *ctx, GRect bounds, bool is_animating, GColor textColor, char *);

void graphics_draw_lower_text(GContext *ctx, GRect bounds, bool is_animating, GColor textColor, char *);

void graphics_draw_inner_text(GContext *ctx, GRect bounds, int min_to_breathe, GColor textColor, char *, char *, char *);

void graphics_draw_main_circle(GContext *ctx, GColor circleColor, GPoint center, int radius);

void graphics_create_triangles_gpath();