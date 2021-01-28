// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance_Root.h"


#include "GameMode_Root.h"
#include "Actors/PlayerStart_Root.h"
#include "Interfaces/OnlineSessionInterface.h" //contains the IOnlineSessionPtr functions
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "MultiplayerWorld/Custom/UI/MainMenu.h"

UGameInstance_Root::UGameInstance_Root(const FObjectInitializer& ObjectInitializer)
{
    //Retrieves the BP classes whilst in the constructor because it is called before the Init()
    const ConstructorHelpers::FClassFinder<UUserWidget> MenuBlueprint(TEXT("/Game/BlueprintChildren/World/WBP_MainMenu"));
    if (!ensure(MenuBlueprint.Class != nullptr)) return;
    MenuBPClass = MenuBlueprint.Class;

    const ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBlueprint(TEXT("/Game/BlueprintChildren/World/WBP_InGameMenu"));
    if (!ensure(InGameMenuBlueprint.Class != nullptr)) return;
    InGameMenuBPClass = InGameMenuBlueprint.Class;

    const ConstructorHelpers::FClassFinder<APlayerStart_Root> PlayerStartBlueprint(TEXT("/Game/BlueprintChildren/World/Actors/BP_PlayerStart"));
    if (!ensure(PlayerStartBlueprint.Class != nullptr)) return;
    PlayerStartBPClass = PlayerStartBlueprint.Class;

    const ConstructorHelpers::FClassFinder<UUserWidget> PlayerHUDBlueprint(TEXT("/Game/BlueprintChildren/Player/WBP_HUD"));
    if (!ensure(PlayerHUDBlueprint.Class != nullptr)) return;
    PlayerHUDBPClass = PlayerHUDBlueprint.Class;
}

void UGameInstance_Root::Init()
{
    Super::Init();
    WorldObject = GetWorld();

    //Gets the subsystem; sets the session interface of that subsystem; Binds the delegate handler functions to that interface
    IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
    if (OnlineSubsystem)
    {
        UE_LOG(LogTemp,Warning,TEXT("Found Subsystem, %s"), *OnlineSubsystem->GetSubsystemName().ToString());
        //Get Session Interface from OnlineSubsystem
        SessionPtr = OnlineSubsystem->GetSessionInterface();
        if (SessionPtr.IsValid()) //Shared Pointer.. Use more complex check of .IsValid()
        {
            
            //When Valid, bind all functions to the session
            SessionPtr->OnCreateSessionCompleteDelegates.AddUObject(this, &UGameInstance_Root::OnCreateSessionComplete);
            SessionPtr->OnDestroySessionCompleteDelegates.AddUObject(this, &UGameInstance_Root::OnDestroySessionComplete);
            SessionPtr->OnFindSessionsCompleteDelegates.AddUObject(this, &UGameInstance_Root::OnFindSessionComplete);
            SessionPtr->OnJoinSessionCompleteDelegates.AddUObject(this, &UGameInstance_Root::OnJoinSessionComplete);  
            
        }
    }
    else  UE_LOG(LogTemp,Warning,TEXT("No Subsystem"));
}

/*Creates menu widget using the UMainMenu child class which enables the menu to call the IMyInterface abstract functions*/
void UGameInstance_Root::LoadMenuWidget()
{
    UE_LOG(LogTemp, Warning, TEXT("Loading Menu"));
    if(!MenuBPClass) return;
    MainMenu = CreateWidget<UMainMenu>(this, MenuBPClass);
    
    if(!MainMenu) return;
    MainMenu->MenuSetup(true);
    MainMenu->SetMyInterface(this);
}

/*Creates a menu widget that appears over the players screen for functionality such as exiting to menu
 * The menu is created from the BP class and uses the base menu c++ class to give basic functionality of displaying the menu or setting the pointer of the interface to this game instance
 */
void UGameInstance_Root::InGameLoadMenu()
{
    if(!InGameMenuBPClass) return;
    UMenuBase* TempMenu = CreateWidget<UMenuBase>(this, InGameMenuBPClass);
    
    if(!TempMenu) return;
    TempMenu->MenuSetup(true);
    TempMenu->SetMyInterface(this);
}

/*Clears the active menu*/
void UGameInstance_Root::InGameDestroyMenu()
{
    if(MainMenu != nullptr)
    {
        MainMenu->MenuSetup(false);
    }
}
/*Travels to the MainMenu level on the client side*/
void UGameInstance_Root::LoadTheMainMenu()
{
    APlayerController* PlayerController = GetFirstLocalPlayerController();
    if (!PlayerController) return;
    PlayerController->ClientTravel("/Game/MainMenu", ETravelType::TRAVEL_Absolute);
}

