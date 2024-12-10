
#ifndef Final_Hlms03AlwaysOnTopAGameState_H
#define Final_Hlms03AlwaysOnTopAGameState_H

#include "OgrePrerequisites.h"
#include "TutorialGameState.h"

namespace Final
{
    class Hlms03AlwaysOnTopAGameState : public TutorialGameState
    {
        void createBar( const bool bAlwaysOnTop );

        void generateDebugText( float timeSinceLast, Ogre::String &outText ) override;

    public:
        Hlms03AlwaysOnTopAGameState( const Ogre::String &helpDescription );

        void createScene01() override;
    };
}  // namespace Final

#endif
