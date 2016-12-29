#include <pebble.h>
#include "localize.h"
#include "src/c/settings.h"
#include "src/c/data.h"

const char * localize_get_locale() {
	return i18n_get_system_locale();
}

char * localize_get_app_glance_text(int type, int minutes) {
	switch (type) {
		case 0:
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
		default:
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
	#if defined(PBL_HEALTH)
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
	#endif
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
	#if defined(PBL_HEALTH)
	}
	#endif
}

char * localize_get_heart_rate_text() {
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

char * localize_get_snooze_text() {
	if (strncmp(localize_get_locale(), "fr", 2) == 0) {
		return "Snooze pendant %d min";
	} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
		return "Recuérdame en %d min";
	} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
		return "Snooze %d min";
	} else {
		return "Snooze for %d min";
	}
}

#if !PBL_PLATFORM_APLITE
	char * localize_get_achievement_text() {
		if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			return "Succès!";
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			return "¡Logro!";
		} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
			return "Errungenschaft!";
		} else {
			return "Achievement!";
		}
	}

	char * localize_get_streak_text(int streak_length) {
		if (streak_length == 1) {
			if (strncmp(localize_get_locale(), "fr", 2) == 0) {
				return "%d JOUR DE SUITE";
			} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
				return "RACHA: %d DÍA";
			} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
				return "SIEGESSERIE: %d TAG";
			} else {
				return "STREAK: %d DAY";
			}
		} else {
			if (strncmp(localize_get_locale(), "fr", 2) == 0) {
				return "%d JOURS DE SUITE";
			} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
				return "RACHA: %d DÍAS";
			} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
				return "SIEGESSERIE: %d TAGE";
			} else {
				return "STREAK: %d DAYS";
			}
		}
	}

	char * localize_get_five_minutes_session_name() {
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

	char * localize_get_minutes_session_description() {
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

	char * localize_get_eight_minutes_session_name() {
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

	char * localize_get_ten_minutes_session_name() {
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

	char * localize_get_changed_settings_name() {
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

	char * localize_get_changed_settings_description() {
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

	char * localize_get_one_week_streak_name() {
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

	char * localize_get_one_week_streak_description() {
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

	char * localize_get_one_month_streak_name() {
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

	char * localize_get_one_month_streak_description() {
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

	char * localize_get_one_year_streak_name() {
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

	char * localize_get_one_year_streak_description() {
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

	char * localize_get_five_minutes_day_name() {
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

	char * localize_get_minutes_day_description() {
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

	char * localize_get_ten_minutes_day_name() {
		if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			return "HABILE";
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			return "EXPERTO";
		} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
			return "GESCHICKLICHKEIT";
		} else {
			return "TRAINED";
		}		
	}

	char * localize_get_thirty_minutes_day_name() {
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

	char * localize_get_one_hour_day_name() {
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

	char * localize_get_one_hour_day_description() {
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

	char * localize_get_completionist_name() {
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

	char * localize_get_completionist_description() {
		if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			return "Obtenez tous les succès.";
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			return "Obtén todos los logros.";
		} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
			return "Holen Sie sich alle Errungenschaften.";
		} else {
			return "Get all the achievements.";
		}
	}

	char * localize_get_achievements_section_title() {
		if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			return "SUCCÈS";
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			return "LOGROS";
		} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
			return "ERRUNGENSCHAFTEN";
		} else {
			return "ACHIEVEMENTS";
		}
	}

	char * localize_get_stats_section_title() {
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

	char * localize_get_locked_title() {
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

	char * localize_get_locked_description() {
		if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			return "Respirez plus!";
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			return "¡Respira más!";
		} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
			return "Atmen mehr!";
		} else {
			return "Keep breathing!";
		}
	}

	char * localize_get_total_breathed_name() {
		if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			return "Total respiré";
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			return "Total respirado";
		} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
			return "Ganz geatmet";
		} else {
			return "Total breathed";
		}
	}

	char * localize_get_total_breathed_description() {
		if (strncmp(localize_get_locale(), "de", 2) == 0) {
			return "%d Min.";
		} else {
			return "%d min.";
		}
	}

	char * localize_get_longest_streak_name() {
		if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			return "Série la plus longue";
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			return "Racha más larga";
		} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
			return "Längste Streifen";
		} else {
			return "Longest streak";
		}
	}

	char * localize_get_longest_streak_description() {
		if (strncmp(localize_get_locale(), "fr", 2) == 0) {
			return "%d journées";
		} else if (strncmp(localize_get_locale(), "es", 2) == 0) {
			return "%d días";
		} else if (strncmp(localize_get_locale(), "de", 2) == 0) {
			return "%d Tage";
		} else {
			return "%d days";
		}
	}
#endif