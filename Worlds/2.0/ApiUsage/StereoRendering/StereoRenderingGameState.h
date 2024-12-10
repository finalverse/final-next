
#ifndef _Final_StereoRenderingGameState_H_
#define _Final_StereoRenderingGameState_H_

#include "OgrePrerequisites.h"
#include "TutorialGameState.h"

namespace Final
{
    class StereoRenderingGameState : public TutorialGameState
    {
        Ogre::SceneNode *mSceneNode[16];

    public:
        StereoRenderingGameState( const Ogre::String &helpDescription );

        void createScene01() override;

        void update( float timeSinceLast ) override;
    };
}  // namespace Final

#endif
