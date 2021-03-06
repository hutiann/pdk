#
# Macro definitions referenced below
#

empty =
space =$(empty) $(empty)

ARMV7OBJDIR ?= ./obj
ARMV7BINDIR ?= ./bin
ARMV7LIBDIR ?= ./lib

ARMV7OBJDIR := $(ARMV7OBJDIR)/qmss/example/InterruptTestMC
ARMV7OBJDIR_INFRADMA := $(ARMV7OBJDIR)/qmss/example/InterruptTestMC/InfraDMA
ARMV7OBJDIR_INFRADMA_SHAREDMEM := $(ARMV7OBJDIR)/qmss/example/InterruptTestMC/InfraDMASharedmem
ARMV7BINDIR := $(ARMV7BINDIR)/qmss/example

#Cross tools
ifdef CROSS_TOOL_INSTALL_PATH
# Support backwards compatibility with KeyStone1 approach
 CC = $(CROSS_TOOL_INSTALL_PATH)/$(CROSS_TOOL_PRFX)gcc
 AC = $(CROSS_TOOL_INSTALL_PATH)/$(CROSS_TOOL_PRFX)as
 AR = $(CROSS_TOOL_INSTALL_PATH)/$(CROSS_TOOL_PRFX)ar
 LD = $(CROSS_TOOL_INSTALL_PATH)/$(CROSS_TOOL_PRFX)gcc
endif

# INCLUDE Directories
QMSS_INC_DIR = $(PDK_INSTALL_PATH)/ti/drv/qmss
CPPI_INC_DIR = $(PDK_INSTALL_PATH)/ti/drv/cppi

QMSS_SRC_DIR ?= $(PDK_INSTALL_PATH)/ti/drv/qmss
QMSS_SAMPLE_DIR = $(QMSS_SRC_DIR)/example/InterruptTestMC
# must put DEVICE_SAMPLE_DIR first in search paths
QMSS_ARM_LIN_DEVICE_SAMPLE_DIR = $(QMSS_SAMPLE_DIR)/$(DEVICE)/armv7/linux
# must put COMMON_SAMPLE_DIR second in search paths
QMSS_ARM_LIN_COMMON_SAMPLE_DIR = $(QMSS_SAMPLE_DIR)/src/armv7/linux

INCDIR := $(PDK_INSTALL_PATH);$(CPPI_INC_DIR);$(QMSS_INC_DIR);$(QMSS_SAMPLE_DIR)/src;$(QMSS_ARM_LIN_DEVICE_SAMPLE_DIR);$(QMSS_ARM_LIN_COMMON_SAMPLE_DIR)

# Libraries
QMSS_LIB = -lqmss
CPPI_LIB = -lcppi
RM_LIB = -lrm
CMEM_LIB = -lticmem

LIBS     = $(QMSS_LIB) $(CPPI_LIB) $(RM_LIB) $(CMEM_LIB)
DYNAMIC_EXTN = _so

# Compiler options
INTERNALDEFS = $(DEBUG_FLAG) -D_FILE_OFFSET_BITS=64 -D__ARMv7 -DSOC_K2E -D_VIRTUAL_ADDR_SUPPORT -D__LINUX_USER_SPACE -D_LITTLE_ENDIAN=1 -DMAKEFILE_BUILD -DINTERNAL_LINKING_RAM -D_GNU_SOURCE
INTERNALDEFS_INFRADMA = $(DEBUG_FLAG) -D_FILE_OFFSET_BITS=64 -D__ARMv7 -DSOC_K2E -D_VIRTUAL_ADDR_SUPPORT -D__LINUX_USER_SPACE -D_LITTLE_ENDIAN=1 -DMAKEFILE_BUILD -DINTERNAL_LINKING_RAM -D_GNU_SOURCE -DWITH_INFRADMA
INTERNALDEFS_INFRADMA_SHAREDMEM = $(DEBUG_FLAG) -D_FILE_OFFSET_BITS=64 -D__ARMv7 -DSOC_K2E -D_VIRTUAL_ADDR_SUPPORT -D__LINUX_USER_SPACE -D_LITTLE_ENDIAN=1 -DMAKEFILE_BUILD -DINTERNAL_LINKING_RAM -D_GNU_SOURCE -DWITH_INFRADMA -DSHARED_MEMORY

# Linker options
INTERNALLINKDEFS = -Wl,--start-group -L $(ARMV7LIBDIR) -Wl,-Bstatic $(LIBS)  -Wl,-Bdynamic -lrt -Wl,--end-group -pthread $(LDFLAGS)
INTERNALLINKDEFS_DYNAMIC = -Wl,--start-group -L $(ARMV7LIBDIR) $(LIBS) -lrt -Wl,--end-group -pthread $(LDFLAGS)

EXE=qmInterruptTestMultiprocess.out
EXE_INFRADMA=qmInterruptTestMultiprocessInfraDMA.out

EXE_DYNAMIC=qmInterruptTestMultiprocess$(DYNAMIC_EXTN).out
EXE_INFRADMA_DYNAMIC=qmInterruptTestMultiprocessInfraDMA$(DYNAMIC_EXTN).out

EXE_INFRADMA_SHAREDMEM_DYNAMIC=qmInterruptTestMultiprocessInfraDMASharedmem$(DYNAMIC_EXTN).out

OBJEXT = o
SRCDIR = $(QMSS_SAMPLE_DIR)/src:$(QMSS_ARM_LIN_DEVICE_SAMPLE_DIR):$(QMSS_ARM_LIN_COMMON_SAMPLE_DIR):

