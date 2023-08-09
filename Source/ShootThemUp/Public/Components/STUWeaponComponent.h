// Shoot Them Up Game. All rights are reserved.

#pragma once

#include "Components/ActorComponent.h"
#include <UI/STUPlayerHUDWidget.h>
#include <Weapon/STUBaseWeapon.h>
#include "CoreMinimal.h"
#include "STUWeaponComponent.generated.h"

class ASTUBaseWeapon;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWeaponSwitch);
DECLARE_MULTICAST_DELEGATE_OneParam(FReloadFinished, ASTUBaseWeapon*);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

  public:
    // Sets default values for this component's properties
    USTUWeaponComponent();

    bool AddAmmoToWeapon(TSubclassOf<ASTUBaseWeapon> Weapon, int32 NumOfClips);

    void StartFire();
    void ReloadWeapon();
    void StopFire();

    void SwitchWeapon();

    bool CanFire() const;
    bool CanEquip() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    bool GetWeaponUIData(FWeaponUIData& WeaponData) const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    bool GetWeaponAmmoData(FAmmoData& AmmoData) const;
  protected:
    // Called when the game starts
    virtual void BeginPlay() override;
    virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    TArray<TSubclassOf<ASTUBaseWeapon>> WeaponClasses;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FName WeaponAttachPointName = "WeaponPoint";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FName SecondWeaponAttachPointName = "ArmourySocket";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    UAnimMontage* EquipAnimMontage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    UAnimMontage *ReloadAnimMontage;

    UPROPERTY(BlueprintAssignable)
    FWeaponSwitch OnWeaponSwitch;

    FReloadFinished OnReloadFinished;

    void SpawnWeapons();

  private:
    UPROPERTY()
    ASTUBaseWeapon *CurrentWeapon = nullptr;
    uint8 CurrentWeaponIndex = 0;
    TArray<ASTUBaseWeapon *> Weapons;

    void PlayAnimMontage(UAnimMontage *AnimMontage);
    void InitAnimations();
    void OnEquipFinished(USkeletalMeshComponent *MeshComp);
    void OnReloadStarted();
    void ReloadFinished();
    bool canEquip = true;
    bool canFire = true;
};
