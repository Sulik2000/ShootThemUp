// Shoot Them Up Game. All rights are reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUBaseWeapon.h"
#include "STURifleWeapon.generated.h"

/**
 *
 */
UCLASS()
class SHOOTTHEMUP_API ASTURifleWeapon : public ASTUBaseWeapon
{
    GENERATED_BODY()

    virtual void StartFire() override;
    virtual void StopFire() override;
    virtual void OnWeaponSwitch() override;

    virtual void MakeShot() override;
    virtual FHitResult MakeLineTrace(AController *Controller = nullptr,
                                     FCollisionQueryParams params = FCollisionQueryParams()) override;

  private:
    FTimerHandle ShootTimerHandle;
};
