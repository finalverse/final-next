
#ifndef _Final_BillboardTestGameState_H_
#define _Final_BillboardTestGameState_H_

#include "OgrePrerequisites.h"

#include "TutorialGameState.h"

namespace Final
{
    class BillboardTestGameState : public TutorialGameState
    {
    public:
        BillboardTestGameState( const Ogre::String &helpDescription );

        void createScene01() override;
    };
}  // namespace Final

#endif
