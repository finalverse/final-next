
#ifndef _Final_EmptyProjectGameState_H_
#define _Final_EmptyProjectGameState_H_

#include "OgrePrerequisites.h"
#include "TutorialGameState.h"

namespace Final
{
    class EmptyProjectGameState : public TutorialGameState
    {
        void generateDebugText( float timeSinceLast, Ogre::String &outText ) override;

    public:
        EmptyProjectGameState( const Ogre::String &helpDescription );

        void createScene01() override;

        void update( float timeSinceLast ) override;

        void keyReleased( const SDL_KeyboardEvent &arg ) override;
    };
}  // namespace Final

#endif
