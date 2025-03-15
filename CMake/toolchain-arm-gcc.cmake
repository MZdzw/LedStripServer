# System Generic - no OS bare-metal application
# set(CMAKE_SYSTEM_NAME Generic)
# 
# # Setup arm processor and bleeding edge toolchain
# set(CMAKE_SYSTEM_PROCESSOR arm)
# set(CMAKE_C_COMPILER arm-none-eabi-gcc)
# set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
# set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
# set(CMAKE_AR arm-none-eabi-ar)
# set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
# set(CMAKE_SIZE arm-none-eabi-size)
# set(CMAKE_OBJDUMP arm-none-eabi-objdump)
# set(CMAKE_NM arm-none-eabi-nm)
# set(CMAKE_STRIP arm-none-eabi-strip)
# set(CMAKE_RANLIB arm-none-eabi-ranlib)
# 
# set(CMAKE_EXECUTABLE_SUFFIX_ASM     ".elf")
# set(CMAKE_EXECUTABLE_SUFFIX_C       ".elf")
# set(CMAKE_EXECUTABLE_SUFFIX_CXX     ".elf")
# 
# set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CROSS_COMPILE "arm-none-linux-gnueabihf")

set(CROSS_COMPILE_CC "${CROSS_COMPILE}-gcc")
set(CROSS_COMPILE_CXX "${CROSS_COMPILE}-g++")
set(CROSS_COMPILE_LD "${CROSS_COMPILE}-ld")
set(CROSS_COMPILE_AR "${CROSS_COMPILE}-ar")
set(CROSS_COMPILE_RANLIB "${CROSS_COMPILE}-ranlib")
set(CROSS_COMPILE_STRIP "${CROSS_COMPILE}-strip")
set(CROSS_COMPILE_NM "${CROSS_COMPILE}-nm")
set(CROSS_COMPILE_OBJCOPY "${CROSS_COMPILE}-objcopy")
set(CROSS_COMPILE_SIZE "${CROSS_COMPILE}-size")

# At the very least, cross compile gcc and g++ have to be set!
find_program (CROSS_COMPILE_CC_FOUND ${CROSS_COMPILE_CC} REQUIRED)
find_program (CROSS_COMPILE_CXX_FOUND ${CROSS_COMPILE_CXX} REQUIRED)

set(CMAKE_CROSSCOMPILING TRUE)
set(CMAKE_SYSROOT "${SYSROOT_PATH}")

# Define name of the target system
set(CMAKE_SYSTEM_NAME "Linux")
set(CMAKE_SYSTEM_PROCESSOR "arm")

# Define the compiler
set(CMAKE_C_COMPILER ${CROSS_COMPILE_CC})
set(CMAKE_CXX_COMPILER ${CROSS_COMPILE_CXX})

set(CMAKE_PREFIX_PATH 
	"${CMAKE_PREFIX_PATH}"
	"${SYSROOT_PATH}/usr/lib/${CROSS_COMPILE}"
)

set(CMAKE_C_FLAGS 
	"-march=armv7-a -mtune=cortex-a8 -mfpu=neon -mfloat-abi=hard ${COMMON_FLAGS}" 
	CACHE STRING "Flags for Beagle Bone Black"
)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS}" 
	CACHE STRING "Flags for Beagle Bone Black"
)

set(CMAKE_FIND_ROOT_PATH 
	"${CMAKE_INSTALL_PREFIX};${CMAKE_PREFIX_PATH};${CMAKE_SYSROOT}"
)

# search for programs in the build host directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
