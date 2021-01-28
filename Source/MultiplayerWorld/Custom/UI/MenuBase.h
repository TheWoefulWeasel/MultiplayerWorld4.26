// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "MyInterface.h"
#include "Blueprint/UserWidget.h"
#include "MenuBase.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERWORLD_API UMenuBase : public UUserWidget
{
	GENERATED_BODY()

	protected:
	
	IMyInterface* MenuInterface;
	
	public:
	void MenuSetup(bool AddOrRemove);
	//Use Setters() to inject the values through the interface
	void SetMyInterface(IMyInterface* MI);
};
