// Shoot Them Up Game. All rights are reserved.

#pragma once

#include "BehaviorTree/BTService.h"
#include "CoreMinimal.h"
#include "STUFireService.generated.h"

struct FBlackboardKeySelector;

UCLASS()
class SHOOTTHEMUP_API USTUFireService : public UBTService
{
    GENERATED_BODY()
  public:
    USTUFireService();

  protected:
    virtual void TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FBlackboardKeySelector HasEnemy;
};
