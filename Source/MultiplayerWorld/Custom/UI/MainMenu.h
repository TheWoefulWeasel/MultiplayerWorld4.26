// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "MenuBase.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ObjectMacros.h"
#include "Components/EditableTextBox.h"
#include "Components/WidgetSwitcher.h"


#include "MainMenu.generated.h"

/**
 * Creating a link between this class and the Game Instance..
 * This class will get a class member pointer of type IMyInterface to be later set a proper value using the setter functions
 * It will then Bind the Buttons from the blueprint child class by using UButton pointers with the same name as the buttons in the blueprint widget
 * After that you need a place to dynamically bind the buttons we just obtained to actual C++ functions. The Constructor is too early in the compiler for this operation and there is no innit()
 * 		so you must look for a parent function (inside UUserWidget or even higher in the hierarchy) that performs similar functionality for initialization..
 * 			The function for this case is UUserWidget::Initialize(). To access this function you need to:
 * 				#declare the same named function, but tell the compiler that it is a virtual function that is looking to override a parent function
 * 				#Implement that function and handle any Super:: return outputs.. (This one returns a bool whether it was successful) therefore we will check if it returns a success before we continue **appending** our
 * 					extra code functionality to it.
 * The bound function will be linked to the Game Instance (which is where we want to actually execute any world level functionality like hosting or joining servers)
 * 		This link will be by calling the pure function inside the interface
 * To 
 */

class UButton;
UCLASS()
class MULTIPLAYERWORLD_API UMainMenu : public UMenuBase
{
	GENERATED_BODY()

	
	TSubclassOf<UUserWidget> ServerRowClass;
	TOptional<uint32> SelectedIndex;

	//Main Screen Buttons
	UPROPERTY(meta = (BindWidget))
	UButton* ButtonHost;
	UPROPERTY(meta = (BindWidget))
	UButton* ButtonJoin;
	//Join Server Sub-Screen Buttons
	UPROPERTY(meta = (BindWidget))
	UButton* ButtonBack;
	UPROPERTY(meta = (BindWidget))
	UButton* ButtonJoinIP;
	//Other
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* MenuSwitcher;
	UPROPERTY(meta = (BindWidget))
	UWidget* MainMenu;
	UPROPERTY(meta = (BindWidget))
	UWidget* JoinMenu;
	UPROPERTY(meta = (BindWidget))
	UPanelWidget* ServerList;
	

	APlayerController* MyPlayerController;

	protected:
	//need to get these buttons but the constructors is too soon in the compile to get buttons. Therefore use a UUserWidget function Initialize()
	virtual bool Initialize() override;

	
	
	
	UFUNCTION()
	virtual void CallHostServer();
	UFUNCTION()
	virtual void CallJoinServer();
	UFUNCTION()
	void OpenMainMenu();
	UFUNCTION()
	void OpenJoinMenu();

	public:
	UMainMenu(const FObjectInitializer & ObjectInitializer);
	
	void SetupServerList(TArray<FString> ServerNames);
	void SelectIndex(uint32 Index);
	
};
