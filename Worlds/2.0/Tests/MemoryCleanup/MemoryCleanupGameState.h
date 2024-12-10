
#ifndef _Final_MemoryCleanupGameState_H_
#define _Final_MemoryCleanupGameState_H_

#include "OgreOverlayPrerequisites.h"
#include "OgrePrerequisites.h"

#include "OgreGpuResource.h"
#include "OgreOverlay.h"
#include "TutorialGameState.h"

namespace Final
{
    class MemoryCleanupGameState : public TutorialGameState
    {
        struct VisibleItem
        {
            Ogre::Item *item;
            Ogre::HlmsDatablock *datablock;
        };

        typedef std::vector<VisibleItem> VisibleItemVec;
        VisibleItemVec mVisibleItems;

        bool mReleaseMemoryOnCleanup;
        bool mReleaseGpuMemory;

        void generateDebugText( float timeSinceLast, Ogre::String &outText ) override;

        void testSequence();

        void createCleanupScene();
        void destroyCleanupScene();
        bool isSceneLoaded() const;

    public:
        MemoryCleanupGameState( const Ogre::String &helpDescription );

        void createScene01() override;
        void destroyScene() override;

        void update( float timeSinceLast ) override;

        void keyReleased( const SDL_KeyboardEvent &arg ) override;
    };
}  // namespace Final

#endif
