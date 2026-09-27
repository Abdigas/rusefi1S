```make
##############################################################################
# see readme.md
# see setup_linux_environment.sh but Windows is also totally fine
#
##############################################################################
# The most advanced open source ECU has the most advanced Makefile!
# Here we do a bit of code generation, compile custom bootloader and embedded firmware and even build user artifacts!
#
# In order for Make to know which board you are trying to build for, it needs some parameters.
# BOARD_DIR - relative to the firmware dir, will be set to config/boards/f407-discovery if left empty
# SHORT_BOARD_NAME - will be set to f407-discovery if left empty
# PROJECT_BOARD - will be set to SHORT_BOARD_NAME if left empty
#
# You can set these as environment variables like this:
# source config/boards/common_script_read_meta_env.inc config/boards/f407-discovery/meta-info.env
# source config/boards/common_script_read_meta_env.inc config/boards/subaru_eg33/meta-info-subaru_eg33_f7.env
# source config/boards/common_script_read_meta_env.inc firmware/config/boards/hellen/hellen154hyundai_f7/meta-info-hellen154hyundai_f7.env
#
# There is also a script to compile the firmware to avoid messing with your environment variables.
# ./bin/compile.sh config/boards/f407-discovery/meta-info.env
# Or to package the bundles:
# ./bin/compile.sh -b config/boards/f407-discovery/meta-info.env
# If not provided with a path to a meta-info file, compile.sh will let you select a target.
#
# If you wish to call make directly, here are the available commands:
#
# make docs-enums    Generate docs and enums
# make config        Generate docs, enums, and configs
# make               Generate docs, enums, configs, and build firmware
# make bundle        Generate docs, enums, configs, build bootloader, build firmware, and package full bundle
# make autoupdate    Generate docs, enums, configs, build bootloader, build firmware, and package autoupdate bundle
# make bundles       Generate docs, enums, configs, build bootloader, build firmware, and package both full and autoupdate bundles
# make bootloader    Generate docs, enums, configs, and build bootloader
# make ../simulator/build/rusefi_simulator.exe build Windows simulator (even if invoked on Linux)
# bin/compile.sh config/boards/f407-discovery/meta-info.env ../simulator/build/rusefi_simulator.linux build Linux simulator
# make ../simulator/build/rusefi_simulator.both build _both_ Windows and Linux simulator (do we need that? shall we drop that?)
#

CHIBIOS = ChibiOS
RULESPATH = $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/mk
RULESFILE = $(RULESPATH)/rules.mk

ifneq ("$(wildcard $(../.git))","")
$(error "../.git not found. We expect source code to be cloned not downloaded as zip file.")
endif

# Define project name here
PROJECT = rusefi
PROJECT_DIR = .

# Configure precompiled header
PCH_DIR = $(PROJECT_DIR)/pch
PCHSRC = $(PCH_DIR)/pch.h

# Imported source files and paths
CHIBIOS_CONTRIB = ChibiOS-Contrib

SUBMODULES = $(CHIBIOS) $(CHIBIOS_CONTRIB) libfirmware

ifneq ($(filter 1,$(foreach d,$(SUBMODULES),$(if $(wildcard $(d)/*),,1))),)
$(info Submodule not found. Chibios: Invoking "git submodule update --init")
DISCARD := $(shell git submodule update --init)
$(info Invoked "git submodule update --init")
endif

include $(PROJECT_DIR)/common_libfirmware.mk

include rusefi.mk

ifeq ($(DEBUG_LEVEL_OPT),)
  DEBUG_LEVEL_OPT = -Os -ggdb -g
endif

ifeq ($(USE_OPT),)
  USE_OPT = $(EXTRA_PARAMS) $(EXTRA_2_PARAMS) $(EXTRA_3_PARAMS) $(DEBUG_LEVEL_OPT) $(RFLAGS) -fomit-frame-pointer -fsingle-precision-constant -fno-inline-functions
endif

USE_OPT += $(RUSEFI_OPT) -DEFI_UNIT_TEST=0 -DEFI_PROD_CODE=1 -DEFI_SIMULATOR=0
USE_OPT += -DWE_HAVE_CRITICAL_ERROR_METHOD

ifeq ($(USE_COPT),)
  USE_COPT = -fgnu89-inline -std=gnu99 -Wno-error=implicit-fallthrough
endif

ifeq ($(USE_CPPOPT),)
  USE_CPPOPT  = -std=c++20 -Wno-register -fno-rtti -fno-threadsafe-statics -fno-exceptions -fno-use-cxa-atexit
  USE_CPPOPT += -Wno-deprecated
endif

USE_CPPOPT += -Wno-error=tautological-compare
USE_CPPOPT += $(RUSEFI_CPPOPT)

ifeq ($(USE_LINK_GC),)
  USE_LINK_GC = yes
endif

ifeq ($(USE_LDOPT),)
  USE_LDOPT = --print-memory-usage
endif

ifeq ($(USE_LTO),)
  USE_LTO = no
  USE_OPT += -flto=auto
endif

ifeq ($(OS),Windows_NT)
   USE_OPT += -DIS_WINDOWS_COMPILER=1
else
   USE_OPT += -DIS_WINDOWS_COMPILER=0
endif

ifeq ($(USE_THUMB),)
  USE_THUMB = yes
endif

ifeq ($(USE_VERBOSE_COMPILE),)
  USE_VERBOSE_COMPILE = no
endif

ifeq ($(USE_SMART_BUILD),)
  USE_SMART_BUILD = no
endif

ifeq ($(USE_OPENBLT),)
  USE_OPENBLT = no
endif

ifeq ($(USE_FATFS),)
  USE_FATFS = yes
endif

#
# Build global options
##############################################################################

##############################################################################
# Architecture or project specific options
#

ifeq ($(USE_PROCESS_STACKSIZE),)
  USE_PROCESS_STACKSIZE = 0x0600
endif

ifeq ($(USE_EXCEPTIONS_STACKSIZE),)
  USE_EXCEPTIONS_STACKSIZE = 0x1000
endif

NUMJOBS=${NUMJOBS:-" -j4 "}
MAKEFLAGS += ${NUMJOBS} -r

#
# Architecture or project specific options
##############################################################################

##############################################################################
# Project, sources and paths
#

BOARD_DIR := ./$(BOARD_DIR)
BOARDINC = $(BOARD_DIR)

include $(BOARD_DIR)/board.mk

BOARDCPPSRC += $(BOARDS_DIR)/board_id.cpp

ifeq (,$(filter clean,$(MAKECMDGOALS)))
ifeq ($(SHORT_BOARD_NAME),)
$(error SHORT_BOARD_NAME not set, something wrong with your meta-info.env file)
endif
endif

DDEFS += -DSHORT_BOARD_NAME=$(SHORT_BOARD_NAME)

TS_PAGE_GUARD_FLAGS = EFI_LTFT_CONTROL|EFI_LUA

TS_PAGE_GUARD_DEFS := $(shell sed -nE 's/^#define[[:space:]]+($(TS_PAGE_GUARD_FLAGS))[[:space:]]+(TRUE|FALSE)[[:space:]]*$$/-D\1=\2/p' $(BOARD_DIR)/prepend.txt $(BOARD_DIR)/prepend_$(SHORT_BOARD_NAME).txt 2>/dev/null)

DDEFS += $(TS_PAGE_GUARD_DEFS)

# Include various ChibiOS mk files

include $(CHIBIOS)/os/license/license.mk

include rusefi_rules.mk

include $(CPU_STARTUP_DIR)

include $(CHIBIOS_CONTRIB)/os/hal/hal.mk
include $(CPU_PLATFORM)
include $(CHIBIOS)/os/hal/osal/rt-nil/osal.mk

include $(CHIBIOS)/os/rt/rt.mk
include $(CHIBIOS)/os/common/ports/ARMv7-M/compilers/GCC/mk/port.mk

ifeq ($(EFI_ONBOARD_MEMS_LIS2DW12),yes)
  DDEFS += -DEFI_ONBOARD_MEMS_LIS2DW12=TRUE
  include $(CHIBIOS)/os/ex/devices/ST/lis2dw12.mk
endif

ifeq ($(EFI_ONBOARD_MEMS_LIS2DH12),yes)
  DDEFS += -DEFI_ONBOARD_MEMS_LIS2DH12=TRUE
  include $(CHIBIOS)/os/ex/devices/ST/lsm303agr.mk
endif

ifeq ($(EFI_ONBOARD_MEMS_LIS302DL),yes)
  DDEFS += -DEFI_ONBOARD_MEMS_LIS302DL=TRUE
  include $(CHIBIOS)/os/ex/devices/ST/lis302dl.mk
endif

ifeq ($(EFI_ONBOARD_MEMS_LIS3DSH),yes)
  DDEFS += -DEFI_ONBOARD_MEMS_LIS3DSH=TRUE
  include $(CHIBIOS)/os/ex/devices/ST/lis3dsh.mk
endif

include $(CHIBIOS)/os/hal/lib/streams/streams.mk
include $(CHIBIOS)/os/various/cpp_wrappers/chcpp.mk
include $(CHIBIOS)/os/hal/lib/complex/mfs/hal_mfs.mk

ifeq ($(USE_FATFS),yes)
  DDEFS += -DEFI_SUPPORT_FATFS=TRUE
  include $(CHIBIOS)/os/various/fatfs_bindings/fatfs.mk
endif

include console/binary/tunerstudio.mk

ifeq ($(USE_FATFS),yes)
  include $(PROJECT_DIR)/ext/FatFS/fatfs.mk
endif

ifeq ($(USE_WIFI),yes)
  LWIP = yes
  include $(PROJECT_DIR)/ext/atwinc1500/atwinc1500.mk
  DDEFS += -DEFI_WIFI=1
endif

ifeq ($(EFI_ETHERNET),yes)
  LWIP = yes
  DDEFS += -DEFI_ETHERNET=TRUE
  DDEFS += -DCH_CFG_USE_DYNAMIC=TRUE
endif

ifeq ($(LWIP),yes)
  include $(CHIBIOS)/os/various/lwip_bindings/lwip.mk
else
  ALLCSRC += $(CHIBIOS)/os/various/evtimer.c
endif

include $(PROJECT_DIR)/hw_layer/mass_storage/mass_storage.mk
include $(PROJECT_DIR)/common.mk
include $(PROJECT_DIR)/controllers/modules/modules.mk

ifeq ($(USE_OPENBLT),yes)
  USE_OPT += -Wl,--defsym=HAS_BOOTLOADER=1
  DDEFS += -DEFI_USE_OPENBLT=TRUE
endif

$(info PROJECT_BOARD: $(PROJECT_BOARD))
$(info BOARD_DIR:     $(BOARD_DIR))
$(info PROJECT_CPU:   $(PROJECT_CPU))
$(info SHORT_BOARD_NAME: $(SHORT_BOARD_NAME))
$(info CPU_HWLAYER:   $(CPU_HWLAYER))
$(info CONFDIR:       $(CONFDIR))
$(info LDSCRIPT:      $(LDSCRIPT))

CSRC = gcc_version_check.c \
  $(ALLCSRC) \
  $(BOARDSRC) \
  $(CHIBIOS_CONTRIB)/os/various/ramdisk.c \
  $(DEV_SRC) \
  $(BOOTLOADERSRC) \
  ${HW_MASS_STORAGE_SRC_C} \
  $(HW_LAYER) \
  $(HW_LAYER_PORT) \
  $(HW_LAYER_DRIVERS_CORE) \
  $(HW_LAYER_DRIVERS) \
  $(RUSEFI_LIB_C) \
  $(PROJECT_DIR)/hw_layer/main_hardfault.c

CPPSRC = \
  $(ALLCPPSRC) \
  $(BOARDCPPSRC) \
  $(DEV_SRC_CPP) \
  $(HW_LAYER_DRIVERS_CORE_CPP) \
  $(HW_LAYER_DRIVERS_CPP) \
  $(CONSOLE_SRC_CPP) \
  $(RUSEFI_LIB_CPP) \
  $(MODULES_CPPSRC) \
  rusefi.cpp \
  main.cpp

ACSRC =
ACPPSRC =
TCSRC =
TCPPSRC =

ASMXSRC = $(ALLXASMSRC) \
  $(RUSEFIASM) \
  $(PROJECT_DIR)/hw_layer/main_hardfault_asm.S

INCDIR += \
  $(PCH_DIR) \
  $(BOARDINC) \
  $(ALLINC) \
  $(MODULES_INC) \
  $(TESTINC) \
  $(CHIBIOS)/os/various \
  $(RUSEFI_LIB_INC) \
  $(CONFDIR) \
  ext/FatFS \
  $(PROJECT_DIR)/hw_layer/mass_storage \
  $(HW_INC) \
  $(PROJECT_DIR)/development/hw_layer \
  development/test \
  $(CHIBIOS_CONTRIB)/os/various

#
# Project, sources and paths
##############################################################################

##############################################################################
# Compiler settings
#

ifeq ($(CROSS_COMPILE),)
  TRGT = arm-none-eabi-
else
  TRGT = $(CROSS_COMPILE)
endif

CC   = $(TRGT)gcc
CPPC = $(TRGT)g++

LD   = $(CC)

CP   = $(TRGT)objcopy
AS   = $(CC) -x assembler-with-cpp
AR   = $(TRGT)ar
OD   = $(TRGT)objdump
SZ   = $(TRGT)size

HEX  = $(CP) -O ihex
BIN  = $(CP) -O binary

ifneq ($(BOARD_IMAGE_SCRIPT),)
PYTHON ?= python3

HEX = $(PYTHON) $(BOARD_IMAGE_SCRIPT) --format hex
BIN = $(PYTHON) $(BOARD_IMAGE_SCRIPT) --format bin

override OUTFILES = $(BUILDDIR)/$(PROJECT).elf $(BUILDDIR)/$(PROJECT).hex \
  $(BUILDDIR)/$(PROJECT).srec $(BUILDDIR)/$(PROJECT).dmp $(BUILDDIR)/$(PROJECT).list
endif

AOPT =

TOPT = -mthumb -DTHUMB

CWARN = -Wall -Wextra -Wstrict-prototypes
CPPWARN = -Wall -Wextra

#
# Compiler settings
##############################################################################

##############################################################################
# Start of user section
#

UDEFS =

UADEFS =

UINCDIR =

ULIBDIR =

ULIBS = -lm --specs=nano.specs -Wl,--sort-section=alignment

#
# End of user defines
##############################################################################

include $(RULESFILE)

$(BUILDDIR)/$(PROJECT).elf: $(LDSCRIPT_DEPS)

ifneq ($(BOARD_IMAGE_SCRIPT),)
$(BUILDDIR)/$(PROJECT).hex $(BUILDDIR)/$(PROJECT).srec: $(BOARD_IMAGE_SCRIPT)
endif

include target_sentinel.mk

include rusefi_config.mk

include docs_enums.mk

include rusefi_pch.mk

include bundle.mk

.PHONY: CLEAN_RULE_HOOK CLEAN_PCH_HOOK CLEAN_BUNDLE_HOOK

CLEAN_RULE_HOOK: CLEAN_PCH_HOOK CLEAN_BUNDLE_HOOK

CLEAN_PCH_HOOK:

CLEAN_BUNDLE_HOOK:

POST_MAKE_ALL_RULE_HOOK: $(BUILDDIR)/$(PROJECT).elf $(GCC_MAP_READER_JAR)
	@java -jar $(GCC_MAP_READER_JAR) $(BUILDDIR)/$(PROJECT).map | grep Total
	@$(TRGT)objdump -h $(BUILDDIR)/$(PROJECT).elf | grep -w ram4

VPATH = $(patsubst ./%,%,$(SRCPATHS))
```
