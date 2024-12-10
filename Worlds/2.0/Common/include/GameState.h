
#ifndef _Final_GameState_H_
#define _Final_GameState_H_

#include "InputListeners.h"

namespace Final
{
    class GameState : public MouseListener, public KeyboardListener, public JoystickListener
    {
    public:
        virtual ~GameState() {}

        virtual void initialize() {}
        virtual void deinitialize() {}

        virtual void createScene01() {}
        virtual void createScene02() {}

        virtual void destroyScene() {}

        virtual void update( float timeSinceLast ) {}
        virtual void finishFrameParallel() {}
        virtual void finishFrame() {}
    };
}  // namespace Final

#endif
