// Shoot Them Up Game. All rights are reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "STURespawnComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTURespawnComponent : public UActorComponent
{
    GENERATED_BODY()

  public:
    // Sets default values for this component's properties
    USTURespawnComponent();

    void Respawn(int32 RespawnTime);
    int32 GetRespawnCountdown() const
    {
        return RespawnCountdown;
    }

    bool IsRespawnInProgress() const
    {
        if (GetWorld())
            return GetWorld()->GetTimerManager().IsTimerActive(RespawnTimerHandle);

        return false;
    }
  protected:

  private:
    FTimerHandle RespawnTimerHandle;
    int32 RespawnCountdown = 0;

    void RespawnTimerUpdate();
};
