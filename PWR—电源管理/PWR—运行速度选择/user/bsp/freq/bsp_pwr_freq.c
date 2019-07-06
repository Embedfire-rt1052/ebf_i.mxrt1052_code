/**
  ******************************************************************
  * @file
  * @author  fire
  * @version V2.0
  * @date    2018-xx-xx
  * @brief
  ******************************************************************
  * @attention
  *
  * 实验平台:野火  i.MXRT1052开发板
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************
  */
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"
#include "fsl_elcdif.h"
#include "fsl_clock.h"
#include "fsl_debug_console.h"

#include "pad_config.h"

#include "./freq/bsp_pwr_freq.h"
#include "lpm.h"


