// Shoot Them Up Game. All rights are reserved.

#include "AI/Tasks/STUNextLocationTask.h"
#include <AIController.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <NavigationSystem.h>

USTUNextLocationTask::USTUNextLocationTask()
{
    NodeName = "NextLocation";
}

EBTNodeResult::Type USTUNextLocationTask::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    const auto Controller = OwnerComp.GetAIOwner();
    const auto Blackboard = OwnerComp.GetBlackboardComponent();

    if (!Controller || !Blackboard)
        return EBTNodeResult::Failed;

    const auto Pawn = Controller->GetPawn();
    if (!Pawn)
        return EBTNodeResult::Failed;

    const auto NavSys = UNavigationSystemV1::GetCurrent(Pawn);
    if (!NavSys)
        return EBTNodeResult::Failed;
    
    auto CenterLocation = Pawn->GetActorLocation();
    if (!SelfCenter)
    {
        auto CenterActor = Cast<AActor>(Blackboard->GetValueAsObject(CenterActorKey.SelectedKeyName));
        if (!CenterActor) return EBTNodeResult::Failed;
        
        CenterLocation = CenterActor->GetActorLocation();
    }

    FVector Location =
        NavSys->GetRandomPointInNavigableRadius(GetWorld(), CenterLocation, Radius);

    Blackboard->SetValueAsVector(AimLocationKey.SelectedKeyName, Location);

    return EBTNodeResult::Succeeded;
}
