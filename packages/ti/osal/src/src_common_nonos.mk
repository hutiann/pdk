
SRCDIR = . src/nonos arch/core
INCDIR = . src/nonos arch/core

SRCS_COMMON += HwiP_nonos.c SwiP_nonos.c Utils_nonos.c SemaphoreP_nonos.c RegisterIntr_nonos.c Core_utils.c

ifeq ($(SOC),$(filter $(SOC),am571x am572x am574x am335x am437x k2h k2k k2e k2l k2g c6678 c6657 omapl137 omapl138 am65xx j721e j7200))
  SRCDIR      += soc/$(SOC)
  SRCS_COMMON += TimerP_default.c
endif

ifeq ($(CORE),mpu1_0)
  SRCDIR += arch/core/a53
  SRCS_COMMON += CacheP_nonos.c Arch_util.c
endif

ifeq ($(CORE),$(filter $(CORE), mcu1_0 mcu2_0 mcu2_1))
  SRCDIR += arch/core/r5
  SRCS_COMMON += CacheP_nonos.c Arch_util.c
  SRCS_ASM_COMMON += TimestampProvider_asm.asm
endif

ifeq ($(CORE),a15_0)
  SRCDIR += arch/core/a15
  SRCS_COMMON += CacheP_nonos.c Arch_util.c
  SRCS_ASM_COMMON += TimestampProvider_asm.asm
endif

ifeq ($(CORE),arm9_0)
  SRCDIR += arch/core/arm9
  SRCS_COMMON += CacheP_nonos.c Arch_util.c
endif

ifeq ($(CORE),$(filter $(CORE), c66x c66xdsp_1 c66xdsp_2 c674x))
  SRCDIR += arch/core/c6x
  SRCS_COMMON += CacheP_nonos.c Arch_util.c EventCombinerP_nonos.c
endif

ifeq ($(CORE),$(filter $(CORE), c7x_1 c7x-hostemu))
  SRCDIR += arch/core/c7x
  SRCS_COMMON += CacheP_nonos.c Arch_util.c
endif

ifeq ($(CORE),ipu1_0)
  ifeq ($(SOC),$(filter $(SOC),am571x am572x am574x dra72x dra75x dra78x))
    SRCDIR += arch/core/m4
    SRCS_COMMON += CacheP_nonos.c Arch_util.c
  endif
endif

ifeq ($(SOC),$(filter $(SOC),am437x am335x))
SRCS_COMMON += cache_arm.c \
				cpu.c

SRCS_ASM_COMMON += cp15.asm
SRCDIR+=$(PDK_INSTALL_PATH)/ti/starterware/soc \
	$(PDK_INSTALL_PATH)/ti/starterware/soc/armv7a \
	$(PDK_INSTALL_PATH)/ti/starterware/soc/armv7a/gcc

INCDIR +=   $(PDK_INSTALL_PATH)/ti/starterware/include \
            $(PDK_INSTALL_PATH)/ti/starterware/include/hw \
            $(PDK_INSTALL_PATH)/ti/starterware/board \
            $(PDK_INSTALL_PATH)/ti/starterware/device \
            $(PDK_INSTALL_PATH)/ti/starterware/include/utils \
            $(PDK_INSTALL_PATH)/ti/starterware/utils \
            $(PDK_INSTALL_PATH)/ti/starterware/soc \
            $(PDK_INSTALL_PATH)/ti/starterware/include/armv7a \
            $(PDK_INSTALL_PATH)/ti/starterware/soc/armv7a \
            $(PDK_INSTALL_PATH)/ti/starterware/soc/armv7a/gcc



ifeq ($(SOC),$(filter $(SOC),am437x))
  SRCS_COMMON += pl310.c CacheP_nonos.c osalgic.c Arch_util.c
  SRCDIR += arch/core/a9
  SRCS_ASM_COMMON += pub2mon.asm
  SRCS_ASM_COMMON += TimestampProvider_asm.asm
  CFLAGS_LOCAL_COMMON += -DOUTER_CACHE_ENABLE
  INCDIR += $(PDK_INSTALL_PATH)/ti/starterware/soc/am43xx/am437x \
		$(PDK_INSTALL_PATH)/ti/starterware/board/am43xx \
        $(PDK_INSTALL_PATH)/ti/starterware/soc/am43xx \
        $(PDK_INSTALL_PATH)/ti/starterware/include/am43xx \
        $(PDK_INSTALL_PATH)/ti/starterware/include/am43xx/am437x
endif


ifeq ($(SOC),$(filter $(SOC),am335x))
  SRCS_COMMON += pl310.c CacheP_nonos.c osalintc.c Arch_util.c
  SRCDIR += arch/core/a8
  SRCS_ASM_COMMON += TimestampProvider_asm.asm
  INCDIR += $(PDK_INSTALL_PATH)/ti/starterware/soc/am33xx/am335x \
		$(PDK_INSTALL_PATH)/ti/starterware/board/am33xx \
        $(PDK_INSTALL_PATH)/ti/starterware/soc/am33xx \
        $(PDK_INSTALL_PATH)/ti/starterware/include/am33xx \
        $(PDK_INSTALL_PATH)/ti/starterware/include/am33xx/am335x
endif


endif
PACKAGE_SRCS_COMMON = makefile HwiP.h SwiP.h MuxIntcP.h osal.h osal_component.mk SemaphoreP.h CacheP.h \
                      TimerP.h RegisterIntr.h EventCombinerP.h Queue.h TaskP.h EventP.h \
                      arch soc src/nonos src/src_common_nonos.mk  src/src_common_tirtos.mk\
                      docs/OSAL_SoftwareManifest.html docs/ReleaseNotes_OSAL.pdf \
                      build/makefile_nonos_indp.mk build/makefile_nonos.mk build/makefile_tirtos_indp.mk build/makefile_tirtos.mk

