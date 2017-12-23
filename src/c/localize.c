#include <pebble.h>
#include "localize.h"
#include "src/c/settings.h"
#include "src/c/data.h"

const char* localize_get_locale() {
	return i18n_get_system_locale();
}

/************************************************************************** App Glance */
#if !PBL_PLATFORM_APLITE
// App Glance Text
char* localize_get_app_glance_text(int type, int minutes) {
	switch (type) {
		case 0: // Last session
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
		case 2: // Streak
			switch (minutes) {
				case 1:
					if (strncmp(localize_get_locale(), "fr", 2) == 0) {
						return "%d jour de suite. Respirez maintenant!";
					} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
						return "%d día seguido. ¡Respira ahora!";
					} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
						return "%d Tag in Folge. Atme jetzt!";
					} else {
						return "%d day in a row. Breathe now!";
					}
				default:
					if (strncmp(localize_get_locale(), "fr", 2) == 0) {
						return "%d jours de suite. Respirez maintenant!";
					} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
						return "%d días seguidos. ¡Respira ahora!";
					} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
						return "%d Tage in Folge. Atme jetzt!";
					} else {
						return "%d days in a row. Breathe now!";
					}
			}
		default: // Total Today
			switch (minutes) {
				case 1:
				if (strncmp(localize_get_locale(), "fr", 2) == 0) {
					return "Aujourd'hui: %d minute. Respirez maintenant!";
				} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
					return "Hoy: %d minuto. ¡Respira ahora!";
				} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
					return "Heute: %d Minute. Atme jetzt!";
				} else {
					return "Today: %d minute. Breathe now!";
				}
			default:
				if (strncmp(localize_get_locale(), "fr", 2) == 0) {
					return "Aujourd'hui: %d minutes. Respirez maintenant!";
				} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
					return "Hoy: %d minutos. ¡Respira ahora!";
				} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
					return "Heute: %d Minuten. Atme jetzt!";
				} else {
					return "Today: %d minutes. Breathe now!";
				}
			}
	}
}
#endif

/************************************************************************** Main app text */
char* localize_get_breathe_text() {
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

char* localize_get_well_done_text() {
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

char* localize_get_inhale_text() {
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

char* localize_get_exhale_text() {
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

char* localize_get_min_breathed_today_text() {
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

#if !PBL_PLATFORM_APLITE
char* localize_get_steps_today_text(int thousands) {
	if (settings_get_heartRateVariation() && data_get_current_heart_rate() != 0) {
		if(thousands >= 10) { // There's a ten thousands digit!
			if (strncmp(localize_get_locale(), "fr", 2) == 0) {
				return "\u2764 %d.%03d PAS AUJ.";
			} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
				return "\u2764 %d.%03d PASOS HOY";
			} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
				return "\u2764 %d.%03d HEUTE";
			} else { // Yes there isn't a heart rate enabled round pebble right now, but who knows?
				#ifdef PBL_ROUND
					return "\u2764 %d,%03d TODAY";
				#else
					return "\u2764 %d,%03d STEPS TODAY";
				#endif
			}
		} else if (thousands > 0) { // There's just one thousands digit
			if (strncmp(localize_get_locale(), "fr", 2) == 0) {
				return "\u2764 %d.%03d PAS AUJ.";
			} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
				return "\u2764 %d.%03d PASOS HOY";
			} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
				#ifdef PBL_ROUND
					return "\u2764 %d.%03d HEUTE";
				#else
					return "\u2764 %d.%03d SCHRITTE HEUTE";
				#endif
			} else {
				return "\u2764 %d,%03d STEPS TODAY";
			}
		} else { // There isn't a thousands digit!
			if (strncmp(localize_get_locale(), "fr", 2) == 0) {
				return "\u2764 %d PAS AUJ.";
			} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
				return "\u2764 %d PASOS HOY";
			} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
				#ifdef PBL_ROUND
					return "\u2764 %d HEUTE";
				#else
					return "\u2764 %d SCHRITTE HEUTE";
				#endif
			} else {
				return "\u2764 %d STEPS TODAY";
			}
		}
	} else {
		if(thousands >= 10) { // There's a ten thousands digit!
			if (strncmp(localize_get_locale(), "fr", 2) == 0) {
				return "%d.%03d PAS AUJ.";
			} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
				return "%d.%03d PASOS HOY";
			} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
				return "%d.%03d HEUTE";
			} else {
				#ifdef PBL_ROUND
					return "%d,%03d TODAY";
				#else
					return "%d,%03d STEPS TODAY";
				#endif
			}
		} else if (thousands > 0) { // There's just one thousands digit
			if (strncmp(localize_get_locale(), "fr", 2) == 0) {
				#ifdef PBL_ROUND
					return "%d.%03d PAS AUJ.";
				#else
					return "%d.%03d PAS AUJOURD'HUI";
				#endif
			} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
				return "%d.%03d PASOS HOY";
			} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
				#ifdef PBL_ROUND
					return "%d.%03d HEUTE";
				#else
					return "%d.%03d SCHRITTE HEUTE";
				#endif
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
				#ifdef PBL_ROUND
					return "%d HEUTE";
				#else
					return "%d SCHRITTE HEUTE";
				#endif
			} else {
				return "%d STEPS TODAY";
			}
		}
	}
}

