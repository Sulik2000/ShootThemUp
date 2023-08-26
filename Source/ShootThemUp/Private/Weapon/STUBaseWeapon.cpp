// Shoot Them Up Game. All rights are reserved.

#include "Weapon/STUBaseWeapon.h"
#include <DrawDebugHelpers.h>
#include <Engine/World.h>
#include <GameFramework/Character.h>
#include <GameFramework/Controller.h>
#include <Math/Rotator.h>
#include <Math/Transform.h>
#include <Math/Vector.h>
#include <NiagaraFunctionLibrary.h>

DEFINE_LOG_CATEGORY_STATIC(LogFireWeapon, All, All)

// Sets default values
ASTUBaseWeapon::ASTUBaseWeapon()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    CurrentAmmo = DefaultAmmo;
    WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
    SetRootComponent(WeaponMeshComponent);
}

void ASTUBaseWeapon::StartFire()
{
    MakeShot();
    GetWorldTimerManager().SetTimer(ShootTimerHandle, this, &ASTUBaseWeapon::MakeShot, ShootCooldown, true);
}

void ASTUBaseWeapon::StopFire()
{
    GetWorldTimerManager().ClearTimer(ShootTimerHandle);
}

void ASTUBaseWeapon::OnWeaponSwitch()
{
}

FTransform ASTUBaseWeapon::GetMuzzleSocketTransform()
{
    return WeaponMeshComponent->GetSocketTransform(MuzzleSocketName);
}

void ASTUBaseWeapon::OnReloadFinished(ASTUBaseWeapon *Weapon)
{
    if (this != Weapon)
        return;
    CurrentAmmo.Bullets = DefaultAmmo.Bullets;
    CurrentAmmo.Clips--;
    LogAmmo();
}

FWeaponUIData ASTUBaseWeapon::GetWeaponUIData() const
{
    return this->UIData;
}

FAmmoData ASTUBaseWeapon::GetCurrentAmmo() const
{
    return CurrentAmmo;
}

void ASTUBaseWeapon::ReloadAmmo()
{
    ChangeClip();
}

void ASTUBaseWeapon::AddAmmo()
{
    if (!DefaultAmmo.Infinite && CurrentAmmo.Clips < DefaultAmmo.Clips)
        CurrentAmmo = DefaultAmmo;
}

// Called when the game starts or when spawned
void ASTUBaseWeapon::BeginPlay()
{
    Super::BeginPlay();
    check(WeaponMeshComponent);

    CurrentAmmo = DefaultAmmo;
}

void ASTUBaseWeapon::MakeShot()
{
    FHitResult HitResult = MakeLineTrace(GetPlayerController());
    const auto ShootDirection = WeaponMeshComponent->GetSocketTransform(MuzzleSocketName).GetRotation().Vector();

    if (HitResult.bBlockingHit)
    {
        FVector HitVector = HitResult.ImpactPoint - GetOwner()->GetActorLocation();
        // Calculating angle between vector of hit using formula A * B = |A| * |B| * Cos(a);
        float HitAngle = FMath::RadiansToDegrees(
            FMath::Acos(FVector::DotProduct(ShootDirection, HitVector) / (HitVector.Size() * ShootDirection.Size())));

        FCollisionQueryParams params;
        params.bReturnPhysicalMaterial = true;
        params.AddIgnoredActor(GetOwner());
        while (HitAngle > 90)
        {
            params.AddIgnoredActor(HitResult.Actor.Get());
            HitResult = MakeLineTrace(GetPlayerController(), params);
            if (HitResult.bBlockingHit)
            {
                HitVector = HitResult.ImpactPoint - GetOwner()->GetActorLocation();
                // Calculating angle between vector of hit using formula A * B = |A| * |B| * Cos(a);
                HitAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ShootDirection, HitVector) /
                                                               (HitVector.Size() * ShootDirection.Size())));
            }
            else
                break;
        }
        if (HitResult.bBlockingHit)
            if (HitResult.Actor.Get() != nullptr)
                HitResult.Actor.Get()->TakeDamage(Damage, FDamageEvent(), GetPlayerController(), GetOwner());
    }

    // For debug information
    if (HitResult.bBlockingHit)
    {
        DrawDebugLine(GetWorld(), WeaponMeshComponent->GetSocketTransform(MuzzleSocketName).GetLocation(),
                      HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);
        DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);
    }
}

UNiagaraComponent *ASTUBaseWeapon::SpawnMuzzleFX()
{
    return UNiagaraFunctionLibrary::SpawnSystemAttached(MuzzleFX,              //
                                                        WeaponMeshComponent,   //
                                                        MuzzleSocketName,      //
                                                        FVector::ZeroVector,   //
                                                        FRotator::ZeroRotator, //
                                                        EAttachLocation::SnapToTarget, true);
}

AController *ASTUBaseWeapon::GetPlayerController()
{
    if (!GetWorld())
        return nullptr;

    const ACharacter *Character = GetOwner<ACharacter>();
    if (!Character)
        return nullptr;

    const auto Controller = Character->GetController<APlayerController>();
    if (!Controller)
        return nullptr;

    return Controller;
}

FHitResult ASTUBaseWeapon::MakeLineTrace(AController *Controller, FCollisionQueryParams params)
{
    if (!Controller)
        return FHitResult();
    FVector ViewLocation;
    FRotator ViewRotation;

    const auto STUCharacter = Cast<ACharacter>(GetOwner());
    if (STUCharacter->IsPlayerControlled())
        Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);

    else
    {
        ViewLocation = GetMuzzleSocketTransform().GetLocation();
        ViewRotation = WeaponMeshComponent->GetSocketRotation(MuzzleSocketName);
    }

    const FTransform SocketTransform = WeaponMeshComponent->GetSocketTransform(MuzzleSocketName);
    const FVector StartTrace = ViewLocation;
    const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), FMath::DegreesToRadians(WeaponRecoil));
    const FVector TraceEnd = StartTrace + ShootDirection * TraceMaxDistance;

    FHitResult HitResult;
    GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, TraceEnd, ECollisionChannel::ECC_Visibility, params);

    return HitResult;
}

void ASTUBaseWeapon::DecreaseAmmo()
{
    CurrentAmmo.Bullets = FMath::Clamp(CurrentAmmo.Bullets - 1, 0, DefaultAmmo.Bullets);
    if (IsClipEmpty())
        ChangeClip();
}

bool ASTUBaseWeapon::IsAmmoEmpty() const
{
    return CurrentAmmo.Bullets <= 0 && CurrentAmmo.Clips <= 0 && !CurrentAmmo.Infinite;
}

bool ASTUBaseWeapon::IsClipEmpty() const
{
    return CurrentAmmo.Bullets <= 0;
}

void ASTUBaseWeapon::ChangeClip()
{
    if (!IsAmmoEmpty())
    {
        OnReloadStarted.Broadcast();
    }
    else
    {
        OnAmmoEmpty.Broadcast();
    }
}

void ASTUBaseWeapon::LogAmmo()
{
    FString AmmoInfo = FString::Printf(TEXT("Ammo clip: %i\nNum of clips: %i"), CurrentAmmo.Bullets,
                                       CurrentAmmo.Infinite ? -1 : CurrentAmmo.Clips);
    UE_LOG(LogFireWeapon, Display, TEXT("%s"), *AmmoInfo);
}