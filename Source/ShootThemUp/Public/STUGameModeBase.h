// Shoot Them Up Game. All rights are reserved.

#pragma once

#include "Player/STUBaseCharacter.h"
#include "Player/STUPlayerController.h"
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include <UI/STUGameHUD.h>
#include "STUGameModeBase.generated.h"

/**
 *
 */
UCLASS()
class SHOOTTHEMUP_API ASTUGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

  public:
    ASTUGameModeBase();
};
