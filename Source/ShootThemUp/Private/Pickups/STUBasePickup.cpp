// Shoot Them Up Game. All rights are reserved.


#include "Pickups/STUBasePickup.h"
#include <Components/SphereComponent.h>

DEFINE_LOG_CATEGORY_STATIC(LogBasePickup, All, All);

// Sets default values
ASTUBasePickup::ASTUBasePickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
    CollisionComponent->InitSphereRadius(50.0f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    SetRootComponent(CollisionComponent);

    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
    StaticMesh->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ASTUBasePickup::BeginPlay()
{
	Super::BeginPlay();
	
    check(CollisionComponent);
    check(StaticMesh);
}

// Called every frame
void ASTUBasePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    AddActorLocalRotation(FRotator(0.0f, RotationYaw, 0.0f));

    GenerateRotationYaw();
}

void ASTUBasePickup::NotifyActorBeginOverlap(AActor *OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    const auto Pawn = Cast<APawn>(OtherActor);

    if (GivePickupTo(Pawn))
    {
        PickupWasTaken();
    }
}

bool ASTUBasePickup::GivePickupTo(APawn *PlayerPawn)
{
    return false;
}

void ASTUBasePickup::PickupWasTaken()
{
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    if (GetRootComponent())
    {
        GetRootComponent()->SetVisibility(false, true);
    }

    FTimerHandle RespawnTimer;

    GetWorldTimerManager().SetTimer(RespawnTimer, this, &ASTUBasePickup::Respawn, RespawnTime);
}

void ASTUBasePickup::Respawn()
{
    GenerateRotationYaw();
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    if (GetRootComponent())
        GetRootComponent()->SetVisibility(true, true);
}

void ASTUBasePickup::GenerateRotationYaw()
{
    RotationYaw = FMath::RandRange(1.0f, 2.0f) * Direction;
}

