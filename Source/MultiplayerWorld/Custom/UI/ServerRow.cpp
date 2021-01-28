// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerRow.h"

#include "MultiplayerWorld/Custom/UI/MainMenu.h"
#include "Components/Button.h"

void UServerRow::OnClicked()
{
    Parent->SelectIndex(Index);
}

void UServerRow::Setup(UMainMenu* InParent, uint32 InIndex)
{
    Parent = InParent;
    Index = InIndex;
    ButtonSelectServer->OnClicked.AddDynamic(this, &UServerRow::OnClicked);

    
}
