// Shoot Them Up Game. All rights are reserved.

#include "UI/STUPlayerHUDWidget.h"
#include <Components/STUHealthComponent.h>
#include <Components/STUWeaponComponent.h>
#include <GameFramework/Character.h>
#include <Player/STUPlayerState.h>
#include <STUGameModeBase.h>

float USTUPlayerHUDWidget::GetHealthPercent() const
{
    if (!GetOwningPlayerPawn())
        return 0.0f;
    const USTUHealthComponent *HealthComponent =
        GetOwningPlayerPawn<APawn>()->FindComponentByClass<USTUHealthComponent>();
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
    if (!GetOwningPlayerPawn()) return false;
    const auto HealthComponent = GetOwningPlayerPawn<APawn>()->FindComponentByClass<USTUHealthComponent>();
    if (!HealthComponent)
        return false;
    return !HealthComponent->IsDead();
}

FString USTUPlayerHUDWidget::GetTimeString() const
{
    if (!GetWorld())
        return FString();

    const auto GameMode = GetWorld()->GetAuthGameMode<ASTUGameModeBase>();

    int32 Countdown = GameMode->GetRoundTime();

    int32 Minutes = Countdown / 60;

    Countdown %= 60;

    return FString::Printf(TEXT("%i:%i"), Minutes, Countdown);
}

FString USTUPlayerHUDWidget::GetRoundString() const
{
    if (!GetWorld()) return FString();

    const auto GameMode = GetWorld()->GetAuthGameMode<ASTUGameModeBase>();
    if (!GameMode) return FString();

    const auto MaxRound = GameMode->GetGameData().RoundsNum;
    const auto CurrentRound = GameMode->GetCurrentRound();

    return FString::Printf(TEXT("Round: %i / %i"), CurrentRound, MaxRound);
}

FString USTUPlayerHUDWidget::GetKillsString() const
{
    const auto Character = GetOwningPlayerPawn<ACharacter>();
    if (!Character)
        return FString();

    const auto Controller = Character->GetController();
    if (!Controller)
        return FString();

    const auto PlayerState = Controller->GetPlayerState<ASTUPlayerState>();

    return FString::Printf(TEXT("Kills: %i"), PlayerState->GetKills());
}

bool USTUPlayerHUDWidget::IsPlayerSpectating() const
{
    const auto Controller = GetOwningPlayer();
    return !Controller && Controller->GetStateName() == NAME_Spectating;
}

bool USTUPlayerHUDWidget::Initialize()
{
    if (GetOwningPlayer())
    {
        GetOwningPlayer()->GetOnNewPawnNotifier().AddUObject(this, &USTUPlayerHUDWidget::OnNewPawn);
        OnNewPawn(GetOwningPlayerPawn());
    }
    
    return Super::Initialize();
}

void USTUPlayerHUDWidget::OnHealthChanged()
{
    OnTakeDamage();
}

void USTUPlayerHUDWidget::OnNewPawn(APawn *NewPawn)
{
    if (!NewPawn) return;
    const auto HealthComponent = NewPawn->FindComponentByClass<USTUHealthComponent>();
    if (HealthComponent)
    {
        HealthComponent->OnDamageTaken.AddUObject(this, &USTUPlayerHUDWidget::OnHealthChanged);
    }
}
