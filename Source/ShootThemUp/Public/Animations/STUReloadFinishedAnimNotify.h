// Shoot Them Up Game. All rights are reserved.

#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "CoreMinimal.h"
#include "STUReloadFinishedAnimNotify.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnReloadFinished);

UCLASS()
class SHOOTTHEMUP_API USTUReloadFinishedAnimNotify : public UAnimNotify
{
    GENERATED_BODY()
  public:
    virtual void Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation) override;
    FOnReloadFinished OnRealoadFinished;
};
