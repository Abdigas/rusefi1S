#include "pch.h"
#include "board_overrides.h"

static void devebox_f407_DefaultConfiguration() {
}

void devebox_f407_boardInitHardware() {
}

void setup_custom_board_overrides() {
	custom_board_InitHardware = devebox_f407_boardInitHardware;
	custom_board_DefaultConfiguration = devebox_f407_DefaultConfiguration;
}
