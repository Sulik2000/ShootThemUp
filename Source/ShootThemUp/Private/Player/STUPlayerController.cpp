// Shoot Them Up Game. All rights are reserved.


#include "Player/STUPlayerController.h"
#include <Components/STURespawnComponent.h>
#include <STUPlayerHUDWidget.h>
#include <GameFramework/HUD.h>

ASTUPlayerController::ASTUPlayerController()
{
    RespawnComponent = CreateDefaultSubobject<USTURespawnComponent>("RespawnComponent");
}
