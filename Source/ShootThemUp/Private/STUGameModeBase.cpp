// Shoot Them Up Game. All rights are reserved.

#include "STUGameModeBase.h"
#include <AIController.h>
#include <Components/STURespawnComponent.h>
#include <Player/STUPlayerState.h>
#include <UI/STUGameHUD.h>
#include <EngineUtils.h>

DEFINE_LOG_CATEGORY_STATIC(LogSTUBaseGameMode, All, All)

constexpr static int32 MinRoundTimeForRespawn = 10;

ASTUGameModeBase::ASTUGameModeBase()
{
    DefaultPawnClass = ASTUBaseCharacter::StaticClass();
    PlayerControllerClass = ASTUPlayerController::StaticClass();
    HUDClass = ASTUGameHUD::StaticClass();
    PlayerStateClass = ASTUPlayerState::StaticClass();
}

void ASTUGameModeBase::RespawnRequest(AController *Controller)
{
    ResetOnePlayer(Controller);
}

UClass *ASTUGameModeBase::GetDefaultPawnClassForController_Implementation(AController *InController)
{
    if (InController && InController->IsA(AAIController::StaticClass()))
        return AIPawnClass;

    return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void ASTUGameModeBase::Killed(AController *KillerController, AController *VictimController)
{
    const auto KillerState = KillerController ? KillerController->GetPlayerState<ASTUPlayerState>() : nullptr;
    const auto VictimState = VictimController ? VictimController->GetPlayerState<ASTUPlayerState>() : nullptr;

    if (KillerState)
        KillerState->AddKill();

    if (VictimState)
        VictimState->AddDeath();

    if (VictimController)
    {
        const auto RespawnAvailable = RoundCountdown > MinRoundTimeForRespawn + GameData.RespawnTime;
        const auto RespawnComponent = VictimController->FindComponentByClass<USTURespawnComponent>();
        if (RespawnComponent || RespawnAvailable)
            RespawnComponent->Respawn(GameData.RespawnTime);
    }
}

void ASTUGameModeBase::StartPlay()
{
    Super::StartPlay();

    SpawnBots();
    CreateTeamsInfo();

    CurrentRound = 1;
    StartRound();
}

void ASTUGameModeBase::SpawnBots()
{
    if (!GetWorld())
        return;

    for (int32 i = 0; i < GameData.PlayersNum - 1; ++i)
    {
        FActorSpawnParameters SpawnInfo;
        SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        const auto ASTUAIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnInfo);

        RestartPlayer(ASTUAIController);
    }
}

void ASTUGameModeBase::StartRound()
{
    RoundCountdown = GameData.RoundTime;
    GetWorldTimerManager().SetTimer(GameRoundTimerHandle, this, &ASTUGameModeBase::GameTimerUpdate, 1.0f, true);
}

void ASTUGameModeBase::GameTimerUpdate()
{
    UE_LOG(LogSTUBaseGameMode, Display, TEXT("Timer rate: %i\n Round num: %i"), RoundCountdown, CurrentRound);

    const auto TimerRate = GetWorldTimerManager().GetTimerRate(GameRoundTimerHandle);
    RoundCountdown -= TimerRate;

    if (RoundCountdown <= 0)
    {
        GetWorldTimerManager().ClearTimer(GameRoundTimerHandle);

        if (CurrentRound + 1 <= GameData.RoundsNum)
        {
            ++CurrentRound;
            StartRound();
            ResetPlayers();
        }
        else
            GameOver();
    }
}

void ASTUGameModeBase::ResetPlayers()
{
    if (!GetWorld())
        return;

    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        ResetOnePlayer(It->Get());
    }
}

void ASTUGameModeBase::ResetOnePlayer(AController *Controller)
{
    if (Controller && Controller->GetPawn())
    {
        Controller->GetPawn()->Reset();
    }
    RestartPlayer(Controller);
    SetPlayerColor(Controller);
}

void ASTUGameModeBase::CreateTeamsInfo()
{
    if (!GetWorld())
        return;

    int32 TeamID = 1;
    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        const auto Controller = It->Get();
        if (!Controller)
            continue;

        const auto PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
        if (!PlayerState)
            continue;

        PlayerState->SetTeamID(TeamID);
        PlayerState->SetTeamColor(DetermineColorByID(TeamID));
        SetPlayerColor(Controller);
        TeamID = TeamID == 1 ? 2 : 1;
    }
}

FLinearColor ASTUGameModeBase::DetermineColorByID(int32 TeamID) const
{
    if (TeamID - 1 < GameData.TeamColors.Num())
        return GameData.TeamColors[TeamID - 1];
    UE_LOG(LogSTUBaseGameMode, Warning, TEXT("Didn't found color for team id: %i. Applies default color!"), TeamID);

    return GameData.DefaultTeamColor;
}

void ASTUGameModeBase::SetPlayerColor(AController *Controller)
{
    if (!Controller)
        return;

    const auto Character = Cast<ASTUBaseCharacter>(Controller->GetPawn());
    if (!Character)
        return;

    const auto PlayerState = Controller->GetPlayerState<ASTUPlayerState>();
    if (!PlayerState)
        return;

    Character->SetPlayerColor(PlayerState->GetTeamColor());
}

void ASTUGameModeBase::LogPlayerInfo()
{
    if (!GetWorld())
        return;
    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        const auto Controller = It->Get();
        if (!Controller)
            continue;

        const auto PlayerState = Controller->GetPlayerState<ASTUPlayerState>();
        if (!PlayerState)
            continue;

        PlayerState->LogInfo();
    }
}

void ASTUGameModeBase::GameOver()
{
    UE_LOG(LogSTUBaseGameMode, Display, TEXT("==== GAME OVER! ====="));
    LogPlayerInfo();
    if (!GetWorld()) return;

    for (auto It = GetWorld()->GetPawnIterator(); It; ++It)
    {
        auto Pawn = It->Get();
        if (Pawn)
        {
            Pawn->TurnOff();
            Pawn->DisableInput(nullptr);
        }
    }
}
