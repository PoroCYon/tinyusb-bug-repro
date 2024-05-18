# -----------------------------------------------------------------------------
# File:        project.mk
# Description: Configuration makefile that sets up the generic Makefile
# -----------------------------------------------------------------------------
EXECUTABLE   := usb_udisk
BUILD_DIR    := ./bin
SDK_LOCATION := ./sdk
SRC_DIRS     := ./src ../common-src/ $(SDK_LOCATION)/src
INC_DIRS     := ./inc $(SDK_LOCATION)/inc
STARTUP_FILE := $(SDK_LOCATION)/Startup/startup_ch32v30x_D8C.S
LIB_DIRS     := 
LIBS         := 
CXXFLAGS     :=
CFLAGS       := -Wall -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common
LDFLAGS      := -static -nostartfiles -Wl,--gc-sections
TOOLCHAIN_PREFIX := riscv32-unknown-elf-
AS := $(TOOLCHAIN_PREFIX)as
CC := $(TOOLCHAIN_PREFIX)gcc
OBJCOPY := $(TOOLCHAIN_PREFIX)objcopy
OBJDUMP := $(TOOLCHAIN_PREFIX)objdump
OPENOCD := 
OCD_CFG := ./wch-riscv.cfg
ARCH := rv32imafc
ABI  := ilp32f

WLINK := $(shell command -v wlink 2>/dev/null)

ifeq ($(OPENOCD),)
ifeq ($(WLINK),)
	$(error "project.mk: Please either install wlink https://github.com/ch32-rs/wlink or set OPENOCD the binary obtained from https://github.com/xu7wong/openocd_wchlink (commit 560db36)")
endif
endif

# TinyUSB config
include ../tinyusb/src/tinyusb.mk
ADDL_SOURCES := $(addprefix ../tinyusb/,$(TINYUSB_SRC_C)) ../tinyusb/src/portable/wch/dcd_ch32_usbhs.c
INC_DIRS += ../tinyusb/src ../common-inc/
CFLAGS += -DCFG_TUSB_MCU=OPT_MCU_CH32V307 -DBOARD_TUD_MAX_SPEED=OPT_MODE_HIGH_SPEED

LINKER_DIRECTORY := $(SDK_LOCATION)/Ld

TARGET = bare_metal
