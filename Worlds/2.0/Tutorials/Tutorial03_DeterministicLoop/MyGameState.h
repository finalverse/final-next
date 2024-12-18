
#ifndef _Final_MyGameState_H_
#define _Final_MyGameState_H_

#include "OgrePrerequisites.h"
#include "TutorialGameState.h"

namespace Final
{
    class MyGameState : public TutorialGameState
    {
        Ogre::SceneNode *mSceneNode;
        float mDisplacement;

        void generateDebugText( float timeSinceLast, Ogre::String &outText ) override;

    public:
        MyGameState( const Ogre::String &helpDescription );

        void createScene01() override;

        void update( float timeSinceLast ) override;

        void keyReleased( const SDL_KeyboardEvent &arg ) override;
    };
}  // namespace Final

#endif
