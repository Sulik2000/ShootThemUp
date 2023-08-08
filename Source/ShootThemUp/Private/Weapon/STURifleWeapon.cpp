// Shoot Them Up Game. All rights are reserved.

#include "Weapon/STURifleWeapon.h"
#include <DrawDebugHelpers.h>

void ASTURifleWeapon::StartFire()
{
    MakeShot();
    GetWorldTimerManager().SetTimer(ShootTimerHandle, this, &ASTURifleWeapon::MakeShot, ShootCooldown, true);
}

void ASTURifleWeapon::StopFire()
{
    GetWorldTimerManager().ClearTimer(ShootTimerHandle);
}

void ASTURifleWeapon::OnWeaponSwitch()
{
    GetWorldTimerManager().ClearTimer(ShootTimerHandle);
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
        DrawDebugLine(GetWorld(), WeaponMeshComponent->GetSocketTransform(MuzzleSocketName).GetLocation(),
                      HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);
        DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);
    }

    DecreaseAmmo();
}

FHitResult ASTURifleWeapon::MakeLineTrace(AController *Controller, FCollisionQueryParams params)
{
    if (!Controller)
        return FHitResult();
    FVector ViewLocation;
    FRotator ViewRotation;
    Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);

    const FTransform SocketTransform = WeaponMeshComponent->GetSocketTransform(MuzzleSocketName);
    const FVector StartTrace = ViewLocation;
    const FVector ShootDirection = ViewRotation.Vector();
    const FVector TraceEnd = StartTrace + ShootDirection * TraceMaxDistance;

    FHitResult HitResult;
    GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, TraceEnd, ECollisionChannel::ECC_Visibility, params);

    return HitResult;
}