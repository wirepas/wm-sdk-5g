
#ifndef _SDK_ENVIRONMENT_H_
#define _SDK_ENVIRONMENT_H_

/**
@page sdk_environment SDK Environment setup

As a prerequisite for this guide, you must be able to successfully build
one of the provided application example from the SDK.

This page contains following sections:
- @subpage installation_of_sdk_environment
- @subpage flashing_guideline
- @subpage nordic_resources
- @subpage nordic_resources_nRF52_and_nRF54L

@section installation_of_sdk_environment Installation of SDK Environment

To ease the management of SDK environement, Wirepas maintains a docker image with
all the required dependencies installed.

For more information to use it, please read guidance from <a href="https://developer.wirepas.com/support/solutions/articles/77000435375">
Wirepas Helpdesk.</a>

It is also possible to install requirement in your native environement but it is not described here.
Requirement are listed in Github SDK main page under Requirement section.

@section flashing_guideline Flashing devices

Checkout flashing guidance from <a href="https://developer.wirepas.com/support/solutions/articles/77000465762">
Wirepas Helpdesk.</a>

@section nordic_resources Resources on Nordic nRF91x1

The nRF91x1 chip version supported by Wirepas Mesh are nRF9131, nRF9151 and nRF9161.
These have minimum 1024kB of flash and 256kB of RAM.

This page contains following sections:
- @subpage flash_memory_nrf91x1
- @subpage ram_memory_nrf91x1
- @subpage peripherals_accessible_by_stack_only_nrf91x1
- @subpage peripherals_shared_between_the_stack_and_the_application_nrf91x1
- @subpage peripherals_available_for_the_application_nrf91x1

@subsection flash_memory_nrf91x1 Flash Memory available for application on nRF91x1

As stated in [description of memory partitioning](@ref memory_partitioning), the
available flash memory for application is limited by size of the memory area
that is used commonly for application and also scratchpad image. If application
size is too large, there is possibility that large scratchpad image will
override application image. The default maximum size of the application has been
set so that it is always safe to use scratchpad image that will contain both
firmware and application.

The _recommended_ maximum size of [flash memory](@ref flash_memory) for an
application, according to processor type is following:

<table>
<tr><th>Processor</th><th>Flash memory</th></tr>
<tr><td>nRF9131</td><td>752 kB</td></tr>
<tr><td>nRF9151</td><td>752 kB</td></tr>
<tr><td>nRF9161</td><td>752 kB</td></tr>
</table>

@subsection ram_memory_nrf91x1 RAM Memory available for application on nRF91x1

Allocated [RAM memory](@ref ram_memory) for application by the processor is
following:
<table>
<tr><th>Processor</th><th>RAM memory</th></tr>
<tr><td>nRF9131</td><td>155 kB</td></tr>
<tr><td>nRF9151</td><td>155 kB</td></tr>
<tr><td>nRF9161</td><td>155 kB</td></tr>
</table>

@subsection peripherals_accessible_by_stack_only_nrf91x1 Peripherals accessible by stack only

Some peripherals are used by the Wirepas Mesh stack and cannot be
used by the application.

<table>
<tr><th>Peripheral</th><th>Associated interrupt (from file @ref mcu/nrf/common/vendor/mdk/nrf9120.h)</th></tr>
<tr><td>Power</td><td><code>CLOCK_POWER_IRQn</code></td></tr>
<tr><td>Radio</td><td><code>IPC_IRQn</code></td></tr>
<tr><td>Timer0</td><td><code>TIMER0_IRQn</code></td></tr>
<tr><td>WDT</td><td><code>WDT_IRQn</code></td></tr>
<tr><td>Rtc1</td><td><code>RTC1_IRQn</code></td></tr>
<tr><td>CRYPTOCELL (AES and RNG)</td><td><code>CRYPTOCELL_IRQn</code></td></tr>
<tr><td>PPI (Channels 0, 1 and 2)</td><td><code>None</code></td></tr>
</table>

All the internal interrupt of cortex M are handled by the stack
directly (NMI, HardFault,...)

@subsection peripherals_shared_between_the_stack_and_the_application_nrf91x1 Peripherals shared between the stack and the application

Some peripherals are used by the stack but can also be accessed by the
application.

@subsection peripherals_available_for_the_application_nrf91x1 Peripherals available for the application

All the other peripherals not listed above are free to be used by the application.










Related Material
================

@anchor relmat3 [3] WP-RM-108 - OTAP Reference Manual

@anchor relmat4 [4] https://github.com/wirepas/wm-sdk/blob/master/source/reference_apps/dualmcu_app/api/DualMcuAPI.md

*/


#endif /* API_DOC_SDK_ENVIRONMENT_H_ */