char* localize_get_heart_rate_text() {
	#if defined(PBL_HEALTH)
	if (settings_get_heartRateVariation() && data_get_current_heart_rate() != 0) {
		if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			return "\u2764 %lu BPM";
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			return "\u2764 %lu LPM";
		} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
			return "\u2764 %lu BPM";
		} else {
			return "\u2764 %lu BPM";
		}
	} else {
	#endif
		if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			return "%lu BPM";
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			return "%lu LPM";
		} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
			return "%lu BPM";
		} else {
			return "%lu BPM";
		}
	#if defined(PBL_HEALTH)
	}
	#endif
}
#endif

char* localize_get_greet_text() {
	#if defined(PBL_HEALTH)
	if (settings_get_heartRateVariation() && data_get_current_heart_rate() != 0) {
		if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			return "\u2764 BONJOUR!";
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			return "\u2764 ¡HOLA!";
		} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
			return "\u2764 HALLO!";
		} else {
			return "\u2764 HELLO!";
		}
	} else {
	#endif
		if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			return "BONJOUR!";
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			return "¡HOLA!";
		} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
			return "HALLO!";
		} else {
			return "HELLO!";
		}
	#if defined(PBL_HEALTH)
	}
	#endif
}

// Sets strings as English, change if watch is set to another language
char* localize_get_top_text(int random_number) {
	char*strings[9] = {"TAKE A MOMENT;", "BE STILL;", "CLEAR YOUR MIND;", "EMPTY YOUR THOUGHTS;", "BE CALM;", "THINK NOTHING;", "RELAX;", "CHILL FOR A SEC;", "SPACE OUT;"};
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		char*french_strings[9] = {"PRENEZ UN MOMENT;", "RÉFLECHISSEZ;", "OUVREZ VOTRE ESPRIT;", "NE PENSEZ À RIEN;", "SOYEZ CALME;", "CONCENTREZ;", "RELAXEZ;", "NE VOUS INQUIETEZ PAS;", "DONNEZ-VOUS DE L'ESPACE;"};
		for (int i = 0; i <= 8; i++) {
			strings[i] = french_strings[i];
		}
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		char*spanish_strings[9] = {"TÓMATE EL TIEMPO;", "NO TE MUEVAS;", "DESPÉJATE;", "NO PIENSA A NADA;", "TRANQUILÍZATE;", "CONCÉNTRATE;", "RELÁJATE;", "NO TE PREOCUPES;", "TOMA UN MOMENTO;"};
		for (int i = 0; i <= 8; i++) {
			strings[i] = spanish_strings[i];
		}
	} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
		char*german_strings[9] = {"NIMM DIR ZEIT;", "STEHE STILL;", "LEERE DEINEN GEIST;", "DENKE AN NICHTS;", "WERDE RUHIG;", "EXISTIERE EINFACH;", "MACH'S DIR GEMÜTLICH;", "SPÜRE DEINE UMGEBUNG;", "KOMM RUNTER;"};
		for (int i = 0; i <= 8; i++) {
			strings[i] = german_strings[i];
		}
	}
	return strings[random_number];
}

