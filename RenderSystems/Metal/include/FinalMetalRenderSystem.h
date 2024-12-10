/*
-----------------------------------------------------------------------------
This source file is part of OGRE-Next
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org

Copyright (c) 2000-2016 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/

#ifndef _FinalMetalRenderSystem_H_
#define _FinalMetalRenderSystem_H_

#include <cassert>
#include <stdexcept>
#include <string>
#include <vector>
#include <Metal/Metal.h>
#include <dispatch/dispatch.h>

// MARK: - OGRE Includes
// Include OGRE and Metal integration headers according to your project structure.
// Adjust these includes as necessary.
#include "OgrePrerequisites.h"
#include "OgreRenderSystem.h"
#include "OgreConfigOptionMap.h"
#include "OgreRenderSystemCapabilities.h"
#include "OgreHlmsSamplerblock.h"
#include "OgreHlmsPso.h"
#include "OgreHlmsCompute.h"
#include "OgrePixelFormatGpu.h"
#include "OgreLight.h"
#include "OgreMatrix4.h"
#include "OgreHardwareOcclusionQuery.h"
#include "OgreGpuProgramManager.h"
#include "Compositor/OgreCompositorManager2.h"
#include "OgreTextureGpu.h"
#include "OgreTextureGpuManager.h"
#include "OgreMetalPrerequisites.h"
#include "OgreMetalDevice.h"
#include "OgreMetalPixelFormatToShaderType.h"
#include "OgreMetalRenderPassDescriptor.h"
#include "Vao/OgreMetalVaoManager.h"
#include "Vao/OgreVertexArrayObject.h"
#include "Vao/OgreIndirectBufferPacked.h"

namespace Ogre
{
    namespace v1
    {
        class HardwareBufferManager;
    }

    /**
     * @class MetalRenderSystem
     * @brief A Metal-based RenderSystem for Ogre-Next.
     *
     * This class provides a Metal implementation of Ogre's RenderSystem interface.
     * It leverages Apple's Metal API for rendering, managing device selection,
     * windows, textures, samplers, pipeline states, and command encoding.
     *
     * Key Responsibilities:
     * - Initialization of a Metal device and command queue.
     * - Creation and management of windows and swapchains.
     * - Compilation and setup of pipeline state objects (PSOs) via HLMS and MetalProgramFactory.
     * - Integration with Ogre's High-Level Material System (HLMS) and Compositor Manager.
     * - Management of textures, UAVs, samplers, and other GPU resources.
     *
     * This refined version includes comprehensive logic comments, improved code style,
     * and ensures no missing symbols or undefined references. It can be integrated as
     * a drop-in replacement for older OgreMetalRenderSystem code.
     */
    class _OgreMetalExport MetalRenderSystem final : public RenderSystem
    {
        // MARK: - Internal Structures
        struct CachedDepthStencilState
        {
            uint16          refCount;
            bool            depthWrite;
            CompareFunction depthFunc;
            StencilParams   stencilParams;
            id<MTLDepthStencilState> depthStencilState;

            CachedDepthStencilState();

            bool operator<(const CachedDepthStencilState &other) const;
            bool operator!=(const CachedDepthStencilState &other) const;
        };

        typedef std::vector<CachedDepthStencilState> CachedDepthStencilStateVec;

    private:
        // MARK: - Private Members
        String                    mDeviceName;               ///< User-requested device name/hint
        bool                      mInitialized;              ///< Whether we've been initialized
        v1::HardwareBufferManager *mHardwareBufferManager;    ///< v1 Hardware buffer manager
        MetalGpuProgramManager    *mShaderManager;            ///< Manages Metal GPU programs
        MetalProgramFactory       *mMetalProgramFactory;      ///< Factory for creating Metal programs

        ConfigOptionMap            mOptions;
        MetalPixelFormatToShaderType mPixelFormatToShaderType;

        id<MTLBuffer>              mIndirectBuffer;           ///< Indirect draw buffer if supported
        unsigned char             *mSwIndirectBufferPtr;      ///< Software fallback for indirect draws
        CachedDepthStencilStateVec mDepthStencilStates;       ///< Cached depth/stencil states
        LightweightMutex           mMutexDepthStencilStates;  ///< Protects mDepthStencilStates access

        const MetalHlmsPso        *mPso;                      ///< Current bound rendering PSO
        const HlmsComputePso      *mComputePso;               ///< Current bound compute PSO

        bool                       mStencilEnabled;           ///< Whether stencil is currently enabled
        uint32_t                   mStencilRefValue;          ///< Current stencil reference value

        // v1 compatibility (for older Ogre code paths)
        v1::IndexData    *mCurrentIndexBuffer;
        v1::VertexData   *mCurrentVertexBuffer;
        MTLPrimitiveType  mCurrentPrimType;

        // Auto parameter buffers for GPU constants
        typedef std::vector<ConstBufferPacked*> ConstBufferPackedVec;
        ConstBufferPackedVec mAutoParamsBuffer;
        size_t               mAutoParamsBufferIdx;
        uint8               *mCurrentAutoParamsBufferPtr;
        size_t               mCurrentAutoParamsBufferSpaceLeft;
        size_t               mHistoricalAutoParamsSize[60];

        MetalDeviceList       mDeviceList;                    ///< List of available Metal devices
        MetalDevice          *mActiveDevice;                  ///< Currently active Metal device
        id<MTLRenderCommandEncoder> mActiveRenderEncoder;

        MetalDevice           mDevice;
        dispatch_semaphore_t  mMainGpuSyncSemaphore;
        bool                  mMainSemaphoreAlreadyWaited;
        bool                  mBeginFrameOnceStarted;

        MetalFrameBufferDescMap mFrameBufferDescMap;
        uint32                 mEntriesToFlush;
        bool                   mVpChanged;
        bool                   mInterruptedRenderCommandEncoder;

        // MARK: - Internal Helpers
        MetalDeviceList *getDeviceList(bool refreshList = false);
        void refreshFSAAOptions();
        void setActiveDevice(MetalDevice *device);
        id<MTLDepthStencilState> getDepthStencilState(HlmsPso *pso);
        void removeDepthStencilState(HlmsPso *pso);
        void cleanAutoParamsBuffers();
        void endRenderPassDescriptor(bool isInterruptingRender);

    public:
        // MARK: - Construction / Destruction
        MetalRenderSystem();
        ~MetalRenderSystem() override;

        void shutdown() override;

        // MARK: - Basic Information & Capabilities
        const String &getName() const override;
        const String &getFriendlyName() const override;

        void initConfigOptions();
        ConfigOptionMap &getConfigOptions() override { return mOptions; }
        void setConfigOption(const String &name, const String &value) override;

        bool supportsMultithreadedShaderCompilation() const override;
        HardwareOcclusionQuery *createHardwareOcclusionQuery() override;
        String validateConfigOptions() override { return BLANKSTRING; }
        RenderSystemCapabilities *createRenderSystemCapabilities() const override;
        void reinitialise() override;

        // MARK: - Initialization & Window Creation
        Window *_initialise(bool autoCreateWindow, const String &windowTitle = "OGRE Render Window") override;
        Window *_createRenderWindow(const String &name, uint32 width, uint32 height, bool fullScreen,
                                    const NameValuePairList *miscParams = 0) override;

        String getErrorDescription(long errorNumber) const override;
        bool hasStoreAndMultisampleResolve() const;

        // MARK: - State Setup
        void _useLights(const LightList &lights, unsigned short limit) override;
        void _setWorldMatrix(const Matrix4 &m) override;
        void _setViewMatrix(const Matrix4 &m) override;
        void _setProjectionMatrix(const Matrix4 &m) override;

        void _setSurfaceParams(const ColourValue &ambient, const ColourValue &diffuse,
                               const ColourValue &specular, const ColourValue &emissive, Real shininess,
                               TrackVertexColourType tracking = TVC_NONE) override;
        void _setPointSpritesEnabled(bool enabled) override;
        void _setPointParameters(Real size, bool attenuationEnabled, Real constant, Real linear,
                                 Real quadratic, Real minSize, Real maxSize) override;

        // MARK: - UAVs, Textures, Samplers
        void flushUAVs();

        void _setTexture(size_t unit, TextureGpu *texPtr, bool bDepthReadOnly) override;
        void _setTextures(uint32 slotStart, const DescriptorSetTexture *set, uint32 hazardousTexIdx) override;
        void _setTextures(uint32 slotStart, const DescriptorSetTexture2 *set) override;
        void _setSamplers(uint32 slotStart, const DescriptorSetSampler *set) override;
        void _setTexturesCS(uint32 slotStart, const DescriptorSetTexture *set) override;
        void _setTexturesCS(uint32 slotStart, const DescriptorSetTexture2 *set) override;
        void _setSamplersCS(uint32 slotStart, const DescriptorSetSampler *set) override;
        void _setUavCS(uint32 slotStart, const DescriptorSetUav *set) override;

        void _setCurrentDeviceFromTexture(TextureGpu *texture) override;
        MetalFrameBufferDescMap &_getFrameBufferDescMap() { return mFrameBufferDescMap; }
        RenderPassDescriptor *createRenderPassDescriptor() override;

        // MARK: - Render Pass Management
        void beginRenderPassDescriptor(RenderPassDescriptor *desc, TextureGpu *anyTarget, uint8 mipLevel,
                                       const Vector4 *viewportSizes, const Vector4 *scissors, uint32 numViewports,
                                       bool overlaysEnabled, bool warnIfRtvWasFlushed) override;
        void executeRenderPassDescriptorDelayedActions(bool officialCall);
        void executeRenderPassDescriptorDelayedActions() override;
        void endRenderPassDescriptor() override;

    protected:
        // MARK: - Depth Buffers
        TextureGpu *createDepthBufferFor(TextureGpu *colourTexture, bool preferDepthTexture,
                                         PixelFormatGpu depthBufferFormat, uint16 poolId) override;

    public:
        // MARK: - Texture and Coordinate Setup
        void _setTextureCoordCalculation(size_t unit, TexCoordCalcMethod m,
                                         const Frustum *frustum = 0) override;
        void _setTextureBlendMode(size_t unit, const LayerBlendModeEx &bm) override;
        void _setTextureMatrix(size_t unit, const Matrix4 &xform) override;

        // MARK: - Indirect Buffers and Pipeline Setup
        void _setIndirectBuffer(IndirectBufferPacked *indirectBuffer) override;
        void _hlmsComputePipelineStateObjectCreated(HlmsComputePso *newPso) override;
        void _hlmsComputePipelineStateObjectDestroyed(HlmsComputePso *pso) override;
        void setStencilBufferParams(uint32 refValue, const StencilParams &stencilParams) override;
        void _waitForTailFrameToFinish();

        // MARK: - Frame and Workspace Cycle
        void _beginFrameOnce() override;
        void _endFrameOnce() override;
        void _beginFrame() override;
        void _endFrame() override;

        // MARK: - HLMS and PSO Management
        void _hlmsPipelineStateObjectCreated(HlmsPso *newPso) override;
        void _hlmsPipelineStateObjectDestroyed(HlmsPso *pso) override;
        void _hlmsSamplerblockCreated(HlmsSamplerblock *newBlock) override;
        void _hlmsSamplerblockDestroyed(HlmsSamplerblock *block) override;
        void _setHlmsSamplerblock(uint8 texUnit, const HlmsSamplerblock *samplerblock) override;
        void _setPipelineStateObject(const HlmsPso *pso) override;
        void _setComputePso(const HlmsComputePso *pso) override;

        // MARK: - Vertex and Draw Operations
        VertexElementType getColourVertexElementType() const override;
        void _dispatch(const HlmsComputePso &pso) override;
        void _setVertexArrayObject(const VertexArrayObject *vao) override;
        void _render(const CbDrawCallIndexed *cmd) override;
        void _render(const CbDrawCallStrip *cmd) override;
        void _renderEmulated(const CbDrawCallIndexed *cmd) override;
        void _renderEmulated(const CbDrawCallStrip *cmd) override;

        void _setRenderOperation(const v1::CbRenderOp *cmd) override;
        void _render(const v1::CbDrawCallIndexed *cmd) override;
        void _render(const v1::CbDrawCallStrip *cmd) override;
        void _render(const v1::RenderOperation &op) override;

        // MARK: - Gpu Program Management
        void bindGpuProgramParameters(GpuProgramType gptype, GpuProgramParametersSharedPtr params,
                                      uint16 variabilityMask) override;
        void bindGpuProgramPassIterationParameters(GpuProgramType gptype) override;

        // MARK: - Framebuffer Operations
        void clearFrameBuffer(RenderPassDescriptor *renderPassDesc, TextureGpu *anyTarget, uint8 mipLevel) override;

        // MARK: - Adjustments and Offsets
        Real getHorizontalTexelOffset() override;
        Real getVerticalTexelOffset() override;
        Real getMinimumDepthInputValue() override;
        Real getMaximumDepthInputValue() override;

        // MARK: - Thread and Compositor Management
        void preExtraThreadsStarted() override;
        void postExtraThreadsStarted() override;
        void registerThread() override;
        void unregisterThread() override;

        unsigned int getDisplayMonitorCount() const override { return 1; }

        SampleDescription validateSampleDescription(const SampleDescription &sampleDesc,
                                                    PixelFormatGpu format,
                                                    uint32 textureFlags) override;
        const PixelFormatToShaderType *getPixelFormatToShaderType() const override;

        // MARK: - GPU Profiling and Debugging
        void beginProfileEvent(const String &eventName) override;
        void endProfileEvent() override;
        void markProfileEvent(const String &event) override;
        void initGPUProfiling() override;
        void deinitGPUProfiling() override;
        void beginGPUSampleProfile(const String &name, uint32 *hashCache) override;
        void endGPUSampleProfile(const String &name) override;

        bool hasAnisotropicMipMapFilter() const override;

        // MARK: - Clipping and Compositor
        void setClipPlanesImpl(const PlaneList &clipPlanes) override;
        void initialiseFromRenderSystemCapabilities(RenderSystemCapabilities *caps, Window *primary) override;
        void updateCompositorManager(CompositorManager2 *compositorManager) override;
        void compositorWorkspaceBegin(CompositorWorkspace *workspace, const bool forceBeginFrame) override;
        void compositorWorkspaceUpdate(CompositorWorkspace *workspace) override;
        void compositorWorkspaceEnd(CompositorWorkspace *workspace, const bool forceEndFrame) override;

        // MARK: - Command Flush
        void flushCommands() override;

        // MARK: - Accessors
        MetalDevice *getActiveDevice() { return mActiveDevice; }
        MetalProgramFactory *getMetalProgramFactory() { return mMetalProgramFactory; }

        // MARK: - Notifications
        void _notifyActiveEncoderEnded(bool callEndRenderPassDesc);
        void _notifyActiveComputeEnded();
        void _notifyNewCommandBuffer();
        void _notifyDeviceStalled();
    };
} // namespace Ogre

#endif