INCS = -I. -I$(strip $(subst ;, -I,$(INCDIR)))

VPATH=$(SRCDIR)

#List the Source Files
TEST_SRC = \
    qmss_interrupt_test_multiprocess.c \
    linuxutil.c \
    fw_main.c \
    fw_init.c \
    fw_osal.c \
    fw_mem_allocator.c \
    sockutils.c

# FLAGS for the SourceFiles
SRC_CFLAGS = -I. $(CFLAGS)

# Make Rule for the SRC Files
TEST_SRC_OBJS = $(patsubst %.c, $(ARMV7OBJDIR)/%.$(OBJEXT), $(TEST_SRC))
TEST_SRC_OBJS_INFRADMA = $(patsubst %.c, $(ARMV7OBJDIR_INFRADMA)/%.$(OBJEXT), $(TEST_SRC))
TEST_SRC_OBJS_INFRADMA_SHAREDMEM = $(patsubst %.c, $(ARMV7OBJDIR_INFRADMA_SHAREDMEM)/%.$(OBJEXT), $(TEST_SRC))

all:$(ARMV7BINDIR)/$(EXE) $(ARMV7BINDIR)/$(EXE_INFRADMA) $(ARMV7BINDIR)/$(EXE_DYNAMIC) $(ARMV7BINDIR)/$(EXE_INFRADMA_DYNAMIC) $(ARMV7BINDIR)/$(EXE_INFRADMA_SHAREDMEM_DYNAMIC)

$(ARMV7BINDIR)/$(EXE): $(TEST_SRC_OBJS) $(ARMV7BINDIR)/.created $(ARMV7OBJDIR)/.created
	@echo linking $(TEST_SRC_OBJS) into $@ ...
	@$(CC) $(TEST_SRC_OBJS) $(INTERNALLINKDEFS) -o $@

$(ARMV7BINDIR)/$(EXE_INFRADMA): $(TEST_SRC_OBJS_INFRADMA) $(ARMV7BINDIR)/.created $(ARMV7OBJDIR_INFRADMA)/.created
	@echo linking $(TEST_SRC_OBJS_INFRADMA) into $@ ...
	@$(CC) $(TEST_SRC_OBJS_INFRADMA) $(INTERNALLINKDEFS) -o $@

$(ARMV7BINDIR)/$(EXE_DYNAMIC): $(TEST_SRC_OBJS) $(ARMV7BINDIR)/.created $(ARMV7OBJDIR)/.created
	@echo linking $(TEST_SRC_OBJS) into $@ ...
	@$(CC) $(TEST_SRC_OBJS) $(INTERNALLINKDEFS_DYNAMIC) -o $@

$(ARMV7BINDIR)/$(EXE_INFRADMA_DYNAMIC): $(TEST_SRC_OBJS_INFRADMA) $(ARMV7BINDIR)/.created $(ARMV7OBJDIR_INFRADMA)/.created
	@echo linking $(TEST_SRC_OBJS_INFRADMA) into $@ ...
	@$(CC) $(TEST_SRC_OBJS_INFRADMA) $(INTERNALLINKDEFS_DYNAMIC) -o $@

$(ARMV7BINDIR)/$(EXE_INFRADMA_SHAREDMEM_DYNAMIC): $(TEST_SRC_OBJS_INFRADMA_SHAREDMEM) $(ARMV7BINDIR)/.created $(ARMV7OBJDIR_INFRADMA)/.created
	@echo linking $(TEST_SRC_OBJS_INFRADMA_SHAREDMEM) into $@ ...
	@$(CC) $(TEST_SRC_OBJS_INFRADMA_SHAREDMEM) $(INTERNALLINKDEFS_DYNAMIC) -o $@

$(ARMV7OBJDIR)/%.$(OBJEXT): %.c $(ARMV7OBJDIR)/.created
	@echo compiling $< ...
	@$(CC) -c $(SRC_CFLAGS) $(INTERNALDEFS) $(INCS)  $< -o $@

$(ARMV7OBJDIR_INFRADMA)/%.$(OBJEXT): %.c $(ARMV7OBJDIR_INFRADMA)/.created
	@echo compiling $< ...
	@$(CC) -c $(SRC_CFLAGS) $(INTERNALDEFS_INFRADMA) $(INCS)  $< -o $@

$(ARMV7OBJDIR_INFRADMA_SHAREDMEM)/%.$(OBJEXT): %.c $(ARMV7OBJDIR_INFRADMA_SHAREDMEM)/.created
	@echo compiling $< ...
	@$(CC) -c $(SRC_CFLAGS) $(INTERNALDEFS_INFRADMA_SHAREDMEM) $(INCS)  $< -o $@

$(ARMV7OBJDIR)/.created:
	@mkdir -p $(ARMV7OBJDIR)
	@touch $(ARMV7OBJDIR)/.created

$(ARMV7OBJDIR_INFRADMA)/.created:
	@mkdir -p $(ARMV7OBJDIR_INFRADMA)
	@touch $(ARMV7OBJDIR_INFRADMA)/.created

$(ARMV7OBJDIR_INFRADMA_SHAREDMEM)/.created:
	@mkdir -p $(ARMV7OBJDIR_INFRADMA_SHAREDMEM)
	@touch $(ARMV7OBJDIR_INFRADMA_SHAREDMEM)/.created

clean:
	@rm -fr $(ARMV7OBJDIR)
	@rm -fr $(ARMV7OBJDIR_INFRADMA)
	@rm -fr $(ARMV7BINDIR)
