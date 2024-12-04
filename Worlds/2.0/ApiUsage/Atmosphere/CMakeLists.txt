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

ogre_add_component_include_dir(Atmosphere)
include_directories(${CMAKE_SOURCE_DIR}/Components/Hlms/Common/include)
ogre_add_component_include_dir(Hlms/Pbs)

add_recursive( ./ SOURCE_FILES )

ogre_add_executable(World_Atmosphere WIN32 MACOSX_BUNDLE ${SOURCE_FILES} ${WORLD_COMMON_RESOURCES})

target_link_libraries(World_Atmosphere ${OGRE_LIBRARIES} ${OGRE_WORLDS_LIBRARIES} ${OGRE_NEXT}Atmosphere)
ogre_config_world_lib(World_Atmosphere)
ogre_config_world_pkg(World_Atmosphere)
