#include <cstring>
#include "libGameLogic.h"

// cd clients/linux_updated/PwnAdventure3_Data/PwnAdventure3/PwnAdventure3/Binaries/Linux
// g++ part8_eggs.cpp -shared -fPIC -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=0 -o part8_eggs.so
// LD_PRELOAD=part8_eggs.so ./PwnAdventure3-Linux-Shipping

bool frozen = false;
Vector3 frozen_pos;

void Player::Chat(const char *msg)
{
    printf("[chat] msg=\"%s\"\n", msg);
    if(strncmp("tp ", msg, 3) == 0) {
        Vector3* new_pos = new Vector3();
        sscanf(&msg[3], "%f %f %f", &new_pos->x, &new_pos->y, &new_pos->z);
        SetPosition(*new_pos);
        frozen_pos = GetPosition();
    } else if(strncmp("tpz ", msg, 4) == 0) {
        Vector3 new_pos = GetPosition();
        sscanf(&msg[4], "%f", &new_pos.z);
        SetPosition(new_pos);
        frozen_pos = GetPosition();
    } else if(strncmp("!", msg, 1) == 0) {
        frozen_pos = GetPosition();
        frozen = !frozen;
    } else if(strncmp("actors", msg, 6)==0) {
        // get the address of the global variable GameWorld
		ClientWorld *world = (ClientWorld *) GameWorld;
        // loop over all actors in the world
        for (ActorRef<IActor> _iactor : world->m_actors) {
            Actor* actor = (Actor*) _iactor.Get();
            Vector3 pos = actor->GetPosition();
            printf("[actor] %s: %.2f %.2f %.2f\n", actor->GetDisplayName(), pos.x, pos.y, pos.z);
        }
    }
}

bool Player::CanJump()
{
    return true;
}

void World::Tick(float f)
{
    // see libGameLogic.h for the class/object definitions
	ClientWorld *world = (ClientWorld *) GameWorld;
    IPlayer* iplayer = world->m_activePlayer.Get();
    Player* player = (Player*) iplayer;

    player->m_walkingSpeed = 99999.f;
    player->m_jumpSpeed = 999.f;
    player->m_jumpHoldTime = 99999.f;
    if(frozen) {
        Vector3 vel = player->GetVelocity();
        printf("velo: %.2f / %.2f / %.2f\n", vel.x, vel.y, vel.z);
        player->SetPosition(frozen_pos);
    }
}
