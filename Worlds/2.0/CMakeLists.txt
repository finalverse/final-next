# Configure Worlds build

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Common/include)
include_directories(${CMAKE_SOURCE_DIR}/Components/Overlay/include)

set( WORLD_COMMON_BASIC_RESOURCES "" )
set( WORLD_COMMON_RESOURCES "" )
set( WORLDS_PLIST "" )
set( WORLD_COMMON_STORYBOARD_RESOURCE "" )

set (OGRE_BUILD_WORLDS_SKIP 0 )

if( NOT OGRE_BUILD_PLATFORM_APPLE_IOS AND NOT ANDROID )
	if( OGRE_BUILD_WORLDS AND NOT SDL2_FOUND )
		message( STATUS "Could not find dependency for worlds: SDL2" )
		set( OGRE_BUILD_WORLDS_SKIP 1 )
	else()
		include_directories(${SDL2_INCLUDE_DIR})
	endif()
endif()

if( NOT OGRE_BUILD_COMPONENT_OVERLAY )
	message( STATUS "Worlds require Overlays (OGRE_BUILD_COMPONENT_OVERLAY) to be set." )
	set( OGRE_BUILD_WORLDS_SKIP 1 )
endif()

if( OGRE_BUILD_WORLDS_SKIP )
	message(STATUS "Skipping worlds & tests build")
endif()

if( APPLE )
    if( OGRE_BUILD_PLATFORM_APPLE_IOS )
        # This needs to be first in the list in order show up in the Copy Bundle Resources build phase.
        set( WORLD_COMMON_STORYBOARD_RESOURCE ${CMAKE_CURRENT_SOURCE_DIR}/Common/src/System/iOS/Resources/Main.storyboard )
    endif()

	set( WORLD_COMMON_BASIC_RESOURCES ${WORLD_COMMON_BASIC_RESOURCES}
                 ${WORLD_COMMON_STORYBOARD_RESOURCE}
		 ${CMAKE_BINARY_DIR}/bin/resources2.cfg
		 ${CMAKE_BINARY_DIR}/bin/plugins.cfg
		 ${CMAKE_SOURCE_DIR}/Worlds/Media/2.0
		 ${CMAKE_SOURCE_DIR}/Worlds/Media/Hlms
		 ${CMAKE_SOURCE_DIR}/Worlds/Media/packs/DebugPack.zip
		 ${CMAKE_SOURCE_DIR}/Worlds/Media/packs/CornellBox.zip )

	set( WORLD_COMMON_RESOURCES ${WORLD_COMMON_BASIC_RESOURCES}
		 ${CMAKE_SOURCE_DIR}/Worlds/Media/models
		 ${CMAKE_SOURCE_DIR}/Worlds/Media/materials/textures )

	 set( WORLD_COMMON_COMPUTE_RESOURCES
		  ${CMAKE_SOURCE_DIR}/Worlds/Media/Compute
		  ${CMAKE_SOURCE_DIR}/Worlds/Media/VCT )

        if( OGRE_BUILD_PLATFORM_APPLE_IOS )
            set( WORLDS_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Common/src/System/iOS/Resources/Info.plist )
        else()
            set( WORLDS_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Common/src/System/OSX/Resources/Info.plist )
		endif()
	set( WORLD_COMMON_RESOURCES ${WORLD_COMMON_RESOURCES} ${WORLD_COMMON_COMPUTE_RESOURCES} )
endif()
if( WINDOWS_STORE OR WINDOWS_PHONE )
	set( WORLD_COMMON_BASIC_RESOURCES ${WORLD_COMMON_BASIC_RESOURCES}
		 ${CMAKE_BINARY_DIR}/bin/release/SDL2.dll
		 ${CMAKE_BINARY_DIR}/bin/release/resources2.cfg
		 ${CMAKE_BINARY_DIR}/bin/release/plugins.cfg
		 ${CMAKE_BINARY_DIR}/bin/debug/plugins_d.cfg
		 ${CMAKE_SOURCE_DIR}/Worlds/Media/2.0
		 ${CMAKE_SOURCE_DIR}/Worlds/Media/Hlms
		 ${CMAKE_SOURCE_DIR}/Worlds/Media/packs/DebugPack.zip
		 ${CMAKE_SOURCE_DIR}/Worlds/Media/packs/CornellBox.zip )

	set( WORLD_COMMON_RESOURCES ${WORLD_COMMON_BASIC_RESOURCES}
		 ${CMAKE_SOURCE_DIR}/Worlds/Media/models
		 ${CMAKE_SOURCE_DIR}/Worlds/Media/materials/textures )

	set( WORLD_COMMON_COMPUTE_RESOURCES
		  ${CMAKE_SOURCE_DIR}/Worlds/Media/Compute
		  ${CMAKE_SOURCE_DIR}/Worlds/Media/VCT )