// Same thing as above but for bottom text
char* localize_get_bottom_text(int random_number) {
	char* strings[4] = {"BREATHE.", "EXHALE.", "CONCENTRATE.", "FOCUS."};
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		char* french_strings[4] = {"RESPIREZ.", "EXHALEZ.", "RESPIREZ.", "EXHALEZ."};
		for (int i = 0; i <= 3; i++) {
			strings[i] = french_strings[i];
		}	
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		char* spanish_strings[4] = {"RESPIRA.", "EXHALA.", "RESPIRA.", "EXHALA."};
		for (int i = 0; i <= 3; i++) {
			strings[i] = spanish_strings[i];
		}
	} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
		char* german_strings[4] = {"ATME.", "ATME AUS.", "KONZENTRIER DICH.", "FOKUSSIERE DICH."};
		for (int i = 0; i <= 3; i++) {
			strings[i] = german_strings[i];
		}
	}
	
	return strings[random_number];
}

/************************************************************************** Reminder window text */
char* localize_get_reminder_action_menu_text() {
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

char* localize_get_reminder_text() {
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

char* localize_get_snooze_text() {
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		return "Snooze pendant %d min";
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		return "Recuérdame en %d min";
	} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
		return "Schlummern %d min";
	} else {
		return "Snooze for %d min";
	}
}

/************************************************************************** Settings */
#if !PBL_PLATFORM_APLITE
/************************************************************* Section Titles */
char* localize_get_in_app_section_title() {
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		return "DANS L'APP";
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		return "APLICACIÓN";
	} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
		return "APP";
	} else {
		return "IN-APP";
	}
}

char* localize_get_health_section_title() {
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		return "SANTÉ";
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		return "SALUD";
	} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
		return "GESUNDHEIT";
	} else {
		return "HEALTH";
	}
}

char* localize_get_reminders_section_title() {
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		return "RAPPELS";
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		return "RECORDATORIOS";
	} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
		return "ERINNERUNGEN";
	} else {
		return "REMINDERS";
	}
}

char* localize_get_app_glance_section_title() {
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		return "APP GLANCE";
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		return "APP GLANCE";
	} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
		return "APP GLANCE";
	} else {
		return "APP GLANCE";
	}
}

char* localize_get_achievements_title() {
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		return "SUCCÈS";
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		return "LOGROS";
	} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
		return "ERFOLGE";
	} else {
		return "ACHIEVEMENTS";
	}
}

char* localize_get_about_section_title() {
	if (strncmp(localize_get_locale(), "en", 2) == 0) {
		return "ABOUT";
	} else {
		return "INFO";
	}
}

/************************************************************* Row Text */
char* localize_get_enabled_text() {
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		return "Activé";
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		return "Activado";
	} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
		return "Aktiviert";
	} else {
		return "Enabled";
	}	
}

char* localize_get_disabled_text() {
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		return "Désactivé";
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		return "Desactivado";
	} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
		return "Deaktiviert";
	} else {
		return "Disabled";
	}	
}

char* localize_get_vibration_shakes_inhale_type_text() {
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		return "Pulsations - inhalation";
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		return "Repentina - inhalación";
	} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
		return "Graduell - nur Einatmen";
	} else {
		return "Rumble - inhale only";
	}	
}

char* localize_get_vibration_shakes_type_text() {
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		return "Pulsations";
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		return "Repentina";
	} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
		return "Graduell";
	} else {
		return "Rumble";
	}	
}

char* localize_get_vibration_taps_type_text() {
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		return "Tapes";
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		return "Toques";
	} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
		return "Doppeltippen";
	} else {
		return "Pulses";
	}	
}

