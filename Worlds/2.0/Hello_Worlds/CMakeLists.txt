#-------------------------------------------------------------------
# This file is part of the CMake build system for OGRE-Next
#     (Object-oriented Graphics Rendering Engine)
# For the latest info, see http://www.ogre3d.org/
#
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------

macro( add_recursive dir retVal )
	file( GLOB_RECURSE ${retVal} ${dir}/*.h ${dir}/*.cpp ${dir}/*.c )
endmacro()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_recursive( ./ SOURCE_FILES )

set( TUTORIAL_RESOURCES ${WORLD_COMMON_BASIC_RESOURCES} )
if( OGRE_BUILD_PLATFORM_APPLE_IOS )
	set( TUTORIAL_RESOURCES ${WORLD_COMMON_BASIC_RESOURCES}
			${CMAKE_CURRENT_SOURCE_DIR}/iOS/HelloWorlds.storyboard)
	set( SOURCE_FILES ${SOURCE_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/iOS/HelloViewController.mm )
endif()

if( OGRE_BUILD_WORLDS_AS_BUNDLES )
    ogre_add_executable(Hello_Worlds WIN32 MACOSX_BUNDLE ${SOURCE_FILES} ${TUTORIAL_RESOURCES} )
else()
    ogre_add_executable(Hello_Worlds WIN32 ${SOURCE_FILES} ${TUTORIAL_RESOURCES} )
endif( OGRE_BUILD_WORLDS_AS_BUNDLES )

target_link_libraries(Hello_Worlds ${OGRE_LIBRARIES} ${OGRE_WORLDS_LIBRARIES})
ogre_config_world_lib(Hello_Worlds)

if( APPLE )
	set_target_properties( Hello_Worlds PROPERTIES RESOURCE "${TUTORIAL_RESOURCES}" )
	set_target_properties( Hello_Worlds PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/iOS/Info.plist )
endif()
