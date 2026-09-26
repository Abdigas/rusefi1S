/**
 * @file board_configuration.cpp
 *
 * Stage 1 custom DevEBox F407 configuration.
 *
 * IMPORTANT:
 * Stage 1 intentionally keeps most of the original
 * f407-discovery configuration.
 *
 * The purpose of this stage is to prove that a new
 * board directory named "devebox-f407" can compile.
 *
 * Only the following functional changes are made:
 *
 *   1. Engine cylinder count = 1
 *   2. Crank trigger input = PB1
 *
 * Other Discovery/Frankenso dependencies will be removed
 * in later stages, one change at a time.
 */

#include "pch.h"
#include "board_overrides.h"


static void setDefaultFrankensoStepperIdleParameters() {
	engineConfiguration->idle.stepperDirectionPin = Gpio::E10;
	engineConfiguration->idle.stepperStepPin = Gpio::E12;
	engineConfiguration->stepperEnablePin = Gpio::E14;

	engineConfiguration->idleStepperReactionTime = 10;
	engineConfiguration->idleStepperTotalSteps = 150;
}


static void setCanFrankensoDefaults() {
	engineConfiguration->canTxPin = Gpio::B6;
	engineConfiguration->canRxPin = Gpio::B12;
}


Gpio getWarningLedPin() {
	// No warning LED assigned for Stage 1.
	return Gpio::Unassigned;
}


Gpio getCommsLedPin() {

#if defined(HW_NOT_COMMUNITY_FRANKENSO) || defined(EFI_BOOTLOADER)

	// Reuse the existing Discovery LED configuration.
	return Gpio::D15;

#else

	// f407-discovery community board configuration.
	return config->communityCommsLedPin;

#endif
}


Gpio getRunningLedPin() {
	// No running LED assigned for Stage 1.
	return Gpio::Unassigned;
}


#if EFI_ONBOARD_MEMS

static void configureAccelerometerPins() {

	// stm32f4discovery defaults
	engineConfiguration->spi1mosiPin = Gpio::A7;
	engineConfiguration->spi1misoPin = Gpio::A6;
	engineConfiguration->spi1sckPin = Gpio::A5;
}

#endif // EFI_ONBOARD_MEMS


/**
 * @brief Hardware board-specific default configuration.
 *
 * Stage 1:
 *
 *   Engine:
 *       1 cylinder
 *
 *   Crank:
 *       PB1
 *
 *   Everything else:
 *       temporarily retained from f407-discovery
 *
 * This allows us to validate the new board directory before
 * progressively removing Discovery/Frankenso hardware.
 */
static void devebox_f407_DefaultConfiguration() {

	/*
	 * ============================================================
	 * STAGE 1
	 * ============================================================
	 *
	 * New board directory:
	 *
	 *     config/boards/devebox-f407/
	 *
	 * MCU:
	 *
	 *     STM32F407
	 *
	 * Engine:
	 *
	 *     1 cylinder
	 *
	 * Crank:
	 *
	 *     PB1
	 *
	 * ============================================================
	 */

	engineConfiguration->cylindersCount = 1;


	/*
	 * Keep existing Discovery/Frankenso defaults temporarily.
	 *
	 * These will be removed in later stages after this board
	 * successfully compiles.
	 */

	setDefaultFrankensoStepperIdleParameters();

	setCanFrankensoDefaults();


#ifndef HW_NOT_COMMUNITY_FRANKENSO

	// Reuse Discovery blue LED.
	config->communityCommsLedPin = Gpio::D15;

#endif


	/*
	 * Optional onboard MEMS.
	 *
	 * Kept for Stage 1 compatibility.
	 */

#if EFI_ONBOARD_MEMS

	configureAccelerometerPins();

#endif


	/*
	 * Existing Discovery ADC assignments.
	 *
	 * These are NOT final DevEBox assignments.
	 *
	 * They will be replaced later with the actual DevEBox
	 * ADC pins.
	 */

	engineConfiguration->map.sensor.hwChannel = EFI_ADC_4;

	engineConfiguration->clt.adcChannel = EFI_ADC_6;

	engineConfiguration->iat.adcChannel = EFI_ADC_7;

	engineConfiguration->afr.hwChannel = EFI_ADC_14;


	/*
	 * Existing Discovery accelerometer SPI device.
	 */

	engineConfiguration->accelerometerSpiDevice = SPI_DEVICE_1;


	/*
	 * Trigger simulator pins.
	 *
	 * Temporarily retained from Discovery.
	 */

	engineConfiguration->triggerSimulatorPins[0] = Gpio::D1;

	engineConfiguration->triggerSimulatorPins[1] = Gpio::D2;


	/*
	 * ============================================================
	 * IMPORTANT CRANK INPUT
	 * ============================================================
	 *
	 * MAX9926 digital output -> STM32F407 PB1
	 *
	 * PB1 is our selected crank input.
	 *
	 * PA9 is NOT used.
	 *
	 * PC6 is NOT used.
	 *
	 * ============================================================
	 */

	engineConfiguration->triggerInputPins[0] = Gpio::B1;

	// No second trigger input for this single-cylinder Stage 1 test.
	engineConfiguration->triggerInputPins[1] = Gpio::Unassigned;


	/*
	 * Existing digital potentiometer configuration.
	 *
	 * Temporarily retained for compilation compatibility.
	 */

	engineConfiguration->digitalPotentiometerChipSelect[0] = Gpio::D7;

	engineConfiguration->digitalPotentiometerChipSelect[1] = Gpio::Unassigned;

	engineConfiguration->digitalPotentiometerChipSelect[2] = Gpio::D5;

	engineConfiguration->digitalPotentiometerChipSelect[3] = Gpio::Unassigned;


	/*
	 * Existing Discovery SPI1 configuration.
	 */

	engineConfiguration->spi1mosiPin = Gpio::B5;

	engineConfiguration->spi1misoPin = Gpio::B4;

	engineConfiguration->spi1sckPin = Gpio::B3;


	/*
	 * Existing Discovery SPI2 configuration.
	 */

	engineConfiguration->spi2mosiPin = Gpio::B15;

	engineConfiguration->spi2misoPin = Gpio::B14;

	engineConfiguration->spi2sckPin = Gpio::B13;


	/*
	 * Existing Discovery SPI3 configuration.
	 */

	engineConfiguration->spi3mosiPin = Gpio::B5;

	engineConfiguration->spi3misoPin = Gpio::B4;

	engineConfiguration->spi3sckPin = Gpio::B3;


#if EFI_FILE_LOGGING

	setDefaultSdCardParameters();

#endif /* EFI_FILE_LOGGING */


	/*
	 * Keep the original Discovery SPI enable state for Stage 1.
	 */

	engineConfiguration->is_enabled_spi_1 = false;

	engineConfiguration->is_enabled_spi_2 = false;

	engineConfiguration->is_enabled_spi_3 = true;
}


