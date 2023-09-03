// Shoot Them Up Game. All rights are reserved.

#pragma once

#include "CoreMinimal.h"
#include "Player/STUBaseCharacter.h"
#include "STUPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USphereComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUPlayerCharacter : public ASTUBaseCharacter
{
    GENERATED_BODY()

  public:
    ASTUPlayerCharacter(const FObjectInitializer &ObjInit);

  protected:
    virtual void OnDeathHandle() override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent *CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USpringArmComponent *SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USphereComponent *CameraCollisionComponent;

    // Called every frame
    virtual void Tick(float DeltaTime) override;
    virtual void BeginPlay() override;

  private:
    void MoveForward(float Amount);
    void MoveRight(float Amount);
    void StartRun();
    void StopRun();

    UFUNCTION()
    void OnCameraCollisionBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor,
                                       UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                       const FHitResult &SweepResult);

    UFUNCTION()
    void OnCameraCollisionEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor,
                                     UPrimitiveComponent *OtherComp, int32 OtherBodyIndex);

    void CheckCameraOverlapping();
};
