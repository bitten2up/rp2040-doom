//
// Copyright(C) 1993-1996 Id Software, Inc.
// Copyright(C) 2005-2014 Simon Howard
// Copyright(C) 2021-2022 Graham Sanderson
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// DESCRIPTION:
//	Main program, simply calls D_DoomMain high level loop.
//

#include "config.h"

#include <stdlib.h>
#include <stdio.h>

#if !LIB_PICO_STDLIB
#include "SDL.h"
#else
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/sem.h"
#include "pico/multicore.h"
#if PICO_ON_DEVICE
#include "hardware/clocks.h"
#include "hardware/vreg.h"
#include "mipi_display.h"
#endif
#endif
#if USE_PICO_NET
#include "piconet.h"
#endif
#include "doomtype.h"
#include "i_system.h"
#include "m_argv.h"

//
// D_DoomMain()
// Not a globally visible function, just included for source reference,
// calls all startup code, parses command line options.
//

void D_DoomMain (void);

#if PICO_ON_DEVICE
#include "pico/binary_info.h"
bi_decl(bi_3pins_with_names(PICO_AUDIO_I2S_DATA_PIN, "I2S DIN", PICO_AUDIO_I2S_CLOCK_PIN_BASE, "I2S BCK", PICO_AUDIO_I2S_CLOCK_PIN_BASE+1, "I2S LRCK"))
#endif

int main(int argc, char **argv)
{
    // save arguments
#if !NO_USE_ARGS
    myargc = argc;
    myargv = argv;
#endif
#if PICO_ON_DEVICE
    vreg_set_voltage(VREG_VOLTAGE_1_10);
    sleep_ms(10);
#define PLL (MIPI_DISPLAY_SPI_CLOCK_SPEED_HZ/1000 * 4) // assume 60-70MHz
    set_sys_clock_khz(PLL, true);
    clock_configure(
            clk_peri,
            0,                                                // No glitchless mux
            CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLKSRC_PLL_SYS, // System PLL on AUX mux
            PLL*1000,                               // Input frequency
            PLL*1000                                // Output (must be same as no divider)
        );
#if !USE_PICO_NET
    // debug ?
//    gpio_debug_pins_init();
#endif
#ifdef PICO_SMPS_MODE_PIN
    gpio_init(PICO_SMPS_MODE_PIN);
    gpio_set_dir(PICO_SMPS_MODE_PIN, GPIO_OUT);
    gpio_put(PICO_SMPS_MODE_PIN, 1);
#endif
    gpio_init(DOOM_GPIO_UP);
    gpio_set_dir(DOOM_GPIO_UP, GPIO_IN);

    gpio_init(DOOM_GPIO_DOWN);
    gpio_set_dir(DOOM_GPIO_DOWN, GPIO_IN);

    gpio_init(DOOM_GPIO_LEFT);
    gpio_set_dir(DOOM_GPIO_LEFT, GPIO_IN);

    gpio_init(DOOM_GPIO_RIGHT);
    gpio_set_dir(DOOM_GPIO_RIGHT, GPIO_IN);

    gpio_init(DOOM_GPIO_A);
    gpio_set_dir(DOOM_GPIO_A, GPIO_IN);

    gpio_init(DOOM_GPIO_B);
    gpio_set_dir(DOOM_GPIO_B, GPIO_IN);

    gpio_init(DOOM_GPIO_X);
    gpio_set_dir(DOOM_GPIO_X, GPIO_IN);

    gpio_init(DOOM_GPIO_Y);
    gpio_set_dir(DOOM_GPIO_RIGHT, GPIO_IN);
#endif
#if LIB_PICO_STDIO
    stdio_init_all();
#if LIB_PICO_STDIO_USB
    sleep_ms(1500);
#endif
#endif
#if PICO_BUILD
    I_Init();
#endif
#if USE_PICO_NET
    // do init early to set pulls
    piconet_init();
#endif
//!
    // Print the program version and exit.
    //
    if (M_ParmExists("-version") || M_ParmExists("--version")) {
        puts(PACKAGE_STRING);
        exit(0);
    }

#if !NO_USE_ARGS
    M_FindResponseFile();
#endif

    #ifdef SDL_HINT_NO_SIGNAL_HANDLERS
    SDL_SetHint(SDL_HINT_NO_SIGNAL_HANDLERS, "1");
    #endif

    // start doom

    D_DoomMain ();

    return 0;
}

