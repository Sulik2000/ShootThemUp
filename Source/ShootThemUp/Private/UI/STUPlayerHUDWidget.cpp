// Shoot Them Up Game. All rights are reserved.

#include "UI/STUPlayerHUDWidget.h"
#include <Components/STUHealthComponent.h>
#include <Components/STUWeaponComponent.h>

float USTUPlayerHUDWidget::GetHealthPercent() const
{
    const USTUHealthComponent *HealthComponent = GetHealthComponent();
    if (!HealthComponent)
        return 0.0f;

    return HealthComponent->GetHealthPercent();
}

bool USTUPlayerHUDWidget::GetWeaponUIData(FWeaponUIData &WeaponData) const
{
    const auto Player = GetOwningPlayerPawn();
    if (!Player)
        return 0.0f;

    const auto Component = Player->GetComponentByClass(USTUWeaponComponent::StaticClass());
    const auto WeaponComponent = Cast<USTUWeaponComponent>(Component);
    if (!WeaponComponent)
        return false;

    return WeaponComponent->GetWeaponUIData(WeaponData);
}

bool USTUPlayerHUDWidget::IsPlayerAlive() const
{
    const auto HealthComponent = GetHealthComponent();
    if (!HealthComponent) return false;
    return !HealthComponent->IsDead();
}

bool USTUPlayerHUDWidget::IsPlayerSpectating() const
{
    const auto Controller = GetOwningPlayer();
    return !Controller && Controller->GetStateName() == NAME_Spectating;
}

bool USTUPlayerHUDWidget::Initialize()
{
    const auto HealthComponent = GetHealthComponent();
    if (HealthComponent)
    {
        HealthComponent->OnDamageTaken.AddUObject(this, &USTUPlayerHUDWidget::OnHealthChanged);
    }

    return Super::Initialize();
}

USTUHealthComponent *USTUPlayerHUDWidget::GetHealthComponent() const
{
    auto Character = GetOwningPlayerPawn();
    if (!Character)
        return nullptr;

    auto Component = Cast<USTUHealthComponent>(Character->GetComponentByClass(USTUHealthComponent::StaticClass()));

    return Component;
}

void USTUPlayerHUDWidget::OnHealthChanged()
{
    OnTakeDamage();
}   
