#pragma once

const char * localize_get_locale();

char * localize_get_app_glance_text(int type, int minutes); 
char * localize_get_breathe_text();
char * localize_get_well_done_text();
char * localize_get_inhale_text();
char * localize_get_exhale_text();
char * localize_get_min_breathed_today_text();
char * localize_get_steps_today_text(int thousands);
char * localize_get_heart_rate_text();
char * localize_get_reminder_action_menu_text();
char * localize_get_reminder_text();
char * localize_get_greet_text();
char * localize_get_snooze_text();

#if !PBL_PLATFORM_APLITE
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
	char * localize_get_longest_streak_description();
#endif