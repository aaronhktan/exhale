#pragma once

const char * localize_get_locale();

#if !PBL_PLATFORM_APLITE
char * localize_get_app_glance_text(int type, int minutes);
#endif
char * localize_get_breathe_text();
char * localize_get_well_done_text();
char * localize_get_inhale_text();
char * localize_get_exhale_text();
char * localize_get_min_breathed_today_text();
#if !PBL_PLATFORM_APLITE
char * localize_get_steps_today_text(int thousands);
char * localize_get_heart_rate_text();
#endif
char * localize_get_reminder_action_menu_text();
char * localize_get_reminder_text();
char * localize_get_greet_text();
char * localize_get_snooze_text();

// Section titles
char * localize_get_in_app_section_title();
char * localize_get_reminders_section_title();
#if !PBL_PLATFORM_APLITE
char * localize_get_health_section_title();
char * localize_get_app_glance_section_title();
#endif

// Row subtitles
char * localize_get_enabled_text();
char * localize_get_disabled_text();
char * localize_get_vibration_shakes_inhale_type_text();
char * localize_get_vibration_shakes_type_text();
char * localize_get_vibration_taps_type_text();
char * localize_get_top_text_greeting_type_text();
#if !PBL_PLATFORM_APLITE
char * localize_get_top_text_steps_type_text();
char * localize_get_top_text_heart_rate_type_text();
#endif
char * localize_get_reminder_frequency_text(int value);
char * localize_get_reminder_frequency_start_text();
#if !PBL_PLATFORM_APLITE
char * localize_get_app_glance_last_session_text();
char * localize_get_app_glance_daily_total_text();
#endif

// Row titles
char * localize_get_remember_duration_row_title();
char * localize_get_vibrations_row_title();
char * localize_get_breaths_per_minute_row_title();
#if PBL_PLATFORM_DIORITE || PBL_PLATFORM_EMERY
char * localize_get_heart_rate_variation_row_title();
#endif
char * localize_get_top_text_row_title();
char * localize_get_reminder_frequency_row_title();
char * localize_get_reminder_start_row_title();
#if !PBL_PLATFORM_APLITE
char * localize_get_app_glance_row_title();
#endif

#if !PBL_PLATFORM_APLITE
	char * localize_get_achievements_title();
	char * localize_get_achievement_row_title();
	char * localize_get_bottom_text_row_title();
	char * localize_get_bottom_text_total_type_text();
	char * localize_get_bottom_text_streak_type_text();
	char * localize_get_achievement_text();
	char * localize_get_streak_text(int streak_length);
 	char * localize_get_five_minutes_session_name();
	char * localize_get_minutes_session_description();
	char * localize_get_eight_minutes_session_name();
	char * localize_get_ten_minutes_session_name();
	char * localize_get_changed_settings_name();
	char * localize_get_changed_settings_description();
	char * localize_get_one_week_streak_name();
	char * localize_get_one_week_streak_description();
	char * localize_get_one_month_streak_name();
	char * localize_get_one_month_streak_description();
	char * localize_get_one_year_streak_name();
	char * localize_get_one_year_streak_description();
	char * localize_get_five_minutes_day_name();
	char * localize_get_minutes_day_description();
	char * localize_get_ten_minutes_day_name();
	char * localize_get_thirty_minutes_day_name();
	char * localize_get_one_hour_day_name();
	char * localize_get_one_hour_day_description();
	char * localize_get_completionist_name();
	char * localize_get_completionist_description();

	char * localize_get_achievements_section_title();
	char * localize_get_stats_section_title();
	
	char * localize_get_locked_title();
	char * localize_get_locked_description();	

	char * localize_get_total_breathed_name();
	char * localize_get_total_breathed_description();
	char * localize_get_longest_streak_name();
	char * localize_get_longest_streak_description(int days);
	char * localize_get_new_version_title();
	char * localize_get_new_version_description();
#endif