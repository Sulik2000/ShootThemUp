// Shoot Them Up Game. All rights are reserved.

#include "AI/Decorators/STUAmmoDecorator.h"
#include <AIController.h>
#include <BehaviorTree/BehaviorTreeComponent.h>
#include <Components/STUAIWeaponComponent.h>

USTUAmmoDecorator::USTUAmmoDecorator()
{
    NodeName = "Need ammo";
}

bool USTUAmmoDecorator::CalculateRawConditionValue(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) const
{
    const auto Controller = OwnerComp.GetAIOwner();
    if (!Controller)
        return false;

    const auto Pawn = Controller->GetPawn();
    if (!Pawn)
        return false;

    const auto WeaponComponent = Pawn->FindComponentByClass<USTUAIWeaponComponent>();
    if (!WeaponComponent)
        return false;

    FAmmoData AmmoData;
    if (WeaponComponent->GetWeaponAmmoData(AmmoData, *WeaponClass))
        return !AmmoData.Infinite && (AmmoData.Bullets * AmmoData.Clips < NumOfBulletsNeed);

    return false;
}
