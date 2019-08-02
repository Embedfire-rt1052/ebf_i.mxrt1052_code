/*
 * Copyright (c) 2017-2018, NXP Semiconductors, Inc.
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_codec_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
status_t CODEC_I2C_WriteReg(uint8_t i2cAddr,
                            codec_reg_addr_t addrType,
                            uint32_t reg,
                            codec_reg_width_t regWidth,
                            uint32_t value,
                            codec_i2c_send_func_t i2cSendFunc)
{
    uint8_t data[4];
    uint8_t i;

    i = regWidth;
    while (i--)
    {
        data[i] = (uint8_t)value;
        value >>= 8;
    }

    return i2cSendFunc(i2cAddr, reg, addrType, data, regWidth);
}

/*!
 * @brief Read the register value.
 *
 * @param i2cAddr I2C address.
 * @param addrType Register address type.
 * @param reg The register to read.
 * @param regWidth The width of the register.
 * @param value The value read out.
 * @param i2cReceiveFunc The actual I2C receive function.
 * @return Returns @ref kStatus_Success if success, otherwise returns error code.
 */
status_t CODEC_I2C_ReadReg(uint8_t i2cAddr,
                           codec_reg_addr_t addrType,
                           uint32_t reg,
                           codec_reg_width_t regWidth,
                           void *value,
                           codec_i2c_receive_func_t i2cReceiveFunc)
{
    uint8_t data[4];
    uint8_t i = 0;
    status_t status;
    uint8_t width = (uint8_t) regWidth;

    status = i2cReceiveFunc(i2cAddr, reg, addrType, data, width);

    if (kStatus_Success == status)
    {
        while (width--)
        {
            ((uint8_t *)value)[i++] = data[width];
        }
    }

    return status;
}

status_t CODEC_I2C_ModifyReg(uint8_t i2cAddr,
                             codec_reg_addr_t addrType,
                             uint32_t reg,
                             codec_reg_width_t regWidth,
                             uint32_t clrMask,
                             uint32_t value,
                             codec_i2c_receive_func_t i2cReceiveFunc,
                             codec_i2c_send_func_t i2cSendFunc)
{
    status_t status;
    uint32_t regVal;

    status = CODEC_I2C_ReadReg(i2cAddr, addrType, reg, regWidth, &regVal, i2cReceiveFunc);

    if (kStatus_Success != status)
    {
        return status;
    }

    regVal = (regVal & ~(clrMask)) | (value & clrMask);

    return CODEC_I2C_WriteReg(i2cAddr, addrType, reg, regWidth, regVal, i2cSendFunc);
}


//codec_config_t boardCodecConfig = {.I2C_SendFunc = BOARD_Codec_I2C_Send,
//                                   .I2C_ReceiveFunc = BOARD_Codec_I2C_Receive,
//                                   .op.Init = WM8960_Init,
//                                   .op.Deinit = WM8960_Deinit,
//                                   .op.SetFormat = WM8960_ConfigDataFormat};


///*! @brief Initialize structure of WM8960 */
//typedef struct codec_config
//{
//    /* Pointer to the user-defined I2C Send Data function. */
//    codec_i2c_send_func_t I2C_SendFunc;
//    /* Pointer to the user-defined I2C Receive Data function. */
//    codec_i2c_receive_func_t I2C_ReceiveFunc;
//    void *codecConfig; /* Codec specific configuration */
//    codec_operation_t op;
//} codec_config_t;

status_t CODEC_Init(codec_handle_t *handle, codec_config_t *config)
{
    /* Set the handle information */
    handle->I2C_SendFunc = config->I2C_SendFunc;
    handle->I2C_ReceiveFunc = config->I2C_ReceiveFunc;
    memcpy(&handle->op, &config->op, sizeof(codec_operation_t));
    return handle->op.Init(handle, config->codecConfig);
}
