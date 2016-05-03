#define CLIENT_DESCRIPTION "Character Controller"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#include <Ogre.h>
#include <OIS/OIS.h>


using namespace Ogre;

class InputController : public FrameListener, 
  public OIS::KeyListener, 
  public OIS::MouseListener
{

public:
  InputController(Root* root, OIS::Keyboard *keyboard, OIS::Mouse *mouse) : mRoot(root), mKeyboard(keyboard), mMouse(mouse)
  {
    mIdleState = root->getSceneManager("main")->getEntity("Professor")->getAnimationState("Idle");
    mWalkState = root->getSceneManager("main")->getEntity("Professor")->getAnimationState("Walk");

    mIdleState->setLoop(true);
    mWalkState->setLoop(true);

    mIdleState->setEnabled(true);
    mWalkState->setEnabled(false);

    mCharacterRoot = mRoot->getSceneManager("main")->getSceneNode("ProfessorRoot");
    mCharacterYaw = mRoot->getSceneManager("main")->getSceneNode("ProfessorYaw");
    mCharacterDirection = Ogre::Vector3::ZERO;

    mCameraYaw = mRoot->getSceneManager("main")->getSceneNode("CameraYaw");
    mCameraPitch = mRoot->getSceneManager("main")->getSceneNode("CameraPitch");
    mCameraHolder = mRoot->getSceneManager("main")->getSceneNode("CameraHolder");


    mContinue = true;

    mRotating = false;
    mRotatingTime = 0.0f;

    keyboard->setEventCallback(this);
    mouse->setEventCallback(this);
  }


  bool frameStarted(const FrameEvent &evt)
  {


    mKeyboard->capture();
    mMouse->capture();


    // Fill Here -------------------------------------------------------------------


    // -----------------------------------------------------------------------------

    return mContinue;
  }

  // Key Linstener Interface Implementation

  bool keyPressed( const OIS::KeyEvent &evt )
  {
    // Fill Here -----------------------------------------------

    // ---------------------------------------------------------
  }

  bool keyReleased( const OIS::KeyEvent &evt )
  {
    // Fill Here -----------------------------------------------


    // ---------------------------------------------------------

  }


  // Mouse Listener Interface Implementation

  bool mouseMoved( const OIS::MouseEvent &evt )
  {
    mCameraYaw->yaw(Degree(-evt.state.X.rel));
    mCameraPitch->pitch(Degree(-evt.state.Y.rel));

    mCameraHolder->translate(Ogre::Vector3(0, 0, -evt.state.Z.rel * 0.1f));

    return true;
  }

  bool mousePressed( const OIS::MouseEvent &evt, OIS::MouseButtonID id )
  {
    return true;
  }

  bool mouseReleased( const OIS::MouseEvent &evt, OIS::MouseButtonID id )
  {
    return true;
  }


private:
  bool mContinue;
  Ogre::Root* mRoot;
  OIS::Keyboard* mKeyboard;
  OIS::Mouse* mMouse;


  Ogre::AnimationState* mWalkState;
  Ogre::AnimationState* mIdleState;

  SceneNode* mCharacterRoot;
  SceneNode* mCharacterYaw;

  SceneNode* mCameraHolder;
  SceneNode* mCameraYaw;
  SceneNode* mCameraPitch;

  Ogre::Vector3 mCharacterDirection;

  bool mRotating;  
  Quaternion mSrcQuat, mDestQuat;  
  float mRotatingTime;
};







class LectureApp {





public:

  LectureApp() {}

  ~LectureApp() {}

