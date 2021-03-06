/**
 *  \file   board_gpioLed.c
 *
 *  \brief:
 *  GPIO LED configurations
 *
 *
 */

/* Copyright (C) 2012 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 */

#include <stdio.h>
#include <ti/drv/gpio/GPIO.h>
#include <ti/csl/soc.h>
#include <ti/drv/gpio/soc/GPIO_soc.h>
#include <ti/starterware/include/am43xx/chipdb_defs.h>
#include <ti/drv/iolink/test/stack_test/src/board_gpioLed.h>


extern int32_t PINMUXModuleConfig(chipdbModuleID_t moduleId, uint32_t instNum,
                                  void *pParam1);

/***********************************************************************/
/* Macros                                        */
/***********************************************************************/

/* GPIO Driver board specific pin configuration structure */
GPIO_PinConfig gpioPinConfigs[] =
{
    /* Output pin : AM437x_IDK_RED0_LED */
    GPIO_DEVICE_CONFIG(2u + 1u, 24u) |
    GPIO_CFG_OUTPUT ,

    /* Output pin :AM437x_IDK_GRN0_LED */
    GPIO_DEVICE_CONFIG(2u + 1u, 25u) |
    GPIO_CFG_OUTPUT,

    /* Output pin : AM437x_IDK_YEL0_LED */
    GPIO_DEVICE_CONFIG(2u + 1u, 22u) |
    GPIO_CFG_OUTPUT,

    /* Output pin : AM437x_IDK_RED1_LED */
    GPIO_DEVICE_CONFIG(2u + 1u, 23u) |
    GPIO_CFG_OUTPUT ,

    /* Output pin : AM437x_IDK_GRN1_LED */
    GPIO_DEVICE_CONFIG(4u + 1u, 13u) |
    GPIO_CFG_OUTPUT,

    /* Output pin : AM437x_IDK_YEL1_LED */
    GPIO_DEVICE_CONFIG(4u + 1u, 16u) |
    GPIO_CFG_OUTPUT,

    /* Output pin : SN65HVS882 LD pin */
    GPIO_DEVICE_CONFIG(3u + 1u, 1u) |
    GPIO_CFG_OUTPUT,

    /* Output pin : IO Link Enable Port 0 */
    GPIO_DEVICE_CONFIG(1u + 1u, 9u) | GPIO_CFG_OUTPUT,

    /* Output pin : IO Link Enable Port 1 */
    GPIO_DEVICE_CONFIG(0u + 1u, 3u) | GPIO_CFG_OUTPUT,

    /* Output pin : IO Link Enable Port 2 */
    GPIO_DEVICE_CONFIG(1u + 1u, 14u) | GPIO_CFG_OUTPUT,

    /* Output pin : IO Link Enable Port 3 */
    GPIO_DEVICE_CONFIG(1u + 1u, 15u) | GPIO_CFG_OUTPUT,

    /* Output pin : IO Link Enable Port 4 */
    GPIO_DEVICE_CONFIG(1u + 1u, 12u) | GPIO_CFG_OUTPUT,

    /* Output pin : IO Link Enable Port 5 */
    GPIO_DEVICE_CONFIG(1u + 1u, 13u) | GPIO_CFG_OUTPUT,

    /* Output pin : IO Link Enable Port 6 */
    GPIO_DEVICE_CONFIG(5u + 1u, 1u) | GPIO_CFG_OUTPUT,

    /* Output pin : IO Link Enable Port 7 */
    GPIO_DEVICE_CONFIG(2u + 1u, 12u) | GPIO_CFG_OUTPUT,

    /* Output pin : TPS4H160 L_SEL 15*/
    GPIO_DEVICE_CONFIG(5u + 1u, 20u) | GPIO_CFG_OUTPUT,

    /* Output pin : TPS4H160 L_SEH 16*/
    GPIO_DEVICE_CONFIG(4u + 1u, 21u) | GPIO_CFG_OUTPUT,

    /* Output pin : TPS4H160 H_SEL 17*/
    GPIO_DEVICE_CONFIG(0u + 1u, 15u) | GPIO_CFG_OUTPUT,

    /* Output pin : TPS4H160 H_SEH 18*/
    GPIO_DEVICE_CONFIG(4u + 1u, 14u) | GPIO_CFG_OUTPUT,

    /* Input pin : TPS4H160 L_Fault 19*/
    GPIO_DEVICE_CONFIG(4u + 1u, 15u) | GPIO_CFG_INPUT,

    /* Input pin : TPS4H160 H_Fault 20*/
    GPIO_DEVICE_CONFIG(0u + 1u, 14u) | GPIO_CFG_INPUT,
};

/* GPIO Driver call back functions */
GPIO_CallbackFxn gpioCallbackFunctions[] =
{
    NULL
};

/* GPIO Driver configuration structure */
GPIO_v1_Config GPIO_v1_config =
{
    gpioPinConfigs,
    gpioCallbackFunctions,
    sizeof(gpioPinConfigs) / sizeof(GPIO_PinConfig),
    sizeof(gpioCallbackFunctions) / sizeof(GPIO_CallbackFxn),
    0x2U,
};

void gpioLedPinmuxConfig()
{
    PINMUXModuleConfig(CHIPDB_MOD_ID_GPIO, 2U, NULL);
}

void  Board_setTriColorLED(uint32_t gpioLeds, uint8_t value)
{
    if(gpioLeds & BOARD_TRICOLOR0_RED)
    {
        GPIO_write(0, value);
    }

    if(gpioLeds & BOARD_TRICOLOR0_GREEN)
    {
        GPIO_write(1, value);
    }

    if(gpioLeds & BOARD_TRICOLOR0_YELLOW)
    {
        GPIO_write(2, value);
    }

    if(gpioLeds & BOARD_TRICOLOR1_RED)
    {
        GPIO_write(3, value);
    }

    if(gpioLeds & BOARD_TRICOLOR1_GREEN)
    {
        GPIO_write(4, value);
    }

    if(gpioLeds & BOARD_TRICOLOR1_YELLOW)
    {
        GPIO_write(5, value);
    }
}
