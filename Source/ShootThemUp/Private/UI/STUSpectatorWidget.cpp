// Shoot Them Up Game. All rights are reserved.


#include "UI/STUSpectatorWidget.h"
#include <Components/STURespawnComponent.h>

bool USTUSpectatorWidget::GetRespawnTime(int32 &RespawnCountdown) const
{
    const auto RespawnComponent = GetOwningPlayer()->FindComponentByClass<USTURespawnComponent>();
    if (!RespawnComponent || !RespawnComponent->IsRespawnInProgress()) return false;
    
    RespawnCountdown = RespawnComponent->GetRespawnCountdown();
    return true;
}
