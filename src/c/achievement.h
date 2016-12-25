#pragma once

#define ACHIEVEMENT_KEY 10
#define ACHIEVEMENT_VERSION_KEY 11

// Struct for each achievement, with completion status and date complete
typedef struct Achievement {
	char *date_completed;
	int complete;
} Achievement;

// Struct for list of the achievements
typedef struct AchievementList {
	Achievement one_week_streak;
	Achievement one_month_streak;
	Achievement one_year_streak;
	Achievement five_minutes_day;
	Achievement ten_minutes_day;
	Achievement thirty_minutes_day;
	Achievement one_hour_day;
	Achievement five_minutes_session;
	Achievement eight_minutes_session;
	Achievement ten_minutes_session;
	Achievement changed_settings;
	Achievement completionist;
} AchievementList;

void achievement_init();

Achievement achievement_get_one_week_streak();
void achievement_set_one_week_streak(char *date_completed, int complete);

Achievement achievement_get_one_month_streak();
void achievement_set_one_month_streak(char *date_completed, int complete);

Achievement achievement_get_one_year_streak();
void achievement_set_one_year_streak(char *date_completed, int complete);

Achievement achievement_get_five_minutes_day();
void achievement_set_five_minutes_day(char *date_completed, int complete);

Achievement achievement_get_ten_minutes_day();
void achievement_set_ten_minutes_day(char *date_completed, int complete);

Achievement achievement_get_thirty_minutes_day();
void achievement_set_thirty_minutes_day(char *date_completed, int complete);

Achievement achievement_get_one_hour_day();
void achievement_set_one_hour_day(char *date_completed, int complete);

Achievement achievement_get_five_minutes_session();
void achievement_set_five_minutes_session(char *date_completed, int complete);

Achievement achievement_get_eight_minutes_session();
void achievement_set_eight_minutes_session(char *date_completed, int complete);

Achievement achievement_get_ten_minutes_session();
void achievement_set_ten_minutes_session(char *date_completed, int complete);

Achievement achievement_get_completionist();
void achievement_set_completionist(char *date_completed, int complete);