#=============================================================================#
# project configuration
#=============================================================================#

ifndef PROJ_TYPE
#PROJ_TYPE = Release
PROJ_TYPE = Debug
endif

ifndef PROJECT
PROJECT = iButtonLogger
endif

include proj_settings.mk

# extension of C++ files
CXX_EXT = cpp

# wildcard for C++ source files (all files with CXX_EXT extension found in
# current folder and SRCS_DIRS folders will be compiled and linked)
CXX_SRCS = $(wildcard $(patsubst %, %/*.$(CXX_EXT), . $(SRCS_DIRS)))

# extension of C files
C_EXT = c

# wildcard for C source files (all files with C_EXT extension found in current
# folder and SRCS_DIRS folders will be compiled and linked)
C_SRCS = $(wildcard $(patsubst %, %/*.$(C_EXT), . $(SRCS_DIRS)))

# extension of ASM files
AS_EXT = S

# wildcard for ASM source files (all files with AS_EXT extension found in
# current folder and SRCS_DIRS folders will be compiled and linked)
AS_SRCS = $(wildcard $(patsubst %, %/*.$(AS_EXT), . $(SRCS_DIRS)))

# optimization flags ("-O0" - no optimization, "-O1" - optimize, "-O2" -
# optimize even more, "-Os" - optimize for size or "-O3" - optimize yet more) 
ifeq ($(PROJ_TYPE), Debug)
OPTIMIZATION = $(OPTIMIZATION_DEBUG)
SYMBOLS += DEBUG
endif
ifeq ($(PROJ_TYPE), Release)
OPTIMIZATION = $(OPTIMIZATION_RELEASE)
endif

# linker script
LD_SCRIPT = linker.ld

# output folder (absolute or relative path, leave empty for in-tree compilation)
OUT_DIR = $(PROJ_TYPE)

# define warning options here
CXX_WARNINGS = -Wall -Wextra
C_WARNINGS = -Wall -Wstrict-prototypes -Wextra -Wno-strict-aliasing

# C++ language standard ("c++98", "gnu++98" - default, "c++0x", "gnu++0x")
CXX_STD = gnu++11

# C language standard ("c89" / "iso9899:1990", "iso9899:199409",
# "c99" / "iso9899:1999", "gnu89" - default, "gnu99")
C_STD = gnu99

#=============================================================================#
# set the VPATH according to SRCS_DIRS
#=============================================================================#

VPATH = $(SRCS_DIRS)

#=============================================================================#
# when using output folder, append trailing slash to its name
#=============================================================================#

ifeq ($(strip $(OUT_DIR)), )
	OUT_DIR_F =
else
	OUT_DIR_F = $(strip $(OUT_DIR))/
endif

#=============================================================================#
# various compilation flags
#=============================================================================#

# core flags
CORE_FLAGS = -mcpu=$(CORE) -mthumb
ifeq ($(CORE), cortex-m4)
CORE_FLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16 -ffast-math
endif

# flags for C++ compiler
CXX_FLAGS = -std=$(CXX_STD) -fno-rtti -fno-exceptions -fverbose-asm -Wa,-ahlms=$(OUT_DIR_F)$(notdir $(<:.$(CXX_EXT)=.lst))
ifeq ($(PROJ_TYPE), Debug)
CXX_FLAGS += -g -ggdb3
endif

# flags for C compiler
C_FLAGS = -std=$(C_STD) -fverbose-asm -Wa,-ahlms=$(OUT_DIR_F)$(notdir $(<:.$(C_EXT)=.lst)) 
ifeq ($(PROJ_TYPE), Debug)
C_FLAGS += -g -ggdb3
endif
ifeq ($(PACK_STRUCT), 1)
C_FLAGS += -fpack-struct 
endif
ifeq ($(SHORT_ENUMS), 1)
C_FLAGS += -fshort-enums
endif


# flags for assembler
AS_FLAGS = -Wa,-amhls=$(OUT_DIR_F)$(notdir $(<:.$(AS_EXT)=.lst))
ifeq ($(PROJ_TYPE), Debug)
AS_FLAGS += -g -ggdb3  
endif

# flags for linker
LD_FLAGS = -T$(LD_SCRIPT) -g3 -Wl,-Map=$(OUT_DIR_F)$(PROJECT).map,--cref,--no-warn-mismatch

# process option for removing unused code
ifeq ($(REMOVE_UNUSED), 1)
	LD_FLAGS += -Wl,--gc-sections
	OPTIMIZATION += -ffunction-sections -fdata-sections
	C_FLAGS += -Wno-unused-function
endif

# if __USES_CXX is defined for ASM then code for global/static constructors /
# destructors is compiled; if -nostartfiles option for linker is added then C++
# initialization / finalization code is not linked
ifeq ($(USES_CXX), 1)
	AS_DEFS += -D__USES_CXX
else
	LD_FLAGS += -nostartfiles
endif


ifeq ($(USE_NANOLIB), 1)
AS_DEFS += -DUSE_NANOLIB
SYMBOLS += USE_NANOLIB
LD_FLAGS += --specs=nano.specs 
endif

ifeq ($(USE_SEMIHOSTING), 1)
ifeq ($(PROJ_TYPE), Debug)
SYMBOLS += SEMIHOSTING
endif
endif

#=============================================================================#
# do some formatting
#=============================================================================#

CXX_OBJS = $(addprefix $(OUT_DIR_F), $(notdir $(CXX_SRCS:.$(CXX_EXT)=.o)))
C_OBJS = $(addprefix $(OUT_DIR_F), $(notdir $(C_SRCS:.$(C_EXT)=.o)))
AS_OBJS = $(addprefix $(OUT_DIR_F), $(notdir $(AS_SRCS:.$(AS_EXT)=.o)))
OBJS = $(AS_OBJS) $(C_OBJS) $(CXX_OBJS) $(USER_OBJS)
DEPS = $(OBJS:.o=.d)
INC_DIRS_F = -I. $(patsubst %, -I%, $(INC_DIRS))
LIB_DIRS_F = $(patsubst %, -L%, $(LIB_DIRS))
GLOBAL_SYMB = $(patsubst %, -D%, $(SYMBOLS)) $(patsubst %, -D%, $(CPU))

ELF = $(OUT_DIR_F)$(PROJECT).elf
HEX = $(OUT_DIR_F)$(PROJECT).hex
BIN = $(OUT_DIR_F)$(PROJECT).bin
LSS = $(OUT_DIR_F)$(PROJECT).lss
DMP = $(OUT_DIR_F)$(PROJECT).dmp

# format final flags for tools, request dependancies for C++, C and asm
CXX_FLAGS_F = $(CORE_FLAGS) $(OPTIMIZATION) $(CXX_WARNINGS) $(GLOBAL_SYMB) -MD -MP -MF $(OUT_DIR_F)$(@F:.o=.d) $(INC_DIRS_F)
C_FLAGS_F = $(CORE_FLAGS) $(OPTIMIZATION) $(C_WARNINGS) $(C_FLAGS) $(GLOBAL_SYMB) -MD -MP -MF $(OUT_DIR_F)$(@F:.o=.d) $(INC_DIRS_F)
AS_FLAGS_F = $(CORE_FLAGS) $(AS_FLAGS) $(AS_DEFS) -MD -MP -MF $(OUT_DIR_F)$(@F:.o=.d) $(INC_DIRS_F)
LD_FLAGS_F = $(CORE_FLAGS) $(LD_FLAGS) $(LIB_DIRS_F)

#contents of output directory
GENERATED = $(wildcard $(patsubst %, $(OUT_DIR_F)*.%, bin d dmp elf hex lss lst map o))

#=============================================================================#
# toolchain configuration
#=============================================================================#

TOOLCHAIN = arm-none-eabi-

CXX = $(TOOLCHAIN)g++
CC = $(TOOLCHAIN)gcc
AS = $(TOOLCHAIN)gcc -x assembler-with-cpp
OBJCOPY = $(TOOLCHAIN)objcopy
OBJDUMP = $(TOOLCHAIN)objdump
SIZE = $(TOOLCHAIN)size
RM = rm -f

#=============================================================================#
# make all
#=============================================================================#

all : make_output_dir $(ELF) $(LSS) $(DMP) $(HEX) $(BIN) print_size

# make object files dependent on Makefile
$(OBJS) : makefile proj_settings.mk
# make .elf file dependent on linker script
$(ELF) : $(LD_SCRIPT)
print_size : $(ELF)

#-----------------------------------------------------------------------------#
# linking - objects -> elf
#-----------------------------------------------------------------------------#

$(ELF) : $(OBJS)
	@echo ' '
	@echo '###	Linking target:      $(ELF)'
	$(CXX) $(LD_FLAGS_F) $(OBJS) $(LIBS) -o $@
	@echo ' '

#-----------------------------------------------------------------------------#
# compiling - C++ source -> objects
#-----------------------------------------------------------------------------#

$(OUT_DIR_F)%.o : %.$(CXX_EXT)
	@echo '###	Compiling  file:     $<'
	$(CXX) -c $(CXX_FLAGS_F) $< -o $@ 
	@echo ' '

#-----------------------------------------------------------------------------#
# compiling - C source -> objects
#-----------------------------------------------------------------------------#

$(OUT_DIR_F)%.o : %.$(C_EXT)
	@echo '###	Compiling  file:     $<'
	$(CC) -c $(C_FLAGS_F) $< -o $@
#	@echo ' '

#-----------------------------------------------------------------------------#
# assembling - ASM source -> objects
#-----------------------------------------------------------------------------#

$(OUT_DIR_F)%.o : %.$(AS_EXT)
	@echo '###	Assembling file:     $<'
	$(AS) -c $(AS_FLAGS_F) $< -o $@
#	@echo ' '
 
#-----------------------------------------------------------------------------#
# memory images - elf -> hex, elf -> bin
#-----------------------------------------------------------------------------#

$(HEX) : $(ELF)
	@echo '###	Creating IHEX image:       $(HEX)'
	$(OBJCOPY) -j .text -j .data -O ihex $< $@
	@cp $(OUT_DIR_F)/$(PROJECT).hex  $(OUT_DIR_F)/output.hex
	@echo ' '

$(BIN) : $(ELF)
	@echo '###	Creating binary image:     $(BIN)'
	$(OBJCOPY) -j .text -j .data -O binary $< $@
	@cp $(OUT_DIR_F)/$(PROJECT).bin  $(OUT_DIR_F)/output.bin
	@echo ' '

#-----------------------------------------------------------------------------#
# memory dump - elf -> dmp
#-----------------------------------------------------------------------------#

$(DMP) : $(ELF)
	@echo '###	Creating memory dump:      $(DMP)           '
	$(OBJDUMP) -x --syms $< > $@
	@echo ' '

#-----------------------------------------------------------------------------#
# extended listing - elf -> lss
#-----------------------------------------------------------------------------#

$(LSS) : $(ELF)
	@echo '###	Creating extended listing: $(LSS)           '
	$(OBJDUMP) -S $< > $@
	@echo ' '

#-----------------------------------------------------------------------------#
# print the size of the objects and the .elf file
#-----------------------------------------------------------------------------#

print_size : $(ELF)
	@echo "###	Size of modules:                            "  
	@$(SIZE) -B -t --common $(OBJS) $(USER_OBJS)
	@echo ' '
	@echo "###Size of target .elf file:                     "  
	@$(SIZE) -B $(ELF)
	@echo ' '
	@$(SIZE) -B $(ELF) | gawk 'NR > 1 {printf "FLASH: %6.2f%%%10dB%7dkB\n  RAM: %6.2f%%%10dB%7dkB", (($$1+$$2)*100/($(FLASH))) ,$$1+$$2, ($$1+$$2)/1024, (($$2+$$3)*100/($(RAM))), $$2+$$3, ($$2+$$3)/1024 }' 
	@echo ' '

#-----------------------------------------------------------------------------#
# create the desired output directory
#-----------------------------------------------------------------------------#

make_output_dir :
	$(shell mkdir $(OUT_DIR_F) 2>/dev/null)

#=============================================================================#
# make clean
#=============================================================================#

clean:
ifeq ($(strip $(OUT_DIR_F)), )
	@echo 'Removing all generated output files'
else
	@echo 'Removing all generated output files from output directory: $(OUT_DIR_F)'
endif
ifneq ($(strip $(GENERATED)), )
	$(RM) $(GENERATED)
else
	@echo 'Nothing to remove...'
endif

#=============================================================================#
# global exports
#=============================================================================#

.PHONY: all clean dependents

.SECONDARY:

# include dependancy files
-include $(DEPS)
