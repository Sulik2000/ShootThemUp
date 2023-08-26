// Shoot Them Up Game. All rights are reserved.

#pragma once

#include "BehaviorTree/BTDecorator.h"
#include "CoreMinimal.h"
#include "STUHelathPercentDecorator.generated.h"

/**
 *
 */
UCLASS()
class SHOOTTHEMUP_API USTUHelathPercentDecorator : public UBTDecorator
{
    GENERATED_BODY()

  public:
    USTUHelathPercentDecorator();

  protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float HealthPercent = .6f;

    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) const override;
};
