#include <pebble.h>
#include "reminder_window.h"
#include "src/c/breathe_window.h"
#include "localize.h"

#define DELTA 33

static Window *s_reminder_window;
static ActionMenu *s_action_menu;
static ActionMenuLevel *s_root_level;
static int s_min_to_breathe;
static Layer *s_canvas_layer;
static TextLayer *s_text_layer;
static GDrawCommandSequence *s_command_seq;
GColor random_color;

int s_index = 0;

static void next_frame_handler(void *context) {
	layer_mark_dirty(s_canvas_layer);
	app_timer_register(DELTA, next_frame_handler, NULL);
}

static void canvas_update_proc(Layer *layer, GContext *ctx) {
	GRect bounds = layer_get_bounds(layer);
  GSize seq_bounds = gdraw_command_sequence_get_bounds_size(s_command_seq);

  GDrawCommandFrame *frame = gdraw_command_sequence_get_frame_by_index(s_command_seq, s_index);

  if (frame) {
    gdraw_command_frame_draw(ctx, s_command_seq, frame, GPoint(
      (bounds.size.w - seq_bounds.w) / 2,
      (bounds.size.h - seq_bounds.h) / 2
    ));
  }

  // Advance to the next frame, wrapping if neccessary
  int num_frames = gdraw_command_sequence_get_num_frames(s_command_seq);
  s_index++;
  if (s_index == num_frames) {
    s_index = 0;
  }
	
	graphics_context_set_fill_color(ctx, GColorBlack);
	graphics_fill_circle(ctx, GPoint(bounds.size.w + 5, bounds.size.h / 2), 10);
}

static void action_performed_callback(ActionMenu *action_menu, const ActionMenuItem *action, void *context) {
  // An action was selected, determine which one
  s_min_to_breathe = (int)action_menu_item_get_action_data(action);
	window_stack_remove(s_reminder_window, false);
	breathe_window_push(s_min_to_breathe);
}

static void init_action_menu() {
  // Create the root level
  s_root_level = action_menu_level_create(10);

  // Set up the actions for this level, using action context to pass types
	
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		action_menu_level_add_action(s_root_level, "Respirer 1 min", action_performed_callback, 
																(void *)1);
		action_menu_level_add_action(s_root_level, "Respirer 2 min", action_performed_callback, 
																 (void *)2);
		action_menu_level_add_action(s_root_level, "Respirer 3 min", action_performed_callback, 
																 (void *)3);
		action_menu_level_add_action(s_root_level, "Respirer 4 min", action_performed_callback, 
																 (void *)4);
		action_menu_level_add_action(s_root_level, "Respirer 5 min", action_performed_callback, 
																 (void *)5);
		action_menu_level_add_action(s_root_level, "Respirer 6 min", action_performed_callback, 
																 (void *)6);
		action_menu_level_add_action(s_root_level, "Respirer 7 min", action_performed_callback, 
																 (void *)7);
		action_menu_level_add_action(s_root_level, "Respirer 8 min", action_performed_callback, 
																 (void *)8);
		action_menu_level_add_action(s_root_level, "Respirer 9 min", action_performed_callback, 
																 (void *)9);
		action_menu_level_add_action(s_root_level, "Respirer 10 min", action_performed_callback, 
																 (void *)10);
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		action_menu_level_add_action(s_root_level, "Respirar 1 min", action_performed_callback, 
																(void *)1);
		action_menu_level_add_action(s_root_level, "Respirar 2 min", action_performed_callback, 
																 (void *)2);
		action_menu_level_add_action(s_root_level, "Respirar 3 min", action_performed_callback, 
																 (void *)3);
		action_menu_level_add_action(s_root_level, "Respirar 4 min", action_performed_callback, 
																 (void *)4);
		action_menu_level_add_action(s_root_level, "Respirar 5 min", action_performed_callback, 
																 (void *)5);
		action_menu_level_add_action(s_root_level, "Respirar 6 min", action_performed_callback, 
																 (void *)6);
		action_menu_level_add_action(s_root_level, "Respirar 7 min", action_performed_callback, 
																 (void *)7);
		action_menu_level_add_action(s_root_level, "Respirar 8 min", action_performed_callback, 
																 (void *)8);
		action_menu_level_add_action(s_root_level, "Respirar 9 min", action_performed_callback, 
																 (void *)9);
		action_menu_level_add_action(s_root_level, "Respirar 10 min", action_performed_callback, 
																 (void *)10);
	} else {
		action_menu_level_add_action(s_root_level, "Breathe 1 min", action_performed_callback, 
																(void *)1);
		action_menu_level_add_action(s_root_level, "Breathe 2 min", action_performed_callback, 
																 (void *)2);
		action_menu_level_add_action(s_root_level, "Breathe 3 min", action_performed_callback, 
																 (void *)3);
		action_menu_level_add_action(s_root_level, "Breathe 4 min", action_performed_callback, 
																 (void *)4);
		action_menu_level_add_action(s_root_level, "Breathe 5 min", action_performed_callback, 
																 (void *)5);
		action_menu_level_add_action(s_root_level, "Breathe 6 min", action_performed_callback, 
																 (void *)6);
		action_menu_level_add_action(s_root_level, "Breathe 7 min", action_performed_callback, 
																 (void *)7);
		action_menu_level_add_action(s_root_level, "Breathe 8 min", action_performed_callback, 
																 (void *)8);
		action_menu_level_add_action(s_root_level, "Breathe 9 min", action_performed_callback, 
																 (void *)9);
		action_menu_level_add_action(s_root_level, "Breathe 10 min", action_performed_callback, 
																 (void *)10);
	}
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
	// ActionMenu
  ActionMenuConfig config = (ActionMenuConfig) {
    .root_level = s_root_level,
    .colors = {
      .background = GColorBlack,
      .foreground = random_color,
    },
    .align = ActionMenuAlignCenter
  };
	
	// Show the ActionMenu
	s_action_menu = action_menu_open(&config);
}

