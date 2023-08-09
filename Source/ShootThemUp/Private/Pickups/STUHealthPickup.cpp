// Shoot Them Up Game. All rights are reserved.


#include "Pickups/STUHealthPickup.h"
#include <GameFramework/Character.h>
#include <Components/STUHealthComponent.h>

DEFINE_LOG_CATEGORY_STATIC(LogHealthPickup, All, All);

bool ASTUHealthPickup::GivePickupTo(APawn *PlayerPawn)
{
    ACharacter *character = Cast<ACharacter>(PlayerPawn);

    const auto Component = character->GetComponentByClass(USTUHealthComponent::StaticClass());
    if (!Component) return false;

    auto HealthComponent = Cast<USTUHealthComponent>(Component);
    
    HealthComponent->OnHealthPickup.Broadcast();

    return true;
}
