CFLAGS  = -W -Wall -Wextra -Wundef -Wshadow -Wdouble-promotion -fno-common -Wconversion -Wno-sign-conversion # -Werror -Wformat-truncation
CFLAGS += -g3 -Os -ffunction-sections -fdata-sections
CFLAGS += -I. -ICMSIS -Iboard -Ifsl -Iheaders
CFLAGS += -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16
CFLAGS += -DMG_ENABLE_PACKED_FS=1
CFLAGS += -DMG_ARCH=MG_ARCH_CUSTOM -DMG_ENABLE_TCPIP=1 -DMG_ENABLE_DRIVER_IMXRT1020 -DMG_ENABLE_CUSTOM_MILLIS=1
CFLAGS += $(CFLAGS_EXTRA)
LDFLAGS ?= -Tlink.ld -nostdlib -nostartfiles --specs nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map

SOURCES += mongoose.c net.c packed_fs.c main.c hal.c web_server.c syscalls.c sysinit.c
SOURCES += startup/startup_mimxrt1021.c board/evkmimxrt1020_flexspi_nor_config.c board/fsl_flexspi_nor_boot.c

ifeq ($(OS),Windows_NT)
  RM = cmd /C del /Q /F /S
else
  RM = rm -rf
endif

all build example: firmware.bin

firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary $< $@

firmware.elf: $(SOURCES) link_red.ld
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $@

flash: firmware.bin
	JLinkExe -device MIMXRT1021XXX5A -CommandFile jlink.cmd

clean:
	@rm -rf firmware.* *.su
