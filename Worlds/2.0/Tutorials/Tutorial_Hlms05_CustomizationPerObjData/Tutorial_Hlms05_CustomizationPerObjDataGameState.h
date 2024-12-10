
#ifndef Final_Hlms05CustomizationPerObjDataGameState_H
#define Final_Hlms05CustomizationPerObjDataGameState_H

#include "OgrePrerequisites.h"
#include "TutorialGameState.h"

namespace Final
{
    class Hlms05CustomizationPerObjDataGameState : public TutorialGameState
    {
        std::vector<Ogre::Item *> mChangingItems;

    public:
        Hlms05CustomizationPerObjDataGameState( const Ogre::String &helpDescription );

        void createScene01() override;

        void update( float timeSinceLast ) override;
    };
}  // namespace Final

#endif
