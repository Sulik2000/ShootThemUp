// Shoot Them Up Game. All rights are reserved.

#pragma once

#include "BehaviorTree/BTService.h"
#include "CoreMinimal.h"
#include "STUChangeWeaponService.generated.h"

UCLASS()
class SHOOTTHEMUP_API USTUChangeWeaponService : public UBTService
{
    GENERATED_BODY()

    public:
    USTUChangeWeaponService();
  protected:
    virtual void TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds) override final;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (ClampMax = "1.0", ClampMin = "0.0"))
    float Probablity = 0.2f;
};
