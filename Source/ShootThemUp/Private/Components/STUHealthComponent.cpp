// Shoot Them Up Game. All rights are reserved.

#include "Components/STUHealthComponent.h"
#include <GameFramework/Actor.h>

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All);

USTUHealthComponent::USTUHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

bool USTUHealthComponent::IsDead() const
{
    return Health <= 0.0f;
}

float USTUHealthComponent::GetHealthPercent() const
{
    return Health / MaxHealth;
}

// Called when the game starts
void USTUHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    check(MaxHealth > 0);

    Health = MaxHealth;
    OnHealthChanged.Broadcast(Health);

    AActor *ComponentOwner = GetOwner();
    if (ComponentOwner)
    {
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage);
    }
}

void USTUHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (IsHealing)
    {
        Health = FMath::Clamp<float>(Health + DeltaTime * HealingSpeed, 0, 100);
    }
}

void USTUHealthComponent::StartHealing()
{
    this->IsHealing = true;
}

void USTUHealthComponent::OnTakeAnyDamage(AActor *DamagedActor, float Damage, const UDamageType *DamageType,
                                          AController *InstigatedBy, AActor *DamageCauser)
{
    if (Damage <= 0.0f || IsDead())
        return;

    IsHealing = false;
    Health = FMath::Clamp<float>(Health - Damage, 0, MaxHealth);
    OnHealthChanged.Broadcast(Health);

    GetWorld()->GetTimerManager().ClearTimer(HealTimer);

    if (IsDead())
        OnDeath.Broadcast();

    if (!IsDead() && AutoHeal)
    {
        GetWorld()->GetTimerManager().SetTimer(HealTimer, this, &USTUHealthComponent::StartHealing, HealingDelay);
    }
}