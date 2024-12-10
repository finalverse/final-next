
#ifndef _Final_RefractionsGameState_H_
#define _Final_RefractionsGameState_H_

#include "OgrePrerequisites.h"
#include "TutorialGameState.h"

namespace Ogre
{
    class HlmsPbsDatablock;
}

namespace Final
{
    class RefractionsGameState : public TutorialGameState
    {
        enum DebugVisibility
        {
            NoSpheres,
            NormalSpheres,
            ShowOnlyRefractions,
            ShowOnlyPlaceholder
        };

        Ogre::SceneNode *mSceneNode[16];

        Ogre::SceneNode *mLightNodes[3];

        bool mAnimateObjects;

        size_t mNumSpheres;
        float mTransparencyValue;

        DebugVisibility mDebugVisibility;

        void createRefractivePlaceholder( Ogre::Item *item, Ogre::SceneNode *sceneNode,
                                          Ogre::HlmsPbsDatablock *datablock );

        void createRefractiveWall();
        void createRefractiveSphere( const int x, const int z, const int numX, const int numZ,
                                     const float armsLength, const float startX, const float startZ );

        void generateDebugText( float timeSinceLast, Ogre::String &outText ) override;

        void setTransparencyToMaterials();

    public:
        RefractionsGameState( const Ogre::String &helpDescription );

        void createScene01() override;

        void update( float timeSinceLast ) override;

        void keyReleased( const SDL_KeyboardEvent &arg ) override;
    };
}  // namespace Final

#endif
