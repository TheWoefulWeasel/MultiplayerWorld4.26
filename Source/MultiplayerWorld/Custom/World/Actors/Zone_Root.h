// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerStart.h"

#include "Zone_Root.generated.h"

UCLASS()
class MULTIPLAYERWORLD_API AZone_Root : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ForceFieldWallMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* SafeAreaComponent;

	UFUNCTION()
	void MakeSafe(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	/*UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	uint8 NumberOfSpawnLocations = 2;*/
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	TArray<APlayerStart*> SpawnLocations;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	bool ZoneIsActive = false;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Sets default values for this actor's properties

	AZone_Root();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ToggleActiveZone();
	
	//Collision with box volume to determine safety of runner
	

};
