// Shoot Them Up Game. All rights are reserved.


#include "Animations/STUReloadFinishedAnimNotify.h"

void USTUReloadFinishedAnimNotify::Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation)
{
    Super::Notify(MeshComp, Animation);
    OnRealoadFinished.Broadcast();
}
