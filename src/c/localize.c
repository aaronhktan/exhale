#include <pebble.h>
#include "localize.h"

const char * localize_get_locale() {
	return i18n_get_system_locale();
}

char * localize_get_app_glance_text() {
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			return "La journée plus récente: %d minutes. Respirez maintenant!";
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			return "El día más reciente: %d minutos. ¡Respira ahora!";
		} else {
			return "Most recent day: %d minutes. Breathe now!";
	}
}

char * localize_get_breathe_text() {
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		return "RESPIRER";
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		return "RESPIRAR";
	} else {
		return "BREATHE";
	}
}

char * localize_get_well_done_text() {
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		return "Bien fait.";
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		return "Bien hecho.";
	} else {
		return "Well done.";
	}
}

char * localize_get_inhale_text() {
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		return "INHALEZ";
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		return "AHORA INHALA...";
	} else {
		return "NOW INHALE...";
	}
}

char * localize_get_exhale_text() {
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		return "...ET EXHALEZ.";
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		return "...Y EXHALA.";
	} else {
		return "AND EXHALE.";
	}
}

char * localize_get_hello_text() {
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		return "ALLO!";
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		return "¡HOLA!";
	} else {
		return "HELLO.";
	}
}

char * localize_get_min_breathed_today_text() {
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		return "AUJOURD'HUI: %d MIN";
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		return "HOY: %d MIN";
	} else {
		return "TODAY: %d MIN";
	}
}

char * localize_get_steps_today_text(int thousands) {
	if(thousands >= 10) { // There's a ten thousands digit!
		if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			return "%d,%03d PAS AUJ.";
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			return "%d,%03d PASOS HOY";
		} else {
			return "%d,%03d STEPS TODAY";
		}
	} else if (thousands > 0) { // There's just a thousands digit
		if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			return "%d,%03d PAS AUJOURD'HUI";
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			return "%d,%03d PASOS HOY";
		} else {
			return "%d,%03d STEPS TODAY";
		}
	} else { // There isn't a thousands digit!
		if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			return "%d PAS AUJOURD'HUI";
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			return "%d PASOS HOY";
		} else {
			return "%d STEPS TODAY";
		}
  }
}

char * localize_get_heart_rate_text() {
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		return "%lu BPM";
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		return "%lu LPM";
	} else {
		return "%lu BPM";
	}
}

char * localize_get_reminder_action_menu_text() {
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		return "Respirer %d min";
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		return "Respirar %d min";
	} else {
		return "Breathe %d min";
	}
}

char * localize_get_reminder_text() {
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		return "Respirez!";
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		return "¡Respira ahora!";
	} else {
		return "Time to breathe!";
	}
}