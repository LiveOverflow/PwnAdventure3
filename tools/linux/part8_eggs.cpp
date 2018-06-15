#include <dlfcn.h>
#include <set>
#include <map>
#include <functional>
#include <string>
#include <vector>
#include "libGameLogic.h"

// cd clients/linux_updated/PwnAdventure3_Data/PwnAdventure3/PwnAdventure3/Binaries/Linux
// g++ part8_eggs.cpp -shared -fPIC -std=c++11 -o part8_eggs.so
// LD_PRELOAD=part8_eggs.so ./PwnAdventure3-Linux-Shipping

bool frozen = false;
Vector3 frozen_pos;

void Player::Chat(const char *msg) {
    printf("[chat] msg=\"%s\"\n", msg);
    if(strncmp("tp ", msg, 3) == 0) {
        Vector3 new_pos;
        sscanf(msg+3, "%f %f %f", &(new_pos.x), &(new_pos.y), &(new_pos.z));
        this->SetPosition(new_pos);
        frozen_pos = this->GetPosition();
    } else if(strncmp("tpz ", msg, 4) == 0) {
        Vector3 new_pos = this->GetPosition();
        sscanf(msg+4, "%f", &(new_pos.z));
        this->SetPosition(new_pos);
        frozen_pos = this->GetPosition();
    } else if(strncmp("!", msg, 1) == 0) {
        frozen_pos = this->GetPosition();
        if(frozen) frozen = false;
        else frozen = true;
    } else if(strncmp("actors", msg, 6)==0) {
        // get the address of the global variable GameWorld
        ClientWorld* world = *((ClientWorld**)(dlsym(RTLD_NEXT, "GameWorld")));
        // loop over all actors in the world
        for (ActorRef<IActor> _iactor : world->m_actors) {
            Actor* actor = ((Actor*)(_iactor.m_object));
            Vector3 pos = actor->GetPosition();
            printf("[actor] %s: %.2f %.2f %.2f\n", actor->GetDisplayName(), pos.x, pos.y, pos.z);
        }
    }
}

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
    //printf("[LO] IPlayer.GetPlayerName: %s\n", iplayer->GetPlayerName());
    //printf("[LO] player->m_mana: %d\n", player->m_mana);
    player->m_walkingSpeed = 99999;
    player->m_jumpSpeed = 999;
    player->m_jumpHoldTime = 99999;
    if(frozen) {
        Vector3 vel = player->GetVelocity();
        printf("velo: %.2f / %.2f / %.2f\n", vel.x, vel.y, vel.z);
        player->SetPosition(frozen_pos);
        //player->SetVelocity(Vector3(0,0,60));
    }
}
