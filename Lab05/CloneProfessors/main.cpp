

#include <Ogre.h>
#include <OIS/OIS.h>


using namespace Ogre;

Ogre::Camera *circleCamera;


class ESCListener : public FrameListener {
  OIS::Keyboard *mKeyboard;

public:
  ESCListener(OIS::Keyboard *keyboard) : mKeyboard(keyboard) {}
  bool frameStarted(const FrameEvent &evt)
  {
    static float x = 0.0f;
    static float z = 0.0f;
    static float deg = 90.0f;

    x = 800.0f * cos(deg / 360.0f * 2.0f * 3.14f);
    z = -800.0f * sin(deg / 360.0f * 2.0f * 3.14f);
    deg = (deg > 360.0f) ? 0.0f : deg + 0.5f;

    circleCamera->setPosition(x, 400.0f, z);
    circleCamera->lookAt(0.0f, 100.0f, 0.0f);

    mKeyboard->capture();
    return !mKeyboard->isKeyDown(OIS::KC_ESCAPE);
  }
};

class LectureApp {

  Root* mRoot;
  RenderWindow* mWindow;
  SceneManager* mSceneMgr;
  Camera* mCamera;
  Viewport* mViewport;
  OIS::Keyboard* mKeyboard;
  OIS::InputManager *mInputManager;
  ESCListener* mESCListener;

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
    mWindow = mRoot->initialise(true, "Professor Clones : Copyleft by Dae-Hyun Lee");

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

    circleCamera = mCamera = mSceneMgr->createCamera("camera");

    mCamera->setPosition(0.0f, 100.0f, 500.0f);
    mCamera->lookAt(0.0f, 100.0f, 0.0f);

    mCamera->setPosition(0.0f, 100.0f, -500.0f);
    mCamera->lookAt(0.0f, 100.0f, 0.0f);

    mCamera->setPosition(0.0f, 400.0f, -800.0f);
    mCamera->lookAt(0.0f, 100.0f, 0.0f);

    mCamera->setNearClipDistance(5.0f);
    mCamera->setNearClipDistance(5.0f);

    mViewport = mWindow->addViewport(mCamera);
    mViewport->setBackgroundColour(ColourValue(0.0f,0.0f,0.5f));
//    mViewport->setBackgroundColour(ColourValue(1.0f,1.0f,1.0f));
    mCamera->setAspectRatio(Real(mViewport->getActualWidth()) / Real(mViewport->getActualHeight()));


    ResourceGroupManager::getSingleton().addResourceLocation("resource.zip", "Zip");
    ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    mSceneMgr->setAmbientLight(ColourValue(1.0f, 1.0f, 1.0f));

    // 좌표축 표시
    Ogre::Entity* mAxesEntity = mSceneMgr->createEntity("Axes", "axes.mesh");
    mSceneMgr->getRootSceneNode()->createChildSceneNode("AxesNode",Ogre::Vector3(0,0,0))->attachObject(mAxesEntity);
    mSceneMgr->getSceneNode("AxesNode")->setScale(5, 5, 5);

    _drawGridPlane();


    Entity* entity[12];
    SceneNode* node[12];

    for (int i = 0; i < 12; i++) {
      char name[20];
      sprintf(name, "Professor%d", i);
      entity[i] = mSceneMgr->createEntity(name, "DustinBody.mesh");
      float x = 250.0f * cos((float)i / 12.0f * 2.0f * 3.14f);
      float z = -250.0f * sin((float)i / 12.0f * 2.0f * 3.14f);
      node[i] = mSceneMgr->getRootSceneNode()->createChildSceneNode(Vector3(x, 0, z));
      node[i]->attachObject(entity[i]);
    }


    mRoot->startRendering();

    mInputManager->destroyInputObject(mKeyboard);
    OIS::InputManager::destroyInputSystem(mInputManager);

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
    for(int i=0; i<21; i++)
    {
      gridPlane->position(-500, 0, 500-i*50);
      gridPlane->position(500, 0, 500-i*50);

      gridPlane->position(-500+i*50, 0, 500);
      gridPlane->position(-500+i*50, 0, -500);
    }

    gridPlane->end(); 

    gridPlaneNode->attachObject(gridPlane);
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

