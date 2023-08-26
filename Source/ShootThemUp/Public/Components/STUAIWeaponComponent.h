// Shoot Them Up Game. All rights are reserved.

#pragma once

#include "Components/STUWeaponComponent.h"
#include "CoreMinimal.h"
#include "STUAIWeaponComponent.generated.h"

/**
 *
 */
UCLASS()
class SHOOTTHEMUP_API USTUAIWeaponComponent : public USTUWeaponComponent
{
    GENERATED_BODY()

  public:
    USTUAIWeaponComponent();

  protected:
    virtual void StopFire() override;
};
