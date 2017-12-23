#include <pebble.h>
#include "achievement.h"
#include "src/c/data.h"

AchievementList achievements;
int achievements_version = 1; // The current achievement version

// Load the default settings for achievement, and read if there are already achievements
void achievement_init() {
	achievements.one_week_streak.complete = 0;
	achievements.one_month_streak.complete = 0;
	achievements.one_year_streak.complete = 0;
	achievements.five_minutes_day.complete = 0;
	achievements.ten_minutes_day.complete = 0;
	achievements.thirty_minutes_day.complete = 0;
	achievements.one_hour_day.complete = 0;
	achievements.five_minutes_session.complete = 0;
	achievements.eight_minutes_session.complete = 0;
	achievements.ten_minutes_session.complete = 0;
	achievements.changed_settings.complete = 0;
	achievements.completionist.complete = 0;
	if (persist_exists(ACHIEVEMENT_KEY)) {
		persist_read_data(ACHIEVEMENT_KEY, &achievements, sizeof(achievements));
	}
	persist_write_int(ACHIEVEMENT_VERSION_KEY, 1);
}

// Save achievements
void achievement_save_achievements() {
	persist_write_data(ACHIEVEMENT_KEY, &achievements, sizeof(achievements));
	persist_write_int(ACHIEVEMENT_VERSION_KEY, 1);
}

// Getters and setters for each achievement in the AchievementList Struct
Achievement achievement_get_one_week_streak() {
	return achievements.one_week_streak;
}

void achievement_set_one_week_streak(char *date_completed, int complete) {
	achievements.one_week_streak.date_completed = date_completed;
	achievements.one_week_streak.complete = complete;
	achievement_save_achievements();
}

Achievement achievement_get_one_month_streak() {
	return achievements.one_month_streak;
}

void achievement_set_one_month_streak(char *date_completed, int complete) {
	achievements.one_month_streak.date_completed = date_completed;
	achievements.one_month_streak.complete = complete;
	achievement_save_achievements();
}

Achievement achievement_get_one_year_streak() {
	return achievements.one_year_streak;
}

void achievement_set_one_year_streak(char *date_completed, int complete) {
	achievements.one_year_streak.date_completed = date_completed;
	achievements.one_year_streak.complete = complete;
	achievement_save_achievements();
}

Achievement achievement_get_five_minutes_day() {
	return achievements.five_minutes_day;
}

void achievement_set_five_minutes_day(char *date_completed, int complete) {
	achievements.five_minutes_day.date_completed = date_completed;
	achievements.five_minutes_day.complete = complete;
	achievement_save_achievements();
}

Achievement achievement_get_ten_minutes_day() {
	return achievements.ten_minutes_day;
}

void achievement_set_ten_minutes_day(char *date_completed, int complete) {
	achievements.ten_minutes_day.date_completed = date_completed;
	achievements.ten_minutes_day.complete = complete;
	achievement_save_achievements();
}

Achievement achievement_get_thirty_minutes_day() {
	return achievements.thirty_minutes_day;
}

void achievement_set_thirty_minutes_day(char *date_completed, int complete) {
	achievements.thirty_minutes_day.date_completed = date_completed;
	achievements.thirty_minutes_day.complete = complete;
	achievement_save_achievements();
}

Achievement achievement_get_one_hour_day() {
	return achievements.one_hour_day;
}

void achievement_set_one_hour_day(char *date_completed, int complete) {
	achievements.one_hour_day.date_completed = date_completed;
	achievements.one_hour_day.complete = complete;
	achievement_save_achievements();
}

Achievement achievement_get_five_minutes_session() {
	return achievements.five_minutes_session;
}

void achievement_set_five_minutes_session(char *date_completed, int complete) {
	achievements.five_minutes_session.date_completed = date_completed;
	achievements.five_minutes_session.complete = complete;
	achievement_save_achievements();
}

Achievement achievement_get_eight_minutes_session() {
	return achievements.eight_minutes_session;
}

void achievement_set_eight_minutes_session(char *date_completed, int complete) {
	achievements.eight_minutes_session.date_completed = date_completed;
	achievements.eight_minutes_session.complete = complete;
	achievement_save_achievements();
}

