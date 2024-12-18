
#ifndef _Final_UnalignedBufferUavCopyGameState_H_
#define _Final_UnalignedBufferUavCopyGameState_H_

#include "OgrePrerequisites.h"
#include "TutorialGameState.h"

namespace Final
{
    class UnalignedBufferUavCopyGameState : public TutorialGameState
    {
        void verifyBuffer( const Ogre::uint16 *referenceBuffer, Ogre::IndexBufferPacked *indexBuffer,
                           Ogre::UavBufferPacked *uavBuffer, size_t dstElemStart, size_t srcElemStart,
                           size_t srcNumElements );

    public:
        UnalignedBufferUavCopyGameState( const Ogre::String &helpDescription );

        void createScene01() override;
        void destroyScene() override;
    };
}  // namespace Final

#endif
