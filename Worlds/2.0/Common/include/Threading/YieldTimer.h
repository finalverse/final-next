
#ifndef _Final_YieldTimer_H_
#define _Final_YieldTimer_H_

#include "OgreTimer.h"

namespace Final
{
    class YieldTimer
    {
        Ogre::Timer *mExternalTimer;

    public:
        YieldTimer( Ogre::Timer *externalTimer ) : mExternalTimer( externalTimer ) {}

        Ogre::uint64 yield( double frameTime, Ogre::uint64 startTime )
        {
            Ogre::uint64 endTime = mExternalTimer->getMicroseconds();

            while( frameTime * 1000000.0 > double( endTime - startTime ) )
            {
                endTime = mExternalTimer->getMicroseconds();

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
                SwitchToThread();
#elif OGRE_PLATFORM == OGRE_PLATFORM_LINUX || OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
                sched_yield();
#endif
            }

            return endTime;
        }
    };
}  // namespace Final

#endif
