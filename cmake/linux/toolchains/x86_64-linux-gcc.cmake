set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR x86_64)
set(CMAKE_CROSSCOMPILING TRUE)

set(CMAKE_C_COMPILER /usr/bin/x86_64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER /usr/bin/x86_64-linux-gnu-g++)

set(CMAKE_FIND_ROOT_PATH /usr/x86_64-linux-gnu)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

set(PKG_CONFIG_EXECUTABLE /usr/bin/x86_64-linux-gnu-pkg-config CACHE FILEPATH "pkg-config executable")

set(CPACK_READELF_EXECUTABLE /usr/bin/x86_64-linux-gnu-readelf)
set(CPACK_OBJCOPY_EXECUTABLE /usr/bin/x86_64-linux-gnu-objcopy)
set(CPACK_OBJDUMP_EXECUTABLE /usr/bin/x86_64-linux-gnu-objdump)
set(CPACK_PACKAGE_ARCHITECTURE x86_64)
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE x86_64)
