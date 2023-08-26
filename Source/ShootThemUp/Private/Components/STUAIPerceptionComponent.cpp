// Shoot Them Up Game. All rights are reserved.


#include "Components/STUAIPerceptionComponent.h"
#include <AIController.h>
#include <Components/STUHealthComponent.h>
#include <Perception/AISense_Sight.h>
#include <Player/STUBaseCharacter.h>

AActor *USTUAIPerceptionComponent::GetClosestEnemy() const
{
    TArray<AActor*> PercieveActors;

    GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PercieveActors);
    if (PercieveActors.Num() == 0) return nullptr;

    const auto Controller = Cast<AAIController>(GetOwner());
    if (!Controller) return nullptr;

    const auto Pawn = Controller->GetPawn();
    if (!Pawn) return nullptr;

    float BestDistance = MAX_FLT;
    AActor *BestActor = nullptr;

    for (const auto actor : PercieveActors)
    {
        if (!actor->IsA(ASTUBaseCharacter::StaticClass())) continue;

        const auto Component = actor->GetComponentByClass(USTUHealthComponent::StaticClass());
        const auto HealthComponent = Cast<USTUHealthComponent>(Component);

        if (HealthComponent && HealthComponent->IsDead()) continue;

        const auto CurrentDistance = (actor->GetActorLocation() - Pawn->GetActorLocation()).Size();

        if (CurrentDistance < BestDistance)
        {
            BestDistance = CurrentDistance;
            BestActor = actor;
        }
    }

    return BestActor;
}
