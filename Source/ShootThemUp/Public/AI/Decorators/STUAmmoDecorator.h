// Shoot Them Up Game. All rights are reserved.

#pragma once

#include "BehaviorTree/BTDecorator.h"
#include "CoreMinimal.h"
#include "STUAmmoDecorator.generated.h"

class ASTUBaseWeapon;

/**
 *
 */
UCLASS()
class SHOOTTHEMUP_API USTUAmmoDecorator : public UBTDecorator
{
    GENERATED_BODY()

  public:
    USTUAmmoDecorator();

  protected:
    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) const override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    int32 NumOfBulletsNeed = 2;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<ASTUBaseWeapon> WeaponClass;
};
