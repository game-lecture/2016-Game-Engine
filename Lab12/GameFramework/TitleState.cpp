#include "TitleState.h"
#include "PlayState.h"

using namespace Ogre;

TitleState TitleState::mTitleState;

void TitleState::enter(void)
{
  mContinue = true;
  mTitleOverlay = OverlayManager::getSingleton().getByName("Overlay/Title");
  mStartMsg = OverlayManager::getSingleton().getOverlayElement("StartMsg");
  mTitleOverlay->show();
  mStartMsg->show();
}

void TitleState::exit(void)
{
  mTitleOverlay->hide();
}

void TitleState::pause(void)
{
  std::cout << "TitleState pause\n";
}

void TitleState::resume(void)
{
  std::cout << "TitleState resume\n";
}

bool TitleState::frameStarted(GameManager* game, const FrameEvent& evt)
{
  static float elapsed = 0.0f;

  elapsed += evt.timeSinceLastFrame;

  if ((int)(elapsed * 4) % 2)
  {
    mStartMsg->show();
  }
  else
  {
    mStartMsg->hide();
  }

  return true;
}

bool TitleState::frameEnded(GameManager* game, const FrameEvent& evt)
{
  return mContinue;
}

bool TitleState::keyPressed(GameManager* game, const OIS::KeyEvent &e)
{
  // Fill Here -------------------------------------------------------

  // -----------------------------------------------------------------

  return true;
}
