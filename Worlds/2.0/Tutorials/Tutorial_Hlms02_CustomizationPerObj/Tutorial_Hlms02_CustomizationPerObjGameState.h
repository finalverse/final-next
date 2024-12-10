
#ifndef Final_Hlms02CustomizationPerObjGameState_H
#define Final_Hlms02CustomizationPerObjGameState_H

#include "OgrePrerequisites.h"
#include "TutorialGameState.h"

namespace Final
{
    class Hlms02CustomizationPerObjGameState : public TutorialGameState
    {
        float mAccumWindTime;

        void generateDebugText( float timeSinceLast, Ogre::String &outText ) override;

    public:
        Hlms02CustomizationPerObjGameState( const Ogre::String &helpDescription );

        void createScene01() override;

        void update( float timeSinceLast ) override;

        void keyReleased( const SDL_KeyboardEvent &arg ) override;
    };
}  // namespace Final

#endif
