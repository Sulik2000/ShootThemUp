// Shoot Them Up Game. All rights are reserved.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/STUBasePickup.h"
#include "STUAmmoPickup.generated.h"

class ASTUBaseWeapon;

/**
 *
 */
UCLASS()
class SHOOTTHEMUP_API ASTUAmmoPickup : public ASTUBasePickup
{
    GENERATED_BODY()

  protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Pickups", meta = (ClampMin = "1", ClampMax = "10"))
    int32 ClipsGiven = 2;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Pickups")
    TSubclassOf<ASTUBaseWeapon> WeaponClass;

    virtual bool GivePickupTo(APawn *PlayerPawn) override;
};
