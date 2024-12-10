
#include "GraphicsGameState.h"
#include "GraphicsSystem.h"

#include "OgreItem.h"
#include "OgreSceneManager.h"

#include "OgreTextAreaOverlayElement.h"

using namespace Final;

extern const double cFrametime;

namespace Final
{
    GraphicsGameState::GraphicsGameState( const Ogre::String &helpDescription ) :
        TutorialGameState( helpDescription )
    {
    }
    //-----------------------------------------------------------------------------------
    void GraphicsGameState::generateDebugText( float timeSinceLast, Ogre::String &outText )
    {
        TutorialGameState::generateDebugText( timeSinceLast, outText );

        // Show the current weight.
        // The text doesn't get updated every frame while displaying
        // help, so don't show the weight as it is inaccurate.
        if( mDisplayHelpMode != 0 )
        {
            outText += "\nSEE HELP DESCRIPTION!";

            float weight = mGraphicsSystem->getAccumTimeSinceLastLogicFrame() / (float)cFrametime;
            weight = std::min( 1.0f, weight );

            outText += "\nBlend weight: ";
            outText += Ogre::StringConverter::toString( weight );
        }
    }
}  // namespace Final
