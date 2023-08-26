// Shoot Them Up Game. All rights are reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class USTUWeaponFXComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUProjectile : public AActor
{
    GENERATED_BODY()

  public:
    // Sets default values for this actor's properties
    ASTUProjectile();

  protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
    USphereComponent *CollisionComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
    UProjectileMovementComponent *MovementComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float RadiusOfDamage = 50.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float Damage = 20.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    bool RadialDamage = true;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float LifeSeconds = 5.0f;

    UPROPERTY(VisibleAnywhere, Category = "VFX")
    USTUWeaponFXComponent* WeaponFXComponent;

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnProjectileHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
                         FVector NormalImpulse, const FHitResult &Hit);
    
    AController *GetController() const;
  public:
    void SetShotDirection(const FVector &Direction)
    {
        ShotDirection = Direction;
    }

  private:
    FVector ShotDirection = FVector::ZeroVector;
};
