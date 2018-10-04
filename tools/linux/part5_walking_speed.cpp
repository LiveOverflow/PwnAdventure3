#include "libGameLogic.h"

// cd clients/linux_updated/PwnAdventure3_Data/PwnAdventure3/PwnAdventure3/Binaries/Linux
// g++ part5_walking_speed.cpp -shared -fPIC -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=0 -o part5_walking_speed.so
// LD_PRELOAD=part5_walking_speed.so ./PwnAdventure3-Linux-Shipping

void World::Tick(float f)
{
    // see libGameLogic.h for the class/object definitions
	ClientWorld *world = (ClientWorld *) GameWorld;
    IPlayer* iplayer = world->m_activePlayer.Get();
    Player* player = (Player*) iplayer;
    
    player->m_walkingSpeed = 99999.f;
}
