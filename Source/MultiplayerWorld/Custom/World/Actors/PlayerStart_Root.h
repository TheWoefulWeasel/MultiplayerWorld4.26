// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "PlayerStart_Root.generated.h"

/**
 * 
 */

//include here because unreal doesn't parse UENUM files without a USTRUCT() or UCLASS() 
UENUM()
enum EPlayer_TagType
{
	Chaser UMETA(DisplayName = "Chaser"),
    Runner UMETA(DisplayName = "Runner")
};
UCLASS()
class MULTIPLAYERWORLD_API APlayerStart_Root : public APlayerStart
{
	GENERATED_BODY()

	
	public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
	TEnumAsByte<EPlayer_TagType> CustomPlayerTag;
};
