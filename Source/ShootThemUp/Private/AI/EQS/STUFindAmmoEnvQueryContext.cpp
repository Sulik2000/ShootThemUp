// Shoot Them Up Game. All rights are reserved.

#include "AI/EQS/STUFindAmmoEnvQueryContext.h"
#include <BehaviorTree/BlackboardComponent.h>
#include <Blueprint/AIBlueprintHelperLibrary.h>
#include <Components/STUAIPerceptionComponent.h>
#include <EnvironmentQuery/EnvQueryTypes.h>
#include <EnvironmentQuery/Items/EnvQueryItemType_Actor.h>
#include <Pickups/STUAmmoPickup.h>
#include <Kismet/GameplayStatics.h>

void USTUFindAmmoEnvQueryContext::ProvideContext(FEnvQueryInstance &QueryInstance,
                                                 FEnvQueryContextData &ContextData) const
{
    AActor *actor = Cast<AActor>(QueryInstance.Owner.Get());
    if (!actor)
        return;

    TArray<AActor *> actors;

    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASTUAmmoPickup::StaticClass(), actors);

    UEnvQueryItemType_Actor::SetContextHelper(ContextData, actors);
}
