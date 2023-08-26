// Shoot Them Up Game. All rights are reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUBaseWeapon.h"
#include "STURifleWeapon.generated.h"

class USTUWeaponFXComponent;
class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class SHOOTTHEMUP_API ASTURifleWeapon : public ASTUBaseWeapon
{
    GENERATED_BODY()

    public:
    ASTURifleWeapon();

    virtual void StartFire() override;
    virtual void StopFire() override;
    virtual void OnWeaponSwitch() override;

    virtual void MakeShot() override;
    virtual FHitResult MakeLineTrace(AController *Controller = nullptr,
                                     FCollisionQueryParams params = FCollisionQueryParams()) override;
    
    UPROPERTY(VisibleAnywhere, Category = "VFX")
    USTUWeaponFXComponent* FXComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    UNiagaraSystem* TraceFX;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FName TraceTargetName = "TraceTarget";

    virtual void BeginPlay() override;
  private:
    FTimerHandle ShootTimerHandle;

    UPROPERTY()
    UNiagaraComponent* MuzzleFXComponent;
    void InitMuzzleFX();
    void SetMuzzleFXVisibility(bool Visibility);
    void SpawnTraceFX(const FVector& StartTrace, const FVector& EndTrace);
};
