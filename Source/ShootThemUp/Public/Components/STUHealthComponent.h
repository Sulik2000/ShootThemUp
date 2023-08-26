// Shoot Them Up Game. All rights are reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "STUHealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeath)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float)
DECLARE_MULTICAST_DELEGATE(FOnHealthPickup)
DECLARE_MULTICAST_DELEGATE(FOnDamageTaken)

class UCameraShakeBase;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUHealthComponent : public UActorComponent
{
    GENERATED_BODY()

  public:
    // Sets default values for this component's properties
    USTUHealthComponent();

    float GetHealth() const { return Health; }

    UFUNCTION(BlueprintCallable, Category = "Health")
    bool IsDead() const;

    FOnDeath OnDeath;
    FOnHealthChanged OnHealthChanged;
    FOnDamageTaken OnDamageTaken;

    void OnHealthPickupTake();

    UFUNCTION(BlueprintCallable, Category = "Health")
    float GetHealthPercent() const;

    FOnHealthPickup OnHealthPickup;
  protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
    bool AutoHeal = true;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0.0", ClampMax = "100.0"))
    float MaxHealth = 100.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
    float HealingSpeed = 5.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
    float HealingDelay = 5.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    TSubclassOf<UCameraShakeBase> CameraShake;

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType,
                               FActorComponentTickFunction *ThisTickFunction) override;

  private:
    float Health = MaxHealth;

    void StartHealing();

    UFUNCTION()
    void OnTakeAnyDamage(AActor *DamagedActor, float Damage, const UDamageType *DamageType,
                               AController *InstigatedBy, AActor *DamageCauser);
    
    bool IsHealing = false;
    FTimerHandle HealTimer;

    void PlayCameraShake(float Scale = 1.0f);
};