/**
 * @brief Stage 1 hardware initialization.
 *
 * MC33810 is intentionally retained temporarily because this
 * stage is only a compilation/board-directory validation step.
 *
 * It will be removed in a later stage.
 */
void devebox_f407_boardInitHardware() {

	static const struct mc33810_config mc33810 = {

		.spi_bus = &SPID3,

		.spi_config = {

			.circular = false,

#ifdef _CHIBIOS_RT_CONF_VER_6_1_

			.end_cb = nullptr,

#else

			.slave = false,

			.data_cb = nullptr,

			.error_cb = nullptr,

#endif

			// Temporary Stage 1 compatibility configuration.
			.ssport = GPIOC,

			.sspad = 5,

			.cr1 =
				SPI_CR1_16BIT_MODE |
				SPI_CR1_SSM |
				SPI_CR1_SSI |
				((3 << SPI_CR1_BR_Pos) & SPI_CR1_BR) |
				SPI_CR1_MSTR |
				SPI_CR1_CPHA |
				0,

			.cr2 = SPI_CR2_16BIT_MODE
		},

		.direct_io = {

			/* injector drivers */

			[0] = {
				.port = GPIOA,
				.pad = 3
			},

			[1] = {
				.port = GPIOA,
				.pad = 4
			},

			[2] = {
				.port = nullptr,
				.pad = 0
			},

			[3] = {
				.port = nullptr,
				.pad = 0
			},

			/* ignition pre-drivers */

			[4] = {
				.port = GPIOA,
				.pad = 0
			},

			[5] = {
				.port = GPIOA,
				.pad = 1
			},

			/*
			 * GPGD mode is not supported yet.
			 * Ignition mode does not support SPI on/off commands,
			 * so ignition signals should be directly driven.
			 */

			[6] = {
				.port = GPIOD,
				.pad = 0
			},

			/*
			 * Even if unused, a pin is currently required.
			 */

			[7] = {
				.port = GPIOD,
				.pad = 1
			}
		},

		.en = {
			.port = GPIOA,
			.pad = 6
		},

		.sck = {
			.port = GPIOB,
			.pad = 3
		},

		.spkdur = Gpio::Unassigned,

		.nomi = Gpio::Unassigned,

		.maxi = Gpio::Unassigned
	};


	if (engineConfiguration->engineType == engine_type_e::FRANKENSO_TEST_33810) {

		int ret = mc33810_add(
			Gpio::MC33810_0_OUT_0,
			0,
			&mc33810
		);

		efiPrintf(
			"*****************+ mc33810_add %d +*******************",
			ret
		);


#ifndef EFI_BOOTLOADER

		/*
		 * Temporary diagnostic command retained from
		 * the original Discovery configuration.
		 */

		addConsoleAction("injinfo", []() {

			efiPrintf(
				"injinfo index=%d",
				engine->fuelComputer.brokenInjector
			);

		});

#endif // EFI_BOOTLOADER

	}
}


/**
 * @brief Register the custom DevEBox board callbacks.
 */
void setup_custom_board_overrides() {

	custom_board_InitHardware =
		devebox_f407_boardInitHardware;

	custom_board_DefaultConfiguration =
		devebox_f407_DefaultConfiguration;
}
