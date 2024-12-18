
#include "GraphicsGameState.h"
#include "GraphicsSystem.h"

#include "OgreItem.h"
#include "OgreSceneManager.h"

#include "OgreTextAreaOverlayElement.h"

using namespace Final;

extern const double cFrametime;
extern bool gFakeSlowmo;
extern bool gFakeFrameskip;

namespace Final
{
    GraphicsGameState::GraphicsGameState( const Ogre::String &helpDescription ) :
        TutorialGameState( helpDescription ),
        mEnableInterpolation( true )
    {
    }
    //-----------------------------------------------------------------------------------
    void GraphicsGameState::generateDebugText( float timeSinceLast, Ogre::String &outText )
    {
        TutorialGameState::generateDebugText( timeSinceLast, outText );
        outText += "\nPress F2 to fake a GPU bottleneck (frame skip). ";
        outText += gFakeFrameskip ? "[On]" : "[Off]";
        outText += "\nPress F3 to fake a CPU Logic bottleneck. ";
        outText += gFakeSlowmo ? "[On]" : "[Off]";
        outText += "\nPress F4 to enable interpolation. ";
        outText += mEnableInterpolation ? "[On]" : "[Off]";

        // Show the current weight.
        // The text doesn't get updated every frame while displaying
        // help, so don't show the weight as it is inaccurate.
        if( mDisplayHelpMode != 0 )
        {
            float weight = mGraphicsSystem->getAccumTimeSinceLastLogicFrame() / (float)cFrametime;
            weight = std::min( 1.0f, weight );

            if( !mEnableInterpolation )
                weight = 0;

            outText += "\nBlend weight: ";
            outText += Ogre::StringConverter::toString( weight );
        }
    }
    //-----------------------------------------------------------------------------------
    void GraphicsGameState::update( float timeSinceLast )
    {
        float weight = mGraphicsSystem->getAccumTimeSinceLastLogicFrame() / (float)cFrametime;
        weight = std::min( 1.0f, weight );

        if( !mEnableInterpolation )
            weight = 0;

        mGraphicsSystem->updateGameEntities( mGraphicsSystem->getGameEntities( Ogre::SCENE_DYNAMIC ),
                                             weight );

        TutorialGameState::update( timeSinceLast );
    }
    //-----------------------------------------------------------------------------------
    void GraphicsGameState::keyReleased( const SDL_KeyboardEvent &arg )
    {
        if( ( arg.keysym.mod & ~( KMOD_NUM | KMOD_CAPS ) ) != 0 )
        {
            TutorialGameState::keyReleased( arg );
            return;
        }

        if( arg.keysym.sym == SDLK_F2 )
        {
            gFakeFrameskip = !gFakeFrameskip;
        }
        else if( arg.keysym.sym == SDLK_F3 )
        {
            gFakeSlowmo = !gFakeSlowmo;
        }
        else if( arg.keysym.sym == SDLK_F4 )
        {
            mEnableInterpolation = !mEnableInterpolation;
        }
        else
        {
            TutorialGameState::keyReleased( arg );
        }
    }
}  // namespace Final
