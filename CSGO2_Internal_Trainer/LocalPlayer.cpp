#include "LocalPlayer.h"

Entity* LocalPlayer::Get()
{
    return *(Entity**)(GameOffset::Client::localPlayer);
}