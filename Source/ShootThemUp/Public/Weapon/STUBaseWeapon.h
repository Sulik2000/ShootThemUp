// Shoot Them Up Game. All rights are reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <STUPlayerHUDWidget.h>
#include "STUBaseWeapon.generated.h"

DECLARE_MULTICAST_DELEGATE(FReloadStarted);

USTRUCT(BlueprintType)
struct FAmmoData
{
    GENERATED_USTRUCT_BODY()
  public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    int32 Bullets;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (EditCondition = "!Infinite"))
    int32 Clips;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    bool Infinite;
};

UCLASS()
class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor
{
    GENERATED_BODY()

  public:
    // Sets default values for this actor's properties
    ASTUBaseWeapon();

    virtual void StartFire();
    virtual void StopFire();

    UFUNCTION()
    virtual void OnWeaponSwitch();

    FTransform GetMuzzleSocketTransform();
    FReloadStarted OnReloadStarted;

    void OnReloadFinished(ASTUBaseWeapon* Weapon);

    FWeaponUIData GetWeaponUIData() const;

    FAmmoData GetCurrentAmmo() const;

    virtual void ReloadAmmo();

    virtual void AddAmmo(int32 NumOfClips);
  protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FWeaponUIData UIData;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float ShootCooldown = 0.3f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float WeaponRecoil = 1.5f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent *WeaponMeshComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FName MuzzleSocketName = "MuzzleSocket";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FAmmoData DefaultAmmo;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float TraceMaxDistance = 1500.0f;

    AController *GetPlayerController();

    virtual FHitResult MakeLineTrace(AController *Controller = nullptr,
                                     FCollisionQueryParams params = FCollisionQueryParams());

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float Damage = 10.0f;

    void DecreaseAmmo();
    bool IsAmmoEmpty() const;
    bool IsClipEmpty() const;
    void ChangeClip();
    void LogAmmo();

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    virtual void MakeShot();

  private:
    FTimerHandle ShootTimerHandle;
    FAmmoData CurrentAmmo;
};
