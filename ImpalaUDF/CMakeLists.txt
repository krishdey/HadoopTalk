# @author krishdey

cmake_minimum_required(VERSION 2.6)

# where to put generated libraries
set(LIBRARY_OUTPUT_PATH "build")

# First look in LLVM_HOME
find_program(CLANG_EXECUTABLE clang++
    PATHS
    $ENV{LLVM_HOME}
    PATH_SUFFIXES bin
    NO_DEFAULT_PATH
)
# Then try default locations if env var not set
find_program(CLANG_EXECUTABLE clang++)

message(STATUS "Found clang executable: ${CLANG_EXECUTABLE}")

# find jni headers and libs
find_package(JNI REQUIRED)
include_directories(${JNI_INCLUDE_DIRS})
set(LIBS ${LIBS} ${JNI_LIBRARIES})
message(STATUS "JNI_INCLUDE_DIRS: ${JNI_INCLUDE_DIRS}")
message(STATUS "JNI_LIBRARIES: ${JNI_LIBRARIES}")


# this function generates a rule to cross compile a source file to an IR module
set(IR_COMPILE_FLAGS "-emit-llvm" "-O3" "-c")
function(COMPILE_TO_IR SRC_FILE)
  get_filename_component(BASE_NAME ${SRC_FILE} NAME_WE)
  set(OUTPUT_FILE "build/${BASE_NAME}.ll")
  add_custom_command(
    OUTPUT ${OUTPUT_FILE}
    COMMAND ${CLANG_EXECUTABLE} ${IR_COMPILE_FLAGS} ${SRC_FILE} -o ${OUTPUT_FILE}
    DEPENDS ${SRC_FILE})
  add_custom_target(${BASE_NAME}-ir ALL DEPENDS ${OUTPUT_FILE})
endfunction(COMPILE_TO_IR)

# Build the UDA/UDFs into a shared library.  You can have multiple UDFs per
# file, and/or specify multiple files here.
add_library(impalavoltageudf SHARED ImpalaVoltageUDF.cpp)

# The resulting LLVM IR module will have the same name as the .cc file
if (CLANG_EXECUTABLE)
  COMPILE_TO_IR(ImpalaVoltageUDF.cpp)
endif(CLANG_EXECUTABLE)
