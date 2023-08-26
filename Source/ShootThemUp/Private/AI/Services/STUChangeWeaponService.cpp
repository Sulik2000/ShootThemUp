// Shoot Them Up Game. All rights are reserved.

#include "AI/Services/STUChangeWeaponService.h"
#include <AI/STUAIController.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <Components/STUAIWeaponComponent.h>
#include <GameFramework/Character.h>

USTUChangeWeaponService::USTUChangeWeaponService()
{
    NodeName = "Change Weapon";
}

void USTUChangeWeaponService::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    const auto Controller = Cast<ASTUAIController>(OwnerComp.GetAIOwner());

    if (!Blackboard || !Controller)
        return;

    const auto Character = Cast<ACharacter>(Controller->GetPawn());

    if (!Character)
        return;
    const auto WeaponComponent = Character->FindComponentByClass<USTUAIWeaponComponent>();
    if (!WeaponComponent)
        return;

    if (FMath::FRand() < Probablity)
        WeaponComponent->SwitchWeapon();
}
