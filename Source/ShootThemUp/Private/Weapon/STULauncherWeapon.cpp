// Shoot Them Up Game. All rights are reserved.


#include "Weapon/STULauncherWeapon.h"
#include <Weapon/STUProjectile.h>
#include <Kismet/GameplayStatics.h>

DEFINE_LOG_CATEGORY_STATIC(LogLauncherWeapon, All, All)

void ASTULauncherWeapon::StartFire()
{
    MakeShot();
}

void ASTULauncherWeapon::MakeShot()
{
    if (IsAmmoEmpty()) return;
    const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleSocketTransform().GetLocation());
    ASTUProjectile* Projectile = GetWorld()->SpawnActorDeferred<ASTUProjectile>(ProjectileClass, SpawnTransform);

    FCollisionQueryParams params;
    params.AddIgnoredActor(GetOwner());
    FHitResult HitResult = MakeLineTrace(GetPlayerController(), params);
    FVector Location;
    FRotator Rotation;
    GetPlayerController()->GetPlayerViewPoint(Location, Rotation);

    // Set projectile params
    const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : GetActorLocation() + Rotation.Vector() * TraceMaxDistance;
    const FVector Direction = (EndPoint - GetMuzzleSocketTransform().GetLocation()).GetSafeNormal();

    UE_LOG(LogLauncherWeapon, Display, TEXT("%s"), *Direction.ToString());

    if (Projectile)
    {
        Projectile->SetShotDirection(Direction);
        Projectile->SetOwner(GetOwner());
        Projectile->FinishSpawning(SpawnTransform);
    }
    DecreaseAmmo();
}