Achievement achievement_get_ten_minutes_session() {
	return achievements.ten_minutes_session;
}

void achievement_set_ten_minutes_session(char *date_completed, int complete) {
	achievements.ten_minutes_session.date_completed = date_completed;
	achievements.ten_minutes_session.complete = complete;
	achievement_save_achievements();
}

Achievement achievement_get_changed_settings() {
	return achievements.changed_settings;
}

void achievement_set_changed_settings(char *date_completed, int complete) {
	achievements.changed_settings.date_completed = date_completed;
	achievements.changed_settings.complete = complete;
	achievement_save_achievements();
}

Achievement achievement_get_completionist() {
	return achievements.completionist;
}

void achievement_set_completionist(char *date_completed, int complete) {
	achievements.completionist.date_completed = date_completed;
	achievements.completionist.complete = complete;
	achievement_save_achievements();
}

bool achievement_get_all_complete() {
	if (achievements.five_minutes_session.complete == 1 && achievements.eight_minutes_session.complete == 1 && achievements.ten_minutes_session.complete == 1 && 
		 achievements.changed_settings.complete == 1 && achievements.one_week_streak.complete == 1 && achievements.one_month_streak.complete == 1 &&
		 achievements.one_year_streak.complete == 1 && achievements.five_minutes_day.complete == 1 && achievements.ten_minutes_day.complete == 1 &&
		 achievements.thirty_minutes_day.complete == 1 && achievements.one_hour_day.complete == 1) {
		return true;
	} else {
		return false;
	}
}

void achievement_request_achievements() {
	DictionaryIterator *iter;
	
	app_message_outbox_begin(& iter);
	
	int request_settings = 1;
	dict_write_int(iter, MESSAGE_KEY_requestAchievements, &request_settings, 1, false);
	
	dict_write_end(iter);
	
	app_message_outbox_send();
}

void achievement_send_achievements() {
	// First, create dictionary with all the achievements

	// Iterator variable, keeps the state of the creation serialization process:
	DictionaryIterator *iter;
	
	// Start AppMessage
	app_message_outbox_begin(&iter);

	#if !PBL_PLATFORM_APLITE
	int longest_streak = data_get_longest_streak();
	int total_breathed = data_get_total_minutes_breathed();
	int last_breathed = data_get_streak_date_persist_data();
	int streak_length = data_get_streak_length();
	#endif
	int one_week_complete = achievements.one_week_streak.complete;
	int one_month_complete = achievements.one_month_streak.complete;
	int one_year_complete = achievements.one_year_streak.complete;
	int five_minutes_day_complete = achievements.five_minutes_day.complete;
	int ten_minutes_day_complete = achievements.ten_minutes_day.complete;
	int thirty_minutes_day_complete = achievements.thirty_minutes_day.complete;
	int one_hour_day_complete = achievements.one_hour_day.complete;
	int five_minutes_session_complete = achievements.five_minutes_session.complete;
	int eight_minutes_session_complete = achievements.eight_minutes_session.complete;
	int ten_minutes_session_complete = achievements.ten_minutes_session.complete;
	int changed_settings_complete = achievements.changed_settings.complete;
	int completionist_complete = achievements.completionist.complete;
	
	// Write the achievements:
	#if !PBL_PLATFORM_APLITE
	dict_write_int(iter, 0, &longest_streak, sizeof(longest_streak), false);
	dict_write_int(iter, 1, &total_breathed, sizeof(longest_streak), false);
	dict_write_int(iter, 2, &last_breathed, sizeof(last_breathed), false);
	dict_write_int(iter, 3, &streak_length, sizeof(streak_length), false);
	#endif
	dict_write_int(iter, 4, &one_week_complete, 1, false);
	dict_write_int(iter, 5, &one_month_complete, 1, false);
	dict_write_int(iter, 6, &one_year_complete, 1, false);
	dict_write_int(iter, 7, &five_minutes_day_complete, 1, false);
	dict_write_int(iter, 8, &ten_minutes_day_complete, 1, false);
	dict_write_int(iter, 9, &thirty_minutes_day_complete, 1, false);
	dict_write_int(iter, 10, &one_hour_day_complete, 1, false);
	dict_write_int(iter, 11, &five_minutes_session_complete, 1, false);
	dict_write_int(iter, 12, &eight_minutes_session_complete, 1, false);
	dict_write_int(iter, 13, &ten_minutes_session_complete, 1, false);
	dict_write_int(iter, 14, &changed_settings_complete, 1, false);
	dict_write_int(iter, 15, &completionist_complete, 1, false);
	
	// End:
	dict_write_end(iter);
	
	// Send to phone
	app_message_outbox_send();
	
	APP_LOG(APP_LOG_LEVEL_DEBUG, "The AppMessage containing the achievements has been sent.");
}

