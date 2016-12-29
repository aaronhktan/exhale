#pragma once
#include "src/c/breathe_window.h"

#define MIN_BREATHED_TODAY_KEY 0
#define DATE_STORED_KEY 1
#define LAST_DURATION_KEY 2
#define WAKEUP_TIME_KEY 3
#define STREAK_DATE_KEY 4
#define STREAK_LENGTH_KEY 5

void data_init();

#if defined(PBL_HEALTH)
int data_get_current_steps();
void data_set_current_steps(int value);

int data_get_current_heart_rate();
void data_set_current_heart_rate(int value);

void data_set_heart_rate_period(int val);

void data_update_steps_buffer();
void data_update_heart_rate_buffer();

char* data_get_current_steps_buffer();
char* data_get_current_heart_rate_buffer();
#endif

char* data_get_date_today();
void data_write_breathe_persist_data(int min_to_breathe);
void data_write_date_persist_data();
int data_read_breathe_persist_data();
void data_write_last_duration_data(int last_duration);
int data_read_last_duration_data();
void data_write_wakeup_time_data(char* wakeup_time);
char* data_read_wakeup_time_data();

void data_set_streak_date_persist_data();
void data_calculate_streak_length();
char* data_get_streak_buffer();
int data_get_streak_length();