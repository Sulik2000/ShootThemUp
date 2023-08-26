// Shoot Them Up Game. All rights are reserved.

#include "STUGameModeBase.h"
#include <AIController.h>

DEFINE_LOG_CATEGORY_STATIC(LogSTUBaseGameMode, All, All)

ASTUGameModeBase::ASTUGameModeBase()
{
    DefaultPawnClass = ASTUBaseCharacter::StaticClass();
    PlayerControllerClass = ASTUPlayerController::StaticClass();
    HUDClass = ASTUGameHUD::StaticClass();
}

UClass *ASTUGameModeBase::GetDefaultPawnClassForController_Implementation(AController *InController)
{
    if (InController && InController->IsA(AAIController::StaticClass()))
        return AIPawnClass;

    return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void ASTUGameModeBase::StartPlay()
{
    Super::StartPlay();

    SpawnBots();
    
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
        }
        else
            UE_LOG(LogSTUBaseGameMode, Display, TEXT("==== GAME OVER! ====="));
    }
}
