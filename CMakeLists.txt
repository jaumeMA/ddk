cmake_minimum_required(VERSION 3.13.0)

project("${SolutionName}")


include("${CMAKE_SOURCE_DIR}/SetupConfig.cmake")

set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(TEST_DIR "${CMAKE_SOURCE_DIR}/test")

set(TARGET_DIR "${CMAKE_SOURCE_DIR}/build")
set(OUTPUT_DIR "${CMAKE_SOURCE_DIR}/bin")
set(CONFIG_DIR "${CMAKE_SOURCE_DIR}/config")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

###	SETTING INSTALLED LIBRARIES	###

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
	
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")

endif()

if(${SolutionName} STREQUAL "Test")
	include("${TEST_DIR}/dependencies.cmake")
else()
	include("${SRC_DIR}/dependencies.cmake")
endif()
