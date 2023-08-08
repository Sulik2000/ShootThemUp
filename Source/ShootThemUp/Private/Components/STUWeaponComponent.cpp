// Shoot Them Up Game. All rights are reserved.

#include "Components/STUWeaponComponent.h"
#include <Animations/STUEquipFinishedAnimNotify.h>
#include <Animations/STUReloadFinishedAnimNotify.h>
#include <GameFramework/Character.h>
#include <Player/STUBaseCharacter.h>
#include <Weapon/STUBaseWeapon.h>
#define WEAPON_NUM 2

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All);

// Sets default values for this component's properties
USTUWeaponComponent::USTUWeaponComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these
    // features off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = false;

    // ...
}

void USTUWeaponComponent::StartFire()
{
    if (!CurrentWeapon || !CanFire())
        return;

    CurrentWeapon->StartFire();
}

void USTUWeaponComponent::StopFire()
{
    if (!CurrentWeapon)
        return;

    CurrentWeapon->StopFire();
}

// Called when the game starts
void USTUWeaponComponent::BeginPlay()
{
    Super::BeginPlay();
    InitAnimations();
    SpawnWeapons();
    checkf(Weapons.Num() == WEAPON_NUM, TEXT("Our character can hold only %i weapons"), WEAPON_NUM);
}

void USTUWeaponComponent::EndPlay(EEndPlayReason::Type EndPlayReason)
{
    for (const auto Weapon : Weapons)
    {
        Weapon->Destroy();
    }
}

void USTUWeaponComponent::SwitchWeapon()
{
    if (!CanEquip())
        return;
    canFire = false;
    canEquip = false;
    OnWeaponSwitch.Broadcast();
    ACharacter *Character = Cast<ACharacter>(GetOwner());
    if (!Character)
        return;

    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);

    if (CurrentWeaponIndex == 0)
    {
        PlayAnimMontage(EquipAnimMontage);
        CurrentWeapon->AttachToComponent(Character->GetMesh(), AttachmentRules, SecondWeaponAttachPointName);

        CurrentWeapon = Weapons[1];
        CurrentWeaponIndex = 1;

        CurrentWeapon->AttachToComponent(Character->GetMesh(), AttachmentRules, WeaponAttachPointName);
    }

    else if (CurrentWeaponIndex == 1)
    {
        PlayAnimMontage(EquipAnimMontage);
        CurrentWeapon->AttachToComponent(Character->GetMesh(), AttachmentRules, SecondWeaponAttachPointName);

        CurrentWeapon = Weapons[0];
        CurrentWeaponIndex = 0;

        CurrentWeapon->AttachToComponent(Character->GetMesh(), AttachmentRules, WeaponAttachPointName);
    }
}

bool USTUWeaponComponent::CanFire() const
{
    return canFire;
}

bool USTUWeaponComponent::CanEquip() const
{
    return canEquip;
}

bool USTUWeaponComponent::GetWeaponUIData(FWeaponUIData &WeaponData) const
{
    if (CurrentWeapon)
    {
        WeaponData = CurrentWeapon->GetWeaponUIData();
        return true;
    }
    return false;
}

bool USTUWeaponComponent::GetWeaponAmmoData(FAmmoData &AmmoData) const
{
    if (!CurrentWeapon) return false;

    AmmoData = CurrentWeapon->GetCurrentAmmo();
    return true;
}

void USTUWeaponComponent::ReloadWeapon()
{
    CurrentWeapon->ReloadAmmo();
}

void USTUWeaponComponent::SpawnWeapons()
{
    if (!GetWorld())
        return;

    ACharacter *Character = Cast<ACharacter>(GetOwner());
    if (!Character)
        return;

    if (WeaponClasses.Num() >= 1)
    {
        Weapons.Add(GetWorld()->SpawnActor<ASTUBaseWeapon>(WeaponClasses[0].Get()));
        CurrentWeapon = Weapons[0];

        if (!CurrentWeapon)
            return;

        OnWeaponSwitch.AddDynamic(CurrentWeapon, &ASTUBaseWeapon::OnWeaponSwitch);
        Weapons[0]->OnReloadStarted.AddUObject(this, &USTUWeaponComponent::OnReloadStarted);
        OnReloadFinished.AddUObject(Weapons[0], &ASTUBaseWeapon::OnReloadFinished);
    }

    if (WeaponClasses.Num() >= 2)
    {
        Weapons.Add(GetWorld()->SpawnActor<ASTUBaseWeapon>(WeaponClasses[1].Get()));

        if (Weapons.Num() < 2 || !Weapons[1])
            return;

        OnWeaponSwitch.AddDynamic(Weapons[1], &ASTUBaseWeapon::OnWeaponSwitch);
        Weapons[1]->OnReloadStarted.AddUObject(this, &USTUWeaponComponent::OnReloadStarted);
        OnReloadFinished.AddUObject(Weapons[1], &ASTUBaseWeapon::OnReloadFinished);
    }

    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);

    CurrentWeapon->AttachToComponent(Character->GetMesh(), AttachmentRules, WeaponAttachPointName);
    CurrentWeapon->SetOwner(Character);

    Weapons[1]->AttachToComponent(Character->GetMesh(), AttachmentRules, SecondWeaponAttachPointName);
    Weapons[1]->SetOwner(Character);
}

void USTUWeaponComponent::PlayAnimMontage(UAnimMontage *AnimMontage)
{
    ACharacter *Character = Cast<ACharacter>(GetOwner());
    if (!Character)
        return;

    Character->PlayAnimMontage(AnimMontage);
}

void USTUWeaponComponent::InitAnimations()
{
    if (!EquipAnimMontage)
        return;

    auto NotifyEvents = EquipAnimMontage->Notifies;
    USTUEquipFinishedAnimNotify *EquipFinishedNotify = nullptr;
    for (auto Notify : NotifyEvents)
    {
        EquipFinishedNotify = Cast<USTUEquipFinishedAnimNotify>(Notify.Notify);
        if (EquipFinishedNotify)
        {
            EquipFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);
            break;
        }
    }
    if (!EquipFinishedNotify)
    {
        UE_LOG(LogWeaponComponent, Error, TEXT("Anim notify is forgotten to set!"));
        checkNoEntry();
    }

    if (!ReloadAnimMontage)
        return;
    NotifyEvents = ReloadAnimMontage->Notifies;

    USTUReloadFinishedAnimNotify *ReloadFinishedNotify = nullptr;

    for (auto Notify : NotifyEvents)
    {
        ReloadFinishedNotify = Cast<USTUReloadFinishedAnimNotify>(Notify.Notify);
        if (ReloadFinishedNotify)
        {
            ReloadFinishedNotify->OnRealoadFinished.AddUObject(this, &USTUWeaponComponent::ReloadFinished);
            break;
        }
    }

    if (!ReloadFinishedNotify)
    {
        UE_LOG(LogWeaponComponent, Error, TEXT("Anim notify is forgotten to set!"));
        checkNoEntry();
    }
}

void USTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent *MeshComp)
{
    ACharacter *Character = Cast<ACharacter>(GetOwner());
    if (!Character)
        return;

    if (Character->GetMesh() != MeshComp)
        return;

    UE_LOG(LogWeaponComponent, Display, TEXT("Equip finished"));
    canFire = true;
    canEquip = true;
}

void USTUWeaponComponent::OnReloadStarted()
{
    CurrentWeapon->StopFire();
    canFire = false;
    PlayAnimMontage(ReloadAnimMontage);
}

void USTUWeaponComponent::ReloadFinished()
{
    OnReloadFinished.Broadcast(Weapons[CurrentWeaponIndex]);
    canFire = true;
}
