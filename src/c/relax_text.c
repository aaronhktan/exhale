#include <pebble.h>
#include "relax_text.h"

char generate_upper_relax_text(int randomNumber) {
	char *strings[10];
	strings[0] = "TAKE A MOMENT";
	strings[1] = "BE STILL";
	strings[2] = "CLEAR YOUR MIND";
	strings[3] = "DON'T TALK";
	strings[4] = "EMPTY YOUR THOUGHTS";
	strings[5] = "BE CALM";
	strings[6] = "THINK NOTHING";
	strings[7] = "RELAX";
	strings[8] = "CHILL FOR A SEC";
	strings[9] = "SPACE OUT";
	return *strings[randomNumber];
}

char generate_lower_relax_text(int randomNumber) {
	char *strings[4];
	strings[0] = "AND BREATHE";
	strings[1] = "AND EXHALE";
	strings[2] = "AND CONCENTRATE";
	strings[3] = "AND FOCUS";
	return *strings[randomNumber];
}