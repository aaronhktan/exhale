#pragma once
#include "src/c/breathe_window.h"

void data_init();

void data_set_current_steps(int value);
int data_get_current_steps();

void data_update_steps_buffer();

char* data_get_current_steps_buffer();