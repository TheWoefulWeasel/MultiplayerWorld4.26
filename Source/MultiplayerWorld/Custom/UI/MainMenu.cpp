// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "MultiplayerWorld/Custom/UI/ServerRow.h"

UMainMenu::UMainMenu(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
    ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/BlueprintChildren/World/WBP_ServerRow")); 
    if(!ServerRowBPClass.Class) return;
    ServerRowClass = ServerRowBPClass.Class;
}

bool UMainMenu::Initialize()
{
    //bool bSuccessfulInitialization = Super::Initialize();

    if(!Super::Initialize()) return false;

    UE_LOG(LogTemp, Warning, TEXT("Binding Host Button"));
    if (!ensure(ButtonHost != nullptr)) return false;
    ButtonHost->OnClicked.AddDynamic(this, &UMainMenu::CallHostServer);  //TODO:: Maybe the binding pre calls teh fuinction?
    

    
    if (!ensure(ButtonJoin != nullptr)) return false;
    ButtonJoin->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

    if (!ensure(ButtonBack != nullptr)) return false;
    ButtonBack->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);
    
    if (!ensure(ButtonJoinIP != nullptr)) return false;
    ButtonJoinIP->OnClicked.AddDynamic(this, &UMainMenu::CallJoinServer);
    
    return true;
}

void UMainMenu::SetupServerList(TArray<FString> ServerNames)
{
    UWorld* World = this->GetWorld();
    if (!World) return;

    ServerList->ClearChildren();
    uint32 i = 0;
    for(const FString& ServerName : ServerNames)
    {
        UServerRow* Row = CreateWidget<UServerRow>(World, ServerRowClass);
        if(!Row) return;

        Row->ServerName->SetText(FText::FromString(ServerName));
        Row->Setup(this, i);
        ++i;
        ServerList->AddChild(Row);
    }
}





void UMainMenu::CallHostServer() //used by game instance
{
    if (MenuInterface != nullptr)
    {
        MenuInterface->HostAServer(); //call to the pure function in the interface
    }
}

void UMainMenu::SelectIndex(uint32 Index)
{
    SelectedIndex = Index;
}


void UMainMenu::CallJoinServer()
{
    if (MenuInterface != nullptr && SelectedIndex.IsSet())
    {
        MenuInterface->JoinAServer(SelectedIndex.GetValue()); //call to the pure function in the interface
    }
}

void UMainMenu::OpenMainMenu()
{
    if(!MenuSwitcher) return;
    if(!MainMenu) return;

    MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::OpenJoinMenu()
{
    if(!MenuSwitcher) return;
    if(!JoinMenu) return;

    MenuSwitcher->SetActiveWidget(JoinMenu);
    if(!MenuInterface) return;
    MenuInterface->RefreshServerList();
}







