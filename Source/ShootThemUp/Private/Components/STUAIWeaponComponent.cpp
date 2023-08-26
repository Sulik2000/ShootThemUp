// Shoot Them Up Game. All rights are reserved.


#include "Components/STUAIWeaponComponent.h"

USTUAIWeaponComponent::USTUAIWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUAIWeaponComponent::StopFire()
{
    if (!CurrentWeapon) return;

    FAmmoData AmmoData = CurrentWeapon->GetCurrentAmmo();
    if (AmmoData.Infinite ? false : (AmmoData.Bullets == 0 && AmmoData.Clips == 0))
        SwitchWeapon();
}
