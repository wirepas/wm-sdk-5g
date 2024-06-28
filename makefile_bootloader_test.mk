include makefile_common.mk

.DEFAULT_GOAL := all


# Linker for the bootloader
LDSCRIPT := bootloader_test/linker/$(MCU)/gcc_bl_test_$(MCU)$(MCU_SUB)$(MCU_MEM_VAR).ld

BOOTLOADER_TEST_ELF := $(BUILDPREFIX_TEST_BOOTLOADER)bootloader_test.elf

# Include HAL drivers code (needed to build power.c (DCDC))
-include $(HAL_API_PATH)makefile

# Include bootloader test makefile
-include bootloader_test/makefile

OBJS_ = $(SRCS:.c=.o)
OBJS = $(addprefix $(BUILDPREFIX_TEST_BOOTLOADER), $(OBJS_))

$(BUILDPREFIX_TEST_BOOTLOADER)%.o : %.c
	$(DD)$(MKDIR) $(@D)
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

$(BOOTLOADER_TEST_ELF): $(OBJS)
	$(DD)$(MKDIR) $(@D)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ \
	      -Wl,-Map=$(BUILDPREFIX_TEST_BOOTLOADER)bootloader_test.map \
	      -Wl,-T,$(LDSCRIPT) $(LIBS)


$(BOOTLOADER_TEST_HEX): $(BOOTLOADER_TEST_ELF)
	$(DD)$(MKDIR) $(@D)
	$(OBJCOPY) $(BOOTLOADER_TEST_ELF) -O ihex $@

.PHONY: all
all: $(BOOTLOADER_TEST_HEX)

clean:
	$(D)$(RM) $(BOOTLOADER_TEST_HEX)
