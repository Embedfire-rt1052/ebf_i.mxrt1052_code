/*
 * The Clear BSD License
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 * that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v4.0
processor: MIMXRT1052xxxxx
package_id: MIMXRT1052DVL6B
mcu_data: i_mx_1_0
processor_version: 0.0.0
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "pin_mux.h"

/* FUNCTION ************************************************************************************************************
 * 
 * Function Name : BOARD_InitBootPins
 * Description   : Calls initialization functions.
 * 
 * END ****************************************************************************************************************/
void BOARD_InitBootPins(void) {
    BOARD_InitPins();
}

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: L14, peripheral: LPUART1, signal: RX, pin_signal: GPIO_AD_B0_13, software_input_on: Disable, hysteresis_enable: Disable, pull_up_down_config: Pull_Down_100K_Ohm,
    pull_keeper_select: Keeper, pull_keeper_enable: Enable, open_drain: Disable, speed: MHZ_100, drive_strength: R0_6, slew_rate: Slow}
  - {pin_num: K14, peripheral: LPUART1, signal: TX, pin_signal: GPIO_AD_B0_12, software_input_on: Disable, hysteresis_enable: Disable, pull_up_down_config: Pull_Down_100K_Ohm,
    pull_keeper_select: Keeper, pull_keeper_enable: Enable, open_drain: Disable, speed: MHZ_100, drive_strength: R0_6, slew_rate: Slow}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_Iomuxc);           /* iomuxc clock (iomuxc_clk_enable): 0x03u */

  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B0_02_GPIO1_IO02,        /* GPIO_AD_B0_02 is configured as GPIO1_IO02 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B0_12_LPUART1_TX,        /* GPIO_AD_B0_12 is configured as LPUART1_TX */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B0_13_LPUART1_RX,        /* GPIO_AD_B0_13 is configured as LPUART1_RX */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_B0_00_LCD_CLK,              /* GPIO_B0_00 is configured as LCD_CLK */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_B0_01_LCD_ENABLE,           /* GPIO_B0_01 is configured as LCD_ENABLE */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_B0_02_LCD_HSYNC,            /* GPIO_B0_02 is configured as LCD_HSYNC */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_B0_03_LCD_VSYNC,            /* GPIO_B0_03 is configured as LCD_VSYNC */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_B0_04_LCD_DATA00,           /* GPIO_B0_04 is configured as LCD_DATA00 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_B0_05_LCD_DATA01,           /* GPIO_B0_05 is configured as LCD_DATA01 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_B0_06_LCD_DATA02,           /* GPIO_B0_06 is configured as LCD_DATA02 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_B0_07_LCD_DATA03,           /* GPIO_B0_07 is configured as LCD_DATA03 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_B0_08_LCD_DATA04,           /* GPIO_B0_08 is configured as LCD_DATA04 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_B0_09_LCD_DATA05,           /* GPIO_B0_09 is configured as LCD_DATA05 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_B0_10_LCD_DATA06,           /* GPIO_B0_10 is configured as LCD_DATA06 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_B0_11_LCD_DATA07,           /* GPIO_B0_11 is configured as LCD_DATA07 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_B0_12_LCD_DATA08,           /* GPIO_B0_12 is configured as LCD_DATA08 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_B0_13_LCD_DATA09,           /* GPIO_B0_13 is configured as LCD_DATA09 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_B0_14_LCD_DATA10,           /* GPIO_B0_14 is configured as LCD_DATA10 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_B0_15_LCD_DATA11,           /* GPIO_B0_15 is configured as LCD_DATA11 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_B1_00_LCD_DATA12,           /* GPIO_B1_00 is configured as LCD_DATA12 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_B1_01_LCD_DATA13,           /* GPIO_B1_01 is configured as LCD_DATA13 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_B1_02_LCD_DATA14,           /* GPIO_B1_02 is configured as LCD_DATA14 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_B1_03_LCD_DATA15,           /* GPIO_B1_03 is configured as LCD_DATA15 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B0_15_GPIO1_IO15,           /* GPIO_B1_15 is configured as GPIO2_IO31 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_B0_02_GPIO1_IO02,        /* GPIO_AD_B0_02 PAD functional properties : */
      0x10B0u);                               /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_B0_12_LPUART1_TX,        /* GPIO_AD_B0_12 PAD functional properties : */
      0x10B0u);                               /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_B0_13_LPUART1_RX,        /* GPIO_AD_B0_13 PAD functional properties : */
      0x10B0u);                               /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_B0_00_LCD_CLK,              /* GPIO_B0_00 PAD functional properties : */
      0x01B0B0u);                             /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Enabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_B0_01_LCD_ENABLE,           /* GPIO_B0_01 PAD functional properties : */
      0x01B0B0u);                             /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Enabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_B0_02_LCD_HSYNC,            /* GPIO_B0_02 PAD functional properties : */
      0x01B0B0u);                             /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Enabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_B0_03_LCD_VSYNC,            /* GPIO_B0_03 PAD functional properties : */
      0x01B0B0u);                             /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Enabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_B0_04_LCD_DATA00,           /* GPIO_B0_04 PAD functional properties : */
      0x01B0B0u);                             /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Enabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_B0_05_LCD_DATA01,           /* GPIO_B0_05 PAD functional properties : */
      0x01B0B0u);                             /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Enabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_B0_06_LCD_DATA02,           /* GPIO_B0_06 PAD functional properties : */
      0x01B0B0u);                             /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Enabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_B0_07_LCD_DATA03,           /* GPIO_B0_07 PAD functional properties : */
      0x01B0B0u);                             /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Enabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_B0_08_LCD_DATA04,           /* GPIO_B0_08 PAD functional properties : */
      0x01B0B0u);                             /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Enabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_B0_09_LCD_DATA05,           /* GPIO_B0_09 PAD functional properties : */
      0x01B0B0u);                             /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Enabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_B0_10_LCD_DATA06,           /* GPIO_B0_10 PAD functional properties : */
      0x01B0B0u);                             /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Enabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_B0_11_LCD_DATA07,           /* GPIO_B0_11 PAD functional properties : */
      0x01B0B0u);                             /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Enabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_B0_12_LCD_DATA08,           /* GPIO_B0_12 PAD functional properties : */
      0x01B0B0u);                             /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Enabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_B0_13_LCD_DATA09,           /* GPIO_B0_13 PAD functional properties : */
      0x01B0B0u);                             /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Enabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_B0_14_LCD_DATA10,           /* GPIO_B0_14 PAD functional properties : */
      0x01B0B0u);                             /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Enabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_B0_15_LCD_DATA11,           /* GPIO_B0_15 PAD functional properties : */
      0x01B0B0u);                             /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Enabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_B1_00_LCD_DATA12,           /* GPIO_B1_00 PAD functional properties : */
      0x01B0B0u);                             /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Enabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_B1_01_LCD_DATA13,           /* GPIO_B1_01 PAD functional properties : */
      0x01B0B0u);                             /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Enabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_B1_02_LCD_DATA14,           /* GPIO_B1_02 PAD functional properties : */
      0x01B0B0u);                             /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Enabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_B1_03_LCD_DATA15,           /* GPIO_B1_03 PAD functional properties : */
      0x01B0B0u);                             /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Enabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_B0_15_GPIO1_IO15,           /* GPIO_B1_15 PAD functional properties : */
      0x10B0u);                               /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
}
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitCSIPins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: H13, peripheral: CSI, signal: 'csi_data, 09', pin_signal: GPIO_AD_B1_08}
  - {pin_num: M13, peripheral: CSI, signal: 'csi_data, 08', pin_signal: GPIO_AD_B1_09}
  - {pin_num: L13, peripheral: CSI, signal: 'csi_data, 07', pin_signal: GPIO_AD_B1_10}
  - {pin_num: J13, peripheral: CSI, signal: 'csi_data, 06', pin_signal: GPIO_AD_B1_11}
  - {pin_num: H12, peripheral: CSI, signal: 'csi_data, 05', pin_signal: GPIO_AD_B1_12}
  - {pin_num: H11, peripheral: CSI, signal: 'csi_data, 04', pin_signal: GPIO_AD_B1_13}
  - {pin_num: G12, peripheral: CSI, signal: 'csi_data, 03', pin_signal: GPIO_AD_B1_14}
  - {pin_num: J14, peripheral: CSI, signal: 'csi_data, 02', pin_signal: GPIO_AD_B1_15}
  - {pin_num: L12, peripheral: CSI, signal: csi_pixclk, pin_signal: GPIO_AD_B1_04}
  - {pin_num: K12, peripheral: CSI, signal: csi_mclk, pin_signal: GPIO_AD_B1_05}
  - {pin_num: J12, peripheral: CSI, signal: csi_vsync, pin_signal: GPIO_AD_B1_06}
  - {pin_num: K10, peripheral: CSI, signal: csi_hsync, pin_signal: GPIO_AD_B1_07}
  - {pin_num: J11, peripheral: LPI2C1, signal: SCL, pin_signal: GPIO_AD_B1_00, slew_rate: Slow, software_input_on: Enable, open_drain: Enable, speed: MHZ_100, drive_strength: R0_6,
    pull_keeper_select: Keeper, pull_keeper_enable: Enable, pull_up_down_config: Pull_Up_22K_Ohm, hysteresis_enable: Disable}
  - {pin_num: K11, peripheral: LPI2C1, signal: SDA, pin_signal: GPIO_AD_B1_01, slew_rate: Slow, software_input_on: Enable, open_drain: Enable, speed: MHZ_100, drive_strength: R0_6,
    pull_keeper_select: Keeper, pull_keeper_enable: Enable, pull_up_down_config: Pull_Up_22K_Ohm, hysteresis_enable: Disable}
  - {pin_num: F11, peripheral: GPIO1, signal: 'gpio_io, 04', pin_signal: GPIO_AD_B0_04}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitCSIPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/

