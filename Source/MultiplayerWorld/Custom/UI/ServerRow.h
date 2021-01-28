// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"


#include "ServerRow.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERWORLD_API UServerRow : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	class UButton* ButtonSelectServer;

	UPROPERTY() //helps with garbage collection
	class UMainMenu* Parent;

	uint32 Index;

	UFUNCTION()
	void OnClicked();
	
	public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ServerName;
	

	void Setup(class UMainMenu* Parent, uint32 Index);
	
};
