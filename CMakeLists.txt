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
	
	# Configure LOG4CXX (bundled library)
	add_library(log4cxx STATIC IMPORTED)
	set_target_properties(log4cxx PROPERTIES
		INTERFACE_INCLUDE_DIRECTORIES "${EXTERNAL_DIR}/log4cxx/include"
		IMPORTED_LOCATION "${EXTERNAL_DIR}/log4cxx/lib/liblog4cxx.a")
	target_link_libraries(log4cxx INTERFACE apr aprutil libexpat)

elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")

		# Configure pthread (bundled library)
	add_library(pthread SHARED IMPORTED)
	set_target_properties(pthread PROPERTIES
		INTERFACE_INCLUDE_DIRECTORIES "${EXTERNAL_DIR}/pthread/include"
		IMPORTED_IMPLIB_RELEASE "${EXTERNAL_DIR}/pthread/lib/pthreadVC3.lib"
		IMPORTED_IMPLIB_DEBUG "${EXTERNAL_DIR}/pthread/lib/pthreadVC3d.lib"
		IMPORTED_LOCATION_RELEASE "${EXTERNAL_DIR}/pthread/bin/pthreadVC3.dll"
		IMPORTED_LOCATION_DEBUG "${EXTERNAL_DIR}/pthread/bin/pthreadVC3d.dll"
		MAP_IMPORTED_CONFIG_RELEASE_DEBUG Release)

	# Configure log4cxx (bundled library)
	add_library(log4cxx SHARED IMPORTED)
	set_target_properties(log4cxx PROPERTIES
		INTERFACE_INCLUDE_DIRECTORIES "${EXTERNAL_DIR}/log4cxx/include"
		IMPORTED_IMPLIB_RELEASE "${EXTERNAL_DIR}/log4cxx/lib/log4cxx.lib"
		IMPORTED_IMPLIB_DEBUG "${EXTERNAL_DIR}/log4cxx/lib/log4cxx_d.lib"
		IMPORTED_LOCATION_RELEASE "${EXTERNAL_DIR}/log4cxx/bin/log4cxx.dll"
		IMPORTED_LOCATION_DEBUG "${EXTERNAL_DIR}/log4cxx/bin/log4cxx_d.dll"
		MAP_IMPORTED_CONFIG_RELEASE_DEBUG Release)

elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")

endif()

if(${SolutionName} STREQUAL "Test")
	include("${TEST_DIR}/dependencies.cmake")
else()
	include("${SRC_DIR}/dependencies.cmake")
endif()