char* localize_get_top_text_greeting_type_text() {
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		return "Salut";
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		return "Saludo";
	} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
		return "Begrüßung";
	} else {
		return "Greeting";
	}	
}

char* localize_get_top_text_steps_type_text() {
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		return "# de pas";
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		return "# de pasos";
	} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
		return "Schritte";
	} else {
		return "Steps";
	}		
}

char* localize_get_top_text_heart_rate_type_text() {
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		return "Rhythme cardiaqueRitmo cardiaco";
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		return "Ritmo cardiaco";
	} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
		return "Herzfrequenz";
	} else {
		return "Heart rate";
	}		
}

char* localize_get_reminder_frequency_text(int value) {
	switch (value) {
		case 0:
			if (strncmp(localize_get_locale(), "fr", 2) == 0) {
				return "Jamais";
			} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
				return "Nunca";
			} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
				return "Nie";
			} else {
				return "Never";
			}
		case 1:
			if (strncmp(localize_get_locale(), "fr", 2) == 0) {
				return "Toutes les heures";
			} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
				return "Cada hora";
			} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
				return "Jede Stunde";
			} else {
				return "Every hour";
			}
		default:
			if (strncmp(localize_get_locale(), "fr", 2) == 0) {
				return "Toutes les %d heures";
			} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
				return "Cada %d horas";
			} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
				return "Alle %d Stunden";
			} else {
				return "Every %d hours";
			}
	}
}

char* localize_get_reminder_frequency_start_text() {
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		return "%d heures";
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		return "A las %d";
	} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
		return "%d Uhr";
	} else {
		return "%d o'clock";
	}	
}

char* localize_get_app_glance_last_session_text() {
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		return "Dernière session";
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		return "Última sesión";
	} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
		return "Letzte Sitzung";
	} else {
		return "Last session";
	}
}

char* localize_get_app_glance_daily_total_text() {
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		return "Total auj.";
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		return "Total hoy";
	} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
		return "Insgesamt heute";
	} else {
		return "Total today";
	}	
}

char* localize_get_bottom_text_total_type_text() {
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		return "Total aujourd'hui";
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		return "Total hoy";
	} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
		return "Ingesamt heute";
	} else {
		return "Total today";
	}	
}

char* localize_get_bottom_text_streak_type_text() {
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		return "Série";
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		return "Racha";
	} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
		return "Serie";
	} else {
		return "Streak";
	}	
}

/************************************************************* Settings row titles */
char* localize_get_remember_duration_row_title() {
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		return "Utiliser dur. préc.";
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		return "Usar dur. ant.";
	} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
		return "Dauer merken";
	} else {
		return "Use last duration";
	}	
}

char* localize_get_vibrations_row_title() {
	if (strncmp(localize_get_locale(), "es", 2) == 0) {
		return "Vibraciones";
	} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
		return "Vibrationen";
	} else { // French and English have the same spelling; not a mistake
		return "Vibrations";
	}		
}

char* localize_get_breaths_per_minute_row_title() {
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		return "Respirations/min.";
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		return "Respiraciones/min.";
	} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
		return "Atemzüge/Minute";
	} else {
		return "Breaths/minute";
	}
}

#if PBL_PLATFORM_DIORITE || PBL_PLATFORM_EMERY
char* localize_get_heart_rate_variation_row_title() {
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		return "Vitesse selon RC";
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		return "Rapidez según RC";
	} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
		return "nach Herzfrequenz";
	} else {
		return "HR Variation";
	}
}
#endif

char* localize_get_top_text_row_title() {
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		return "Texte en haut";
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		return "Texto superior";
	} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
		return "Oberer Text";
	} else {
		return "Top Text";
	}	
}

char* localize_get_reminder_frequency_row_title() {
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		return "Fréq. des rappels";
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		return "Frec. de rdo.";
	} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
		return "Erinnerungshäufigkeit";
	} else {
		return "Reminder Freq.";
	}			
}

char* localize_get_reminder_start_row_title() {
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		return "Début des rappels";
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		return "Empiezo de rdo.";
	} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
		return "Erste Erinnerung";
	} else {
		return "Reminder Start";
	}	
}

