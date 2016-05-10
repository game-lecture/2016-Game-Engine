#pragma once

#include "GameState.h"

class TitleState : public GameState
{
public:
  // Fill Here --------------------------------------------------------------------------------------------

  // -------------------------------------------------------------------------------------------------------

  static TitleState* getInstance() { return &mTitleState; }

private:
  static TitleState mTitleState;

  bool mContinue;

  Ogre::Overlay* mTitleOverlay;
  Ogre::OverlayElement* mStartMsg;
};
