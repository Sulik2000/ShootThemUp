// Shoot Them Up Game. All rights are reserved.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Character.h"
#include "STUBaseCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USTUHealthComponent;
class UTextRenderComponent;
class USTUWeaponComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseCharacter : public ACharacter
{
    GENERATED_BODY()

  public:
    // Sets default values for this character's properties
    ASTUBaseCharacter(const FObjectInitializer &ObjInit);

  protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent *CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USpringArmComponent *SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USTUHealthComponent *HealthComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USTUWeaponComponent *WeaponComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage *DeathAnimMontage;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    bool IsRunning = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float RunAccceleration = 300.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
    FVector2D LandedDamage = FVector2D(10.0f, 100.0f);

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
    FVector2D LandedDamageVelocity = FVector2D(900.0f, 1200.0f);

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    float GetMovementDirection() const;

    virtual void OnDeathHandle();
    UFUNCTION()
    void OnGroundLanded(const FHitResult &Hit);

  public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

  private:
    void MoveForward(float Amount);
    void MoveRight(float Amount);
    void StartRun();
    void StopRun();

    
};
