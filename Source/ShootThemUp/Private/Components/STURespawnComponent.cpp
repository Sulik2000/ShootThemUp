// Shoot Them Up Game. All rights are reserved.


#include "Components/STURespawnComponent.h"
#include <STUGameModeBase.h>

// Sets default values for this component's properties
USTURespawnComponent::USTURespawnComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USTURespawnComponent::Respawn(int32 RespawnTime)
{
    if (!GetWorld()) return;

    RespawnCountdown = RespawnTime;
    GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &USTURespawnComponent::RespawnTimerUpdate, 1.0f, true);
}

void USTURespawnComponent::RespawnTimerUpdate()
{
    if (--RespawnCountdown <= 0)
    {
        if (!GetWorld()) return;
        GetWorld()->GetTimerManager().ClearTimer(RespawnTimerHandle);

        const auto GameMode = GetWorld()->GetAuthGameMode<ASTUGameModeBase>();
        if (!GameMode) return;

        GameMode->RespawnRequest(Cast<AController>(GetOwner()));
    }
}
