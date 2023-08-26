// Shoot Them Up Game. All rights are reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Player/STUBaseCharacter.h"
#include "Player/STUPlayerController.h"
#include <UI/STUGameHUD.h>
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
    int32 RoundsNum= 4;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "3", ClampMax = "300"))
    int32 RoundTime = 10; // In seconds
};

UCLASS()
class SHOOTTHEMUP_API ASTUGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

  public:
    ASTUGameModeBase();

    virtual UClass *GetDefaultPawnClassForController_Implementation(AController *InController) override;

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
};