static void click_config_provider(void *context) {
	ButtonId id_select = BUTTON_ID_SELECT; // The Select button
	window_single_click_subscribe(id_select, select_click_handler);
}

static void reminder_window_load(Window *window) {
	// Information about screen
	Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

	// Layer for PDC
	s_canvas_layer = layer_create(bounds);
	layer_set_update_proc(s_canvas_layer, canvas_update_proc);
	
	layer_add_child(window_layer, s_canvas_layer);
	
	vibes_double_pulse();
  window_set_background_color(s_reminder_window, PBL_IF_COLOR_ELSE(random_color, GColorWhite));
	
	app_timer_register(DELTA, next_frame_handler, NULL);
	
	// Layer for text
	s_text_layer = text_layer_create(GRect(0, bounds.size.w - PBL_IF_RECT_ELSE(16, 40), bounds.size.w, bounds.size.h / 2));
	text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
	text_layer_set_background_color(s_text_layer, random_color);
	text_layer_set_text_alignment(s_text_layer, GTextAlignmentCenter);
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		text_layer_set_text(s_text_layer, "Respirez maintenant!");
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		text_layer_set_text(s_text_layer, "¡Respira ahora!");
	} else {
		text_layer_set_text(s_text_layer, "Time to breathe!");
	}
	
	layer_add_child(window_layer, text_layer_get_layer(s_text_layer));
	
	init_action_menu();
}

static void reminder_window_unload(Window *window) {
	layer_destroy(s_canvas_layer);
	layer_destroy(text_layer_get_layer(s_text_layer));
  gdraw_command_sequence_destroy(s_command_seq);
	action_menu_hierarchy_destroy(s_root_level, NULL, NULL);
	window_destroy(s_reminder_window);
}

void reminder_window_push() {
	#if PBL_COLOR
		random_color = (GColor){ .a = 3, .r = rand() % 4, .g = rand() % 4, .b = rand() % 4 };
	#else
		random_color = GColorWhite;
	#endif
	
	s_command_seq = gdraw_command_sequence_create_with_resource(RESOURCE_ID_CLOCK_SEQUENCE);
	
	s_reminder_window = window_create();
	
	window_set_window_handlers(s_reminder_window, (WindowHandlers) {
		.load = reminder_window_load, 
		.unload = reminder_window_unload});
	
	window_set_click_config_provider(s_reminder_window, click_config_provider);
	
	window_stack_push(s_reminder_window, true);
}