void achievement_handle_achievements(DictionaryIterator *iter, void *context) {
	#if !PBL_PLATFORM_APLITE
	Tuple *longest_streak_t = dict_find(iter, 0);
	if (longest_streak_t) {
		data_set_longest_streak(longest_streak_t->value->int32);
	}
	
	Tuple *total_breathed_t = dict_find(iter, 1);
	if (total_breathed_t) {
		data_set_total_minutes_breathed(total_breathed_t->value->int32);
	}
	
	Tuple *last_breathed_t = dict_find(iter, 2);
	if (last_breathed_t) {
		persist_write_int(STREAK_DATE_KEY, last_breathed_t->value->uint32);
	}
	
	Tuple *streak_length_t = dict_find(iter, 3);
	if (streak_length_t) {
		persist_write_int(STREAK_LENGTH_KEY, streak_length_t->value->int32);
	}
	#endif
	
	Tuple *one_week_complete_t = dict_find(iter, 4);
	if (one_week_complete_t) {
		achievements.one_week_streak.complete = one_week_complete_t->value->int32;
	}
	
	Tuple *one_month_complete_t = dict_find(iter, 5);
	if (one_month_complete_t) {
		achievements.one_month_streak.complete = one_month_complete_t->value->int32;
	}
	
	Tuple *one_year_complete_t = dict_find(iter, 6);
	if (one_year_complete_t) {
		achievements.one_year_streak.complete = one_year_complete_t->value->int32;
	}

	Tuple *five_minutes_day_complete_t = dict_find(iter, 7);
	if (five_minutes_day_complete_t) {
		achievements.five_minutes_day.complete = five_minutes_day_complete_t->value->int32;
	}
	
	Tuple *ten_minutes_day_complete_t = dict_find(iter, 8);
	if (ten_minutes_day_complete_t) {
		achievements.ten_minutes_day.complete = ten_minutes_day_complete_t->value->int32;
	}

	Tuple *thirty_minutes_day_complete_t = dict_find(iter, 9);
	if (thirty_minutes_day_complete_t) {
		achievements.thirty_minutes_day.complete = thirty_minutes_day_complete_t->value->int32;
	}
	
	Tuple *one_hour_day_complete_t = dict_find(iter, 10);
	if (one_hour_day_complete_t) {
		achievements.one_hour_day.complete = one_hour_day_complete_t->value->int32;
	}
	
	Tuple *five_minutes_session_complete_t = dict_find(iter, 11);
	if (five_minutes_session_complete_t) {
		achievements.five_minutes_session.complete = five_minutes_session_complete_t->value->int32;
	}
	
	Tuple *eight_minutes_session_complete_t = dict_find(iter, 12);
	if (eight_minutes_session_complete_t) {
		achievements.eight_minutes_session.complete = eight_minutes_session_complete_t->value->int32;
	}
	
	Tuple *ten_minutes_session_complete_t = dict_find(iter, 13);
	if (ten_minutes_session_complete_t) {
		achievements.ten_minutes_session.complete = ten_minutes_session_complete_t->value->int32;
	}
	
	Tuple *changed_settings_complete_t = dict_find(iter, 14);
	if (changed_settings_complete_t) {
		achievements.changed_settings.complete = changed_settings_complete_t->value->int32;
	}
	
	Tuple *completionist_complete_t = dict_find(iter, 15);
	if (completionist_complete_t) {
		achievements.completionist.complete = completionist_complete_t->value->int32;
	}
	
	achievement_save_achievements();
}