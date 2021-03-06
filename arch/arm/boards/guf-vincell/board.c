/*
 * Copyright (C) 2007 Sascha Hauer, Pengutronix
 * Copyright (C) 2011 Marc Kleine-Budde <mkl@pengutronix.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#include <common.h>
#include <environment.h>
#include <init.h>
#include <linux/sizes.h>
#include <bbu.h>
#include <gpio.h>
#include <io.h>
#include <linux/clk.h>

#include <mach/devices-imx53.h>
#include <mach/generic.h>
#include <mach/iim.h>
#include <mach/bbu.h>
#include <mach/imx5.h>

#define LOCO_FEC_PHY_RST		IMX_GPIO_NR(7, 6)

static void vincell_fec_reset(void)
{
	gpio_direction_output(LOCO_FEC_PHY_RST, 0);
	mdelay(1);
	gpio_set_value(LOCO_FEC_PHY_RST, 1);
}

static int vincell_devices_init(void)
{
	if (!of_machine_is_compatible("guf,imx53-vincell"))
		return 0;

	writel(0, MX53_M4IF_BASE_ADDR + 0xc);

	clk_set_rate(clk_lookup("emi_slow_podf"), 133333334);
	clk_set_rate(clk_lookup("nfc_podf"), 33333334);

	vincell_fec_reset();

	imx53_bbu_internal_nand_register_handler("nand",
		BBU_HANDLER_FLAG_DEFAULT, SZ_512K);

	return 0;
}

coredevice_initcall(vincell_devices_init);
