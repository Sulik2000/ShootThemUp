// Shoot Them Up Game. All rights are reserved.


#include "AI/Services/STUFireService.h"
#include <BehaviorTree/BlackboardComponent.h>
#include <Components/STUWeaponComponent.h>
#include <AIController.h>

USTUFireService::USTUFireService()
{
    NodeName = "Fire";
}

void USTUFireService::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
    auto Blackboard = OwnerComp.GetBlackboardComponent();
    const auto Controller = OwnerComp.GetAIOwner();

    const bool HasAim = Blackboard && Blackboard->GetValueAsBool(HasEnemy.SelectedKeyName);

    if (Controller)
    {
        const auto Pawn = Controller->GetPawn();
        auto WeaponComponent = Pawn->FindComponentByClass<USTUWeaponComponent>();
        if (WeaponComponent)
            HasAim ? WeaponComponent->StartFire() : WeaponComponent->StopFire();
    }

    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
