
#ifndef _Final_CustomRenderableGameState_H_
#define _Final_CustomRenderableGameState_H_

#include "OgrePrerequisites.h"
#include "TutorialGameState.h"

namespace Ogre
{
    class MyCustomRenderable;
}

namespace Final
{
    class CustomRenderableGameState : public TutorialGameState
    {
        Ogre::MyCustomRenderable *mMyCustomRenderable;

    public:
        CustomRenderableGameState( const Ogre::String &helpDescription );

        void createScene01() override;
        void destroyScene() override;
    };
}  // namespace Final

#endif
