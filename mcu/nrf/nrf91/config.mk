# Mcu instruction set
ARCH=armv8-m.main
CFLAGS += -mfloat-abi=hard -mfpu=fpv5-sp-d16 -mcmse
# Libraries to be build for Cortex-M33
CM33 := yes
CFLAGS += -DARM_MATH_ARMV8MML
# This mcu has a bootloader (enough memory)
HAS_BOOTLOADER=yes

CFLAGS += -DNRF91_PLATFORM
mac_profile?=dect_nr_19_ghz
radio?=none
ifeq ($(MCU_SUB),61)
    # Hardware magic used for this architecture
    HW_MAGIC=12
    HW_VARIANT_ID=18

    CFLAGS += -DNRF9120_XXAA
else
    $(error "Invalid MCU_SUB for nrf91! $(MCU_SUB) only 61 supported")
endif

# Program Flash start address
FLASH_BASE_ADDR=0x00000000

# Different bootloader sizes available: size=mem_variant_byte,...
BOOTLOADER_SIZES="32k=0x01"

modemfw_name=$(notdir $(modem_fw))
ifeq ($(suffix $(modem_fw)), .cbor)
modemfw_area_id=0x300000$(HW_MAGIC)
else ifeq ($(suffix $(modem_fw)), .bin)
modemfw_area_id=0x300001$(HW_MAGIC)
endif

# Add custom flags
# Remove the -Wunused-parameter flag added by -Wextra as some cortex M4 header do not respect it
CFLAGS += -Wno-unused-parameter
