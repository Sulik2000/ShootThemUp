// Shoot Them Up Game. All rights are reserved.

#include "Weapon/STUProjectile.h"
#include <Components/SphereComponent.h>
#include <DrawDebugHelpers.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Components/STUWeaponFXComponent.h>

ASTUProjectile::ASTUProjectile()
{
    PrimaryActorTick.bCanEverTick = false;

    this->CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
    CollisionComponent->InitSphereRadius(5.0f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    CollisionComponent->bReturnMaterialOnMove = true;
    SetRootComponent(CollisionComponent);

    MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
    MovementComponent->InitialSpeed = 2000.0f;
    MovementComponent->ProjectileGravityScale = 0.5f;

    WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent");
}

// Called when the game starts or when spawned
void ASTUProjectile::BeginPlay()
{
    Super::BeginPlay();

    check(MovementComponent);
    check(CollisionComponent);
    check(WeaponFXComponent);

    MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;
    CollisionComponent->OnComponentHit.AddDynamic(this, &ASTUProjectile::OnProjectileHit);
    CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);

    SetLifeSpan(LifeSeconds);
}

void ASTUProjectile::OnProjectileHit(UPrimitiveComponent *HitComponent, AActor *OtherActor,
                                     UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
    if (!GetWorld())
        return;

    MovementComponent->StopMovementImmediately();

    // Make radial damage
    UGameplayStatics::ApplyRadialDamage(GetWorld(),                 //
                                        Damage,                     //
                                        GetActorLocation(),         //
                                        RadiusOfDamage,             //
                                        UDamageType::StaticClass(), //
                                        {GetOwner()},               //
                                        this,                       //
                                        GetController(),            //
                                        RadialDamage                //
    );

    WeaponFXComponent->PlayImpactFX(Hit);

    //DrawDebugSphere(GetWorld(), GetActorLocation(), RadiusOfDamage, 24, FColor::Red, false, 5.0f);

    Destroy();
}

AController *ASTUProjectile::GetController() const
{
    const auto Pawn = Cast<APawn>(GetOwner());
    return Pawn ? Pawn->GetController() : nullptr;
}
