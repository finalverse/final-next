
#ifndef _Final_Morph_H_
#define _Final_Morph_H_

#include "OgrePrerequisites.h"
#include "TutorialGameState.h"

namespace Ogre
{
    class SkeletonAnimation;
}

namespace Final
{
    class MorphAnimationsGameState : public TutorialGameState
    {
        Ogre::Item *mSmileyItem;
        Ogre::Item *mSpringItem;
        Ogre::Item *mBlobItem;
        float mAccumulator;

    public:
        MorphAnimationsGameState( const Ogre::String &helpDescription );

        void createScene01() override;
        void update( float timeSinceLast ) override;
    };
}  // namespace Final

#endif
