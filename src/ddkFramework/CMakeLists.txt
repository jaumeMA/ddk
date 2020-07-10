include(WhiteList.cmake)
include(CompilerFlags.cmake)

project(EwasFramework)

add_library(EwasFramework STATIC ${SOURCES} ${HEADERS} ${HEADERS_INL})

set(LOCAL_MODULE_DEPENDENCIES EwasUtils)

set(MODULE_DEPENDENCIES ${LOCAL_MODULE_DEPENDENCIES} PARENT_SCOPE)
set(PUBLIC_MODULE_HEADERS ${HEADERS} ${HEADERS_INL} PARENT_SCOPE)

target_link_libraries(EwasFramework
	${LOCAL_MODULE_DEPENDENCIES}
	${THIRD_PARTY_DEPENDENCIES})

set_target_properties(EwasFramework PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(EwasFramework PROPERTIES FOLDER "4.Common/Utils")
