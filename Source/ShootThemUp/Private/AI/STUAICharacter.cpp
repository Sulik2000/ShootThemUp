// Shoot Them Up Game. All rights are reserved.


#include "AI/STUAICharacter.h"
#include <AI/STUAIController.h>
#include <Components/STUAIWeaponComponent.h>
#include <BrainComponent.h>

ASTUAICharacter::ASTUAICharacter(const FObjectInitializer &ObjInit) : 
    Super(ObjInit.SetDefaultSubobjectClass<USTUAIWeaponComponent>("WeaponComponent"))
{
    AutoPossessAI = EAutoPossessAI::Disabled;
    AIControllerClass = ASTUAIController::StaticClass();
}

void ASTUAICharacter::OnDeathHandle()
{
    Super::OnDeathHandle();

    const auto STUController = Cast<AAIController>(Controller);
    if (!STUController || !STUController->BrainComponent) return;

    STUController->BrainComponent->Cleanup();
}
