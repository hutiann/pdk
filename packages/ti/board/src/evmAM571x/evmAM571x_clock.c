/******************************************************************************
 * Copyright (c) 2010-2015 Texas Instruments Incorporated - http://www.ti.com
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *****************************************************************************/

#include "board_internal.h"

/* Set the desired DDR3 configuration -- assumes 66.67 MHz DDR3 clock input */
Board_STATUS Board_moduleClockInit()
{
    CSL_cam_cm_coreRegs *camCmReg =
        (CSL_cam_cm_coreRegs *) CSL_MPU_CAM_CM_CORE_REGS;
    CSL_l4per_cm_core_componentRegs *l4PerCmReg =
        (CSL_l4per_cm_core_componentRegs *) CSL_MPU_L4PER_CM_CORE_REGS;
    CSL_l3init_cm_coreRegs *l3InitCmReg =
        (CSL_l3init_cm_coreRegs *) CSL_MPU_L3INIT_CM_CORE_REGS;
    CSL_core_cm_coreRegs *coreCmReg =
        (CSL_core_cm_coreRegs *) CSL_MPU_CORE_CM_CORE_REGS;
    CSL_coreaon_cm_coreRegs *coreAonCmReg =
        (CSL_coreaon_cm_coreRegs *) CSL_MPU_COREAON_CM_CORE_REGS;
    CSL_dss_cm_coreRegs *dssCmReg =
        (CSL_dss_cm_coreRegs *) CSL_MPU_DSS_CM_CORE_REGS;
    CSL_ipu_cm_core_aonRegs *ipuCmReg =
        (CSL_ipu_cm_core_aonRegs *) CSL_MPU_IPU_CM_CORE_AON_REGS;
    CSL_rtc_cm_core_aonRegs *rtcCmReg =
        (CSL_rtc_cm_core_aonRegs *) CSL_MPU_RTC_CM_CORE_AON_REGS;
    CSL_vpe_cm_core_aonRegs *vpeCmReg =
        (CSL_vpe_cm_core_aonRegs *) CSL_MPU_VPE_CM_CORE_AON_REGS;
    CSL_wkupaon_cmRegs *wkupAonCmReg =
        (CSL_wkupaon_cmRegs *) CSL_MPU_WKUPAON_CM_REGS;
    CSL_mpu_cm_core_aonRegs *mpuCmReg =
        (CSL_mpu_cm_core_aonRegs *) CSL_MPU_MPU_CM_CORE_AON_REGS;
    CSL_dsp1_cm_core_aonRegs *dsp1CmReg =
        (CSL_dsp1_cm_core_aonRegs *) CSL_MPU_DSP1_CM_CORE_AON_REGS;
    CSL_dsp2_cm_core_aonRegs *dsp2CmReg =
        (CSL_dsp2_cm_core_aonRegs *) CSL_MPU_DSP2_CM_CORE_AON_REGS;
    CSL_iva_cm_coreRegs *ivaCmReg =
        (CSL_iva_cm_coreRegs *) CSL_MPU_IVA_CM_CORE_REGS;

    /* PRCM clock domain state setting functions */
    CSL_FINST(camCmReg->CM_CAM_CLKSTCTRL_REG,
        CAM_CM_CORE_CM_CAM_CLKSTCTRL_REG_CLKTRCTRL, SW_WKUP);
/*
    while(CSL_CAM_CM_CORE_CM_CAM_CLKSTCTRL_REG_CLKACTIVITY_VIP1_GCLK_ACT !=
       CSL_FEXT(camCmReg->CM_CAM_CLKSTCTRL_REG,
        CAM_CM_CORE_CM_CAM_CLKSTCTRL_REG_CLKACTIVITY_VIP1_GCLK));
*/

    CSL_FINST(l4PerCmReg->CM_L4PER_CLKSTCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER_CLKSTCTRL_REG_CLKTRCTRL, SW_WKUP);

    while(CSL_L4PER_CM_CORE_COMPONENT_CM_L4PER_CLKSTCTRL_REG_CLKACTIVITY_L4PER_L3_GICLK_ACT !=
       CSL_FEXT(l4PerCmReg->CM_L4PER_CLKSTCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER_CLKSTCTRL_REG_CLKACTIVITY_L4PER_L3_GICLK));

    CSL_FINST(l4PerCmReg->CM_L4PER2_CLKSTCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER2_CLKSTCTRL_REG_CLKTRCTRL, SW_WKUP);
/*
    while(CSL_L4PER_CM_CORE_COMPONENT_CM_L4PER2_CLKSTCTRL_REG_CLKACTIVITY_ICSS_CLK_ACT !=
       CSL_FEXT(l4PerCmReg->CM_L4PER2_CLKSTCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER2_CLKSTCTRL_REG_CLKACTIVITY_ICSS_CLK));
*/

    CSL_FINST(l4PerCmReg->CM_L4PER3_CLKSTCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER3_CLKSTCTRL_REG_CLKTRCTRL, SW_WKUP);

    while(CSL_L4PER_CM_CORE_COMPONENT_CM_L4PER3_CLKSTCTRL_REG_CLKACTIVITY_L4PER3_L3_GICLK_ACT !=
       CSL_FEXT(l4PerCmReg->CM_L4PER3_CLKSTCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER3_CLKSTCTRL_REG_CLKACTIVITY_L4PER3_L3_GICLK));

    CSL_FINST(l3InitCmReg->CM_L3INIT_CLKSTCTRL_REG,
        L3INIT_CM_CORE_CM_L3INIT_CLKSTCTRL_REG_CLKTRCTRL, SW_WKUP);

    while(CSL_L3INIT_CM_CORE_CM_L3INIT_CLKSTCTRL_REG_CLKACTIVITY_L3INIT_L3_GICLK_ACT !=
       CSL_FEXT(l3InitCmReg->CM_L3INIT_CLKSTCTRL_REG,
        L3INIT_CM_CORE_CM_L3INIT_CLKSTCTRL_REG_CLKACTIVITY_L3INIT_L3_GICLK));

    CSL_FINST(l3InitCmReg->CM_GMAC_CLKSTCTRL_REG,
        L3INIT_CM_CORE_CM_GMAC_CLKSTCTRL_REG_CLKTRCTRL, SW_WKUP);
/*
    while(CSL_L3INIT_CM_CORE_CM_GMAC_CLKSTCTRL_REG_CLKACTIVITY_GMII_250MHZ_CLK_ACT !=
       CSL_FEXT(l3InitCmReg->CM_GMAC_CLKSTCTRL_REG,
        L3INIT_CM_CORE_CM_GMAC_CLKSTCTRL_REG_CLKACTIVITY_GMII_250MHZ_CLK));
*/

    CSL_FINST(coreCmReg->CM_EMIF_CLKSTCTRL_REG,
        CORE_CM_CORE_CM_EMIF_CLKSTCTRL_REG_CLKTRCTRL, SW_WKUP);

    while(CSL_CORE_CM_CORE_CM_EMIF_CLKSTCTRL_REG_CLKACTIVITY_EMIF_L3_GICLK_ACT !=
       CSL_FEXT(coreCmReg->CM_EMIF_CLKSTCTRL_REG,
        CORE_CM_CORE_CM_EMIF_CLKSTCTRL_REG_CLKACTIVITY_EMIF_L3_GICLK));

    CSL_FINST(coreCmReg->CM_L4CFG_CLKSTCTRL_REG,
        CORE_CM_CORE_CM_L4CFG_CLKSTCTRL_REG_CLKTRCTRL, NO_SLEEP);
/*
    while(CSL_CORE_CM_CORE_CM_L4CFG_CLKSTCTRL_REG_CLKACTIVITY_L4CFG_L4_GICLK_ACT !=
       CSL_FEXT(coreCmReg->CM_L4CFG_CLKSTCTRL_REG,
        CORE_CM_CORE_CM_L4CFG_CLKSTCTRL_REG_CLKACTIVITY_L4CFG_L4_GICLK));
*/
    CSL_FINST(coreCmReg->CM_DMA_CLKSTCTRL_REG,
        CORE_CM_CORE_CM_DMA_CLKSTCTRL_REG_CLKTRCTRL, SW_WKUP);

    while(CSL_CORE_CM_CORE_CM_DMA_CLKSTCTRL_REG_CLKACTIVITY_DMA_L3_GICLK_ACT !=
       CSL_FEXT(coreCmReg->CM_DMA_CLKSTCTRL_REG,
        CORE_CM_CORE_CM_DMA_CLKSTCTRL_REG_CLKACTIVITY_DMA_L3_GICLK));

    CSL_FINST(coreCmReg->CM_L3MAIN1_CLKSTCTRL_REG,
        CORE_CM_CORE_CM_L3MAIN1_CLKSTCTRL_REG_CLKTRCTRL, NO_SLEEP);
/*
    while(CSL_CORE_CM_CORE_CM_L3MAIN1_CLKSTCTRL_REG_CLKACTIVITY_L3MAIN1_L3_GICLK_ACT !=
       CSL_FEXT(coreCmReg->CM_L3MAIN1_CLKSTCTRL_REG,
        CORE_CM_CORE_CM_L3MAIN1_CLKSTCTRL_REG_CLKACTIVITY_L3MAIN1_L3_GICLK));
*/
    CSL_FINST(coreCmReg->CM_IPU2_CLKSTCTRL_REG,
        CORE_CM_CORE_CM_IPU2_CLKSTCTRL_REG_CLKTRCTRL, SW_WKUP);
/*
    while(CSL_CORE_CM_CORE_CM_IPU2_CLKSTCTRL_REG_CLKACTIVITY_IPU2_GFCLK_ACT !=
       CSL_FEXT(coreCmReg->CM_IPU2_CLKSTCTRL_REG,
        CORE_CM_CORE_CM_IPU2_CLKSTCTRL_REG_CLKACTIVITY_IPU2_GFCLK));
*/

    CSL_FINST(coreAonCmReg->CM_COREAON_CLKSTCTRL_REG,
        COREAON_CM_CORE_CM_COREAON_CLKSTCTRL_REG_CLKTRCTRL, SW_WKUP);
/*
    while(CSL_COREAON_CM_CORE_CM_COREAON_CLKSTCTRL_REG_CLKACTIVITY_COREAON_L4_GICLK_ACT !=
       CSL_FEXT(coreAonCmReg->CM_COREAON_CLKSTCTRL_REG,
        COREAON_CM_CORE_CM_COREAON_CLKSTCTRL_REG_CLKACTIVITY_COREAON_L4_GICLK));
*/

    CSL_FINST(dssCmReg->CM_DSS_CLKSTCTRL_REG,
        DSS_CM_CORE_CM_DSS_CLKSTCTRL_REG_CLKTRCTRL, SW_WKUP);
/*
    while(CSL_DSS_CM_CORE_CM_DSS_CLKSTCTRL_REG_CLKACTIVITY_DSS_L3_GICLK_ACT !=
       CSL_FEXT(dssCmReg->CM_DSS_CLKSTCTRL_REG,
        DSS_CM_CORE_CM_DSS_CLKSTCTRL_REG_CLKACTIVITY_DSS_L3_GICLK));
*/

    CSL_FINST(ipuCmReg->CM_IPU1_CLKSTCTRL_REG,
        IPU_CM_CORE_AON_CM_IPU1_CLKSTCTRL_REG_CLKTRCTRL, SW_WKUP);
/*
    while(CSL_IPU_CM_CORE_AON_CM_IPU1_CLKSTCTRL_REG_CLKACTIVITY_IPU1_GFCLK_ACT !=
       CSL_FEXT(ipuCmReg->CM_IPU1_CLKSTCTRL_REG,
        IPU_CM_CORE_AON_CM_IPU1_CLKSTCTRL_REG_CLKACTIVITY_IPU1_GFCLK));
*/

    CSL_FINST(ipuCmReg->CM_IPU_CLKSTCTRL_REG,
        IPU_CM_CORE_AON_CM_IPU_CLKSTCTRL_REG_CLKTRCTRL, SW_WKUP);
/*
    while(CSL_IPU_CM_CORE_AON_CM_IPU_CLKSTCTRL_REG_CLKACTIVITY_IPU_L3_GICLK_ACT !=
       CSL_FEXT(ipuCmReg->CM_IPU_CLKSTCTRL_REG,
        IPU_CM_CORE_AON_CM_IPU_CLKSTCTRL_REG_CLKACTIVITY_IPU_L3_GICLK));
*/

    CSL_FINST(rtcCmReg->CM_RTC_CLKSTCTRL_REG,
        RTC_CM_CORE_AON_CM_RTC_CLKSTCTRL_REG_CLKTRCTRL, SW_WKUP);
/*
    while(CSL_RTC_CM_CORE_AON_CM_RTC_CLKSTCTRL_REG_CLKACTIVITY_RTC_L4_GICLK_ACT !=
       CSL_FEXT(rtcCmReg->CM_RTC_CLKSTCTRL_REG,
        RTC_CM_CORE_AON_CM_RTC_CLKSTCTRL_REG_CLKACTIVITY_RTC_L4_GICLK));
*/

    CSL_FINST(vpeCmReg->CM_VPE_CLKSTCTRL_REG,
        VPE_CM_CORE_AON_CM_VPE_CLKSTCTRL_REG_CLKTRCTRL, SW_WKUP);
/*
    while(CSL_VPE_CM_CORE_AON_CM_VPE_CLKSTCTRL_REG_CLKACTIVITY_VPE_GCLK_ACT !=
       CSL_FEXT(vpeCmReg->CM_VPE_CLKSTCTRL_REG,
        VPE_CM_CORE_AON_CM_VPE_CLKSTCTRL_REG_CLKACTIVITY_VPE_GCLK));
*/

    CSL_FINST(wkupAonCmReg->CM_WKUPAON_CLKSTCTRL_REG,
        WKUPAON_CM_CM_WKUPAON_CLKSTCTRL_REG_CLKTRCTRL, SW_WKUP);
/*
    while(CSL_WKUPAON_CM_CM_WKUPAON_CLKSTCTRL_REG_CLKACTIVITY_SYS_CLK_ACT !=
       CSL_FEXT(wkupAonCmReg->CM_WKUPAON_CLKSTCTRL_REG,
        WKUPAON_CM_CM_WKUPAON_CLKSTCTRL_REG_CLKACTIVITY_SYS_CLK));
*/

    CSL_FINST(mpuCmReg->CM_MPU_CLKSTCTRL_REG,
        MPU_CM_CORE_AON_CM_MPU_CLKSTCTRL_REG_CLKTRCTRL, SW_WKUP);
/*
    while(CSL_MPU_CM_CORE_AON_CM_MPU_CLKSTCTRL_REG_CLKACTIVITY_MPU_GCLK_ACT !=
       CSL_FEXT(mpuCmReg->CM_MPU_CLKSTCTRL_REG,
        MPU_CM_CORE_AON_CM_MPU_CLKSTCTRL_REG_CLKACTIVITY_MPU_GCLK));
*/

    CSL_FINST(dsp1CmReg->CM_DSP1_CLKSTCTRL_REG,
        DSP1_CM_CORE_AON_CM_DSP1_CLKSTCTRL_REG_CLKTRCTRL, HW_AUTO);
/*
    while(CSL_DSP1_CM_CORE_AON_CM_DSP1_CLKSTCTRL_REG_CLKACTIVITY_DSP1_GFCLK_ACT !=
       CSL_FEXT(dsp1CmReg->CM_DSP1_CLKSTCTRL_REG,
        DSP1_CM_CORE_AON_CM_DSP1_CLKSTCTRL_REG_CLKACTIVITY_DSP1_GFCLK));
*/

    CSL_FINST(dsp2CmReg->CM_DSP2_CLKSTCTRL_REG,
        DSP2_CM_CORE_AON_CM_DSP2_CLKSTCTRL_REG_CLKTRCTRL, HW_AUTO);
/*
    while(CSL_DSP2_CM_CORE_AON_CM_DSP2_CLKSTCTRL_REG_CLKACTIVITY_DSP2_GFCLK_ACT !=
       CSL_FEXT(dsp2CmReg->CM_DSP2_CLKSTCTRL_REG,
        DSP2_CM_CORE_AON_CM_DSP2_CLKSTCTRL_REG_CLKACTIVITY_DSP2_GFCLK));
*/

    CSL_FINST(ivaCmReg->CM_IVA_CLKSTCTRL_REG,
        IVA_CM_CORE_CM_IVA_CLKSTCTRL_REG_CLKTRCTRL, SW_WKUP);
/*
    while(CSL_IVA_CM_CORE_CM_IVA_CLKSTCTRL_REG_CLKACTIVITY_IVA_GCLK_ACT !=
       CSL_FEXT(ivaCmReg->CM_IVA_CLKSTCTRL_REG,
        IVA_CM_CORE_CM_IVA_CLKSTCTRL_REG_CLKACTIVITY_IVA_GCLK));
*/

    /* PRCM Generic module mode setting functions */

    CSL_FINST(camCmReg->CM_CAM_VIP1_CLKCTRL_REG,
        CAM_CM_CORE_CM_CAM_VIP1_CLKCTRL_REG_MODULEMODE, AUTO);

    while(CSL_CAM_CM_CORE_CM_CAM_VIP1_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(camCmReg->CM_CAM_VIP1_CLKCTRL_REG,
        CAM_CM_CORE_CM_CAM_VIP1_CLKCTRL_REG_IDLEST));

    CSL_FINST(camCmReg->CM_CAM_VIP2_CLKCTRL_REG,
        CAM_CM_CORE_CM_CAM_VIP2_CLKCTRL_REG_MODULEMODE, AUTO);

    while(CSL_CAM_CM_CORE_CM_CAM_VIP2_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(camCmReg->CM_CAM_VIP2_CLKCTRL_REG,
        CAM_CM_CORE_CM_CAM_VIP2_CLKCTRL_REG_IDLEST));

    CSL_FINST(camCmReg->CM_CAM_VIP3_CLKCTRL_REG,
        CAM_CM_CORE_CM_CAM_VIP3_CLKCTRL_REG_MODULEMODE, AUTO);

    while(CSL_CAM_CM_CORE_CM_CAM_VIP3_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(camCmReg->CM_CAM_VIP3_CLKCTRL_REG,
        CAM_CM_CORE_CM_CAM_VIP3_CLKCTRL_REG_IDLEST));

    CSL_FINST(coreCmReg->CM_DMA_DMA_SYSTEM_CLKCTRL_REG,
        CORE_CM_CORE_CM_DMA_DMA_SYSTEM_CLKCTRL_REG_MODULEMODE, AUTO);

    while(CSL_CORE_CM_CORE_CM_DMA_DMA_SYSTEM_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(coreCmReg->CM_DMA_DMA_SYSTEM_CLKCTRL_REG,
        CORE_CM_CORE_CM_DMA_DMA_SYSTEM_CLKCTRL_REG_IDLEST));

    CSL_FINST(coreCmReg->CM_EMIF_DMM_CLKCTRL_REG,
        CORE_CM_CORE_CM_EMIF_DMM_CLKCTRL_REG_MODULEMODE, AUTO);

    while(CSL_CORE_CM_CORE_CM_EMIF_DMM_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(coreCmReg->CM_EMIF_DMM_CLKCTRL_REG,
        CORE_CM_CORE_CM_EMIF_DMM_CLKCTRL_REG_IDLEST));

    CSL_FINST(coreCmReg->CM_EMIF_EMIF1_CLKCTRL_REG,
        CORE_CM_CORE_CM_EMIF_EMIF1_CLKCTRL_REG_MODULEMODE, AUTO);

    while(CSL_CORE_CM_CORE_CM_EMIF_EMIF1_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(coreCmReg->CM_EMIF_EMIF1_CLKCTRL_REG,
        CORE_CM_CORE_CM_EMIF_EMIF1_CLKCTRL_REG_IDLEST));

    CSL_FINST(coreCmReg->CM_EMIF_EMIF2_CLKCTRL_REG,
        CORE_CM_CORE_CM_EMIF_EMIF2_CLKCTRL_REG_MODULEMODE, AUTO);

    while(CSL_CORE_CM_CORE_CM_EMIF_EMIF2_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(coreCmReg->CM_EMIF_EMIF2_CLKCTRL_REG,
        CORE_CM_CORE_CM_EMIF_EMIF2_CLKCTRL_REG_IDLEST));

    CSL_FINST(coreCmReg->CM_EMIF_EMIF_OCP_FW_CLKCTRL_REG,
        CORE_CM_CORE_CM_EMIF_EMIF_OCP_FW_CLKCTRL_REG_MODULEMODE, AUTO);

    while(CSL_CORE_CM_CORE_CM_EMIF_EMIF_OCP_FW_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(coreCmReg->CM_EMIF_EMIF_OCP_FW_CLKCTRL_REG,
        CORE_CM_CORE_CM_EMIF_EMIF_OCP_FW_CLKCTRL_REG_IDLEST));

    CSL_FINST(coreCmReg->CM_L3INSTR_L3_MAIN_2_CLKCTRL_REG,
        CORE_CM_CORE_CM_L3INSTR_L3_MAIN_2_CLKCTRL_REG_MODULEMODE, AUTO);

    while(CSL_CORE_CM_CORE_CM_L3INSTR_L3_MAIN_2_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(coreCmReg->CM_L3INSTR_L3_MAIN_2_CLKCTRL_REG,
        CORE_CM_CORE_CM_L3INSTR_L3_MAIN_2_CLKCTRL_REG_IDLEST));

    CSL_FINST(coreCmReg->CM_L3MAIN1_GPMC_CLKCTRL_REG,
        CORE_CM_CORE_CM_L3MAIN1_GPMC_CLKCTRL_REG_MODULEMODE, AUTO);

    while(CSL_CORE_CM_CORE_CM_L3MAIN1_GPMC_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(coreCmReg->CM_L3MAIN1_GPMC_CLKCTRL_REG,
        CORE_CM_CORE_CM_L3MAIN1_GPMC_CLKCTRL_REG_IDLEST));

    CSL_FINST(coreCmReg->CM_L3MAIN1_L3_MAIN_1_CLKCTRL_REG,
        CORE_CM_CORE_CM_L3MAIN1_L3_MAIN_1_CLKCTRL_REG_MODULEMODE, AUTO);

    while(CSL_CORE_CM_CORE_CM_L3MAIN1_L3_MAIN_1_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(coreCmReg->CM_L3MAIN1_L3_MAIN_1_CLKCTRL_REG,
        CORE_CM_CORE_CM_L3MAIN1_L3_MAIN_1_CLKCTRL_REG_IDLEST));

    CSL_FINST(coreCmReg->CM_L3MAIN1_MMU_EDMA_CLKCTRL_REG,
        CORE_CM_CORE_CM_L3MAIN1_MMU_EDMA_CLKCTRL_REG_MODULEMODE, AUTO);

    while(CSL_CORE_CM_CORE_CM_L3MAIN1_MMU_EDMA_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(coreCmReg->CM_L3MAIN1_MMU_EDMA_CLKCTRL_REG,
        CORE_CM_CORE_CM_L3MAIN1_MMU_EDMA_CLKCTRL_REG_IDLEST));

    CSL_FINST(coreCmReg->CM_L3MAIN1_MMU_PCIESS_CLKCTRL_REG,
        CORE_CM_CORE_CM_L3MAIN1_MMU_PCIESS_CLKCTRL_REG_MODULEMODE, AUTO);

    while(CSL_CORE_CM_CORE_CM_L3MAIN1_MMU_PCIESS_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(coreCmReg->CM_L3MAIN1_MMU_PCIESS_CLKCTRL_REG,
        CORE_CM_CORE_CM_L3MAIN1_MMU_PCIESS_CLKCTRL_REG_IDLEST));

    CSL_FINST(coreCmReg->CM_L3MAIN1_OCMC_RAM1_CLKCTRL_REG,
        CORE_CM_CORE_CM_L3MAIN1_OCMC_RAM1_CLKCTRL_REG_MODULEMODE, AUTO);

    while(CSL_CORE_CM_CORE_CM_L3MAIN1_OCMC_RAM1_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(coreCmReg->CM_L3MAIN1_OCMC_RAM1_CLKCTRL_REG,
        CORE_CM_CORE_CM_L3MAIN1_OCMC_RAM1_CLKCTRL_REG_IDLEST));

    CSL_FINST(coreCmReg->CM_L3MAIN1_OCMC_RAM2_CLKCTRL_REG,
        CORE_CM_CORE_CM_L3MAIN1_OCMC_RAM2_CLKCTRL_REG_MODULEMODE, AUTO);

    while(CSL_CORE_CM_CORE_CM_L3MAIN1_OCMC_RAM2_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(coreCmReg->CM_L3MAIN1_OCMC_RAM2_CLKCTRL_REG,
        CORE_CM_CORE_CM_L3MAIN1_OCMC_RAM2_CLKCTRL_REG_IDLEST));

    CSL_FINST(coreCmReg->CM_L3MAIN1_OCMC_RAM3_CLKCTRL_REG,
        CORE_CM_CORE_CM_L3MAIN1_OCMC_RAM3_CLKCTRL_REG_MODULEMODE, AUTO);

    while(CSL_CORE_CM_CORE_CM_L3MAIN1_OCMC_RAM3_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(coreCmReg->CM_L3MAIN1_OCMC_RAM3_CLKCTRL_REG,
        CORE_CM_CORE_CM_L3MAIN1_OCMC_RAM3_CLKCTRL_REG_IDLEST));

    CSL_FINST(coreCmReg->CM_L3MAIN1_OCMC_ROM_CLKCTRL_REG,
        CORE_CM_CORE_CM_L3MAIN1_OCMC_ROM_CLKCTRL_REG_MODULEMODE, AUTO);

    while(CSL_CORE_CM_CORE_CM_L3MAIN1_OCMC_ROM_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(coreCmReg->CM_L3MAIN1_OCMC_ROM_CLKCTRL_REG,
        CORE_CM_CORE_CM_L3MAIN1_OCMC_ROM_CLKCTRL_REG_IDLEST));

    CSL_FINST(coreCmReg->CM_L3MAIN1_TPCC_CLKCTRL_REG,
        CORE_CM_CORE_CM_L3MAIN1_TPCC_CLKCTRL_REG_MODULEMODE, AUTO);

    while(CSL_CORE_CM_CORE_CM_L3MAIN1_TPCC_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(coreCmReg->CM_L3MAIN1_TPCC_CLKCTRL_REG,
        CORE_CM_CORE_CM_L3MAIN1_TPCC_CLKCTRL_REG_IDLEST));

    CSL_FINST(coreCmReg->CM_L3MAIN1_TPTC1_CLKCTRL_REG,
        CORE_CM_CORE_CM_L3MAIN1_TPTC1_CLKCTRL_REG_MODULEMODE, AUTO);

    while(CSL_CORE_CM_CORE_CM_L3MAIN1_TPTC1_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(coreCmReg->CM_L3MAIN1_TPTC1_CLKCTRL_REG,
        CORE_CM_CORE_CM_L3MAIN1_TPTC1_CLKCTRL_REG_IDLEST));

    CSL_FINST(coreCmReg->CM_L3MAIN1_TPTC2_CLKCTRL_REG,
        CORE_CM_CORE_CM_L3MAIN1_TPTC2_CLKCTRL_REG_MODULEMODE, AUTO);

    while(CSL_CORE_CM_CORE_CM_L3MAIN1_TPTC2_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(coreCmReg->CM_L3MAIN1_TPTC2_CLKCTRL_REG,
        CORE_CM_CORE_CM_L3MAIN1_TPTC2_CLKCTRL_REG_IDLEST));

// TODO: Check if enabled or AUTO
    CSL_FINS(coreCmReg->CM_L3MAIN1_VCP1_CLKCTRL_REG,
        CORE_CM_CORE_CM_L3MAIN1_VCP1_CLKCTRL_REG_MODULEMODE, 2U);

    while(CSL_CORE_CM_CORE_CM_L3MAIN1_VCP1_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(coreCmReg->CM_L3MAIN1_VCP1_CLKCTRL_REG,
        CORE_CM_CORE_CM_L3MAIN1_VCP1_CLKCTRL_REG_IDLEST));

    CSL_FINS(coreCmReg->CM_L3MAIN1_VCP2_CLKCTRL_REG,
        CORE_CM_CORE_CM_L3MAIN1_VCP2_CLKCTRL_REG_MODULEMODE, 2U);

    while(CSL_CORE_CM_CORE_CM_L3MAIN1_VCP2_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(coreCmReg->CM_L3MAIN1_VCP2_CLKCTRL_REG,
        CORE_CM_CORE_CM_L3MAIN1_VCP2_CLKCTRL_REG_IDLEST));

    CSL_FINST(coreCmReg->CM_L4CFG_L4_CFG_CLKCTRL_REG,
        CORE_CM_CORE_CM_L4CFG_L4_CFG_CLKCTRL_REG_MODULEMODE, AUTO);

    while(CSL_CORE_CM_CORE_CM_L4CFG_L4_CFG_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(coreCmReg->CM_L4CFG_L4_CFG_CLKCTRL_REG,
        CORE_CM_CORE_CM_L4CFG_L4_CFG_CLKCTRL_REG_IDLEST));

    CSL_FINST(coreCmReg->CM_L4CFG_MAILBOX1_CLKCTRL_REG,
        CORE_CM_CORE_CM_L4CFG_MAILBOX1_CLKCTRL_REG_MODULEMODE, AUTO);

    while(CSL_CORE_CM_CORE_CM_L4CFG_MAILBOX1_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(coreCmReg->CM_L4CFG_MAILBOX1_CLKCTRL_REG,
        CORE_CM_CORE_CM_L4CFG_MAILBOX1_CLKCTRL_REG_IDLEST));

    CSL_FINST(coreCmReg->CM_L4CFG_MAILBOX10_CLKCTRL_REG,
        CORE_CM_CORE_CM_L4CFG_MAILBOX1_CLKCTRL_REG_MODULEMODE, AUTO);

    while(CSL_CORE_CM_CORE_CM_L4CFG_MAILBOX1_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(coreCmReg->CM_L4CFG_MAILBOX10_CLKCTRL_REG,
        CORE_CM_CORE_CM_L4CFG_MAILBOX1_CLKCTRL_REG_IDLEST));

    CSL_FINST(coreCmReg->CM_L4CFG_MAILBOX11_CLKCTRL_REG,
        CORE_CM_CORE_CM_L4CFG_MAILBOX11_CLKCTRL_REG_MODULEMODE, AUTO);

    while(CSL_CORE_CM_CORE_CM_L4CFG_MAILBOX11_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(coreCmReg->CM_L4CFG_MAILBOX11_CLKCTRL_REG,
        CORE_CM_CORE_CM_L4CFG_MAILBOX11_CLKCTRL_REG_IDLEST));

    CSL_FINST(coreCmReg->CM_L4CFG_MAILBOX12_CLKCTRL_REG,
        CORE_CM_CORE_CM_L4CFG_MAILBOX12_CLKCTRL_REG_MODULEMODE, AUTO);

    while(CSL_CORE_CM_CORE_CM_L4CFG_MAILBOX12_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(coreCmReg->CM_L4CFG_MAILBOX12_CLKCTRL_REG,
        CORE_CM_CORE_CM_L4CFG_MAILBOX12_CLKCTRL_REG_IDLEST));

    CSL_FINST(coreCmReg->CM_L4CFG_MAILBOX13_CLKCTRL_REG,
        CORE_CM_CORE_CM_L4CFG_MAILBOX13_CLKCTRL_REG_MODULEMODE, AUTO);

    while(CSL_CORE_CM_CORE_CM_L4CFG_MAILBOX13_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(coreCmReg->CM_L4CFG_MAILBOX13_CLKCTRL_REG,
        CORE_CM_CORE_CM_L4CFG_MAILBOX13_CLKCTRL_REG_IDLEST));

    CSL_FINST(coreCmReg->CM_L4CFG_MAILBOX2_CLKCTRL_REG,
        CORE_CM_CORE_CM_L4CFG_MAILBOX2_CLKCTRL_REG_MODULEMODE, AUTO);

    while(CSL_CORE_CM_CORE_CM_L4CFG_MAILBOX2_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(coreCmReg->CM_L4CFG_MAILBOX2_CLKCTRL_REG,
        CORE_CM_CORE_CM_L4CFG_MAILBOX2_CLKCTRL_REG_IDLEST));

    CSL_FINST(coreCmReg->CM_L4CFG_MAILBOX3_CLKCTRL_REG,
        CORE_CM_CORE_CM_L4CFG_MAILBOX3_CLKCTRL_REG_MODULEMODE, AUTO);

    while(CSL_CORE_CM_CORE_CM_L4CFG_MAILBOX3_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(coreCmReg->CM_L4CFG_MAILBOX3_CLKCTRL_REG,
        CORE_CM_CORE_CM_L4CFG_MAILBOX3_CLKCTRL_REG_IDLEST));

    CSL_FINST(coreCmReg->CM_L4CFG_MAILBOX4_CLKCTRL_REG,
        CORE_CM_CORE_CM_L4CFG_MAILBOX4_CLKCTRL_REG_MODULEMODE, AUTO);

    while(CSL_CORE_CM_CORE_CM_L4CFG_MAILBOX4_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(coreCmReg->CM_L4CFG_MAILBOX4_CLKCTRL_REG,
        CORE_CM_CORE_CM_L4CFG_MAILBOX4_CLKCTRL_REG_IDLEST));

    CSL_FINST(coreCmReg->CM_L4CFG_MAILBOX5_CLKCTRL_REG,
        CORE_CM_CORE_CM_L4CFG_MAILBOX5_CLKCTRL_REG_MODULEMODE, AUTO);

    while(CSL_CORE_CM_CORE_CM_L4CFG_MAILBOX5_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(coreCmReg->CM_L4CFG_MAILBOX5_CLKCTRL_REG,
        CORE_CM_CORE_CM_L4CFG_MAILBOX5_CLKCTRL_REG_IDLEST));

    CSL_FINST(coreCmReg->CM_L4CFG_MAILBOX6_CLKCTRL_REG,
        CORE_CM_CORE_CM_L4CFG_MAILBOX6_CLKCTRL_REG_MODULEMODE, AUTO);

    while(CSL_CORE_CM_CORE_CM_L4CFG_MAILBOX6_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(coreCmReg->CM_L4CFG_MAILBOX6_CLKCTRL_REG,
        CORE_CM_CORE_CM_L4CFG_MAILBOX6_CLKCTRL_REG_IDLEST));

    CSL_FINST(coreCmReg->CM_L4CFG_MAILBOX7_CLKCTRL_REG,
        CORE_CM_CORE_CM_L4CFG_MAILBOX7_CLKCTRL_REG_MODULEMODE, AUTO);

    while(CSL_CORE_CM_CORE_CM_L4CFG_MAILBOX7_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(coreCmReg->CM_L4CFG_MAILBOX7_CLKCTRL_REG,
        CORE_CM_CORE_CM_L4CFG_MAILBOX7_CLKCTRL_REG_IDLEST));

    CSL_FINST(coreCmReg->CM_L4CFG_MAILBOX8_CLKCTRL_REG,
        CORE_CM_CORE_CM_L4CFG_MAILBOX8_CLKCTRL_REG_MODULEMODE, AUTO);

    while(CSL_CORE_CM_CORE_CM_L4CFG_MAILBOX8_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(coreCmReg->CM_L4CFG_MAILBOX8_CLKCTRL_REG,
        CORE_CM_CORE_CM_L4CFG_MAILBOX8_CLKCTRL_REG_IDLEST));

    CSL_FINST(coreCmReg->CM_L4CFG_MAILBOX9_CLKCTRL_REG,
        CORE_CM_CORE_CM_L4CFG_MAILBOX9_CLKCTRL_REG_MODULEMODE, AUTO);

    while(CSL_CORE_CM_CORE_CM_L4CFG_MAILBOX9_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(coreCmReg->CM_L4CFG_MAILBOX9_CLKCTRL_REG,
        CORE_CM_CORE_CM_L4CFG_MAILBOX9_CLKCTRL_REG_IDLEST));

    CSL_FINST(coreCmReg->CM_L4CFG_SPINLOCK_CLKCTRL_REG,
        CORE_CM_CORE_CM_L4CFG_SPINLOCK_CLKCTRL_REG_MODULEMODE, AUTO);

    while(CSL_CORE_CM_CORE_CM_L4CFG_SPINLOCK_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(coreCmReg->CM_L4CFG_SPINLOCK_CLKCTRL_REG,
        CORE_CM_CORE_CM_L4CFG_SPINLOCK_CLKCTRL_REG_IDLEST));

    CSL_FINST(ipuCmReg->CM_IPU_TIMER5_CLKCTRL_REG,
        IPU_CM_CORE_AON_CM_IPU_TIMER5_CLKCTRL_REG_MODULEMODE, ENABLE);

    while(CSL_IPU_CM_CORE_AON_CM_IPU_TIMER5_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(ipuCmReg->CM_IPU_TIMER5_CLKCTRL_REG,
        IPU_CM_CORE_AON_CM_IPU_TIMER5_CLKCTRL_REG_IDLEST));

    CSL_FINST(ipuCmReg->CM_IPU_TIMER6_CLKCTRL_REG,
        IPU_CM_CORE_AON_CM_IPU_TIMER6_CLKCTRL_REG_MODULEMODE, ENABLE);

    while(CSL_IPU_CM_CORE_AON_CM_IPU_TIMER6_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(ipuCmReg->CM_IPU_TIMER6_CLKCTRL_REG,
        IPU_CM_CORE_AON_CM_IPU_TIMER6_CLKCTRL_REG_IDLEST));

    CSL_FINST(ipuCmReg->CM_IPU_TIMER7_CLKCTRL_REG,
        IPU_CM_CORE_AON_CM_IPU_TIMER7_CLKCTRL_REG_MODULEMODE, ENABLE);

    while(CSL_IPU_CM_CORE_AON_CM_IPU_TIMER7_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(ipuCmReg->CM_IPU_TIMER7_CLKCTRL_REG,
        IPU_CM_CORE_AON_CM_IPU_TIMER7_CLKCTRL_REG_IDLEST));

    CSL_FINST(ipuCmReg->CM_IPU_TIMER8_CLKCTRL_REG,
        IPU_CM_CORE_AON_CM_IPU_TIMER8_CLKCTRL_REG_MODULEMODE, ENABLE);

    while(CSL_IPU_CM_CORE_AON_CM_IPU_TIMER8_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(ipuCmReg->CM_IPU_TIMER8_CLKCTRL_REG,
        IPU_CM_CORE_AON_CM_IPU_TIMER8_CLKCTRL_REG_IDLEST));

    CSL_FINST(l3InitCmReg->CM_L3INIT_MMC1_CLKCTRL_REG,
        L3INIT_CM_CORE_CM_L3INIT_MMC1_CLKCTRL_REG_MODULEMODE, ENABLED);

    while(CSL_L3INIT_CM_CORE_CM_L3INIT_MMC1_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(l3InitCmReg->CM_L3INIT_MMC1_CLKCTRL_REG,
        L3INIT_CM_CORE_CM_L3INIT_MMC1_CLKCTRL_REG_IDLEST));

    CSL_FINST(l3InitCmReg->CM_L3INIT_MMC2_CLKCTRL_REG,
        L3INIT_CM_CORE_CM_L3INIT_MMC2_CLKCTRL_REG_MODULEMODE, ENABLED);

    while(CSL_L3INIT_CM_CORE_CM_L3INIT_MMC2_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(l3InitCmReg->CM_L3INIT_MMC2_CLKCTRL_REG,
        L3INIT_CM_CORE_CM_L3INIT_MMC2_CLKCTRL_REG_IDLEST));

    CSL_FINST(l3InitCmReg->CM_GMAC_GMAC_CLKCTRL_REG,
        L3INIT_CM_CORE_CM_GMAC_GMAC_CLKCTRL_REG_MODULEMODE, ENABLED);

    while(CSL_L3INIT_CM_CORE_CM_GMAC_GMAC_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(l3InitCmReg->CM_GMAC_GMAC_CLKCTRL_REG,
        L3INIT_CM_CORE_CM_GMAC_GMAC_CLKCTRL_REG_IDLEST));

    CSL_FINST(l4PerCmReg->CM_L4PER2_L4_PER2_CLKCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER2_L4_PER2_CLKCTRL_REG_MODULEMODE, AUTO);

    while(CSL_L4PER_CM_CORE_COMPONENT_CM_L4PER2_L4_PER2_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(l4PerCmReg->CM_L4PER2_L4_PER2_CLKCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER2_L4_PER2_CLKCTRL_REG_IDLEST));

    CSL_FINST(l4PerCmReg->CM_L4PER3_L4_PER3_CLKCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER3_L4_PER3_CLKCTRL_REG_MODULEMODE, AUTO);

    while(CSL_L4PER_CM_CORE_COMPONENT_CM_L4PER3_L4_PER3_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(l4PerCmReg->CM_L4PER3_L4_PER3_CLKCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER3_L4_PER3_CLKCTRL_REG_IDLEST));

    CSL_FINST(l4PerCmReg->CM_L4PER_GPIO2_CLKCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER_GPIO2_CLKCTRL_REG_MODULEMODE, AUTO);

    while(CSL_L4PER_CM_CORE_COMPONENT_CM_L4PER_GPIO2_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(l4PerCmReg->CM_L4PER_GPIO2_CLKCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER_GPIO2_CLKCTRL_REG_IDLEST));

    CSL_FINST(l4PerCmReg->CM_L4PER_GPIO3_CLKCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER_GPIO3_CLKCTRL_REG_MODULEMODE, AUTO);

    while(CSL_L4PER_CM_CORE_COMPONENT_CM_L4PER_GPIO3_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(l4PerCmReg->CM_L4PER_GPIO3_CLKCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER_GPIO3_CLKCTRL_REG_IDLEST));

    CSL_FINST(l4PerCmReg->CM_L4PER_GPIO4_CLKCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER_GPIO4_CLKCTRL_REG_MODULEMODE, AUTO);

    while(CSL_L4PER_CM_CORE_COMPONENT_CM_L4PER_GPIO4_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(l4PerCmReg->CM_L4PER_GPIO4_CLKCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER_GPIO4_CLKCTRL_REG_IDLEST));

    CSL_FINST(l4PerCmReg->CM_L4PER_GPIO5_CLKCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER_GPIO5_CLKCTRL_REG_MODULEMODE, AUTO);

    while(CSL_L4PER_CM_CORE_COMPONENT_CM_L4PER_GPIO5_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(l4PerCmReg->CM_L4PER_GPIO5_CLKCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER_GPIO5_CLKCTRL_REG_IDLEST));

    CSL_FINST(l4PerCmReg->CM_L4PER_GPIO6_CLKCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER_GPIO6_CLKCTRL_REG_MODULEMODE, AUTO);

    while(CSL_L4PER_CM_CORE_COMPONENT_CM_L4PER_GPIO6_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(l4PerCmReg->CM_L4PER_GPIO6_CLKCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER_GPIO6_CLKCTRL_REG_IDLEST));

    CSL_FINST(l4PerCmReg->CM_L4PER_GPIO7_CLKCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER_GPIO7_CLKCTRL_REG_MODULEMODE, AUTO);

    while(CSL_L4PER_CM_CORE_COMPONENT_CM_L4PER_GPIO7_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(l4PerCmReg->CM_L4PER_GPIO7_CLKCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER_GPIO7_CLKCTRL_REG_IDLEST));

    CSL_FINST(l4PerCmReg->CM_L4PER_GPIO8_CLKCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER_GPIO8_CLKCTRL_REG_MODULEMODE, AUTO);

    while(CSL_L4PER_CM_CORE_COMPONENT_CM_L4PER_GPIO8_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(l4PerCmReg->CM_L4PER_GPIO8_CLKCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER_GPIO8_CLKCTRL_REG_IDLEST));

    CSL_FINST(l4PerCmReg->CM_L4PER_I2C1_CLKCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER_I2C1_CLKCTRL_REG_MODULEMODE, ENABLE);

    while(CSL_L4PER_CM_CORE_COMPONENT_CM_L4PER_I2C1_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(l4PerCmReg->CM_L4PER_I2C1_CLKCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER_I2C1_CLKCTRL_REG_IDLEST));

    CSL_FINST(l4PerCmReg->CM_L4PER_I2C2_CLKCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER_I2C2_CLKCTRL_REG_MODULEMODE, ENABLE);

    while(CSL_L4PER_CM_CORE_COMPONENT_CM_L4PER_I2C2_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(l4PerCmReg->CM_L4PER_I2C2_CLKCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER_I2C2_CLKCTRL_REG_IDLEST));

    CSL_FINST(l4PerCmReg->CM_L4PER_I2C3_CLKCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER_I2C3_CLKCTRL_REG_MODULEMODE, ENABLE);

    while(CSL_L4PER_CM_CORE_COMPONENT_CM_L4PER_I2C3_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(l4PerCmReg->CM_L4PER_I2C3_CLKCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER_I2C3_CLKCTRL_REG_IDLEST));

    CSL_FINST(l4PerCmReg->CM_L4PER_I2C4_CLKCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER_I2C4_CLKCTRL_REG_MODULEMODE, ENABLE);

    while(CSL_L4PER_CM_CORE_COMPONENT_CM_L4PER_I2C4_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(l4PerCmReg->CM_L4PER_I2C4_CLKCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER_I2C4_CLKCTRL_REG_IDLEST));

    CSL_FINST(l4PerCmReg->CM_L4PER_L4_PER1_CLKCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER_L4_PER1_CLKCTRL_REG_MODULEMODE, AUTO);

    while(CSL_L4PER_CM_CORE_COMPONENT_CM_L4PER_L4_PER1_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(l4PerCmReg->CM_L4PER_L4_PER1_CLKCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER_L4_PER1_CLKCTRL_REG_IDLEST));

    CSL_FINST(l4PerCmReg->CM_L4PER_MCSPI1_CLKCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER_MCSPI1_CLKCTRL_REG_MODULEMODE, ENABLE);

    while(CSL_L4PER_CM_CORE_COMPONENT_CM_L4PER_MCSPI1_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(l4PerCmReg->CM_L4PER_MCSPI1_CLKCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER_MCSPI1_CLKCTRL_REG_IDLEST));

    CSL_FINST(l4PerCmReg->CM_L4PER_MMC3_CLKCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER_MMC3_CLKCTRL_REG_MODULEMODE, ENABLE);

    while(CSL_L4PER_CM_CORE_COMPONENT_CM_L4PER_MMC3_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(l4PerCmReg->CM_L4PER_MMC3_CLKCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER_MMC3_CLKCTRL_REG_IDLEST));

    CSL_FINST(l4PerCmReg->CM_L4PER_MMC4_CLKCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER_MMC4_CLKCTRL_REG_MODULEMODE, ENABLE);

    while(CSL_L4PER_CM_CORE_COMPONENT_CM_L4PER_MMC4_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(l4PerCmReg->CM_L4PER_MMC4_CLKCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER_MMC4_CLKCTRL_REG_IDLEST));

    CSL_FINST(l4PerCmReg->CM_L4PER_TIMER10_CLKCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER_TIMER10_CLKCTRL_REG_MODULEMODE, ENABLE);

    while(CSL_L4PER_CM_CORE_COMPONENT_CM_L4PER_TIMER10_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(l4PerCmReg->CM_L4PER_TIMER10_CLKCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER_TIMER10_CLKCTRL_REG_IDLEST));

    CSL_FINST(l4PerCmReg->CM_L4PER_TIMER11_CLKCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER_TIMER11_CLKCTRL_REG_MODULEMODE, ENABLE);

    while(CSL_L4PER_CM_CORE_COMPONENT_CM_L4PER_TIMER11_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(l4PerCmReg->CM_L4PER_TIMER11_CLKCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER_TIMER11_CLKCTRL_REG_IDLEST));

    CSL_FINST(l4PerCmReg->CM_L4PER_TIMER2_CLKCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER_TIMER11_CLKCTRL_REG_MODULEMODE, ENABLE);

    while(CSL_L4PER_CM_CORE_COMPONENT_CM_L4PER_TIMER11_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(l4PerCmReg->CM_L4PER_TIMER2_CLKCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER_TIMER11_CLKCTRL_REG_IDLEST));

    CSL_FINST(l4PerCmReg->CM_L4PER_TIMER3_CLKCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER_TIMER3_CLKCTRL_REG_MODULEMODE, ENABLE);

    while(CSL_L4PER_CM_CORE_COMPONENT_CM_L4PER_TIMER3_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(l4PerCmReg->CM_L4PER_TIMER3_CLKCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER_TIMER3_CLKCTRL_REG_IDLEST));

    CSL_FINST(l4PerCmReg->CM_L4PER_TIMER4_CLKCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER_TIMER4_CLKCTRL_REG_MODULEMODE, ENABLE);

    while(CSL_L4PER_CM_CORE_COMPONENT_CM_L4PER_TIMER4_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(l4PerCmReg->CM_L4PER_TIMER4_CLKCTRL_REG,
        RTC_CM_CORE_AON_CM_RTC_RTCSS_CLKCTRL_REG_IDLEST));

    CSL_FINST(l4PerCmReg->CM_L4PER_TIMER9_CLKCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER_TIMER4_CLKCTRL_REG_MODULEMODE, ENABLE);

    while(CSL_L4PER_CM_CORE_COMPONENT_CM_L4PER_TIMER4_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(l4PerCmReg->CM_L4PER_TIMER9_CLKCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER_TIMER4_CLKCTRL_REG_IDLEST));

    CSL_FINST(l4PerCmReg->CM_L4PER2_QSPI_CLKCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER2_QSPI_CLKCTRL_REG_MODULEMODE, ENABLED);

    while(CSL_L4PER_CM_CORE_COMPONENT_CM_L4PER2_QSPI_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(l4PerCmReg->CM_L4PER2_QSPI_CLKCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER2_QSPI_CLKCTRL_REG_IDLEST));

    CSL_FINST(l4PerCmReg->CM_L4PER_UART1_CLKCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER_UART1_CLKCTRL_REG_MODULEMODE, ENABLE);

    while(CSL_L4PER_CM_CORE_COMPONENT_CM_L4PER_UART1_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(l4PerCmReg->CM_L4PER_UART1_CLKCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER_UART1_CLKCTRL_REG_IDLEST));

    CSL_FINST(l4PerCmReg->CM_L4PER_UART3_CLKCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER_UART3_CLKCTRL_REG_MODULEMODE, ENABLE);

    while(CSL_L4PER_CM_CORE_COMPONENT_CM_L4PER_UART3_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(l4PerCmReg->CM_L4PER_UART3_CLKCTRL_REG,
        L4PER_CM_CORE_COMPONENT_CM_L4PER_UART3_CLKCTRL_REG_IDLEST));

    CSL_FINS(mpuCmReg->CM_MPU_MPU_CLKCTRL_REG,
        MPU_CM_CORE_AON_CM_MPU_MPU_CLKCTRL_REG_MODULEMODE, 2U);

    while(CSL_MPU_CM_CORE_AON_CM_MPU_MPU_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(mpuCmReg->CM_MPU_MPU_CLKCTRL_REG,
        MPU_CM_CORE_AON_CM_MPU_MPU_CLKCTRL_REG_IDLEST));

    CSL_FINST(vpeCmReg->CM_VPE_VPE_CLKCTRL_REG,
        VPE_CM_CORE_AON_CM_VPE_VPE_CLKCTRL_REG_MODULEMODE, AUTO);

    while(CSL_VPE_CM_CORE_AON_CM_VPE_VPE_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(vpeCmReg->CM_VPE_VPE_CLKCTRL_REG,
        VPE_CM_CORE_AON_CM_VPE_VPE_CLKCTRL_REG_IDLEST));

    CSL_FINST(wkupAonCmReg->CM_WKUPAON_GPIO1_CLKCTRL_REG,
        WKUPAON_CM_CM_WKUPAON_GPIO1_CLKCTRL_REG_MODULEMODE, AUTO);

    while(CSL_WKUPAON_CM_CM_WKUPAON_GPIO1_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(wkupAonCmReg->CM_WKUPAON_GPIO1_CLKCTRL_REG,
        WKUPAON_CM_CM_WKUPAON_GPIO1_CLKCTRL_REG_IDLEST));

    CSL_FINST(wkupAonCmReg->CM_WKUPAON_TIMER1_CLKCTRL_REG,
        WKUPAON_CM_CM_WKUPAON_TIMER1_CLKCTRL_REG_MODULEMODE, ENABLE);

    while(CSL_WKUPAON_CM_CM_WKUPAON_TIMER1_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(wkupAonCmReg->CM_WKUPAON_TIMER1_CLKCTRL_REG,
        WKUPAON_CM_CM_WKUPAON_TIMER1_CLKCTRL_REG_IDLEST));

    CSL_FINS(wkupAonCmReg->CM_WKUPAON_TIMER12_CLKCTRL_REG,
        WKUPAON_CM_CM_WKUPAON_TIMER12_CLKCTRL_REG_MODULEMODE, 2U);

    while(CSL_WKUPAON_CM_CM_WKUPAON_TIMER12_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(wkupAonCmReg->CM_WKUPAON_TIMER12_CLKCTRL_REG,
        WKUPAON_CM_CM_WKUPAON_TIMER12_CLKCTRL_REG_IDLEST));

    CSL_FINS(wkupAonCmReg->CM_WKUPAON_WD_TIMER1_CLKCTRL_REG,
        WKUPAON_CM_CM_WKUPAON_WD_TIMER1_CLKCTRL_REG_MODULEMODE, 2U);

    while(CSL_WKUPAON_CM_CM_WKUPAON_WD_TIMER1_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(wkupAonCmReg->CM_WKUPAON_WD_TIMER1_CLKCTRL_REG,
        WKUPAON_CM_CM_WKUPAON_WD_TIMER1_CLKCTRL_REG_IDLEST));

    CSL_FINST(wkupAonCmReg->CM_WKUPAON_WD_TIMER2_CLKCTRL_REG,
        WKUPAON_CM_CM_WKUPAON_WD_TIMER2_CLKCTRL_REG_MODULEMODE, ENABLE);

    while(CSL_WKUPAON_CM_CM_WKUPAON_WD_TIMER2_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(wkupAonCmReg->CM_WKUPAON_WD_TIMER2_CLKCTRL_REG,
        WKUPAON_CM_CM_WKUPAON_WD_TIMER2_CLKCTRL_REG_IDLEST));

    /* PRCM Specialized module mode setting functions */
    CSL_FINST(ivaCmReg->CM_IVA_SL2_CLKCTRL_REG,
        IVA_CM_CORE_CM_IVA_SL2_CLKCTRL_REG_MODULEMODE, AUTO);

    while(CSL_IVA_CM_CORE_CM_IVA_SL2_CLKCTRL_REG_IDLEST_DISABLE ==
       CSL_FEXT(ivaCmReg->CM_IVA_SL2_CLKCTRL_REG,
        IVA_CM_CORE_CM_IVA_SL2_CLKCTRL_REG_IDLEST));

    CSL_FINST(ivaCmReg->CM_IVA_IVA_CLKCTRL_REG,
        IVA_CM_CORE_CM_IVA_IVA_CLKCTRL_REG_MODULEMODE, AUTO);

    while(CSL_IVA_CM_CORE_CM_IVA_IVA_CLKCTRL_REG_IDLEST_DISABLE ==
       CSL_FEXT(ivaCmReg->CM_IVA_IVA_CLKCTRL_REG,
        IVA_CM_CORE_CM_IVA_IVA_CLKCTRL_REG_IDLEST));

    CSL_FINST(coreCmReg->CM_IPU2_IPU2_CLKCTRL_REG,
        CORE_CM_CORE_CM_IPU2_IPU2_CLKCTRL_REG_MODULEMODE, AUTO);

    while(CSL_CORE_CM_CORE_CM_IPU2_IPU2_CLKCTRL_REG_IDLEST_DISABLE ==
       CSL_FEXT(coreCmReg->CM_IPU2_IPU2_CLKCTRL_REG,
        CORE_CM_CORE_CM_IPU2_IPU2_CLKCTRL_REG_IDLEST));

    CSL_FINST(dssCmReg->CM_DSS_DSS_CLKCTRL_REG,
        DSS_CM_CORE_CM_DSS_DSS_CLKCTRL_REG_MODULEMODE, ENABLED);
/*
    while(CSL_DSS_CM_CORE_CM_DSS_DSS_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(dssCmReg->CM_DSS_DSS_CLKCTRL_REG,
        DSS_CM_CORE_CM_DSS_DSS_CLKCTRL_REG_IDLEST));
*/

    CSL_FINST(ipuCmReg->CM_IPU1_IPU1_CLKCTRL_REG,
        IPU_CM_CORE_AON_CM_IPU1_IPU1_CLKCTRL_REG_MODULEMODE, AUTO);
/*
    while(CSL_IPU_CM_CORE_AON_CM_IPU1_IPU1_CLKCTRL_REG_IDLEST_FUNC !=
       CSL_FEXT(ipuCmReg->CM_IPU1_IPU1_CLKCTRL_REG,
        IPU_CM_CORE_AON_CM_IPU1_IPU1_CLKCTRL_REG_IDLEST));
*/
    l4PerCmReg->CM_L4PER_GPIO2_CLKCTRL_REG |= (
        (CM_L4PER_GPIO2_CLKCTRL_OPTFCLKEN_DBCLK_FCLK_EN << CM_L4PER_GPIO2_CLKCTRL_OPTFCLKEN_DBCLK_SHIFT) |
        (CM_L4PER_GPIO2_CLKCTRL_MODULEMODE_AUTO << CM_L4PER_GPIO2_CLKCTRL_MODULEMODE_SHIFT));

    l4PerCmReg->CM_L4PER_GPIO3_CLKCTRL_REG |= (
        (CM_L4PER_GPIO3_CLKCTRL_OPTFCLKEN_DBCLK_FCLK_EN << CM_L4PER_GPIO3_CLKCTRL_OPTFCLKEN_DBCLK_SHIFT) |
        (CM_L4PER_GPIO3_CLKCTRL_MODULEMODE_AUTO << CM_L4PER_GPIO3_CLKCTRL_MODULEMODE_SHIFT));

    l4PerCmReg->CM_L4PER_GPIO4_CLKCTRL_REG |= (
        (CM_L4PER_GPIO4_CLKCTRL_OPTFCLKEN_DBCLK_FCLK_EN << CM_L4PER_GPIO4_CLKCTRL_OPTFCLKEN_DBCLK_SHIFT) |
        (CM_L4PER_GPIO4_CLKCTRL_MODULEMODE_AUTO << CM_L4PER_GPIO4_CLKCTRL_MODULEMODE_SHIFT));

    l4PerCmReg->CM_L4PER_GPIO5_CLKCTRL_REG |= (
        (CM_L4PER_GPIO5_CLKCTRL_OPTFCLKEN_DBCLK_FCLK_EN << CM_L4PER_GPIO5_CLKCTRL_OPTFCLKEN_DBCLK_SHIFT) |
        (CM_L4PER_GPIO5_CLKCTRL_MODULEMODE_AUTO << CM_L4PER_GPIO5_CLKCTRL_MODULEMODE_SHIFT));

    l4PerCmReg->CM_L4PER_GPIO6_CLKCTRL_REG |= (
        (CM_L4PER_GPIO6_CLKCTRL_OPTFCLKEN_DBCLK_FCLK_EN << CM_L4PER_GPIO6_CLKCTRL_OPTFCLKEN_DBCLK_SHIFT) |
        (CM_L4PER_GPIO6_CLKCTRL_MODULEMODE_AUTO << CM_L4PER_GPIO6_CLKCTRL_MODULEMODE_SHIFT));

    l4PerCmReg->CM_L4PER_GPIO7_CLKCTRL_REG |= (
        (CM_L4PER_GPIO7_CLKCTRL_OPTFCLKEN_DBCLK_FCLK_EN << CM_L4PER_GPIO7_CLKCTRL_OPTFCLKEN_DBCLK_SHIFT) |
        (CM_L4PER_GPIO7_CLKCTRL_MODULEMODE_AUTO << CM_L4PER_GPIO7_CLKCTRL_MODULEMODE_SHIFT));

    l4PerCmReg->CM_L4PER_GPIO8_CLKCTRL_REG |= (
        (CM_L4PER_GPIO8_CLKCTRL_OPTFCLKEN_DBCLK_FCLK_EN << CM_L4PER_GPIO8_CLKCTRL_OPTFCLKEN_DBCLK_SHIFT) |
        (CM_L4PER_GPIO8_CLKCTRL_MODULEMODE_AUTO << CM_L4PER_GPIO8_CLKCTRL_MODULEMODE_SHIFT));

	return BOARD_SOK;
}
