/*
@licstart  The following is the entire license notice for the
JavaScript code in this file.

Copyright (C) 1997-2019 by Dimitri van Heesch

This program is free software; you can redistribute it and/or modify
it under the terms of version 2 of the GNU General Public License as published by
the Free Software Foundation

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

@licend  The above is the entire license notice
for the JavaScript code in this file
*/
var NAVTREE =
[
  [ "Wirepas SDK", "index.html", [
    [ "Single-MCU Operation Overview", "index.html", [
      [ "Application", "index.html#application", null ],
      [ "Wirepas Mesh Single MCU API", "index.html#single_mcu_api", null ],
      [ "SDK libraries", "index.html#sdk_libraries", null ],
      [ "Application-specific Hardware Abstraction Layer (HAL)", "index.html#sdk_hal", null ],
      [ "Wirepas Mesh Stack", "index.html#wirepas_firmware", null ],
      [ "Wirepas Mesh Bootloader", "index.html#bootloader", null ],
      [ "Custom bootloader extension", "index.html#bootloader_extension", null ],
      [ "The physical Hardware", "index.html#hardware", null ]
    ] ],
    [ "SDK Environment setup", "dd/d6a/sdk_environment.html", [
      [ "Installation of SDK Environment", "dd/d6a/sdk_environment.html#installation_of_sdk_environment", null ],
      [ "Flashing devices", "dd/d6a/sdk_environment.html#flashing_guideline", null ],
      [ "Resources on Nordic nRF52", "dd/d6a/sdk_environment.html#nordic_resources", [
        [ "Flash Memory available for application on nRF52", "dd/d6a/sdk_environment.html#flash_memory_nrf52", null ],
        [ "RAM Memory available for application on nRF52", "dd/d6a/sdk_environment.html#ram_memory_nrf52", null ],
        [ "Peripherals accessible by stack only", "dd/d6a/sdk_environment.html#peripherals_accessible_by_stack_only", null ],
        [ "Peripherals shared between the stack and the application", "dd/d6a/sdk_environment.html#peripherals_shared_between_the_stack_and_the_application", null ],
        [ "Peripherals available for the application", "dd/d6a/sdk_environment.html#peripherals_available_for_the_application", null ]
      ] ],
      [ "Resources on EFR32", "dd/d6a/sdk_environment.html#efr32_resources", [
        [ "Flash Memory available for application on EFR32", "dd/d6a/sdk_environment.html#flash_memory_efr32", null ],
        [ "RAM Memory available for application on EFR32", "dd/d6a/sdk_environment.html#ram_memory_efr32", null ],
        [ "Peripherals accessible by stack only", "dd/d6a/sdk_environment.html#peripherals_accessible_by_stack_only2", null ],
        [ "Peripherals shared between the stack and the application", "dd/d6a/sdk_environment.html#peripherals_shared_between_the_stack_and_the_application2", null ],
        [ "Peripherals available for the application", "dd/d6a/sdk_environment.html#peripherals_available_for_the_application2", null ]
      ] ],
      [ "Related Material", "dd/d6a/sdk_environment.html#autotoc_md5", null ]
    ] ],
    [ "How to Develop Application with SDK", "de/d7a/how_to_develop.html", [
      [ "Pre-requirements", "de/d7a/how_to_develop.html#licensing", [
        [ "Debugging", "de/d7a/how_to_develop.html#debugging", null ],
        [ "Installing firmware image", "de/d7a/how_to_develop.html#firmware_linking", null ]
      ] ],
      [ "Create new application", "de/d7a/how_to_develop.html#development_of_a_new_application", [
        [ "Copy of an application example", "de/d7a/how_to_develop.html#copy_of_an_application_example", null ],
        [ "Change default network", "de/d7a/how_to_develop.html#change_default_network_address_and_channel", null ],
        [ "Change of app_area_id", "de/d7a/how_to_develop.html#change_of_app_area_id", null ],
        [ "Configuration of a node", "de/d7a/how_to_develop.html#configuration_of_a_node", null ]
      ] ],
      [ "Build application", "de/d7a/how_to_develop.html#build_application", null ],
      [ "Test application", "de/d7a/how_to_develop.html#test_application", null ],
      [ "Flashing device", "de/d7a/how_to_develop.html#flashing_device", null ],
      [ "Using OTAP", "de/d7a/how_to_develop.html#using_otap", null ],
      [ "Define custom board definition", "de/d7a/how_to_develop.html#define_custom_board", null ],
      [ "Application startup", "de/d7a/how_to_develop.html#app_init", null ],
      [ "Adding new source files", "de/d7a/how_to_develop.html#adding_new_source_files_to_the_application", null ],
      [ "Recommendations", "de/d7a/how_to_develop.html#recommendations", [
        [ "Optimization of throughput", "de/d7a/how_to_develop.html#optimization_of_network_throughput", null ],
        [ "Free resources", "de/d7a/how_to_develop.html#free_resources", null ],
        [ "Power consumption", "de/d7a/how_to_develop.html#power_consumption", null ],
        [ "How to store data in persistent memory", "de/d7a/how_to_develop.html#persistent_memory", null ],
        [ "Using storage library", "de/d7a/how_to_develop.html#storage_library", null ],
        [ "Using platform specific storage", "de/d7a/how_to_develop.html#platform_specific_storage", null ],
        [ "Using a dedicated area in flash", "de/d7a/how_to_develop.html#dedicated_area", null ]
      ] ],
      [ "Related Material", "de/d7a/how_to_develop.html#autotoc_md6", null ]
    ] ],
    [ "Single-MCU API Operation Principle", "d7/d28/application_operation.html", [
      [ "Build process", "d7/d28/application_operation.html#build", null ],
      [ "Memory Partitioning", "d7/d28/application_operation.html#memory_partitioning", null ],
      [ "Application Detection", "d7/d28/application_operation.html#application_detection", null ],
      [ "Cooperative MCU Access", "d7/d28/application_operation.html#cooperative_mcu_access", [
        [ "Periodic Application Callback Function", "d7/d28/application_operation.html#periodic_application", null ],
        [ "Asynchronous Application Callback Functions", "d7/d28/application_operation.html#asynchronous_application_callback_functions", null ],
        [ "Application Interrupt", "d7/d28/application_operation.html#application_interrupt", null ],
        [ "Deferred interrupt", "d7/d28/application_operation.html#deferred_interrupt", null ],
        [ "Fast interrupt", "d7/d28/application_operation.html#fast_interrupt", null ],
        [ "Which kind of interrupt for which purpose", "d7/d28/application_operation.html#which_kind_of_interrupt_for_which_purpose", null ],
        [ "Execution time limits", "d7/d28/application_operation.html#execution_time_limits", null ]
      ] ]
    ] ],
    [ "API", "dd/d9c/programming_interface.html", [
      [ "Stack Libraries", "dd/d9c/programming_interface.html#stack_api", [
        [ "Application and Library Versioning", "dd/d9c/programming_interface.html#application_and_library_versioning", null ]
      ] ],
      [ "SDK Libraries", "dd/d9c/programming_interface.html#libraries_api", null ],
      [ "Board definitions", "dd/d9c/programming_interface.html#board_api", [
        [ "DCDC converter configuration", "dd/d9c/programming_interface.html#DCDC_converter", null ]
      ] ],
      [ "Bootloader configuration", "dd/d9c/programming_interface.html#bootloader_api", null ],
      [ "Low level hardware services (HAL)", "dd/d9c/programming_interface.html#mcu_api", [
        [ "mcu/common Common MCU files", "dd/d9c/programming_interface.html#mcu_common", null ],
        [ "start.c", "dd/d9c/programming_interface.html#mcu_common_start_c", null ],
        [ "mcu/hal_api Low level hardware API", "dd/d9c/programming_interface.html#mcu_hal_api", null ],
        [ "mcu/<hardware>", "dd/d9c/programming_interface.html#mcu_specific_files", null ],
        [ "Linker file", "dd/d9c/programming_interface.html#linker_file", null ],
        [ "Flash Memory", "dd/d9c/programming_interface.html#flash_memory", null ],
        [ "RAM memory", "dd/d9c/programming_interface.html#ram_memory", null ]
      ] ],
      [ "Utility and helper services", "dd/d9c/programming_interface.html#util_api", null ]
    ] ],
    [ "Application Examples and Tools", "d5/d7b/application_examples.html", [
      [ "Application examples", "d5/d7b/application_examples.html#source_folder", [
        [ "app.c", "d5/d7b/application_examples.html#source_app_c", null ],
        [ "config.mk", "d5/d7b/application_examples.html#source_config_mk", null ],
        [ "app_specific_area_id", "d5/d7b/application_examples.html#app_specific_area_id", null ],
        [ "Application version", "d5/d7b/application_examples.html#app_version", null ],
        [ "TARGET_BOARDS", "d5/d7b/application_examples.html#app_target_boards", null ],
        [ "makefile", "d5/d7b/application_examples.html#source_makefile", null ],
        [ "APP_PRINTING", "d5/d7b/application_examples.html#source_makefile_app_printing", null ],
        [ "APP_SCHEDULER", "d5/d7b/application_examples.html#source_makefile_app_scheduler", null ],
        [ "CFLAGS", "d5/d7b/application_examples.html#source_makefile_cflags", null ],
        [ "HAL_GPIO", "d5/d7b/application_examples.html#source_makefile_hal_gpio", null ],
        [ "HAL_BUTTON", "d5/d7b/application_examples.html#source_makefile_hal_button", null ],
        [ "HAL_HW_DELAY", "d5/d7b/application_examples.html#source_makefile_hal_hw_delay", null ],
        [ "HAL_I2C", "d5/d7b/application_examples.html#source_makefile_hal_i2c", null ],
        [ "HAL_LED", "d5/d7b/application_examples.html#source_makefile_hal_led", null ],
        [ "HAL_PERSISTENT_MEMORY", "d5/d7b/application_examples.html#source_makefile_hal_persistent_memory", null ],
        [ "HAL_SPI", "d5/d7b/application_examples.html#source_makefile_hal_spi", null ],
        [ "HAL_UART", "d5/d7b/application_examples.html#source_makefile_hal_uart", null ],
        [ "INCLUDES", "d5/d7b/application_examples.html#source_makefile_includes", null ],
        [ "LDFLAGS", "d5/d7b/application_examples.html#source_makefile_ldflags", null ],
        [ "LIBS", "d5/d7b/application_examples.html#source_makefile_libs", null ],
        [ "PROVISIONING", "d5/d7b/application_examples.html#source_makefile_provisioning", null ],
        [ "PROVISIONING_PROXY", "d5/d7b/application_examples.html#source_makefile_provisioning_proxy", null ],
        [ "SHARED_DATA", "d5/d7b/application_examples.html#source_makefile_shared_data", null ],
        [ "SRCS", "d5/d7b/application_examples.html#source_makefile_srcs", null ],
        [ "SW_AES", "d5/d7b/application_examples.html#source_makefile_sw_aes", null ]
      ] ],
      [ "Tools", "d5/d7b/application_examples.html#tools_folder", [
        [ "genscratchpad.py", "d5/d7b/application_examples.html#genscratchpad_py", null ],
        [ "INI_FILE", "d5/d7b/application_examples.html#config_mk_ini_file", null ]
      ] ]
    ] ],
    [ "Data Structures", "annotated.html", "annotated" ],
    [ "Data Fields", "functions.html", [
      [ "All", "functions.html", "functions_dup" ],
      [ "Functions", "functions_func.html", null ],
      [ "Variables", "functions_vars.html", "functions_vars" ]
    ] ],
    [ "File List", "files.html", "files" ],
    [ "Globals", "globals.html", [
      [ "All", "globals.html", "globals_dup" ],
      [ "Functions", "globals_func.html", "globals_func" ],
      [ "Variables", "globals_vars.html", null ],
      [ "Typedefs", "globals_type.html", null ],
      [ "Enumerations", "globals_enum.html", null ],
      [ "Enumerator", "globals_eval.html", "globals_eval" ],
      [ "Macros", "globals_defs.html", "globals_defs" ]
    ] ],
    [ "Example applications", "examples.html", "examples" ]
  ] ]
];

