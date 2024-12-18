
#ifndef _Final_GraphicsGameState_H_
#define _Final_GraphicsGameState_H_

#include "OgrePrerequisites.h"
#include "TutorialGameState.h"

#include "OgreVector3.h"

namespace Final
{
    class GraphicsSystem;

    class GraphicsGameState : public TutorialGameState
    {
        bool mEnableInterpolation;

        void generateDebugText( float timeSinceLast, Ogre::String &outText ) override;

    public:
        GraphicsGameState( const Ogre::String &helpDescription );

        void update( float timeSinceLast ) override;

        void keyReleased( const SDL_KeyboardEvent &arg ) override;
    };
}  // namespace Final

#endif