endif()
if( WIN32 AND NOT WINDOWS_STORE AND NOT WINDOWS_PHONE )
	set( WORLD_COMMON_RESOURCES ${CMAKE_SOURCE_DIR}/CMake/Templates/OSCompatibility.Win32.manifest )
endif()

function(ogre_config_world_pkg WORLDNAME)
	if( APPLE )
		set_target_properties( ${WORLDNAME} PROPERTIES RESOURCE "${WORLD_COMMON_RESOURCES}" )
		set_target_properties( ${WORLDNAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${WORLDS_PLIST} )
	endif()
	if( WINDOWS_STORE OR WINDOWS_PHONE )
		set_property( SOURCE ${WORLD_COMMON_RESOURCES} PROPERTY VS_DEPLOYMENT_CONTENT 1 )
		source_group("Resource Files" FILES ${WORLD_COMMON_RESOURCES})
	endif()
endfunction(ogre_config_world_pkg)

add_definitions( -DUSE_JSON_UNIT_TESTING )

if( OGRE_BUILD_WORLDS AND NOT OGRE_BUILD_WORLDS_SKIP )
    add_subdirectory(Common)
endif()

if( OGRE_BUILD_WORLDS AND NOT OGRE_BUILD_WORLDS_SKIP )
	set( OGRE_WORLDS_LIBRARIES "FinalWorldsCommon" "${OGRE_NEXT}Overlay" )

	if( OGRE_STATIC OR WINDOWS_STORE OR WINDOWS_PHONE )
		if( OGRE_BUILD_RENDERSYSTEM_GL3PLUS )
			set( OGRE_WORLDS_LIBRARIES ${OGRE_WORLDS_LIBRARIES} "RenderSystem_GL3Plus" )
		endif()
		if( OGRE_BUILD_RENDERSYSTEM_GLES2 )
			set( OGRE_WORLDS_LIBRARIES ${OGRE_WORLDS_LIBRARIES} "RenderSystem_GLES2" )
		endif()
		if( OGRE_BUILD_RENDERSYSTEM_D3D11 )
			set( OGRE_WORLDS_LIBRARIES ${OGRE_WORLDS_LIBRARIES} "RenderSystem_Direct3D11" )
		endif()
		if( OGRE_BUILD_RENDERSYSTEM_METAL )
			set( OGRE_WORLDS_LIBRARIES ${OGRE_WORLDS_LIBRARIES} "RenderSystem_Metal" )
		endif()
		if( OGRE_BUILD_RENDERSYSTEM_VULKAN )
			set( OGRE_WORLDS_LIBRARIES ${OGRE_WORLDS_LIBRARIES} "RenderSystem_Vulkan" )
			if( UNIX AND NOT ANDROID )
				set( OGRE_WORLDS_LIBRARIES ${OGRE_WORLDS_LIBRARIES} "xcb" "X11-xcb" "xcb-randr" )
			endif()
		endif()
		if( OGRE_BUILD_PLUGIN_PFX )
			set( OGRE_WORLDS_LIBRARIES ${OGRE_WORLDS_LIBRARIES} "Plugin_ParticleFX" )
		endif()
		if( OGRE_BUILD_PLUGIN_PFX2 )
			set( OGRE_WORLDS_LIBRARIES ${OGRE_WORLDS_LIBRARIES} "Plugin_ParticleFX2" )
		endif()
	endif()

	add_subdirectory(ApiUsage/AnimationTagPoint)
	add_subdirectory(ApiUsage/AreaApproxLights)
	if( OGRE_BUILD_COMPONENT_ATMOSPHERE )
		add_subdirectory(ApiUsage/Atmosphere)
	endif()
	add_subdirectory(ApiUsage/ImportAnimationsShareSkeletonInstance)
	add_subdirectory(ApiUsage/CustomRenderable)
	add_subdirectory(ApiUsage/Decals)
	add_subdirectory(ApiUsage/DynamicGeometry)
	add_subdirectory(ApiUsage/IesProfiles)
	add_subdirectory(ApiUsage/ImageVoxelizer)
	add_subdirectory(ApiUsage/InstancedStereo)
	add_subdirectory(ApiUsage/InstantRadiosity)
	add_subdirectory(ApiUsage/LocalCubemaps)
	add_subdirectory(ApiUsage/LocalCubemapsManualProbes)
	if( OGRE_BUILD_COMPONENT_MESHLODGENERATOR )
		add_subdirectory(ApiUsage/MeshLod)
	endif()
	add_subdirectory(ApiUsage/MorphAnimations)
	if( OGRE_BUILD_PLUGIN_PFX )
		add_subdirectory(ApiUsage/ParticleFX)
	endif()
	if( OGRE_BUILD_PLUGIN_PFX2 )
		add_subdirectory(ApiUsage/ParticleFX2)
	endif()
	add_subdirectory(ApiUsage/Refractions)
	add_subdirectory(ApiUsage/V1Interfaces)
	add_subdirectory(ApiUsage/V2Mesh)
	add_subdirectory(ApiUsage/V2ManualObject)
	add_subdirectory(ApiUsage/PccPerPixelGridPlacement)
	if( OGRE_BUILD_COMPONENT_PLANAR_REFLECTIONS )
		add_subdirectory(ApiUsage/PlanarReflections)
	else()
		message(STATUS "Skipping PlanarReflections world (OGRE_BUILD_COMPONENT_PLANAR_REFLECTIONS not set)")
	endif()
	if( OGRE_BUILD_COMPONENT_SCENE_FORMAT )
		add_subdirectory(ApiUsage/SceneFormat)
	else()
		message(STATUS "Skipping SceneFormat world (OGRE_BUILD_COMPONENT_SCENE_FORMAT not set)")
	endif()
	add_subdirectory(ApiUsage/ScreenSpaceReflections)
	add_subdirectory(ApiUsage/StaticShadowMaps)
	add_subdirectory(ApiUsage/StencilTest)
	add_subdirectory(ApiUsage/StereoRendering)
	add_subdirectory(ApiUsage/ShadowMapDebugging)
	add_subdirectory(ApiUsage/ShadowMapFromCode)
	add_subdirectory(ApiUsage/UpdatingDecalsAndAreaLightTex)


	add_subdirectory(Showcase/Forward3D)
	add_subdirectory(Showcase/Hdr)
	add_subdirectory(Showcase/HdrSmaa)
	add_subdirectory(Showcase/PbsMaterials)
	add_subdirectory(Showcase/Postprocessing)

	if( NOT ANDROID )
		add_subdirectory(Tutorials/Tutorial00_Basic)
		add_subdirectory(Tutorials/Tutorial01_Initialization)
		add_subdirectory(Tutorials/Tutorial02_VariableFramerate)
		add_subdirectory(Tutorials/Tutorial03_DeterministicLoop)
		if( NOT APPLE )
			# Not ported yet to Metal
			add_subdirectory(Tutorials/Tutorial04_InterpolationLoop)
			add_subdirectory(Tutorials/Tutorial05_MultithreadingBasics)
			add_subdirectory(Tutorials/Tutorial06_Multithreading)
		endif()
	endif()
	add_subdirectory(Tutorials/TutorialCompute01_UavTexture)
	add_subdirectory(Tutorials/TutorialCompute02_UavBuffer)
	add_subdirectory(Tutorials/Tutorial_Distortion)
	add_subdirectory(Tutorials/Tutorial_DynamicCubemap)
	if( OGRE_GLSUPPORT_USE_EGL_HEADLESS OR OGRE_VULKAN_WINDOW_NULL )
		add_subdirectory(Tutorials/Tutorial_EglHeadless)
	endif()

	if( OGRE_BUILD_COMPONENT_SCENE_FORMAT )
		add_subdirectory(Tutorials/Tutorial_Memory)
	endif()
	if( OpenVR_FOUND )
		add_subdirectory(Tutorials/Tutorial_OpenVR)
	else()
		message(STATUS "Skipping OpenVR world (OpenVR not found)")
	endif()
	add_subdirectory(Tutorials/Tutorial_ReconstructPosFromDepth)
	add_subdirectory(Tutorials/TutorialSky_Postprocess)
	add_subdirectory(Tutorials/Tutorial_SSAO)
	add_subdirectory(Tutorials/Tutorial_SMAA)
	add_subdirectory(Tutorials/Tutorial_Terrain)
	add_subdirectory(Tutorials/Tutorial_TextureBaking)
	add_subdirectory(Tutorials/TutorialUav01_Setup)
	add_subdirectory(Tutorials/TutorialUav02_Setup)
	if( OGRE_BUILD_RENDERSYSTEM_VULKAN AND NOT ANDROID AND NOT OGRE_STATIC )
		add_subdirectory(Tutorials/Tutorial_VulkanExternal)
	endif()
endif()

if( OGRE_BUILD_TESTS )
	add_subdirectory(Tests/ArrayTextures)
	add_subdirectory(Tests/BillboardTest)
	add_subdirectory(Tests/EndFrameOnceFailure)
	add_subdirectory(Tests/InternalCore)
	add_subdirectory(Tests/MemoryCleanup)
	add_subdirectory(Tests/ManyMaterials)
	add_subdirectory(Tests/NearFarProjection)
	add_subdirectory(Tests/Readback)
	add_subdirectory(Tests/Restart)
	add_subdirectory(Tests/TextureResidency)
	add_subdirectory(Tests/Voxelizer)
endif()