/*After pressing the Join button; this function will populate a FOnlineSessionSearch by converting the pointer to a ref as well as applying session attributes such as LAN, Search type or max ping*/
void UGameInstance_Root::RefreshServerList()
{
    SearchForSession = MakeShareable(new FOnlineSessionSearch());
    if(SearchForSession.IsValid())
    {
        /*FOR NULL SUBSYSTEM TESTING */
        SearchForSession->bIsLanQuery = true;
        /*END OF NULL SUBSYSTEM TESTING*/

        /*/*FOR STEAM SUBSYSTEM TESTING #1#
        SearchForSession->bIsLanQuery = false;
        SearchForSession->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
        SearchForSession->MaxSearchResults = 100; // Only needed whilst sharing 480 app id
        /*END OF STEAM SUBSYSTEM TESTING#1#*/

        UE_LOG(LogTemp, Warning, TEXT("Start Finding Session"));
        ULocalPlayer* LocalPlayer = GetLocalPlayerByIndex(0);
        SessionPtr->FindSessions(*LocalPlayer->GetPreferredUniqueNetId().GetUniqueNetId(), SearchForSession.ToSharedRef());
        
    }
    
}

/*Called through the menu interface upon pressing the host button. Clears the active menu widget and starts the session creation*/
void UGameInstance_Root::HostAServer()
{
    if(MainMenu != nullptr)
    {
        MainMenu->MenuSetup(false);
    }
    CreateNewSession();
}

void UGameInstance_Root::JoinAServer(uint32 AddressIndex)
{
    if(!SessionPtr.IsValid()) return;
    if(!SearchForSession.IsValid()) return;
    if(MainMenu != nullptr)
    {
       
        MainMenu->MenuSetup(false);
    }
    ULocalPlayer* LocalPlayer = GetLocalPlayerByIndex(0);
    SessionPtr->JoinSession(*LocalPlayer->GetPreferredUniqueNetId().GetUniqueNetId(), GameSessionName, SearchForSession->SearchResults[AddressIndex]);
    /*APlayerController* PlayerController = GetFirstLocalPlayerController();
    if (!PlayerController) return;
    PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);*/
}

//When the server has called IOnlineSessionPtr->CreateSession() it will return this function delegate
void UGameInstance_Root::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
    if(!bWasSuccessful) return;

    if(MainMenu)
    {
        MainMenu->MenuSetup(false);
    }

    UWorld* WorldPtr = GetWorld();
    

    /*if (!WorldPtr) return;
    AGameMode_Root* GameMode_Root = Cast<AGameMode_Root>(WorldPtr->GetAuthGameMode());
    if(GameMode_Root)
    {
        //set the session interface in the game mode for access
        GameMode_Root->SetSessionInterface(SessionPtr);
    }*/
    
    
    //Now that the session has been created on the server, give the server a map to travel to..
    WorldPtr->ServerTravel("/Game/BasicMap?listen");
}


void UGameInstance_Root::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
}

void UGameInstance_Root::OnFindSessionComplete(bool bWasSuccessful)
{

    if (bWasSuccessful && SearchForSession.IsValid() && MainMenu)
    {
        UE_LOG(LogTemp, Warning, TEXT("Found Sessions: %d"), SearchForSession->SearchResults.Num());
        TArray<FString> ServerNames;
        for(const FOnlineSessionSearchResult& Result : SearchForSession->SearchResults)
        {
            UE_LOG(LogTemp, Warning, TEXT("Found Session Name: %s"), *Result.GetSessionIdStr());
            //Every session result found in the list of search results
            ServerNames.Add(Result.GetSessionIdStr());
        }
           MainMenu->SetupServerList(ServerNames);
    }
}

void UGameInstance_Root::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    if(!SessionPtr.IsValid()) return;
    FString Address;
    if(!SessionPtr->GetResolvedConnectString(SessionName, Address))
    {
        UE_LOG(LogTemp, Warning, TEXT("Couldn't get connect string"));
        return;
    }

    APlayerController* PlayerController = GetFirstLocalPlayerController();
    if (!PlayerController) return;
    PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

//Asynchronous call CreateSession() within the IOnlineSessionPtr Interface functionality.
void UGameInstance_Root::CreateNewSession()
{
    if (SessionPtr.IsValid())
    {
        TSharedPtr<FOnlineSessionSettings> SessionSettings = MakeShareable(new FOnlineSessionSettings());
        
        //This setting for testing will make the session type lan
        SessionSettings->bIsLANMatch = false;
        //This setting determines the max number of public connections allowed in the session.(advertised)
        SessionSettings->NumPublicConnections = 2;
        //This setting makes the session visible to searchers. False would result in only invites able to get into the session
        SessionSettings->bShouldAdvertise = true;
        
        //SessionSettings.bAllowJoinInProgress = true;
        
        //Steam Presence--Determines whether to use LobbySession or InternetSession (i.e. Lobby system or a matchmaking system)
        SessionSettings->bUsesPresence = true;
        SessionSettings->bAllowJoinViaPresence = true;
        ULocalPlayer* LocalPlayer = GetLocalPlayerByIndex(0);

        SessionPtr->CreateSession(*LocalPlayer->GetPreferredUniqueNetId().GetUniqueNetId(), GameSessionName, *SessionSettings); //THIS CALLS TO START THE CREATION PROCESS. WHEN COMPLETED IT WILL RETURN THE DELEGATE FUNCTION
    }
}

