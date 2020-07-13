include(WhiteList.cmake)
include(CompilerFlags.cmake)

project(ddkFramework)

add_library(ddkFramework STATIC ${SOURCES} ${HEADERS} ${HEADERS_INL})

set(LOCAL_MODULE_DEPENDENCIES ddkUtils)

set(MODULE_DEPENDENCIES ${LOCAL_MODULE_DEPENDENCIES} PARENT_SCOPE)
set(PUBLIC_MODULE_HEADERS ${HEADERS} ${HEADERS_INL} PARENT_SCOPE)

target_link_libraries(ddkFramework
	${LOCAL_MODULE_DEPENDENCIES}
	${THIRD_PARTY_DEPENDENCIES})

set_target_properties(ddkFramework PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(ddkFramework PROPERTIES FOLDER "Framework")
