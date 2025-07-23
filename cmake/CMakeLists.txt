cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 17)
project(frcc VERSION 0.0.0 LANGUAGES C CXX)

option(frcc_BUILD_TEST OFF)

set(FRROOT ${PROJECT_SOURCE_DIR}/..)
set(FRROOT_SRC ${XSROOT}/src)
set(THIRD_PARTY_ROOT ${XSROOT}/cmake/external)

include_directories(
        ${XSROOT}/include/
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_VERBOSE_MAKEFILE ON)
endif (CMAKE_BUILD_TYPE STREQUAL "Debug")

# Set flags to build type
set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -Wall -Wpedantic -Wno-narrowing")
set(EXTRA_C_FLAGS_RELEASE "${EXTRA_C_FLAGS_RELEASE}")
set(EXTRA_C_FLAGS_DEBUG   "${EXTRA_C_FLAGS_DEBUG} -g3 -pg -fsanitize=address -fsanitize=leak")

# Set compiler options
set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} ${EXTRA_C_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${EXTRA_C_FLAGS_RELEASE} -O3 -ffast-math")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${EXTRA_C_FLAGS_DEBUG} -ffast-math")
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} ${EXTRA_C_FLAGS_DEBUG} -O0")

include(frcc_common.cmake)

add_library(frcc
        ${frcc_common_src}
)

set_target_properties(frcc PROPERTIES VERSION ${PROJECT_VERSION})

install(TARGETS frcc
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_LIBDIR}/include)

install(DIRECTORY ${FRROOT}/include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/xsdnn)

# if (frcc_BUILD_TEST)
#     include(frcc_unittest.cmake)
# endif (frcc_BUILD_TEST)

include(summary.cmake)
frcc_print_configuration_summary()