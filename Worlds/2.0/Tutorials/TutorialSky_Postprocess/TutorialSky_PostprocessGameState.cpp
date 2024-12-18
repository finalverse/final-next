
#include "TutorialSky_PostprocessGameState.h"
#include "CameraController.h"
#include "GraphicsSystem.h"

#include "OgreItem.h"
#include "OgreMesh.h"
#include "OgreMesh2.h"
#include "OgreMesh2Serializer.h"
#include "OgreMeshManager.h"
#include "OgreMeshManager2.h"
#include "OgreSceneManager.h"
#include "OgreSubMesh2.h"

#include "OgreRoot.h"
#include "Vao/OgreVaoManager.h"
#include "Vao/OgreVertexArrayObject.h"

#include "OgreCamera.h"

using namespace Final;

namespace Final
{
    TutorialSky_PostprocessGameState::TutorialSky_PostprocessGameState(
        const Ogre::String &helpDescription ) :
        TutorialGameState( helpDescription )
    {
    }
    //-----------------------------------------------------------------------------------
    void TutorialSky_PostprocessGameState::createScene01()
    {
        Ogre::SceneManager *sceneManager = mGraphicsSystem->getSceneManager();

        Ogre::Item *item = sceneManager->createItem(
            "Cube_d.mesh", Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME,
            Ogre::SCENE_DYNAMIC );

        Ogre::SceneNode *sceneNode = sceneManager->getRootSceneNode( Ogre::SCENE_DYNAMIC )
                                         ->createChildSceneNode( Ogre::SCENE_DYNAMIC );

        sceneNode->attachObject( item );

        Ogre::Light *light = sceneManager->createLight();
        Ogre::SceneNode *lightNode = sceneManager->getRootSceneNode()->createChildSceneNode();
        lightNode->attachObject( light );
        light->setPowerScale( Ogre::Math::PI );  // Since we don't do HDR, counter the PBS' division by
                                                 // PI
        light->setType( Ogre::Light::LT_DIRECTIONAL );
        light->setDirection( Ogre::Vector3( -1, -1, -1 ).normalisedCopy() );

        mCameraController = new CameraController( mGraphicsSystem, false );

        TutorialGameState::createScene01();
    }
}  // namespace Final
