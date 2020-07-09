cmake_minimum_required(VERSION 3.13.0)

include(WhiteList.cmake)
include(CompilerFlags.cmake)

project(EwasUtilsTests)

add_executable(EwasUtilsTests ${SOURCES} ${HEADERS})

#internal dependencies
set(LOCAL_MODULE_DEPENDENCIES EwasUtils)

set(MODULE_DEPENDENCIES ${LOCAL_MODULE_DEPENDENCIES} PARENT_SCOPE)

target_link_libraries(EwasUtilsTests
	gmock_main
	Boost::date_time
	Boost::regex
	${LOCAL_MODULE_DEPENDENCIES})

set_target_properties(EwasUtilsTests PROPERTIES FOLDER "1.Tests")
set_target_properties(EwasUtilsTests PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${OUTPUT_DIR}/$<CONFIG>")
