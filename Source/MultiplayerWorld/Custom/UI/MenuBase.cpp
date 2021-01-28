// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuBase.h"

void UMenuBase::MenuSetup(bool AddOrRemove) //used by player controller
{
    if(AddOrRemove) this->AddToViewport();
    else this->RemoveFromViewport();
    
    UWorld* World = GetWorld();
    if (!World) return;
    APlayerController* PlayerController = World->GetFirstPlayerController();
    if (!PlayerController) return;
    FInputModeUIOnly InputModeData;
    const FInputModeGameOnly InputModeGameOnly;
    if(AddOrRemove)
    {
        //InputModeData.SetWidgetToFocus(this->TakeWidget());
        InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        PlayerController->SetInputMode(InputModeData);
    }
    else
    {
        PlayerController->SetInputMode(InputModeGameOnly);
    }
    
    if(AddOrRemove) PlayerController->bShowMouseCursor = true;
    else PlayerController->bShowMouseCursor = false; 
}

void UMenuBase::SetMyInterface(IMyInterface* MI) //used by player controller
{
    this->MenuInterface = MI; //setting values received from the game instance
}