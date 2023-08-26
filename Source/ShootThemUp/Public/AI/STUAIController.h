// Shoot Them Up Game. All rights are reserved.

#pragma once

#include "AIController.h"
#include "CoreMinimal.h"
#include "STUAIController.generated.h"

class USTUAIPerceptionComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUAIController : public AAIController
{
    GENERATED_BODY()

  public:
    ASTUAIController();

  protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    USTUAIPerceptionComponent *STUAIPerceptionComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FName FocusOnKeyName = "EnemyActor";

    virtual void OnPossess(APawn* InPawn) override;

    virtual void Tick(float DeltaTime) override;

    private:
    AActor *GetFocusOnActor() const;
};
