/**
 *  \file   k2l/I2C_soc.c
 *
 *  \brief  K2L SoC specific I2C hardware attributes.
 *
 *   This file contains the hardware attributes of I2C peripheral like
 *   base address, interrupt number etc.
 */

/*
 * Copyright (C) 2015 - 2019 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <stdint.h>
#include <ti/csl/soc.h>
#include <ti/csl/csl_device_interrupt.h>
#include <ti/drv/i2c/I2C.h>
#include <ti/drv/i2c/soc/I2C_soc.h>
#include <ti/csl/src/ip/i2c/V0/i2c.h>

/* I2C configuration structure */
I2C_HwAttrs i2cInitCfg[CSL_I2C_PER_CNT] =
{
    {
        CSL_I2C_0_DATA_CFG_REGS,
#ifdef _TMS320C6X
        9,  /* default DSP Interrupt vector number, can be set in I2C_socSetInitCfg() API */
        77, /* default DSP INTC I2C Event ID, can be set in I2C_socSetInitCfg() API */
        0, /* CIC number 0 */
        CSL_CIC0_I2C_0_INT, /* CIC UART Event ID */
        11,  /* default CIC Host Interrupt, map to CIC_OUT11_PLUS_16_MUL_N event */
#else
        CSL_ARM_GIC_I2C_0_INT + 32, /* I2C int0 number for ARM GIC INTC */
        0, /* Event ID not used for ARM INTC */
        INVALID_INTC_MUX_NUM, /* CIC num not used in ARM GIC */
        0, /* cicEventId not used for ARM */
        0, /* HostIntNum not used for ARM */
#endif
        (983000000U/6U),  /* default I2C frequency, system clock/6 */
        true, /* interrupt mode enabled */
        I2C_OWN_ADDR, /* default I2C own slave addresse */
    },
    {
        CSL_I2C_1_DATA_CFG_REGS,
#ifdef _TMS320C6X
        10,
        78,
        0,
        CSL_CIC0_I2C_1_INT,
        12,
#else
        CSL_ARM_GIC_I2C_1_INT + 32,
        0,
        INVALID_INTC_MUX_NUM, /* CIC num not used in ARM GIC */
        0,
        0,
#endif
        (983000000U/6U),  /* default I2C frequency, system clock/6 */
        true,
        I2C_OWN_ADDR,
    },
    {
        CSL_I2C_2_DATA_CFG_REGS,
#ifdef _TMS320C6X
        11,
        79,
        0,
        CSL_CIC0_I2C_2_INT,
        13,
#else
        CSL_ARM_GIC_I2C_2_INT + 32,
        0,
        INVALID_INTC_MUX_NUM, /* CIC num not used in ARM GIC */
        0,
        0,
#endif
        (983000000U/6U),  /* default I2C frequency, system clock/6 */
        true,
        I2C_OWN_ADDR,
    }
};

/* I2C objects */
I2C_v0_Object I2cObjects[CSL_I2C_PER_CNT];


/* I2C configuration structure */
CSL_PUBLIC_CONST I2C_config_list I2C_config = {
    {
        &I2C_v0_FxnTable,
        &I2cObjects[0],
        &i2cInitCfg[0]
    },

    {
         &I2C_v0_FxnTable,
         &I2cObjects[1],
         &i2cInitCfg[1]
    },

    {
         &I2C_v0_FxnTable,
         &I2cObjects[2],
         &i2cInitCfg[2]
    },
    /*"pad to full predefined length of array"*/
    {NULL, NULL, NULL},
    {NULL, NULL, NULL},
    {NULL, NULL, NULL},
    {NULL, NULL, NULL},
    {NULL, NULL, NULL},
    {NULL, NULL, NULL},
    {NULL, NULL, NULL},
    {NULL, NULL, NULL},
    {NULL, NULL, NULL},
    {NULL, NULL, NULL},
    {NULL, NULL, NULL}
};

/**
 * \brief  This API gets the SoC level of I2C intial configuration
 *
 * \param  index     I2C instance index.
 * \param  cfg       Pointer to I2C SOC initial config.
 *
 * \return 0 success: -1: error
 *
 */
int32_t I2C_socGetInitCfg(uint32_t index, I2C_HwAttrs *cfg)
{
	int32_t ret = 0;

	if (index < CSL_I2C_PER_CNT)
    {
        *cfg = i2cInitCfg[index];
    }
    else
    {
        ret = -1;
    }

    return ret;
}

/**
 * \brief  This API sets the SoC level of I2C intial configuration
 *
 * \param  index     I2C instance index.
 * \param  cfg       Pointer to I2C SOC initial config.
 *
 * \return           0 success: -1: error
 *
 */
int32_t I2C_socSetInitCfg(uint32_t index, const I2C_HwAttrs *cfg)
{
	int32_t ret = 0;

	if (index < CSL_I2C_PER_CNT)
    {
        i2cInitCfg[index] = *cfg;
    }
    else
    {
        ret = -1;
    }

    return ret;
}

