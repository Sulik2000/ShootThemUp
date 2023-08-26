// Shoot Them Up Game. All rights are reserved.


#include "AI/STUAIController.h"
#include <AI/STUAICharacter.h>
#include <Components/STUAIPerceptionComponent.h>
#include <BehaviorTree/BlackboardComponent.h>

ASTUAIController::ASTUAIController()
{
    STUAIPerceptionComponent = CreateDefaultSubobject<USTUAIPerceptionComponent>("PerceptionComponent");
    SetPerceptionComponent(*STUAIPerceptionComponent);
}

void ASTUAIController::OnPossess(APawn *InPawn)
{
    Super::OnPossess(InPawn);

    const auto STUCharacter = Cast<ASTUAICharacter>(GetCharacter());
    if (!STUCharacter) return;

    RunBehaviorTree(STUCharacter->BehaviorTreeAsset);
}

void ASTUAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    const auto AimActor = GetFocusOnActor();
    SetFocus(AimActor);
}

AActor *ASTUAIController::GetFocusOnActor() const
{
    if (!GetBlackboardComponent()) return nullptr;
    return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
}
