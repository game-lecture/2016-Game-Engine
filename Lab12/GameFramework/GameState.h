#pragma once

#include "GameManager.h"

class GameState
{
public:
    virtual void enter(void) = 0;
    virtual void exit(void) = 0;

    virtual void pause(void) = 0;
    virtual void resume(void) = 0;

    virtual bool frameStarted(GameManager* game, const Ogre::FrameEvent& evt) = 0;
    virtual bool frameEnded(GameManager* game, const Ogre::FrameEvent& evt) = 0;

    virtual bool mouseMoved(GameManager* game, const OIS::MouseEvent &e) = 0;
    virtual bool mousePressed(GameManager* game, const OIS::MouseEvent &e, OIS::MouseButtonID id ) = 0;
    virtual bool mouseReleased(GameManager* game, const OIS::MouseEvent &e, OIS::MouseButtonID id ) = 0;

    virtual bool keyPressed(GameManager* game, const OIS::KeyEvent &e) = 0;
    virtual bool keyReleased(GameManager* game, const OIS::KeyEvent &e) = 0;

    void changeState(GameManager* game, GameState* state)
    {
        game->changeState(state);
    }

};




