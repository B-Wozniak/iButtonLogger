# global symbols
SYMBOLS =

# ASM definitions
AS_DEFS =

# additional directories with source files (absolute or relative paths to
# folders with source files, current folder is always included)
SRCS_DIRS = 
SRCS_DIRS +=
SRCS_DIRS +=
SRCS_DIRS +=


# include directories (absolute or relative paths to additional folders with
# headers, current folder is always included)
INC_DIRS += CMSIS
INC_DIRS += 
INC_DIRS +=


# library directories (absolute or relative paths to additional folders with
# libraries)
LIB_DIRS =

# libraries (additional libraries for linking, e.g. "-lm -lsome_name" to link
# math library libm.a and libsome_name.a)
LIBS =

# optimization flags ("-O0" - no optimization, "-O1" - optimize, "-O2" -
# optimize even more, "-Os" - optimize for size or "-O3" - optimize yet more) 
OPTIMIZATION_RELEASE = -Os
OPTIMIZATION_DEBUG = -O0

# nanolibc
USE_NANOLIB = 0

# SEMIHOSTING
USE_SEMIHOSTING = 0

# set to 1 to optimize size by removing unused code and data during link phase
REMOVE_UNUSED = 1

# set to 1 to use enum lenght short as posible
SHORT_ENUMS = 0

# set to 1 for use packed structures
PACK_STRUCT = 0

# set to 1 to compile and link additional code required for C++
USES_CXX = 0

CPU = STM32L476xx

CORE = cortex-m4

FLASH = 1024*1024

RAM = 128*1024


