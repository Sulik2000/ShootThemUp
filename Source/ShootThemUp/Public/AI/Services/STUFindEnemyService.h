// Shoot Them Up Game. All rights are reserved.

#pragma once

#include "BehaviorTree/BTService.h"
#include "CoreMinimal.h"
#include "STUFindEnemyService.generated.h"

UCLASS()
class SHOOTTHEMUP_API USTUFindEnemyService : public UBTService
{
    GENERATED_BODY()

  public:
    USTUFindEnemyService();

  protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FBlackboardKeySelector EnemyActorKey;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FBlackboardKeySelector HasAimEnemy;

    virtual void TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds) override;
};
