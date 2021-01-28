// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Actors/PlayerStart_Root.h"
#include "GameFramework/GameMode.h"
#include "MultiplayerWorld/Custom/Player/UserInteractable/Characters/Character_Root.h"


#include "GameMode_Root.generated.h"

//class PlayerState_Root;
/**
 * If people disconnect if may cause unbalanced teams, i.e. 0 chasers left on a team if they dc so need to check for this on every new connection/disconnection.
 */
UCLASS()
class MULTIPLAYERWORLD_API AGameMode_Root : public AGameMode
{
	GENERATED_BODY()

	//Initial Values
	//
	
	//TSubclassOf<ACharacter_Root> BlueprintCharacterClassPtr;

	
	//Timer value
	const float InitialTimerValue = 30.f;

	//Player Types - Chaser / Runner
	TSubclassOf<ACharacter_Root>(PlayerType);

	/*Overriding function that affect the player start system to apply the correct list of player-starts using the team tags*/
	//virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	
	protected:
	UPROPERTY(EditDefaultsOnly, Category = "Time")
	float TimeBetweenRounds;

	FTimerHandle TimerHandleRoundLength;

	void UpdateAllPlayerText();

	void SendTimerToGameState();
	
	public:

	AGameMode_Root();

	//Prepare the round
	void RoundPrepare();
	//Start the round
	void RoundStart();
	//Inorder to start the round, it must know where to set the active end-zone, i.e. the opposite to the one the players are currently in
	void ChooseActiveZone();

	virtual void StartPlay() override;

	//Called when timer ends
	//gather data from each player state that determines whether the SafeMarker is active.
	//Every player that isn't marked safe will be changed to chaser
	void RoundEnd();

	//Timer needs to be a decrementing value on the server that is replicated to players so that they can pass the value to their HUD widget.
	//ServerTick which is handled in the GameState 
	
	//Respawning players at the player starts
	//void RestartPlayerAtPlayerStart(AController* NewPlayer, AActor* StartSpot) override;
	AActor* CustomFindPlayerStart(AController* Player, const EPlayer_TagType& TagType);
	virtual void RestartPlayer(AController* NewPlayer) override;
	
	/// Session Connection Content
	/// Add to number of players and when equals maxPlayers, start the session + round
	virtual void PostLogin(APlayerController* NewPlayer) override;

	bool AssignChaser();
};
