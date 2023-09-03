// Shoot Them Up Game. All rights are reserved.

#include "Player/STUBaseCharacter.h"
#include "../Components/STUHealthComponent.h"
#include <Components/InputComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include <Components/TextRenderComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <GameFramework/Controller.h>
#include <Components/STUWeaponComponent.h>

DEFINE_LOG_CATEGORY_STATIC(LogBaseCharacter, All, All);

// Sets default values
ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer &ObjInit)
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need
    // it.
    PrimaryActorTick.bCanEverTick = true;

    HealthComponent = CreateDefaultSubobject<USTUHealthComponent>("HealthComponent");

    WeaponComponent = CreateDefaultSubobject<USTUWeaponComponent>("WeaponComponent");
}

void ASTUBaseCharacter::SetPlayerColor(const FLinearColor& Color)
{
    const auto MatInstance = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
    if (!MatInstance) return;

    MatInstance->SetVectorParameterValue(MaterialColorName, Color);
}

// Called when the game starts or when spawned
void ASTUBaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    LandedDelegate.AddDynamic(this, &ASTUBaseCharacter::OnGroundLanded);

    check(HealthComponent);
    check(GetMesh());

    HealthComponent->OnDeath.AddUObject(this, &ASTUBaseCharacter::OnDeathHandle);
}

float ASTUBaseCharacter::GetMovementDirection() const
{
    const auto VelocityNormal = GetVelocity().GetSafeNormal();
    const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal) /
                                          (GetActorForwardVector().Size() * VelocityNormal.Size()));

    const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);

    return FMath::RadiansToDegrees(AngleBetween) * FMath::Sign(CrossProduct.Z);
}

void ASTUBaseCharacter::OnDeathHandle()
{
    UE_LOG(LogBaseCharacter, Display, TEXT("%s character died!"), *GetName());

    //PlayAnimMontage(DeathAnimMontage);

    GetCharacterMovement()->DisableMovement();

    SetLifeSpan(5.0f);

    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetMesh()->SetSimulatePhysics(true);
}

void ASTUBaseCharacter::OnGroundLanded(const FHitResult &Hit)
{
    const auto FallingVelocity = -GetCharacterMovement()->Velocity.Z;
    UE_LOG(LogBaseCharacter, Display, TEXT("%s landed with velocity %f.2"), *GetName(), FallingVelocity);

    if (FallingVelocity > LandedDamageVelocity.X)
    {
        float Velocity = FallingVelocity - LandedDamageVelocity.X;
        float Damage = FMath::Clamp<float>(Velocity / (LandedDamageVelocity.Y - LandedDamageVelocity.X) *
                                                   (LandedDamage.Y - LandedDamage.X) +
                                               LandedDamage.X,
                                           LandedDamage.X, LandedDamage.Y);
        TakeDamage(Damage, FDamageEvent(), Controller, this);
    }
}