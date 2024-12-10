
#ifndef _Final_MeshLodGameState_H_
#define _Final_MeshLodGameState_H_

#include "OgrePrerequisites.h"
#include "TutorialGameState.h"

namespace Final
{
    class MeshLodGameState : public TutorialGameState
    {
        Ogre::SceneNode *mLightNodes[3];

    public:
        MeshLodGameState( const Ogre::String &helpDescription );

        void createScene01() override;
    };
}  // namespace Final

#endif
