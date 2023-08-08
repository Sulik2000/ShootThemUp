// Shoot Them Up Game. All rights are reserved.

#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "CoreMinimal.h"
#include "STUEquipFinishedAnimNotify.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnNotifySignature, USkeletalMeshComponent*);

/**
 *
 */
UCLASS()
class SHOOTTHEMUP_API USTUEquipFinishedAnimNotify : public UAnimNotify
{
    GENERATED_BODY()
  public:
    virtual void Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation) override;

    FOnNotifySignature OnNotified;
};
