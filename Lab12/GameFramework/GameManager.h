#pragma once

#include <vector>

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>
#define DEBUG_PRINTF(format, ...)     {fprintf (stderr, format, ##__VA_ARGS__); TCHAR __debugStr[1000]; _stprintf(__debugStr, _T(format), ##__VA_ARGS__); OutputDebugString(__debugStr);}

#endif


#include <Ogre.h>
#include <Overlay/OgreOverlay.h>
#include <Overlay/OgreOverlaySystem.h>
#include <Overlay/OgreOverlayManager.h>
#include <Overlay/OgreOverlayElement.h>
#include <Overlay/OgreTextAreaOverlayElement.h>
#include <Overlay/OgreFontManager.h>
#include <OIS/OIS.h>


class GameState;

class GameManager :  
  public Ogre::FrameListener,
  public OIS::KeyListener, 
  public OIS::MouseListener
{
public:
  GameManager();
  ~GameManager();

  void init(void);
  void changeState(GameState* state);
  void pushState(GameState* state);
  void popState();

  void go(void);

  bool mouseMoved( const OIS::MouseEvent &e );
  bool mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id );
  bool mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id );

  bool keyPressed( const OIS::KeyEvent &e );
  bool keyReleased( const OIS::KeyEvent &e );		

protected:
  Ogre::Root* mRoot;
  Ogre::RenderWindow* mWindow;

  OIS::Keyboard* mKeyboard;
  OIS::Mouse* mMouse;
  OIS::InputManager *mInputManager;

  bool frameStarted(const Ogre::FrameEvent& evt);
  bool frameEnded(const Ogre::FrameEvent& evt);

private:
  std::vector<GameState*> states;

  Ogre::SceneManager* mSceneMgr;
  Ogre::Camera* mCamera;
  Ogre::Viewport* mViewport;
};


