// Shoot Them Up Game. All rights are reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUBasePickup.generated.h"

class USphereComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBasePickup : public AActor
{
    GENERATED_BODY()

  public:
    // Sets default values for this actor's properties
    ASTUBasePickup();

  protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Pickups")
    USphereComponent *CollisionComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Pickups")
    UStaticMeshComponent *StaticMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Pickups")
    float RespawnTime = 10.0f;

    virtual bool GivePickupTo(APawn *PlayerPawn);
  public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    virtual void NotifyActorBeginOverlap(AActor *OtherActor) override;

  private:
    void PickupWasTaken();
    void Respawn();
};
