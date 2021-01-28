// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMenu.h"

#include "Components/Button.h"

bool UInGameMenu::Initialize()
{
    if(!Super::Initialize()) return false;

    if (!ensure(ButtonLeaveServer != nullptr)) return false;
    ButtonLeaveServer->OnClicked.AddDynamic(this, &UInGameMenu::LeaveServer);

    if (!ensure(ButtonResume != nullptr)) return false;
    ButtonResume->OnClicked.AddDynamic(this, &UInGameMenu::RemoveMenu);
    

    return true;
}

void UInGameMenu::LeaveServer()
{
    if (!MenuInterface) return;
    MenuSetup(false);
    MenuInterface->LoadTheMainMenu();
    
}

void UInGameMenu::RemoveMenu()
{
    MenuSetup(false);
}
