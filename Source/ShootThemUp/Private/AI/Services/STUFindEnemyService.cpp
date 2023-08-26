// Shoot Them Up Game. All rights are reserved.

#include "AI/Services/STUFindEnemyService.h"
#include <AIController.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <Components/STUAIPerceptionComponent.h>

USTUFindEnemyService::USTUFindEnemyService()
{
    NodeName = "Find Enemy";
}

void USTUFindEnemyService::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    if (Blackboard)
    {
        const auto Controller = OwnerComp.GetAIOwner();
        const auto PerceptionComponent = Controller->FindComponentByClass<USTUAIPerceptionComponent>();

        if (PerceptionComponent)
        {
            Blackboard->SetValueAsObject(EnemyActorKey.SelectedKeyName, PerceptionComponent->GetClosestEnemy());

            if(PerceptionComponent->GetClosestEnemy())
                Blackboard->SetValueAsBool(HasAimEnemy.SelectedKeyName, true);

            else
                Blackboard->SetValueAsBool(HasAimEnemy.SelectedKeyName, false);
        }
    }

    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
