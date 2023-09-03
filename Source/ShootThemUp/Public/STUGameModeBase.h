// Shoot Them Up Game. All rights are reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Player/STUBaseCharacter.h"
#include "Player/STUPlayerController.h"
#include "STUGameModeBase.generated.h"

class ASTUAICharacter;
class AAIController;

USTRUCT(BlueprintType)
struct FGameData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "1", ClampMax = "100"))
    int32 PlayersNum = 2;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "1", ClampMax = "10"))
    int32 RoundsNum = 4;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "3", ClampMax = "300"))
    int32 RoundTime = 10; // In seconds

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "0.0", ClampMax = "100.0"))
    int32 RespawnTime = 10; // In seconds

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
    FLinearColor DefaultTeamColor = FLinearColor::Red;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
    TArray<FLinearColor> TeamColors;
};

UCLASS()
class SHOOTTHEMUP_API ASTUGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

  public:
    ASTUGameModeBase();

    void RespawnRequest(AController* Controller);

    virtual UClass *GetDefaultPawnClassForController_Implementation(AController *InController) override;

    void Killed(AController *KillerController, AController *VictimController);

    UFUNCTION(BlueprintCallable)
    FGameData GetGameData() const
    {
        return GameData;
    }

    UFUNCTION(BlueprintCallable)
    int32 GetCurrentRound() const
    {
        return CurrentRound;
    }

    UFUNCTION(BlueprintCallable)
    int32 GetRoundTime() const
    {
        return RoundCountdown;
    }
  protected:
    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TSubclassOf<AAIController> AIControllerClass;

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TSubclassOf<APawn> AIPawnClass;

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    FGameData GameData;

  protected:
    virtual void StartPlay() override;

  private:
    int32 CurrentRound = 1;
    int32 RoundCountdown = 0;
    FTimerHandle GameRoundTimerHandle;
    void SpawnBots();
    void StartRound();
    void GameTimerUpdate();

    void ResetPlayers();
    void ResetOnePlayer(AController *Controller);

    void CreateTeamsInfo();
    FLinearColor DetermineColorByID(int32 TeamID) const;
    void SetPlayerColor(AController *Controller);

    void LogPlayerInfo();

    void StartRespawn(AController *Controller);

    void GameOver();
};
