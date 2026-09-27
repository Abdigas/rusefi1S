# rusEFI Firmware Makefile
#
# This is the main firmware build file.
# Board-specific settings belong in:
# config/boards/<board>/board.mk

PROJECT_DIR := $(abspath .)

CHIBIOS = ChibiOS

RULESPATH = $(PROJECT_DIR)/$(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC
RULESFILE = $(RULESPATH)/rules.mk

# ------------------------------------------------------------
# Common firmware configuration
# ------------------------------------------------------------

include $(PROJECT_DIR)/common_libfirmware.mk
include rusefi.mk

# ------------------------------------------------------------
# Default compiler configuration
# ------------------------------------------------------------

DEBUG ?= no

ifeq ($(DEBUG),yes)
	OPT = -Og
	DDEFS += -DDEBUG
else
	OPT = -Os
endif

# ------------------------------------------------------------
# Board selection
# ------------------------------------------------------------

ifndef BOARD_DIR
	BOARD_DIR := ./config/boards/f407-discovery
endif

BOARD_DIR := ./$(BOARD_DIR)

# ------------------------------------------------------------
# Board-specific configuration
#
# This loads:
# config/boards/<board>/board.mk
# ------------------------------------------------------------

include $(BOARD_DIR)/board.mk

# ------------------------------------------------------------
# rusEFI rules
# ------------------------------------------------------------

include rusefi_rules.mk

# ------------------------------------------------------------
# ChibiOS startup
# ------------------------------------------------------------

include $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/Makefile.inc

# ------------------------------------------------------------
# Common source configuration
# ------------------------------------------------------------

include $(PROJECT_DIR)/common.mk

# ------------------------------------------------------------
# Controller modules
# ------------------------------------------------------------

include $(PROJECT_DIR)/controllers/modules/modules.mk

# ------------------------------------------------------------
# Compiler tools
# ------------------------------------------------------------

CC = arm-none-eabi-gcc
CXX = arm-none-eabi-g++
AS = arm-none-eabi-gcc
AR = arm-none-eabi-ar
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump
SIZE = arm-none-eabi-size

# ------------------------------------------------------------
# Build rules
# ------------------------------------------------------------

include $(RULESFILE)

# ------------------------------------------------------------
# rusEFI configuration
# ------------------------------------------------------------

include target_sentinel.mk
include rusefi_config.mk
include docs_enums.mk
include rusefi_pch.mk

# ------------------------------------------------------------
# Firmware bundle
# ------------------------------------------------------------

include bundle.mk

# ------------------------------------------------------------
# Board source
# ------------------------------------------------------------

CPPSRC += $(BOARDCPPSRC)

# ------------------------------------------------------------
# OpenBLT
# ------------------------------------------------------------

ifeq ($(USE_OPENBLT),yes)
	DDEFS += -DUSE_OPENBLT
endif
