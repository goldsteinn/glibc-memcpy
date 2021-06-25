cmake_minimum_required(VERSION 3.1.3)
project(Personal-GLIBC-Development-Suite C ASM)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)

add_compile_options(-D_GNU_SOURCE -O3 -Wall -Wextra -Wno-unused-function -march=native -mtune=native)

set(EXE_SRC "src/driver.c")
set(EXE "driver")


include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src")

set (EXCLUDE_DIR "/core/")
file(GLOB_RECURSE ASM_SOURCES "${CMAKE_SOURCE_DIR}/src/*.S")

foreach (TMP_PATH ${ASM_SOURCES})
    string (FIND ${TMP_PATH} ${EXCLUDE_DIR} EXCLUDE_DIR_FOUND)
    if (NOT ${EXCLUDE_DIR_FOUND} EQUAL -1)
        list (REMOVE_ITEM ASM_SOURCES ${TMP_PATH})
    endif ()
endforeach(TMP_PATH)


set_source_files_properties(${ASM_SOURCES} PROPERTIES COMPILE_FLAGS "-x assembler-with-cpp")

file(GLOB_RECURSE C_SOURCES "${CMAKE_SOURCE_DIR}/src/*.c")
list(REMOVE_ITEM C_SOURCES ${EXE_SRC})

add_executable(${EXE} ${EXE_SRC} ${C_SOURCES} ${ASM_SOURCES})
target_link_libraries(${EXE} "-lm")

