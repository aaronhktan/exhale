#include <pebble.h>
#include "achievement.h"

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
	if (persist_exists(ACHIEVEMENT_KEY)){
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