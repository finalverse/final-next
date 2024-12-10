
#ifndef _Final_NearFarProjectionGameState_H_
#define _Final_NearFarProjectionGameState_H_

#include "OgrePrerequisites.h"
#include "TutorialGameState.h"

namespace Final
{
    class NearFarProjectionGameState : public TutorialGameState
    {
        Ogre::SceneNode *mSceneNode;

        void generateDebugText( float timeSinceLast, Ogre::String &outText ) override;

    public:
        NearFarProjectionGameState( const Ogre::String &helpDescription );

        void createScene01() override;

        void keyReleased( const SDL_KeyboardEvent &arg ) override;
    };
}  // namespace Final

#endif
