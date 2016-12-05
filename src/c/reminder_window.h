#pragma once

#ifdef PBL_PLATFORM_EMERY
	#define FONT_KEY FONT_KEY_GOTHIC_28_BOLD
#elif PBL_PLATFORM_CHALK
	#define FONT_KEY FONT_KEY_GOTHIC_18_BOLD
#else
	#define FONT_KEY FONT_KEY_GOTHIC_24_BOLD
#endif

#define SNOOZE_WAKEUP 44

void reminder_window_push();