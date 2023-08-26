// Shoot Them Up Game. All rights are reserved.

#include "Weapon/Components/STUWeaponFXComponent.h"
#include <Components/DecalComponent.h>
#include <Kismet/GameplayStatics.h>
#include <NiagaraFunctionLibrary.h>
#include <PhysicalMaterials/PhysicalMaterial.h>

// Sets default values for this component's properties
USTUWeaponFXComponent::USTUWeaponFXComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these
    // features off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    // ...
}

void USTUWeaponFXComponent::PlayImpactFX(const FHitResult &HitResult)
{
    auto ImpactData = &DefaultImpactData;

    if (HitResult.PhysMaterial.IsValid())
    {
        const auto PhysMat = HitResult.PhysMaterial.Get();
        if (ImpactDataMap.Find(PhysMat))
        {
            ImpactData = ImpactDataMap.Find(PhysMat);
        }
    }

    // Niagara
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),                         //
                                                   ImpactData->NiagaraEffect,           //
                                                   HitResult.Location,                 //
                                                   HitResult.ImpactNormal.Rotation()); //

    // Decal
    auto DecalComponent = UGameplayStatics::SpawnDecalAtLocation(GetWorld(),                         //
                                                                 ImpactData->DecalData.Material,      //
                                                                 ImpactData->DecalData.Size,          //
                                                                 HitResult.ImpactPoint,              //
                                                                 HitResult.ImpactNormal.Rotation()); //

    if (DecalComponent)
    {
        DecalComponent->SetFadeOut(ImpactData->DecalData.LifeTime, //
                                   ImpactData->DecalData.FadeOutTime);
    }
}
