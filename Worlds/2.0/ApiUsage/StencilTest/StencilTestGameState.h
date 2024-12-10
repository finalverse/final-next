
#ifndef _Final_StencilTestGameState_H_
#define _Final_StencilTestGameState_H_

#include "OgrePrerequisites.h"
#include "TutorialGameState.h"

namespace Final
{
    class StencilTestGameState : public TutorialGameState
    {
        Ogre::SceneNode *mSceneNode;

    public:
        StencilTestGameState( const Ogre::String &helpDescription );

        void createScene01() override;

        void update( float timeSinceLast ) override;
    };
}  // namespace Final

#endif
