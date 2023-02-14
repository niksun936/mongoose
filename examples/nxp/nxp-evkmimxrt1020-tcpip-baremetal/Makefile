CFLAGS  = -W -Wall -Wextra -Wundef -Wshadow -Wdouble-promotion -fno-common -Wconversion -Wno-sign-conversion # -Wformat-truncation -Werror
CFLAGS += -g3 -Os # -ffunction-sections -fdata-sections
CFLAGS += -I. -ICMSIS -Iboard -Ifsl -Iheaders
CFLAGS += -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16
CFLAGS += -DMG_ENABLE_PACKED_FS=1
CFLAGS += -DMG_ARCH=MG_ARCH_CUSTOM -DMG_ENABLE_TCPIP=1 -DMG_ENABLE_DRIVER_IMXRT1020 -DMG_ENABLE_CUSTOM_MILLIS=1
CFLAGS += -DSERIAL_PORT_TYPE_SWO # SERIAL_PORT_TYPE_UART / SERIAL_PORT_TYPE_USBCDC / SERIAL_PORT_TYPE_SWO / SERIAL_PORT_TYPE_VIRTUAL
CFLAGS += -DCPU_MIMXRT1021DAG5A -DCPU_MIMXRT1021DAG5A_cm7
CFLAGS += $(CFLAGS_EXTRA)
LDFLAGS ?= -nostartfiles

SOURCES += mongoose.c net.c packed_fs.c main.c
SOURCES += startup/startup_mimxrt1021.c board/board.c fsl/fsl_debug_console.c

ifeq ($(OS),Windows_NT)
  RM = cmd /C del /Q /F /S
else
  RM = rm -rf
endif

all build example: firmware.bin

firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary $< $@

firmware.elf: $(SOURCES) hal.h link.ld
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $@

flash: firmware.bin
	echo "make flash not implemented"

clean:
	@rm -rf firmware.* *.su
