#pragma once

#include <pebble.h>

#define NEW_VERSION_WINDOW_TITLE "Version 2.2!"
#if !PBL_PLATFORM_CHALK
#define NEW_VERSION_WINDOW_MESSAGE "\nNEW FEATURES:\n\nLong press the down button to see achievements and stats.\nLeave a \u2764 on the Store if you like this app!"
#else
#define NEW_VERSION_WINDOW_MESSAGE "\n\nNEW FEATURES:\nLong press the down button to see achievements and stats.\nLeave a \u2764 on the Store if\nyou like this app!"
#endif
#define NEW_VERSION_WINDOW_MARGIN 10

#define SEEN_NEW_VERSION_KEY 30
#define SEEN_NEW_VERSION_NUMBER_KEY 31

void new_version_window_push();