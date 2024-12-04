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

include_directories(${CMAKE_SOURCE_DIR}/Components/Hlms/Common/include)
ogre_add_component_include_dir(Hlms/Pbs)

add_recursive( ./ SOURCE_FILES )

ogre_add_executable(World_Tutorial_DynamicCubemap WIN32 MACOSX_BUNDLE ${SOURCE_FILES} ${WORLD_COMMON_RESOURCES} ${WORLD_COMMON_COMPUTE_RESOURCES})

target_link_libraries(World_Tutorial_DynamicCubemap ${OGRE_LIBRARIES} ${OGRE_WORLDS_LIBRARIES})
ogre_config_world_lib(World_Tutorial_DynamicCubemap)
ogre_config_world_pkg(World_Tutorial_DynamicCubemap)

if( APPLE )
	set( World_RESOURCES ${WORLD_COMMON_RESOURCES} ${WORLD_COMMON_COMPUTE_RESOURCES} )
	set_target_properties( World_Tutorial_DynamicCubemap PROPERTIES RESOURCE "${World_RESOURCES}" )
	set_target_properties( World_Tutorial_DynamicCubemap PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${WORLDS_PLIST} )
endif()
