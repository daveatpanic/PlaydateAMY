HEAP_SIZE      = 8388208
STACK_SIZE     = 61800

PRODUCT = AMY_Test.pdx

# Locate the SDK
SDK = ${PLAYDATE_SDK_PATH}
ifeq ($(SDK),)
	SDK = $(shell egrep '^\s*SDKRoot' ~/.Playdate/config | head -n 1 | cut -c9-)
endif

ifeq ($(SDK),)
$(error SDK path not found; set ENV value PLAYDATE_SDK_PATH)
endif

######
# IMPORTANT: You must add your source folders to VPATH for make to find them
# ex: VPATH += src1:src2
######

AMYDIR = amy/src

VPATH += src:$(AMYDIR)

AMY = $(AMYDIR)/amy.c \
	  $(AMYDIR)/algorithms.c \
	  $(AMYDIR)/custom.c \
	  $(AMYDIR)/delay.c \
	  $(AMYDIR)/envelope.c \
	  $(AMYDIR)/filters.c \
	  $(AMYDIR)/log2_exp2.c \
	  $(AMYDIR)/oscillators.c \
	  $(AMYDIR)/partials.c \
	  $(AMYDIR)/patches.c \
	  $(AMYDIR)/pcm.c \
	  $(AMYDIR)/sequencer.c \
	  $(AMYDIR)/transfer.c \

# List C source files here
SRC = src/main.c $(AMY)

# List all user directories here
UINCDIR = $(AMYDIR)

# List user asm files
UASRC = 

# List all user C define here, like -D_DEBUG=1
UDEFS = 

# Define ASM defines here
UADEFS = 

# List the user directory to look for the libraries here
ULIBDIR =

# List all user libraries here
ULIBS =

include $(SDK)/C_API/buildsupport/common.mk

CPFLAGS += -Wno-strict-prototypes -Wno-strict-aliasing
LDFLAGS += -specs=nosys.specs
