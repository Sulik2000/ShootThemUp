// Shoot Them Up Game. All rights are reserved.

#include "UI/STUGameHUD.h"
#include <Engine/Canvas.h>
#include <Blueprint/UserWidget.h>

void ASTUGameHUD::BeginPlay()
{
    Super::BeginPlay();

    auto playerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), this->PlayerHUDWidget);
    if (playerHUDWidget)
    {
        playerHUDWidget->AddToViewport();
    }
}

void ASTUGameHUD::DrawHUD()
{
    Super::DrawHUD();
}
