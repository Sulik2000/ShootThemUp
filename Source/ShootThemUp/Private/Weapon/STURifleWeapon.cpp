// Shoot Them Up Game. All rights are reserved.

#include "Weapon/STURifleWeapon.h"
#include <Components/STUWeaponFXComponent.h>
#include <DrawDebugHelpers.h>
#include <NiagaraComponent.h>
#include <NiagaraFunctionLibrary.h>
#include <NiagaraSystem.h>
#include <GameFramework/Character.h>

ASTURifleWeapon::ASTURifleWeapon()
{
    FXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("EffectComponent");
}

void ASTURifleWeapon::StartFire()
{
    InitMuzzleFX();
    MakeShot();
    GetWorldTimerManager().SetTimer(ShootTimerHandle, this, &ASTURifleWeapon::MakeShot, ShootCooldown, true);
}

void ASTURifleWeapon::StopFire()
{
    GetWorldTimerManager().ClearTimer(ShootTimerHandle);
    SetMuzzleFXVisibility(false);
}

void ASTURifleWeapon::OnWeaponSwitch()
{
    StopFire();
}

void ASTURifleWeapon::MakeShot()
{
    if (!GetWorld() || IsAmmoEmpty())
    {
        StopFire();
        return;
    }

    FHitResult HitResult = MakeLineTrace(GetPlayerController());
    const auto ShootDirection = WeaponMeshComponent->GetSocketTransform(MuzzleSocketName).GetRotation().Vector();

    if (HitResult.bBlockingHit)
    {
        FVector HitVector = HitResult.ImpactPoint - GetOwner()->GetActorLocation();
        // Calculating angle between vector of hit using formula A * B = |A| * |B| * Cos(a);
        float HitAngle = FMath::RadiansToDegrees(
            FMath::Acos(FVector::DotProduct(ShootDirection, HitVector) / (HitVector.Size() * ShootDirection.Size())));

        FCollisionQueryParams params;
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
        FXComponent->PlayImpactFX(HitResult);
    }

    DecreaseAmmo();
}

FHitResult ASTURifleWeapon::MakeLineTrace(AController *Controller, FCollisionQueryParams params)
{
    params.bReturnPhysicalMaterial = true;
    
    FVector ViewLocation;
    FRotator ViewRotation;

    const auto STUCharacter = Cast<ACharacter>(GetOwner());
    if (STUCharacter->IsPlayerControlled() && Controller)
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

    FVector TraceFXEnd = TraceEnd;
    if (HitResult.bBlockingHit)
        TraceFXEnd = HitResult.ImpactPoint;

    SpawnTraceFX(GetMuzzleSocketTransform().GetLocation(), TraceFXEnd);

    return HitResult;
}

void ASTURifleWeapon::BeginPlay()
{
    Super::BeginPlay();

    check(FXComponent);
}

void ASTURifleWeapon::InitMuzzleFX()
{
    if (!MuzzleFXComponent)
    {
        MuzzleFXComponent = SpawnMuzzleFX();
    }
    SetMuzzleFXVisibility(true);
}

void ASTURifleWeapon::SetMuzzleFXVisibility(bool Visibility)
{
    if (MuzzleFXComponent)
    {
        MuzzleFXComponent->SetPaused(!Visibility);
        MuzzleFXComponent->SetVisibility(Visibility, true);
    }
}

void ASTURifleWeapon::SpawnTraceFX(const FVector &StartTrace, const FVector &EndTrace)
{
    const auto TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), //
                                                                                 TraceFX,    //
                                                                                 StartTrace);

    if (!TraceFXComponent) return;

    TraceFXComponent->SetPaused(true);
    TraceFXComponent->SetVectorParameter(this->TraceTargetName, EndTrace);
    TraceFXComponent->SetPaused(false);
}
