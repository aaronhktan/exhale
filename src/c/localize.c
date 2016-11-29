#include <pebble.h>
#include "localize.h"

const char * localize_get_locale() {
	return i18n_get_system_locale();
}

char * localize_get_app_glance_text(int minutes) {
	switch (minutes) {
		case 1:
			if (strncmp(localize_get_locale(), "fr", 2) == 0) {
				return "Dernière session: %d minute. Respirez maintenant!";
			} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
				return "Última sesión: %d minuto. ¡Respira ahora!";
			} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
				return "Letzte Sitzung: %d Minute. Atme jetzt!";
			} else {
				return "Last session: %d minute. Breathe now!";
			}
		default:
			if (strncmp(localize_get_locale(), "fr", 2) == 0) {
				return "Dernière session: %d minutes. Respirez maintenant!";
			} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
				return "Última sesión: %d minutos. ¡Respira ahora!";
			} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
				return "Letzte Sitzung: %d Minuten. Atme jetzt!";
			} else {
				return "Last session: %d minutes. Breathe now!";
			}
	}
}

char * localize_get_breathe_text() {
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		return "RESPIRER";
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		return "RESPIRAR";
	} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
		return "ATME";
	} else {
		return "BREATHE";
	}
}

char * localize_get_well_done_text() {
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		return "Bien fait.";
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		return "Bien hecho.";
	} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
		return "Gut gemacht.";
	} else {
		return "Well done.";
	}
}

char * localize_get_inhale_text() {
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		return "INHALEZ...";
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		return "AHORA INHALA...";
	} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
		return "JETZT EINATMEN...";
	} else {
		return "NOW INHALE...";
	}
}

char * localize_get_exhale_text() {
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		return "...ET EXHALEZ.";
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		return "...Y EXHALA.";
	} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
		return "...UND AUSATMEN.";
	} else {
		return "...AND EXHALE.";
	}
}

char * localize_get_min_breathed_today_text() {
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		#ifdef PBL_ROUND
			return "AUJ.: %d MIN";
		#else
			return "AUJOURD'HUI: %d MIN";
		#endif
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		return "HOY: %d MIN";
	} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
		return "HEUTE: %d MIN";
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
		} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
			#ifdef PBL_ROUND
				return "%d %03d HEUTE";
			#else
				return "%d %03d SCHRITTE HEUTE";
			#endif
		} else {
			#ifdef PBL_ROUND
				return "%d,%03d TODAY";
			#else
				return "%d,%03d STEPS TODAY";
			#endif
		}
	} else if (thousands > 0) { // There's just a thousands digit
		if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			#ifdef PBL_ROUND
				return "%d,%03d PAS AUJ.";
			#else
				return "%d,%03d PAS AUJOURD'HUI";
			#endif
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			return "%d,%03d PASOS HOY";
		} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
			return "%d %03d SCHRITTE HEUTE";
		} else {
			return "%d,%03d STEPS TODAY";
		}
	} else { // There isn't a thousands digit!
		if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			#ifdef PBL_ROUND
				return "%d PAS AUJ.";
			#else
				return "%d PAS AUJOURD'HUI";
			#endif
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			return "%d PASOS HOY";
		} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
			return "%d SCHRITTE HEUTE";
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
	} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
		return "%lu BPM";
	} else {
		return "%lu BPM";
	}
}

char * localize_get_reminder_action_menu_text() {
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		return "Respirer %d min";
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		return "Respirar %d min";
	} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
		return "Atme %d min";
	} else {
		return "Breathe %d min";
	}
}

char * localize_get_reminder_text() {
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		return "Respirez!";
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		return "¡Respira ahora!";
	} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
		return "Zeit zum Atmen!";
	} else {
		return "Time to breathe!";
	}
}

char * localize_get_greet_text() {
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		return "BONJOUR!";
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		return "¡HOLA!";
	} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
		return "HALLO!";
	} else {
		return "HELLO!";
	}
}