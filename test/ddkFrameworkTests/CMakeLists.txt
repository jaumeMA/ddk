cmake_minimum_required(VERSION 3.13.0)

include(WhiteList.cmake)
include(CompilerFlags.cmake)

project(EwasFrameworkTests)

add_executable(EwasFrameworkTests ${SOURCES} ${HEADERS})

#internal dependencies
set(LOCAL_MODULE_DEPENDENCIES EwasFramework)

set(MODULE_DEPENDENCIES ${LOCAL_MODULE_DEPENDENCIES} PARENT_SCOPE)

target_include_directories(EwasFrameworkTests PUBLIC
	"${SRC_DIR}/EwasFramework")

target_link_libraries(EwasFrameworkTests
	${THIRD_PARTY_DEPENDENCIES}
	gmock_main
	${LOCAL_MODULE_DEPENDENCIES})

set_target_properties(EwasFrameworkTests PROPERTIES FOLDER "1.Tests")
set_target_properties(EwasFrameworkTests PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${OUTPUT_DIR}/$<CONFIG>")

add_custom_command(TARGET EwasFrameworkTests PRE_BUILD COMMAND ${CMAKE_COMMAND} -E $<IF:$<CONFIG:Debug>,copy_if_different,echo_append> ${THIRD_PARTY_PDBS} $<$<CONFIG:Debug>:"${OUTPUT_DIR}/Debug">)
