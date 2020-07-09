include(WhiteList.cmake)
include(CompilerFlags.cmake)

project(EwasUtils)

add_library(EwasUtils STATIC ${SOURCES} ${HEADERS})

#no dependencies
set(LOCAL_MODULE_DEPENDENCIES)

set(MODULE_DEPENDENCIES ${LOCAL_MODULE_DEPENDENCIES} PARENT_SCOPE)
set(PUBLIC_MODULE_HEADERS ${HEADERS} PARENT_SCOPE)

target_link_libraries(EwasUtils
	${LOCAL_MODULE_DEPENDENCIES})

set_target_properties(EwasUtils PROPERTIES FOLDER "4.Common/Utils")