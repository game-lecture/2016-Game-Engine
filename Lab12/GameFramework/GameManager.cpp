#define CLIENT_DESCRIPTION "Game Framework"

#include "GameManager.h"
#include "GameState.h"

using namespace Ogre;

GameManager::GameManager()
{
  mRoot = 0;
}

GameManager::~GameManager()
{
  while (!states.empty()) {
    states.back()->exit();
    states.pop_back();
  }

  if (mRoot)
    delete mRoot;
}

void GameManager::init(void)
{

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

  Ogre::OverlaySystem* mOverlaySystem = new Ogre::OverlaySystem();
  mSceneMgr->addRenderQueueListener(mOverlaySystem);



  mCamera = mSceneMgr->createCamera("main");

  mViewport = mWindow->addViewport(mCamera);
  mViewport->setBackgroundColour(ColourValue(0.0f,0.0f,0.0f));
  mCamera->setAspectRatio(Real(mViewport->getActualWidth()) / Real(mViewport->getActualHeight()));



  ResourceGroupManager::getSingleton().addResourceLocation("resource.zip", "Zip");
  ResourceGroupManager::getSingleton().addResourceLocation("./", "FileSystem");
  ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

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

  mKeyboard->setEventCallback(this);
  mMouse->setEventCallback(this);

  mRoot->addFrameListener(this);
}


void GameManager::go(void)
{
  if (mRoot)
    mRoot->startRendering();
}

void GameManager::changeState(GameState* state)
{
  if ( !states.empty() ) {
    states.back()->exit();
    states.pop_back();
  }
  states.push_back(state);
  states.back()->enter();
}

void GameManager::pushState(GameState* state)
{
  // pause current state
  if ( !states.empty() ) {
    states.back()->pause();
  }
  // store and init the new state
  states.push_back(state);
  states.back()->enter();
}

void GameManager::popState()
{
  // cleanup the current state
  if ( !states.empty() ) {
    states.back()->exit();
    states.pop_back();
  }
  // resume previous state
  if ( !states.empty() ) {
    states.back()->resume();
  }
}


bool GameManager::frameStarted(const FrameEvent& evt)
{
  if(mMouse)
    mMouse->capture();
  if(mKeyboard) 
    mKeyboard->capture();
  // call frameStarted of current state
  return states.back()->frameStarted(this, evt);
}

bool GameManager::frameEnded(const FrameEvent& evt)
{
  // call frameEnded of current state
  return states.back()->frameEnded(this, evt);
}

bool GameManager::mouseMoved(const OIS::MouseEvent &e) { return states.back()->mouseMoved(this, e); }
bool GameManager::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id ) { return states.back()->mousePressed(this, e, id); }
bool GameManager::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id ) { return states.back()->mouseReleased(this, e, id); }

bool GameManager::keyPressed(const OIS::KeyEvent &e) { return states.back()->keyPressed(this, e); }
bool GameManager::keyReleased(const OIS::KeyEvent &e) { return states.back()->keyReleased(this, e); }	
