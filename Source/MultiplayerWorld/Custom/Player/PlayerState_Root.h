// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MultiplayerWorld/Custom/World/Actors/PlayerStart_Root.h"
#include "UObject/NameTypes.h"

#include "PlayerState_Root.generated.h"

/**
 * 
 */


UCLASS()
class MULTIPLAYERWORLD_API APlayerState_Root : public APlayerState
{
	GENERATED_BODY()
	
	bool bIsImmune = false;
	
	public:
	//chaser 
	APlayerState_Root();
	
	EPlayer_TagType GetPlayerTag() const;
	void SetPlayerTag(const EPlayer_TagType& TagValue);

	//immunity
	bool GetIsImmune();
	void SetIsImmune(bool immuneValue);
	void UpdateText();
	void SetPlayerToInputGameMode();
	
	UPROPERTY(ReplicatedUsing= OnRep_PlayerTag)
	TEnumAsByte<EPlayer_TagType> Player_Tag = Runner;

	UFUNCTION()
	virtual void OnRep_PlayerTag();
	
};
