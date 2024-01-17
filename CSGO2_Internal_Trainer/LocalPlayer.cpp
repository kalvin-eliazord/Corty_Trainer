#include "LocalPlayer.h"

Entity* LocalPlayer::Get()
{
    return *(Entity**)(Offset::Client::localPlayer);
}