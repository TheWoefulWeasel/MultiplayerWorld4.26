// Fill out your copyright notice in the Description page of Project Settings.


#include "Zone_Root.h"


/*
#include "AudioDevice.h"
#include "InputBehavior.h"
#include "Engine/EngineTypes.h"*/
#include "MultiplayerWorld/Custom/Player/PlayerState_Root.h"

// Sets default values
AZone_Root::AZone_Root()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Static Mesh"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	ForceFieldWallMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Force Wall"));
	ForceFieldWallMeshComponent->SetupAttachment(StaticMeshComponent);
	ForceFieldWallMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SafeAreaComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Safe Zone"));
	SafeAreaComponent->SetupAttachment(StaticMeshComponent);
	SafeAreaComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SafeAreaComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SafeAreaComponent->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
	SafeAreaComponent->OnComponentBeginOverlap.AddDynamic(this, &AZone_Root::MakeSafe);
	
	/*for(auto x = 0; x < NumberOfSpawnLocations; x++)
	{
		
		APlayerStart PlayerStart = GetWorld()->SpawnActor<APlayerStart>(APlayerStart::StaticClass());
		SpawnLocations.Emplace(PlayerStart);
	}*/
	
	
}

void AZone_Root::MakeSafe(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if(ZoneIsActive)
	{
		//player is in the zone.. enable a safe variable to make the player as in the active endzone for calculation when the round timer ends
		UE_LOG(LogTemp, Warning, TEXT("Name of Overlapping Actor is %s"), *OtherActor->GetName());
		
		APlayerController* PlayerController = Cast<APlayerController>(OtherActor);
		if(!PlayerController) return;

		PlayerController->GetPlayerState<APlayerState_Root>()->SetIsImmune(true);
	}
	
}

void AZone_Root::ToggleActiveZone()
{
	ZoneIsActive = !ZoneIsActive;
}

// Called when the game starts or when spawned
void AZone_Root::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AZone_Root::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

