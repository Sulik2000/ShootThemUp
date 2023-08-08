// Shoot Them Up Game. All rights are reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <DrawDebugHelpers.h>
#include "STUDevDamageActor.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTUDevDamageActor : public AActor
{
    GENERATED_BODY()

  public:
    // Sets default values for this actor's properties
    ASTUDevDamageActor();

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USceneComponent *SceneComponent;

    UPROPERTY(EditAnywhere, Category = "Design")
    FColor Color = FColor::Red;

    UPROPERTY(EditAnywhere, Category = "Design", meta = (ClampMin = "0"))
    float Radius = 300.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
    float Damage = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
    bool DoFullDamage = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
    TSubclassOf<UDamageType> DamageType;

  protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

  public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
};
