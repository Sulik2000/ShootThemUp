// Shoot Them Up Game. All rights are reserved.

#include "Player/STUPlayerCharacter.h"
#include <Camera/CameraComponent.h>
#include <Components/STUWeaponComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Components/SphereComponent.h>
#include <Components/CapsuleComponent.h>

ASTUPlayerCharacter::ASTUPlayerCharacter(const FObjectInitializer &ObjInit) : Super(ObjInit)
{
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->SocketOffset.Y += 100;
    SpringArmComponent->SocketOffset.Z += 50;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);

    CameraCollisionComponent = CreateDefaultSubobject<USphereComponent>("CameraCollision");
    CameraCollisionComponent->SetupAttachment(CameraComponent);
    CameraCollisionComponent->SetSphereRadius(10.0f);
    CameraCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}

void ASTUPlayerCharacter::OnDeathHandle()
{
    Super::OnDeathHandle();

    if (Controller)
    {
        Controller->ChangeState(NAME_Spectating);
    }
}

// Called to bind functionality to input
void ASTUPlayerCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Binding movement
    PlayerInputComponent->BindAxis("MoveForward", this, &ASTUPlayerCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ASTUPlayerCharacter::MoveRight);
    PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASTUPlayerCharacter::StartRun);
    PlayerInputComponent->BindAction("Run", IE_Released, this, &ASTUPlayerCharacter::StopRun);

    // Binding looking using mouse
    PlayerInputComponent->BindAxis("LookUp", this, &ASTUPlayerCharacter::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("TurnArround", this, &ASTUPlayerCharacter::AddControllerYawInput);

    // Binding jump
    PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ASTUPlayerCharacter::Jump);

    PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Pressed, WeaponComponent, &USTUWeaponComponent::StartFire);
    PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Released, WeaponComponent, &USTUWeaponComponent::StopFire);
    PlayerInputComponent->BindAction("NextWeapon", EInputEvent::IE_Pressed, WeaponComponent,
                                     &USTUWeaponComponent::SwitchWeapon);
    PlayerInputComponent->BindAction("ReloadWeapon", EInputEvent::IE_Pressed, WeaponComponent,
                                     &USTUWeaponComponent::ReloadWeapon);
}

void ASTUPlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (IsRunning)
        MoveForward(1.0);
}

void ASTUPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(CameraCollisionComponent);

    CameraCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ASTUPlayerCharacter::OnCameraCollisionBeginOverlap);
    CameraCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ASTUPlayerCharacter::OnCameraCollisionEndOverlap);

}

void ASTUPlayerCharacter::MoveForward(float Amount)
{
    if (IsRunning)
        AddMovementInput(GetActorForwardVector(), 1.0f);
    else
        AddMovementInput(GetActorForwardVector(), Amount);
}

void ASTUPlayerCharacter::MoveRight(float Amount)
{
    if (!IsRunning)
        AddMovementInput(GetActorRightVector(), Amount);
}

void ASTUPlayerCharacter::StartRun()
{
    GetCharacterMovement()->MaxWalkSpeed += RunAccceleration;
    IsRunning = true;
}

void ASTUPlayerCharacter::StopRun()
{
    GetCharacterMovement()->MaxWalkSpeed -= RunAccceleration;
    IsRunning = false;
}

void ASTUPlayerCharacter::OnCameraCollisionBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor,
                                                        UPrimitiveComponent *OtherComp, int32 OtherBodyIndex,
                                                        bool bFromSweep, const FHitResult &SweepResult)
{
    CheckCameraOverlapping();
}
void ASTUPlayerCharacter::OnCameraCollisionEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor,
                                                      UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
    CheckCameraOverlapping();
}

void ASTUPlayerCharacter::CheckCameraOverlapping()
{
    const auto HideMesh = CameraCollisionComponent->IsOverlappingComponent(GetCapsuleComponent());

    GetMesh()->SetOwnerNoSee(HideMesh);

    TArray<USceneComponent*> Components;
    GetMesh()->GetChildrenComponents(true, Components);

    for (auto Comp : Components)
    {
        const auto MeshChild = Cast<UPrimitiveComponent>(Comp);
        if (!MeshChild) continue;

        MeshChild->SetOwnerNoSee(HideMesh);
    }
}