var NAVTREEINDEX =
[
"annotated.html",
"d2/d9e/cbor_8h.html#a4516af75999d76958b9dccf27611f661",
"d3/dbf/aessw_8h.html#a3e4748528dcc1a0dd120de622084f695",
"d4/dd5/indication__signal_8h_source.html",
"d6/d15/wms__memory__area_8h.html#a315eaca5f84b1f4cf42ea15ed82e32ff",
"d6/da5/attribute__frames_8h.html#a727941d0a408460f847e0d4de02562af",
"d8/d21/msap__frames_8h.html#a746ab8c576a3c936ac0bddf2f4f6ed05",
"d9/d53/wms__time_8h.html#a15e994fb5d9a74436a5d4771b4e13d13",
"da/dc2/sl__list_8h.html#a83852438122eef689f7b7f4f6fc799ec",
"dd/d2c/bl__interface_8h.html#a3819ab9bd420adcf9b7e2df3671190cf",
"dd/ddc/app__scheduler_8h.html#a245a7f2a478b72e2d4a43b3c18676ab3a2379eda407c46922cc110cb05f4c93c7",
"df/d6b/wms__system_8h.html#a055fd8a1c1565c12af35cd82010b2594a5c05e8f4e8d77c95c01bfb4014accf18",
"globals_defs_f.html"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';