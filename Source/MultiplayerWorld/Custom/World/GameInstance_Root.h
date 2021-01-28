// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Actors/PlayerStart_Root.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MultiplayerWorld/Custom/UI/MyInterface.h"


#include "GameInstance_Root.generated.h"

class UMainMenu;
class IOnlineSession;
class FOnlineSessionSearch;

UCLASS()
class MULTIPLAYERWORLD_API UGameInstance_Root : public UGameInstance, public IMyInterface
{
	GENERATED_BODY()

	//Pointer to Session Interface
	IOnlineSessionPtr SessionPtr;
	UWorld* WorldObject;
	
	UPROPERTY(EditAnywhere, Category = "UMG")
	TSubclassOf<UUserWidget> MenuBPClass; //MenuClass
	UPROPERTY(EditAnywhere, Category = "UMG")
	TSubclassOf<UUserWidget> InGameMenuBPClass; //InGameMenuCLass
	UPROPERTY(EditAnywhere, Category = "UMG")
	TSubclassOf<UUserWidget> PlayerHUDBPClass;
	TSubclassOf<APlayerStart_Root> PlayerStartBPClass;
	UMainMenu* MainMenu; //Menu
	TSharedPtr<FOnlineSessionSearch> SearchForSession; //SessionSearch
	//GameSessionName = TEXT("Game Session");
	//After the OnFindSessionComplete delegate.. This (FOnlineSessionSearch) will contain the returned results and search parameters.
	//TSharedPtr<FOnlineSessionSearch> FOnlineSessionSearchSharedPtr;

	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName Name, EOnJoinSessionCompleteResult::Type Result);
	void CreateNewSession();
	
	public:

	//Constructor uses object parameters
	UGameInstance_Root(const FObjectInitializer & ObjectInitializer);
	
	virtual void Init() override;
	
	UFUNCTION()
	virtual void HostAServer() override;
	UFUNCTION()
	virtual void JoinAServer(uint32 AddressIndex) override;

	UFUNCTION(BlueprintCallable)
	void LoadMenuWidget(); //loads the main menu widget
	
	UFUNCTION(BlueprintCallable)
    void InGameLoadMenu(); // loads the in game menu widget

	UFUNCTION(BlueprintCallable)
    void InGameDestroyMenu();


	virtual void LoadTheMainMenu() override; //callback from the leave server button of in game menu

	virtual void RefreshServerList() override;
	
};
