
#ifndef _Final_MyGameState_H_
#define _Final_MyGameState_H_

#include "OgrePrerequisites.h"
#include "TutorialGameState.h"

namespace Final
{
    class LogicSystem;
    struct GameEntity;
    struct MovableObjectDefinition;

    class LogicGameState : public GameState
    {
        float mDisplacement;
        GameEntity *mCubeEntity;
        MovableObjectDefinition *mCubeMoDef;

        LogicSystem *mLogicSystem;

    public:
        LogicGameState();
        ~LogicGameState() override;

        void _notifyLogicSystem( LogicSystem *logicSystem ) { mLogicSystem = logicSystem; }

        void createScene01() override;
        void update( float timeSinceLast ) override;
    };
}  // namespace Final

#endif
