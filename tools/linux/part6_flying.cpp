#include <dlfcn.h>
#include <set>
#include <map>
#include <functional>
#include <string>
#include <vector>
#include "libGameLogic.h"

// cd clients/linux_updated/PwnAdventure3_Data/PwnAdventure3/PwnAdventure3/Binaries/Linux
// g++ part6_flying.cpp -shared -fPIC -std=c++11 -o part6_flying.so
// LD_PRELOAD=part6_flying.so ./PwnAdventure3-Linux-Shipping

bool Player::CanJump() {
    return 1;
}

void World::Tick(float f)
{
    // see libGameLogic.h for the class/object definitions
    ClientWorld* world = *((ClientWorld**)(dlsym(RTLD_NEXT, "GameWorld")));
    IPlayer* iplayer = world->m_activePlayer.m_object;
    Player* player = ((Player*)(iplayer));
    //Actor* actor = ((Actor*)(iplayer));
    player->m_walkingSpeed = 99999;
    //printf("[LO] IPlayer.GetPlayerName: %s\n", iplayer->GetPlayerName());
    //printf("[LO] player->m_mana: %d\n", player->m_mana);
    player->m_jumpSpeed = 999;
    player->m_jumpHoldTime = 99999;
}
