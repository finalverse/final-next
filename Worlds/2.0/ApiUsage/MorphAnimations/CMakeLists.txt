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

ogre_add_executable(World_MorphAnimations WIN32 MACOSX_BUNDLE ${SOURCE_FILES} ${WORLD_COMMON_RESOURCES})

target_link_libraries(World_MorphAnimations ${OGRE_LIBRARIES} ${OGRE_WORLDS_LIBRARIES})
ogre_config_world_lib(World_MorphAnimations)

if( APPLE )
  set_target_properties( World_MorphAnimations PROPERTIES RESOURCE "${WORLD_COMMON_RESOURCES}" )
  set_target_properties( World_MorphAnimations PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${WORLDS_PLIST} )
endif()
