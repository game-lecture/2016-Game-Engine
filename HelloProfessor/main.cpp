#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
  #define WIN32_LEAN_AND_MEAN
  #include "windows.h"
#endif

#include <Ogre.h>
#include <OIS/OIS.h>


using namespace Ogre;

class ESCListener : public FrameListener {
	OIS::Keyboard *mKeyboard;

public:
	ESCListener(OIS::Keyboard *keyboard) : mKeyboard(keyboard) {}
  bool frameStarted(const FrameEvent &evt)
  {
    mKeyboard->capture();
	return !mKeyboard->isKeyDown(OIS::KC_ESCAPE);
  }
};

class HelloSinbadApp {

  Root* mRoot;
  RenderWindow* mWindow;
  SceneManager* mSceneMgr;
  Camera* mCamera;
  Viewport* mViewport;
  OIS::Keyboard* mKeyboard;
  OIS::InputManager *mInputManager;
  ESCListener* mESCListener;

public:

  HelloSinbadApp() {}

  ~HelloSinbadApp() {}

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
    mWindow = mRoot->initialise(true, "Hello Professor : Copyleft Dae-Hyun Lee");

    // ESC key를 눌렀을 경우, 오우거 메인 렌더링 루프의 탈출을 처리
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;
	OIS::ParamList pl;
	mWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
	mInputManager = OIS::InputManager::createInputSystem(pl);
	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, false));

	mESCListener = new ESCListener(mKeyboard);
    mRoot->addFrameListener(mESCListener);

	mSceneMgr = mRoot->createSceneManager(ST_GENERIC);

    mCamera = mSceneMgr->createCamera("camera");
    mCamera->setPosition(0.0f, 200.0f, 300.0f);
    mCamera->lookAt(0.0f, 100.0f, 0.00f);
    mCamera->setNearClipDistance(5.0f);

    mViewport = mWindow->addViewport(mCamera);
    mViewport->setBackgroundColour(ColourValue(0.0f,0.0f,0.5f));
    mCamera->setAspectRatio(Real(mViewport->getActualWidth()) / Real(mViewport->getActualHeight()));

    ResourceGroupManager::getSingleton().addResourceLocation("resource.zip", "Zip");
    ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    mSceneMgr->setAmbientLight(ColourValue(1.0f, 1.0f, 1.0f));

    Entity* daehyunEntity = mSceneMgr->createEntity("Daehyun", "DustinBody.mesh");

    SceneNode* daehyunNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    daehyunNode->attachObject(daehyunEntity);

    mRoot->startRendering();

    mInputManager->destroyInputObject(mKeyboard);
    OIS::InputManager::destroyInputSystem(mInputManager);

    delete mRoot;
  }
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
      HelloSinbadApp app;

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

