// Shoot Them Up Game. All rights are reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "STUWeaponFXComponent.generated.h"

class UNiagaraSystem;

USTRUCT(BlueprintType)
struct FDecalData
{
    GENERATED_USTRUCT_BODY()

  public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    UMaterialInterface *Material;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FVector Size = FVector(10.0f);

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    float LifeTime = 5.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    float FadeOutTime = 0.7f;
};

USTRUCT(BlueprintType)
struct FImpactData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    UNiagaraSystem *NiagaraEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FDecalData DecalData;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUWeaponFXComponent : public UActorComponent
{
    GENERATED_BODY()

  public:
    // Sets default values for this component's properties
    USTUWeaponFXComponent();

    void PlayImpactFX(const FHitResult &HitResult);

  protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FImpactData DefaultImpactData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    TMap<UPhysicalMaterial *, FImpactData> ImpactDataMap;
};