void BOARD_InitCSIPins(void) {
  CLOCK_EnableClock(kCLOCK_Iomuxc);          /* iomuxc clock (iomuxc_clk_enable): 0x03u */

  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B0_00_GPIO1_IO00,        /* CSI_PWR GPIO_AD_B0_04 is configured as GPIO1_IO04 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B0_01_GPIO1_IO01,        /* CSI_RST GPIO_AD_B0_04 is configured as GPIO1_IO04 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */

	IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B1_00_LPI2C1_SCL,        /* GPIO_AD_B1_00 is configured as LPI2C1_SCL */
      1U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B1_01_LPI2C1_SDA,        /* GPIO_AD_B1_01 is configured as LPI2C1_SDA */
      1U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_B1_12_CSI_PIXCLK,        /* GPIO_AD_B1_04 is configured as CSI_PIXCLK */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B1_06_CSI_VSYNC,         /* GPIO_AD_B1_06 is configured as CSI_VSYNC */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B1_07_CSI_HSYNC,         /* GPIO_AD_B1_07 is configured as CSI_HSYNC */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B1_08_CSI_DATA09,        /* GPIO_AD_B1_08 is configured as CSI_DATA09 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B1_09_CSI_DATA08,        /* GPIO_AD_B1_09 is configured as CSI_DATA08 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B1_10_CSI_DATA07,        /* GPIO_AD_B1_10 is configured as CSI_DATA07 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B1_11_CSI_DATA06,        /* GPIO_AD_B1_11 is configured as CSI_DATA06 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B1_12_CSI_DATA05,        /* GPIO_AD_B1_12 is configured as CSI_DATA05 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B1_13_CSI_DATA04,        /* GPIO_AD_B1_13 is configured as CSI_DATA04 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B1_14_CSI_DATA03,        /* GPIO_AD_B1_14 is configured as CSI_DATA03 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B1_15_CSI_DATA02,        /* GPIO_AD_B1_15 is configured as CSI_DATA02 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_B1_00_LPI2C1_SCL,        /* GPIO_AD_B1_00 PAD functional properties : */
      0xD8B0u);                               /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Enabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 22K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_B1_01_LPI2C1_SDA,        /* GPIO_AD_B1_01 PAD functional properties : */
      0xD8B0u);                               /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Enabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 22K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
}

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
