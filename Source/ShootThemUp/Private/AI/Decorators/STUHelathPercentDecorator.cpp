// Shoot Them Up Game. All rights are reserved.


#include "AI/Decorators/STUHelathPercentDecorator.h"
#include <AIController.h>
#include <Components/STUHealthComponent.h>
#include <GameFramework/Character.h>
#include <AIController.h>

USTUHelathPercentDecorator::USTUHelathPercentDecorator()
{
    NodeName = "Health percent";
}

bool USTUHelathPercentDecorator::CalculateRawConditionValue(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) const
{
    const auto Controller = OwnerComp.GetAIOwner();
    if (!Controller) return false;

    const auto Character = Controller->GetPawn();
    if (!Character) return false;

    const auto HealthComponent = Character->FindComponentByClass<USTUHealthComponent>();
    if (!HealthComponent || HealthComponent->IsDead()) return false;

    return HealthComponent->GetHealthPercent() <= HealthPercent;
}
