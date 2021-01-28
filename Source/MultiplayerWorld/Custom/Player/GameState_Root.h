// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Chaos/AABBTree.h"
#include "GameFramework/GameState.h"
#include "MultiplayerWorld/Custom/Player/UserInteractable/PlayerController_Root.h"

#include "GameState_Root.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERWORLD_API AGameState_Root : public AGameState
{
	GENERATED_BODY()

	

	
	//If server-->decrement timer (maybe use a time manager)
	//multicastRPC to send out current timer value to clients


	//List of connected players
	//TArray<APlayerController_Root>(CurrentPlayers);
	//TArray<APlayerController_Root*>(ChaserPlayers);
	//List of chasers

	
	protected:

	

	public:

	void SetTimer(const float& GmTimer);
	void SetTimer();

	//GameState Timer
	UPROPERTY(ReplicatedUsing= OnRep_UpdateClientTimer)
	float CurrentTimerValue;

	UFUNCTION()
    virtual void OnRep_UpdateClientTimer();
	
	/*UFUNCTION(NetMulticast, Reliable)
	void StartRoundTimer();*/

	
	//TArray<APlayerController_Root*> GetChaserList();

	//UFUNCTION(NetMulticast, Reliable)
	bool ChooseInitialChaser();
	void UpdateChaser(int ChaserNum);
	void UpdateAllPlayerText();
	void UpdateImmunity(bool State);
};
