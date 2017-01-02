#if !PBL_PLATFORM_APLITE
#pragma once

#include <pebble.h>

#define NEW_VERSION_WINDOW_MARGIN 10

#define SEEN_NEW_VERSION_KEY 30
#define SEEN_NEW_VERSION_NUMBER_KEY 31

void new_version_window_push();
#endif