char* localize_get_app_glance_row_title() {
	return "App Glance";
}

char* localize_get_bottom_text_row_title() {
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		return "Texte en bas";
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		return "Texto abajo";
	} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
		return "Unterer Text";
	} else {
		return "Bottom Text";
	}		
}

char* localize_get_achievement_row_title() {
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		return "Succès";
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		return "Logros";
	} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
		return "Erfolge";
	} else {
		return "Achievements";
	}	
}

char* localize_get_version_row_title() {
	if (strncmp(localize_get_locale(), "es", 2) == 0) {
		return "Versión";
	} else {
		return "Version";
	}
}

char* localize_get_credits_row_title() {
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		return "Fait avec \u2764 par";
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		return "Hecho con \u2764 por";
	} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
		return "Code mit \u2764 von";
	} else {
		return "Made with \u2764 by";
	}	
}
#endif

/************************************************************************** Achievements */
#if !PBL_PLATFORM_APLITE
	char* localize_get_achievement_text() {
		if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			return "Succès!";
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			return "¡Logro!";
		} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
			return "Erfolg!";
		} else {
			return "Achievement!";
		}
	}

	char* localize_get_streak_text(int streak_length) {
		if (streak_length >= data_get_longest_streak() && data_get_today_epoch_time() == data_get_streak_date_persist_data()) { // This means that it's the longest streak and user has breathed today
			if (streak_length == 1) {
				if (strncmp(localize_get_locale(), "fr", 2) == 0) {
					return "\U0001F44D %d JOUR \U0001F389";
				} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
					return "\U0001F44D %d DÍA \U0001F389";
				} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
					return "\U0001F44D %d TAG \U0001F389";
				} else {
					return "\U0001F44D %d DAY \U0001F389";
				}
			} else {
				if (strncmp(localize_get_locale(), "fr", 2) == 0) {
					return "\U0001F44D %d JOURS \U0001F389";
				} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
					return "\U0001F44D %d DÍAS \U0001F389";
				} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
					return "\U0001F44D %d TAGE \U0001F389";
				} else {
					return "\U0001F44D %d DAYS \U0001F389";
				}
			}
		} else if (streak_length >= data_get_longest_streak() && data_get_today_epoch_time() != data_get_streak_date_persist_data()) { // This means that it's the longest streak but user has not breathed today
			if (streak_length == 1) {
				if (strncmp(localize_get_locale(), "fr", 2) == 0) {
					return "%d JOUR \U0001F389";
				} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
					return "%d DÍA \U0001F389";
				} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
					return "%d TAG \U0001F389";
				} else {
					return "%d DAY \U0001F389";
				}
			} else {
				if (strncmp(localize_get_locale(), "fr", 2) == 0) {
					return "%d JOURS \U0001F389";
				} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
					return "%d DÍAS \U0001F389";
				} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
					return "%d TAGE \U0001F389";
				} else {
					return "%d DAYS \U0001F389";
				}
			}
		} else if (data_get_today_epoch_time() == data_get_streak_date_persist_data()) { // This means that it's not the longest streak but the user has breathed today
			if (streak_length == 1) {
				if (strncmp(localize_get_locale(), "fr", 2) == 0) {
					return "\U0001F44D %d JOUR";
				} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
					return "\U0001F44D %d DÍA";
				} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
					return "\U0001F44D %d TAG";
				} else {
					return "\U0001F44D %d DAY";
				}
			} else {
				if (strncmp(localize_get_locale(), "fr", 2) == 0) {
					return "\U0001F44D %d JOURS";
				} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
					return "\U0001F44D %d DÍAS";
				} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
					return "\U0001F44D %d TAGE";
				} else {
					return "\U0001F44D %d DAYS";
				}
			}
		} else { // This means that it's not the longest streak and the user has not breathed today.
			if (streak_length == 1) {
				if (strncmp(localize_get_locale(), "fr", 2) == 0) {
					return "%d JOUR";
				} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
					return "%d DÍA";
				} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
					return "%d TAG";
				} else {
					return "%d DAY";
				}
			} else {
				if (strncmp(localize_get_locale(), "fr", 2) == 0) {
					return "%d JOURS";
				} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
					return "%d DÍAS";
				} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
					return "%d TAGE";
				} else {
					return "%d DAYS";
				}
			}			
		}
	}

	char* localize_get_five_minutes_session_name() {
		if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			return "PROFOND";
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			return "PROFUNDO";
		} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
			return "TIEFE ATEMZÜGE";
		} else {
			return "DEEP BREATHER";
		}
	}

	char* localize_get_minutes_session_description() {
		if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			return "Respirez %d min. pendant une session.";
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			return "Respira %d min. durante una sesión.";
		} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
			return "Atmen Sie %d Min. in einer Sitzung.";
		} else {
			return "Breathe %d min. in one session.";
		}
	}

	char* localize_get_eight_minutes_session_name() {
		if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			return "CHANCEUX";
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			return "AFORTUNADO";
		} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
			return "GLÜCKLICH";
		} else {
			return "LUCKY";
		}
	}

	char* localize_get_ten_minutes_session_name() {
		if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			return "DÉTENDEUR";
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			return "RELAJANTE";
		} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
			return "ENTSPANNTER";
		} else {
			return "RELAXER";
		}
	}

	char* localize_get_changed_settings_name() {
		if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			return "PERSONNALISATEUR";
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			return "PERSONALIZADOR";
		} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
			return "CUSTOMIZER";
		} else {
			return "CUSTOMIZER";
		}
	}

	char* localize_get_changed_settings_description() {
		if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			return "Changez les réglages pour la première fois.";
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			return "Cambia los ajustes para la primera vez.";
		} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
			return "Ändern Sie die Einstellungen zum ersten Mal.";
		} else {
			return "Change the settings for the first time.";
		}
	}

	char* localize_get_one_week_streak_name() {
		if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			return "CONSTANTE";
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			return "CONSTANTE";
		} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
			return "KONSTANTE";
		} else {
			return "CONSTANT";
		}
	}

	char* localize_get_one_week_streak_description() {
		if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			return "Respirez quotidiennement pour une semaine.";
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			return "Respira cada día durante una semana.";
		} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
			return "Atmen Sie einmal täglich für eine Woche.";
		} else {
			return "Breathe once every day for a week.";
		}
	}

	char* localize_get_one_month_streak_name() {
		if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			return "DÉTERMINÉ";
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			return "DETERMINADO";
		} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
			return "ENTSCHLOSSEN";
		} else {
			return "DETERMINED";
		}
	}

	char* localize_get_one_month_streak_description() {
		if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			return "Respirez quotidiennement pour un mois.";
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			return "Respira diariamente durante un mes.";
		} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
			return "Atmen Sie einmal täglich für eine Monat.";
		} else {
			return "Breathe once every day for a month.";
		}		
	}

	char* localize_get_one_year_streak_name() {
		if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			return "RÉSOLU";
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			return "RESUELTO";
		} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
			return "ENTSCHIEDEN";
		} else {
			return "RESOLUTE";
		}
	}

	char* localize_get_one_year_streak_description() {
		if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			return "Respirez quotidiennement pour une année.";
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			return "Respira diariamente durante un año.";
		} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
			return "Atmen Sie einmal täglich für eine Jahr.";
		} else {
			return "Breathe once every day for a year.";
		}		
	}

	char* localize_get_five_minutes_day_name() {
		if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			return "HABITUEL";
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			return "HABITUAL";
		} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
			return "GEWOHNHEIT";
		} else {
			return "HABITUAL";
		}		
	}

	char* localize_get_minutes_day_description() {
		if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			return "Respirez %d min. en une journée.";
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			return "Respira %d min. en un día.";
		} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
			return "Atmen Sie %d Min. an einem Tag.";
		} else {
			return "Breathe %d min. in one day.";
		}				
	}

	char* localize_get_ten_minutes_day_name() {
		if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			return "HABILE";
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			return "EXPERTO";
		} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
			return "GESCHULT";
		} else {
			return "TRAINED";
		}		
	}

	char* localize_get_thirty_minutes_day_name() {
		if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			return "DISCIPLINÉ";
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			return "DISCIPLINADO";
		} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
			return "DISZIPLINIERT";
		} else {
			return "DISCIPLINED";
		}
	}

	char* localize_get_one_hour_day_name() {
		if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			return "PRO";
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			return "PRO";
		} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
			return "PROFI";
		} else {
			return "PRO";
		}
	}

	char* localize_get_one_hour_day_description() {
		if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			return "Respirez 1 heure en une journée.";
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			return "Respira 1 hora en un día.";
		} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
			return "Atmen Sie 1 Stunde an einem Tag.";
		} else {
			return "Breathe 1 hour in one day.";
		}
	}

	char* localize_get_completionist_name() {
		if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			return "COMPLÉTEUR";
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			return "COMPLETANTE";
		} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
			return "VOLLENDUNG";
		} else {
			return "COMPLETIONIST";
		}
	}

	char* localize_get_completionist_description() {
		if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			return "Obtenez tous les succès.";
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			return "Obtén todos los logros.";
		} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
			return "Holen Sie sich alle Erfolge.";
		} else {
			return "Get all the achievements.";
		}
	}

	char* localize_get_achievements_section_title() {
		if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			return "SUCCÈS";
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			return "LOGROS";
		} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
			return "ERFOLGE";
		} else {
			return "ACHIEVEMENTS";
		}
	}

	char* localize_get_stats_section_title() {
		if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			return "STATISTIQUES";
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			return "ESTADÍSTICAS";
		} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
			return "STATISTIKEN";
		} else {
			return "STATS";
		}
	}

	char* localize_get_locked_title() {
		if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			return "VERROUILLÉ!";
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			return "¡CERRADO!";
		} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
			return "GESPERRT!";
		} else {
			return "LOCKED!";
		}
	}

	char* localize_get_locked_description() {
		if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			return "Respirez plus!";
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			return "¡Respira más!";
		} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
			return "Atme weiter!";
		} else {
			return "Keep breathing!";
		}
	}

	char* localize_get_total_breathed_name() {
		if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			return "Total respiré";
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			return "Total respirado";
		} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
			return "Insgesamt";
		} else {
			return "Total breathed";
		}
	}

	char* localize_get_total_breathed_description() {
		if (strncmp(localize_get_locale(), "de", 2) == 0) {
			return "%d Min.";
		} else {
			return "%d min.";
		}
	}

	char* localize_get_longest_streak_name() {
		if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			return "Série la plus longue";
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			return "Racha más larga";
		} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
			return "Längste Serie";
		} else {
			return "Longest streak";
		}
	}

	char* localize_get_longest_streak_description(int days) {
		if (days != 1) {
			if (strncmp(localize_get_locale(), "fr", 2) == 0) {
				return "%d jours de suite";
			} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
				return "%d días seguidos";
			} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
				return "%d Tage in Folge";
			} else {
				return "%d days in a row";
			}
		} else {
			if (strncmp(localize_get_locale(), "fr", 2) == 0) {
				return "%d jour";
			} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
				return "%d día";
			} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
				return "%d Tag";
			} else {
				return "%d day";
			}
		}
	}

	char* localize_get_new_version_title() {
		if (strncmp(localize_get_locale(), "es", 2) == 0) {
			return "¡Versión 2.52!";
		} else {
			return "Version 2.52!";
		}
	}

	char* localize_get_new_user_title() {
		if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			return "Bienvenue!";
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			return "¡Bienvenido!";
		} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
			return "Willkommen!";
		} else {
			return "WELCOME!";
		}
	}

	char* localize_get_new_version_description() {
		if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			return "NOUVEAUTÉS:\n\n• Vos succès sont maintenant copiés sur votre célullaire, et se restaurent automatiquement si vous supprimez puis réinstallez cette app.\n• Le montant minimum de respirations par minute a diminué à 2.\n• Si vous voulez revoir ces notes, sélectionnez la version dans le menu des paramètres sur votre montre.\n• Corrigé plusieurs plantages.\n\nLaissez un \u2764 sur l'App Store si vous aimez cette app!";
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			return "NUEVO:\n\n• Ahora se realiza una copia de seguridad de todos tus logros, y estos se restauran automáticamente si eliminas y luego reinstalas esta app.\n• El número mínimo de respiraciones por minuto ha bajado a 2.\n• Si quieres ver de nuevo estas notas, selecciona el número de versión en el menú de ajustes en tu reloj.\n• Varios fallos de programa fueron eliminados.\n\n¡Da un \u2764 en el App Store si a tí te gusta esta app!";
		} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
			return "NEU:\n\n• Ihre Erfolge werden nun auf dem Handy gespeichert, sodass sie bei einer Neuinstallation erhalten bleiben.\n• Die Anzahl Atemzüge pro Minute kann jetzt minimal 2 sein\n• Die Änderungen können erneut eingesehen werden, wenn Sie die Versionsnummer in den Einstellungen auf der Uhr anwählen\n• Mehrere Crashes und Speicherlecks behoben, darunter einer nach einer Erinnerung\n• Hinterlassen Sie ein \u2764 im App Store, wenn Ihnen diese App gefällt!";
		} else {
			return "NEW FEATURES:\n\n• Your achievements are now backed up to your phone, and will restore if you delete and redownload this app!\n• Minimum breaths per minute decreased to 2.\n• If you ever want to check these update notes again, just click the version number in the settings menu on your watch.\n• Fixed many crashes and memory leaks, including the one after a reminder.\n\nLeave a \u2764 on the Store if you like this app!";
		}
	}

	char* localize_get_new_user_description() {
		if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			return "Prenez un moment pour respirer.\n\nRESPIREZ.\nChoississez la durée de votre session de respiration avec les boutons haut et bas. Quand vous êtes prêt à commencer, appuyez le bouton du milieu.\n\nPERSONNALISEZ.\nChangez vos paramètres directement sur votre montre ou sur votre céllulaire! Appuyez longuement sur le bouton haut pour les changer sur votre montre.\n\nACHEVEZ.\nAu fur et à mesure que vous utilisez la app, vous gagnerez des succès! Admirez-les en appuyant longuement sur le bouton bas.";
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			return "Tómate un momento para respirar.\n\nRESPIRA.\nEscoge cuántos minutes quieres que dure la sesión con pulsaciones de los botónes arriba y abajo. Cuando estés listo para comenzar, presiona el botón central.\n\nPERSONALIZA.\nCambia los ajustes en tu reloj o en tu móvil. Con una pulsación larga del botón arriba puedes cambiar ajustes en tu reloj.\n\nALCANZA.\n¡Con la respiración regular ganarás logros! Se puede verlos con una pulsación larga del botón abajo.";
		} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
			return "Ein Moment zum Durchatmen.\n\nBREATHE.\nIm Hauptmenü können Sie die Dauer der Atemübung mit dem oberen und dem unteren Knopf. Starten Sie die Übung mit dem mittleren Knopf.\nSie können Einstellungen auf dem Telefon und auf der Uhr vornehmnen. Drücken Sie den oberen Knopf lange, um die Einstellungen auf der Uhr zu öffnen.\n\nERFOLGE.\nWenn Sie Übungen machen, können Sie Erfolge erreichen und Statistiken sehen! Drücken und halten Sie den unteren Knopf um zu sehen, was Sie freigeschaltet haben.";
		} else {
			return "Take a moment to breathe.\n\nBREATHE.\nFrom the main menu, you can choose how many minutes to breathe with the up and down buttons. When you are ready to start your session, click the select button.\n\nCUSTOMIZE.\nYou can customize settings on your phone or on the watch. Long press the up button to change settings on your watch!\n\nACHIEVE.\nAs you breathe, you can earn achievements and see your stats! Long press the down button to see what you've unlocked.";
		}
	}
#endif