// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "MenuBase.h"
#include "InGameMenu.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERWORLD_API UInGameMenu : public UMenuBase
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	class UButton* ButtonLeaveServer;

	UPROPERTY(meta = (BindWidget))
	class UButton* ButtonResume;

	protected:
	virtual bool Initialize() override;

	UFUNCTION()
	void LeaveServer();

	UFUNCTION()
    void RemoveMenu();
	
	
};
