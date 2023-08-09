// Shoot Them Up Game. All rights are reserved.

#include "Pickups/STUAmmoPickup.h"
#include <Components/STUWeaponComponent.h>
#include <GameFramework/Character.h>
#include <Components/STUHealthComponent.h>

DEFINE_LOG_CATEGORY_STATIC(LogAmmoPickup, All, All);

bool ASTUAmmoPickup::GivePickupTo(APawn *PlayerPawn)
{
    ACharacter *character = Cast<ACharacter>(PlayerPawn);

    if (!character)
        return false;

    USTUHealthComponent *HealthComponent =
        Cast<USTUHealthComponent>(character->GetComponentByClass(USTUHealthComponent::StaticClass()));

    if (!HealthComponent || HealthComponent->IsDead()) return false;

    USTUWeaponComponent *weaponComp =
        Cast<USTUWeaponComponent>(character->GetComponentByClass(USTUWeaponComponent::StaticClass()));
    if (!weaponComp)
        return false;

    UE_LOG(LogAmmoPickup, Display, TEXT("Pickup ammo was taken"));

    return weaponComp->AddAmmoToWeapon(WeaponClass, ClipsGiven);
}
