#include <dlfcn.h>
#include <set>
#include <map>
#include <functional>
#include <string>
#include <vector>
#include "libGameLogic.h"

// cd clients/linux_updated/PwnAdventure3_Data/PwnAdventure3/PwnAdventure3/Binaries/Linux
// g++ part5_walking_speed.cpp -shared -fPIC -std=c++11 -o part5_walking_speed.so
// LD_PRELOAD=part5_walking_speed.so ./PwnAdventure3-Linux-Shipping

void World::Tick(float f)
{
    // see libGameLogic.h for the class/object definitions
    ClientWorld* world = *((ClientWorld**)(dlsym(RTLD_NEXT, "GameWorld")));
    IPlayer* iplayer = world->m_activePlayer.m_object;
    Player* player = ((Player*)(iplayer));
    Actor* actor = ((Actor*)(iplayer));
    player->m_walkingSpeed = 99999;
    //printf("[LO] IPlayer.GetPlayerName: %s\n", iplayer->GetPlayerName());
    //printf("[LO] player->m_mana: %d\n", player->m_mana);
}
