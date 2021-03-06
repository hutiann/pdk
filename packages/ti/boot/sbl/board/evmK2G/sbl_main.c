/*
 * Copyright (C) 2016 Texas Instruments Incorporated - http://www.ti.com/
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

#include <ti/board/board.h>
#include <ti/drv/uart/UART_stdio.h>
#include "sbl_soc.h"
#include "sbl_slave_core_boot.h"
#include "sbl_ver.h"

#ifdef SECURE_BOOT
#include "sbl_sec.h"
#include <ti/csl/arch/a15/V0/csl_a15.h>
#include <ti/board/board.h>
#endif

sblEntryPoint_t entry;
typedef void (*EntryFunPtr_t)(void);


int main() {
    void (*func_ptr)(void);

#ifdef SECURE_BOOT
    /* install boot monitor */
    SBL_mon_init(BOARD_SOC_INPUT_CLK_FREQ);

    /* install DSP Secure Server */
    SBL_startDspSecSrv();
#endif
    
    SBL_socInit();

    UART_printf("\n**** PDK SBL ****\n");
    UART_printf("%s (%s - %s)\n", SBL_VERSION_STR, __DATE__, __TIME__);

    UART_printf("Begin parsing user application\n");
    if (E_PASS == SBL_ImageCopy(&entry))
    {
        UART_printf("Jumping to user application...\n");
    }

    if (entry.entryPoint_DSP0 != 0)
    {

#ifdef SECURE_BOOT
        /* setting the boot magic of GEM core 0 */
        *(uint32_t*)(GEM_MAGIC(0)) = entry.entryPoint_DSP0;

        /* cache flush */
        CSL_a15WbInvDataCache((void*)GEM_MAGIC(0), 4);
#else
        SBL_DSPBringUp(0, entry.entryPoint_DSP0);
#endif
    }

    /*Jump to MPU CPU0 APP*/
    if (entry.entryPoint_MPU_CPU0 != 0)
    {
        func_ptr = (EntryFunPtr_t) entry.entryPoint_MPU_CPU0;
        func_ptr();
    }
    else
    {
        while(1);
    }

    return 0;
}

