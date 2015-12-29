#include "IGame.h"

IGame::IGame() :
    m_keyManager(nullptr),
    m_initialized(false)
{
}

void IGame::initialize()
{
    m_initialized = true;
}

void IGame::setKeyManager(KeyManager *keyManager)
{
    m_keyManager = keyManager;
}

bool IGame::isInitialized()
{
    return m_initialized;
}
