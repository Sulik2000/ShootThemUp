// Shoot Them Up Game. All rights are reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "STUPlayerController.generated.h"

class USTURespawnComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUPlayerController : public APlayerController
{
    GENERATED_BODY()
  public:
    ASTUPlayerController();

  protected:
    UPROPERTY(VisibleAnywhere, Category = "Components")
    USTURespawnComponent *RespawnComponent;
};
