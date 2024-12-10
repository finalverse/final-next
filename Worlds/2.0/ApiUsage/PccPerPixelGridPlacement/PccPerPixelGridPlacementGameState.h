
#ifndef _Final_PccPerPixelGridPlacementGameState_H_
#define _Final_PccPerPixelGridPlacementGameState_H_

#include "OgrePrerequisites.h"

#include "TutorialGameState.h"

namespace Ogre
{
    class ParallaxCorrectedCubemapAuto;
    class PccPerPixelGridPlacement;
    class HlmsPbsDatablock;
}  // namespace Ogre

namespace Final
{
    class PccPerPixelGridPlacementGameState : public TutorialGameState
    {
        Ogre::SceneNode *mLightNodes[3];

        Ogre::ParallaxCorrectedCubemapAuto *mParallaxCorrectedCubemap;
        bool mUseDpm2DArray;

        void generateDebugText( float timeSinceLast, Ogre::String &outText ) override;

        void setupParallaxCorrectCubemaps();
        void forceUpdateAllProbes();

    public:
        PccPerPixelGridPlacementGameState( const Ogre::String &helpDescription );

        void createScene01() override;
        void destroyScene() override;

        void update( float timeSinceLast ) override;

        void keyReleased( const SDL_KeyboardEvent &arg ) override;
    };
}  // namespace Final

#endif
