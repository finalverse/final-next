
#ifndef _Final_TutorialUav02_SetupGameState_H_
#define _Final_TutorialUav02_SetupGameState_H_

#include "OgreMesh2.h"
#include "OgrePrerequisites.h"
#include "TutorialGameState.h"

namespace Final
{
    class TutorialUav02_SetupGameState : public TutorialGameState
    {
    public:
        TutorialUav02_SetupGameState( const Ogre::String &helpDescription );

        void createScene01() override;
    };
}  // namespace Final

#endif
