// Shoot Them Up Game. All rights are reserved.

#include "Player/STUBaseCharacter.h"
#include "../Components/STUHealthComponent.h"
#include <Camera/CameraComponent.h>
#include <Components/InputComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include <Components/TextRenderComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <GameFramework/Controller.h>
#include <GameFramework/SpringArmComponent.h>
#include <Components/STUWeaponComponent.h>

DEFINE_LOG_CATEGORY_STATIC(LogBaseCharacter, All, All);

// Sets default values
ASTUBaseCharacter::ASTUBaseCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need
    // it.
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->SocketOffset.Y += 100;
    SpringArmComponent->SocketOffset.Z += 50;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);

    HealthComponent = CreateDefaultSubobject<USTUHealthComponent>("HealthComponent");

    HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
    HealthTextComponent->SetupAttachment(GetRootComponent());

    WeaponComponent = CreateDefaultSubobject<USTUWeaponComponent>("WeaponComponent");

    HealthTextComponent->bOwnerNoSee = true;
}

// Called when the game starts or when spawned
void ASTUBaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    LandedDelegate.AddDynamic(this, &ASTUBaseCharacter::OnGroundLanded);

    check(HealthComponent);

    HealthComponent->OnDeath.AddUObject(this, &ASTUBaseCharacter::OnDeathHandle);
    HealthComponent->OnHealthChanged.AddUObject(this, &ASTUBaseCharacter::OnHealthChangedHandle);
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

    PlayAnimMontage(DeathAnimMontage);

    GetCharacterMovement()->DisableMovement();

    SetLifeSpan(5.0f);
    if (Controller)
    {
        Controller->ChangeState(NAME_Spectating);
    }
}

void ASTUBaseCharacter::OnHealthChangedHandle(float Health)
{
    if (HealthTextComponent)
        HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%i"), Health)));
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

void ASTUBaseCharacter::MoveForward(float Amount)
{
    if (IsRunning)
        AddMovementInput(GetActorForwardVector(), 1.0f);
    else
        AddMovementInput(GetActorForwardVector(), Amount);
}

void ASTUBaseCharacter::MoveRight(float Amount)
{
    if (!IsRunning)
        AddMovementInput(GetActorRightVector(), Amount);
}

void ASTUBaseCharacter::StartRun()
{
    GetCharacterMovement()->MaxWalkSpeed += RunAccceleration;
    IsRunning = true;
}

void ASTUBaseCharacter::StopRun()
{
    GetCharacterMovement()->MaxWalkSpeed -= RunAccceleration;
    IsRunning = false;
}

// Called every frame
void ASTUBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (IsRunning)
        MoveForward(1.0);

    if (HealthTextComponent && HealthComponent)
        HealthTextComponent->SetText(FText::FromString(FString::FromInt(HealthComponent->GetHealth())));
}

// Called to bind functionality to input
void ASTUBaseCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Binding movement
    PlayerInputComponent->BindAxis("MoveForward", this, &ASTUBaseCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ASTUBaseCharacter::MoveRight);
    PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASTUBaseCharacter::StartRun);
    PlayerInputComponent->BindAction("Run", IE_Released, this, &ASTUBaseCharacter::StopRun);

    // Binding looking using mouse
    PlayerInputComponent->BindAxis("LookUp", this, &ASTUBaseCharacter::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("TurnArround", this, &ASTUBaseCharacter::AddControllerYawInput);

    // Binding jump
    PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ASTUBaseCharacter::Jump);

    PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Pressed, WeaponComponent, &USTUWeaponComponent::StartFire);
    PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Released, WeaponComponent, &USTUWeaponComponent::StopFire);
    PlayerInputComponent->BindAction("NextWeapon", EInputEvent::IE_Pressed, WeaponComponent, &USTUWeaponComponent::SwitchWeapon);
    PlayerInputComponent->BindAction("ReloadWeapon", EInputEvent::IE_Pressed, WeaponComponent, &USTUWeaponComponent::ReloadWeapon);

}