  void go(void)
  {
    // OGRE의 메인 루트 오브젝트 생성
#if !defined(_DEBUG)
    mRoot = new Root("plugins.cfg", "ogre.cfg", "ogre.log");
#else
    mRoot = new Root("plugins_d.cfg", "ogre.cfg", "ogre.log");
#endif


    // 초기 시작의 컨피규레이션 설정 - ogre.cfg 이용
    if (!mRoot->restoreConfig()) {
      if (!mRoot->showConfigDialog()) return;
    }

    mWindow = mRoot->initialise(true, CLIENT_DESCRIPTION " : Copyleft by Dae-Hyun Lee");

    mSceneMgr = mRoot->createSceneManager(ST_GENERIC, "main");
    mCamera = mSceneMgr->createCamera("main");

    mViewport = mWindow->addViewport(mCamera);
    mViewport->setBackgroundColour(ColourValue(0.0f,0.0f,0.5f));
    mCamera->setAspectRatio(Real(mViewport->getActualWidth()) / Real(mViewport->getActualHeight()));


    ResourceGroupManager::getSingleton().addResourceLocation("resource.zip", "Zip");
    ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    mSceneMgr->setAmbientLight(ColourValue(1.0f, 1.0f, 1.0f));

    // 좌표축 표시
    Ogre::Entity* mAxesEntity = mSceneMgr->createEntity("Axes", "axes.mesh");
    mSceneMgr->getRootSceneNode()->createChildSceneNode("AxesNode",Ogre::Vector3(0,0,0))->attachObject(mAxesEntity);
    mSceneMgr->getSceneNode("AxesNode")->setScale(5, 5, 5);

    _drawGridPlane();

    // Fill Here -----------------------------------------------


    // ---------------------------------------------------------

    Entity* entity = mSceneMgr->createEntity("Professor", "DustinBody.mesh");
    professorYaw->attachObject(entity);

    cameraHolder->attachObject(mCamera);
    mCamera->lookAt(cameraYaw->getPosition());

    size_t windowHnd = 0;
    std::ostringstream windowHndStr;
    OIS::ParamList pl;
    mWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
    pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
    pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
    pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
    pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
    mInputManager = OIS::InputManager::createInputSystem(pl);

    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, true));
    mMouse = static_cast<OIS::Mouse*>( mInputManager->createInputObject(OIS::OISMouse, true));

    InputController* inputController = new InputController(mRoot, mKeyboard, mMouse);
    mRoot->addFrameListener(inputController);

    mRoot->startRendering();

    mInputManager->destroyInputObject(mKeyboard);
    mInputManager->destroyInputObject(mMouse);
    OIS::InputManager::destroyInputSystem(mInputManager);

    delete inputController;

    delete mRoot;
  }

private:
  void _drawGridPlane(void)
  {
    Ogre::ManualObject* gridPlane =  mSceneMgr->createManualObject("GridPlane"); 
    Ogre::SceneNode* gridPlaneNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("GridPlaneNode"); 

    Ogre::MaterialPtr gridPlaneMaterial = Ogre::MaterialManager::getSingleton().create("GridPlanMaterial","General"); 
    gridPlaneMaterial->setReceiveShadows(false); 
    gridPlaneMaterial->getTechnique(0)->setLightingEnabled(true); 
    gridPlaneMaterial->getTechnique(0)->getPass(0)->setDiffuse(1,1,1,0); 
    gridPlaneMaterial->getTechnique(0)->getPass(0)->setAmbient(1,1,1); 
    gridPlaneMaterial->getTechnique(0)->getPass(0)->setSelfIllumination(1,1,1); 

    gridPlane->begin("GridPlaneMaterial", Ogre::RenderOperation::OT_LINE_LIST); 
	for (int i = 0; i<81; i++)
	{
		gridPlane->position(-2000.0f, 0.0f, 2000.0f - i * 50);
		gridPlane->position(2000.0f, 0.0f, 2000.0f - i * 50);

		gridPlane->position(-2000.f + i * 50, 0.f, 2000.0f);
		gridPlane->position(-2000.f + i * 50, 0.f, -2000.f);
	}

    gridPlane->end(); 

    gridPlaneNode->attachObject(gridPlane);
  }


  Root* mRoot;
  RenderWindow* mWindow;
  SceneManager* mSceneMgr;
  Camera* mCamera;
  Viewport* mViewport;
  OIS::Keyboard* mKeyboard;
  OIS::Mouse* mMouse;

  OIS::InputManager *mInputManager;
};


#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
  INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
  int main(int argc, char *argv[])
#endif
  {
    LectureApp app;

    try {

      app.go();

    } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
      MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
      std::cerr << "An exception has occured: " <<
        e.getFullDescription().c_str() << std::endl;
#endif
    }

    return 0;
  }

#ifdef __cplusplus
}
#endif

