
#ifndef Final_InternalCoreGameState_H
#define Final_InternalCoreGameState_H

#include "OgrePrerequisites.h"

#include "TutorialGameState.h"

namespace Final
{
    class InternalCoreGameState : public TutorialGameState
    {
    public:
        InternalCoreGameState( const Ogre::String &helpDescription );

        void createScene01() override;
    };
}  // namespace Final

#endif
