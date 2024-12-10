
#ifndef _Final_MyGameState_H_
#define _Final_MyGameState_H_

#include "OgrePrerequisites.h"
#include "TutorialGameState.h"

namespace Final
{
    class GraphicsGameState;

    class LogicGameState : public GameState
    {
        float mDisplacement;
        GraphicsGameState *mGraphicsGameState;

    public:
        LogicGameState();

        void _notifyGraphicsGameState( GraphicsGameState *graphicsGameState );

        void update( float timeSinceLast ) override;
    };
}  // namespace Final

#endif
