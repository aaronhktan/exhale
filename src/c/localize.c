#include <pebble.h>
#include "localize.h"

const char * localize_get_locale() {
	 return i18n_get_system_locale();
}