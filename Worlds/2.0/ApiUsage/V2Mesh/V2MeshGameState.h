
#ifndef _Final_V2MeshGameState_H_
#define _Final_V2MeshGameState_H_

#include "OgrePrerequisites.h"
#include "TutorialGameState.h"

namespace Final
{
    class V2MeshGameState : public TutorialGameState
    {
    public:
        V2MeshGameState( const Ogre::String &helpDescription );

        void createScene01() override;
    };
}  // namespace Final

#endif
