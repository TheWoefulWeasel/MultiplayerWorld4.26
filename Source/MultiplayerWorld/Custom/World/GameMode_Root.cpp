// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMode_Root.h"



#include "EngineUtils.h"
#include "OnlineSubsystem.h"
#include "MultiplayerWorld/Custom/Player/GameState_Root.h"
#include "MultiplayerWorld/Custom/Player/PlayerState_Root.h"
#include "OnlineSessionSettings.h"
#include "ToolBuilderUtil.h"
#include "Actors/Zone_Root.h"
#include "GameFramework/GameSession.h"
#include "GameFramework/PlayerStart.h"
#include "Interfaces/OnlineSessionInterface.h"

class APlayerStartPIE;

void AGameMode_Root::RestartPlayer(AController* NewPlayer) //if the controller is not a player controller then it will not do anything
{
    UE_LOG(LogGameMode, Warning, TEXT("Restarting Player"));
    
    if (NewPlayer == nullptr || NewPlayer->IsPendingKillPending())
    {
        return;
    }

    //Go into playercontroller->playerstate and get which team they are on
    APlayerController_Root* PC = Cast<APlayerController_Root>(NewPlayer);
    if(PC)
    {
        AActor* StartSpot = nullptr;
        //use team type(bool) and pass into find player start to specify the player starts that are tagged with that team
        APlayerState_Root* PlayerState_Root = PC->GetPlayerState<APlayerState_Root>();
        if (PlayerState_Root) //TODO::Might need to null check here?
        {
            StartSpot = CustomFindPlayerStart(NewPlayer, PlayerState_Root->GetPlayerTag());
        }
       

        // If a start spot wasn't found,
        if (StartSpot == nullptr)
        {
            // Check for a previously assigned spot
            if (NewPlayer->StartSpot != nullptr)
            {
                StartSpot = NewPlayer->StartSpot.Get();
                UE_LOG(LogGameMode, Warning, TEXT("RestartPlayer: Player start not found, using last start spot"));
            }	
        }

        RestartPlayerAtPlayerStart(NewPlayer, StartSpot); //TODO:: This may potentially return null?
    }
}

AGameMode_Root::AGameMode_Root() : AGameMode()
{
    PlayerControllerClass = APlayerController_Root::StaticClass();
    GameStateClass = AGameState_Root::StaticClass();
    PlayerStateClass = APlayerState_Root::StaticClass();
    //bp class finder
    static ConstructorHelpers::FClassFinder<ACharacter_Root>BP_Character(TEXT("/Game/BlueprintChildren/Player/BP_Character")); //even though you set the defaults in Maps and Modes, you must still assign defaults in code constructors
    if(BP_Character.Class != NULL)
    {
        DefaultPawnClass = BP_Character.Class;
    }

    //Initialize private/protected variables
    TimeBetweenRounds = 3.f;
    bDelayedStart = true;
}
//tick? called from something that tracks the client list(game instance?)
/*
 *after the game mode has registered that the max number of players are in, you need to set up the teams so the program will know where to spawn each player
 *
 *if(NumPlayers == GameSession->MaxPlayers) BuildTeams()
 *
 *TeamBuildingFunction() --Randomize number from 0 - numPlayers, that index of player becomes the chaser
 *Teams are not constant, therefore they must live in the game state so that they can be updated when a player is caught and changed to the chasers
 *GS->Team1.Add(playercontroller)
 *
 *Call HandleStartNewPlayer on every player which should hopefully use the overriden RestartPlayer() to choose a random PlayerStart actor that is correctly tagged
 *PROBLEMS?
 *    -if the player controller already has a pawn spawned/possesed.. How will RestartPlayer() react when it tries to spawn another one at new location
 *Once completed, you want to set the match state to inprogress, and start the match/round [RoundStart]
 *
*/
void AGameMode_Root::RoundPrepare()
{
    //Reset chasers
    AGameState_Root* GameState_Root = GetGameState<AGameState_Root>();
    if (!GameState_Root) return;

    //reset the timer for the round
    GameState_Root->SetTimer(InitialTimerValue);


    //Swap the active SafeZone
    UWorld* World = GetWorld();
    for (TActorIterator<AZone_Root> It(World); It; ++It)
    {
        It->ToggleActiveZone();
    }

    //Make every runner not immune
    GameState_Root->UpdateImmunity(false);

    //Set timer for the next round with a starting delay
    FTimerHandle TimerHandleBetweenRounds;
    GetWorldTimerManager().SetTimer(TimerHandleBetweenRounds, this, &AGameMode_Root::RoundStart, TimeBetweenRounds, false);
    
    //Round has Started
}
void AGameMode_Root::RoundStart()
{
    //Allow Runners through the ForcefieldWall
    
    //looping timer every 1 second that tells the game state to decrement a timer value and push to clients
    GetWorldTimerManager().SetTimer(TimerHandleRoundLength, this, &AGameMode_Root::SendTimerToGameState, 1.f, true, 0.f);
}

/*
AActor* AGameMode_Root::ChoosePlayerStart_Implementation(AController* Player)
{
    APlayerStart* FoundPlayerStart = nullptr;
    UClass* PawnClass = GetDefaultPawnClassForController(Player);
    APawn* PawnToFit = PawnClass ? PawnClass->GetDefaultObject<APawn>() : nullptr;
    TArray<APlayerStart*> UnOccupiedStartPoints;
    TArray<APlayerStart*> OccupiedStartPoints;
    UWorld* World = GetWorld();
    //loop that goes through every playerstart in the world
    for (TActorIterator<APlayerStart> It(World); It; ++It)
    {
        APlayerStart* PlayerStart = *It;

        if (PlayerStart->IsA<APlayerStartPIE>())
        {
            // Always prefer the first "Play from Here" PlayerStart, if we find one while in PIE mode
            FoundPlayerStart = PlayerStart;
            break;
        }
        else
        {
            FVector ActorLocation = PlayerStart->GetActorLocation();
            const FRotator ActorRotation = PlayerStart->GetActorRotation();
            if (!World->EncroachingBlockingGeometry(PawnToFit, ActorLocation, ActorRotation))
            {
                APlayerController_Root* PlayerController_Root = Cast<APlayerController_Root>(Player);
                if(PlayerStart->PlayerStartTag == /*PlayerController->PlayerState->GetPlayerTag()#1#)
                {
                    UnOccupiedStartPoints.Add(PlayerStart);
                }
                
            }
            else if (World->FindTeleportSpot(PawnToFit, ActorLocation, ActorRotation))
            {
                OccupiedStartPoints.Add(PlayerStart);
            }
        }
    }
//this is the part to edit from the AGameModeBase in order to search via Tags
    if (FoundPlayerStart == nullptr)
    {
        if (UnOccupiedStartPoints.Num() > 0)
        {
            FoundPlayerStart = UnOccupiedStartPoints[FMath::RandRange(0, UnOccupiedStartPoints.Num() - 1)];
        }
        else if (OccupiedStartPoints.Num() > 0)
        {
            FoundPlayerStart = OccupiedStartPoints[FMath::RandRange(0, OccupiedStartPoints.Num() - 1)];
        }
    }
    return FoundPlayerStart;
}
*/

void AGameMode_Root::SendTimerToGameState()
{
    //Start the timer -- Communicate with the game state to start a fresh timer
    //AGameState_Root* GameState_Root = Cast<AGameState_Root>(GetWorld()->GetGameState());
    AGameState_Root* GameState_Root = GetGameState<AGameState_Root>();
    if (!GameState_Root) return;
    GameState_Root->SetTimer();
}
void AGameMode_Root::ChooseActiveZone()
{
    
}
void AGameMode_Root::StartPlay() //call this when max players reached
{
    UE_LOG(LogTemp,Warning,TEXT("StartPlay"));
   
    if (MatchState == MatchState::EnteringMap)
    {
        SetMatchState(MatchState::WaitingToStart);
    }

    // Check to see if we should immediately transfer to match start
    if (MatchState == MatchState::WaitingToStart && ReadyToStartMatch())
    {
        StartMatch();
    }
}
void AGameMode_Root::RoundEnd()
{

    GetWorldTimerManager().ClearTimer(TimerHandleRoundLength);
    //check player state->is immune.. anyone not immune is changed to a chaser
    
    //Call Round Prepare
    RoundPrepare();
    /*for( FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator )
    {
        APlayerController* PlayerController = Iterator->Get();
        if (PlayerController && (PlayerController->GetPawn() == nullptr) && PlayerCanRestart(PlayerController))
        {
            RestartPlayer(PlayerController);
        }
    }

    for (TActorIterator<APlayerStart> It(World); It; ++It)
    {
        APlayerStart* Start = *It;
        if (Start && Start->PlayerStartTag == IncomingPlayerStartTag)
        {
            return Start;
        }
    }*/
}

/*Overriding the AGameModeBase function to allow searching of my custom tag*/
AActor* AGameMode_Root::CustomFindPlayerStart(AController* Player, const EPlayer_TagType& TagType)
{
    UWorld* World = GetWorld();

    // If incoming start is specified, then just use it
    for (TActorIterator<APlayerStart> It(World); It; ++It)
    {
        APlayerStart_Root* Start = Cast<APlayerStart_Root>(*It);
        if (Start && Start->CustomPlayerTag == TagType)
        {
            return Start;
        }
    }

    // Always pick StartSpot at start of match
    if (ShouldSpawnAtStartSpot(Player))
    {
        if (AActor* PlayerStartSpot = Player->StartSpot.Get())
        {
            return PlayerStartSpot;
        }
        else
        {
            UE_LOG(LogGameMode, Error, TEXT("FindPlayerStart: ShouldSpawnAtStartSpot returned true but the Player StartSpot was null."));
        }
    }

    AActor* BestStart = ChoosePlayerStart(Player);
    if (BestStart == nullptr)
    {
        // No player start found
        UE_LOG(LogGameMode, Log, TEXT("FindPlayerStart: PATHS NOT DEFINED or NO PLAYERSTART with positive rating"));

        // This is a bit odd, but there was a complex chunk of code that in the end always resulted in this, so we may as well just 
        // short cut it down to this.  Basically we are saying spawn at 0,0,0 if we didn't find a proper player start
        BestStart = World->GetWorldSettings();
    }

    return BestStart;
}


//Basically prepares the round for the very first round only
/*Overriding this and not including the Super:: call because the parent calls would handle spawning the new player into the world before I wanted it to as I firstly need to assign
 *one of the players in the lobby as the first chaser in order for there to be a valid game
 * Therefore all player spawning is moved until after the max players have been reached and the chaser will be denoted with a custom PlayerTag so that they can be spawned in a different area
 */
void AGameMode_Root::PostLogin(APlayerController* NewPlayer)
{
    
    /*--------------AGameMode Stuff----------------*/
    UWorld* World = GetWorld();

    // update player count
    if (MustSpectate(NewPlayer))
    {
        NumSpectators++;
    }
    else if (World->IsInSeamlessTravel() || NewPlayer->HasClientLoadedCurrentWorld())
    {
        NumPlayers++;
    }
    else
    {
        NumTravellingPlayers++;
    }

    // save network address for re-associating with reconnecting player, after stripping out port number
    FString Address = NewPlayer->GetPlayerNetworkAddress();
    int32 pos = Address.Find(TEXT(":"), ESearchCase::CaseSensitive);
    NewPlayer->PlayerState->SavedNetworkAddress = (pos > 0) ? Address.Left(pos) : Address;

    // check if this player is reconnecting and already has PlayerState
    FindInactivePlayer(NewPlayer);

    /*--------------End AGameMode Stuff----------------*/
    /*--------------AGameModeBase Stuff----------------*/
    // Runs shared initialization that can happen during seamless travel as well

    GenericPlayerInitialization(NewPlayer);

    // Perform initialization that only happens on initially joining a server

    NewPlayer->ClientCapBandwidth(NewPlayer->Player->CurrentNetSpeed);

    if (MustSpectate(NewPlayer))
    {
        NewPlayer->ClientGotoState(NAME_Spectating);
    }
    else
    {
        // If NewPlayer is not only a spectator and has a valid ID, add him as a user to the replay.
        const FUniqueNetIdRepl& NewPlayerStateUniqueId = NewPlayer->PlayerState->GetUniqueId();
        if (NewPlayerStateUniqueId.IsValid())
        {
            GetGameInstance()->AddUserToReplay(NewPlayerStateUniqueId.ToString());
        }
    }

    if (GameSession)
    {
        GameSession->PostLogin(NewPlayer);
    }

    // Notify Blueprints that a new player has logged in.  Calling it here, because this is the first time that the PlayerController can take RPCs
    K2_PostLogin(NewPlayer);
    FGameModeEvents::GameModePostLoginEvent.Broadcast(this, NewPlayer);

    

    /*--------------End AGameModeBase Stuff----------------*/


    
    //if current number of human players connected > max number or connections allowed
    //AGameSession* GameSessionPtr = Cast<AGameSession>(GetGameSessionClass());
    IOnlineSessionPtr SessionPtr = IOnlineSubsystem::Get()->GetSessionInterface();
    if(!SessionPtr) return;
    FOnlineSessionSettings* SessionSettings = SessionPtr->GetSessionSettings(GameSessionName);
    if (!SessionSettings) return;
    const int32 MaxPlayers = SessionSettings->NumPublicConnections;
    if (!MaxPlayers) return;    
    if(NumPlayers == MaxPlayers) //> gets game session from this gamemode
    {
        AssignChaser();
        //after knowing there is a chaser avaiable to spawn you can now start them
        // Now that initialization is done, try to spawn the player's pawn and start match
        for(FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
        {
            APlayerController_Root* PlayerController_Root = Cast<APlayerController_Root>(*It);
            if (PlayerController_Root)
            {
                HandleStartingNewPlayer(PlayerController_Root);
            }
        }
        StartMatch();
        UpdateAllPlayerText();
        RoundPrepare();
        UE_LOG(LogTemp, Warning, TEXT("TestTest"));
    }
}

//This function should assign the value of "Chaser" to 1 player so that the server can spawn every player in the correct location
//Set the chaser using the PlayerState, because replicating values to the character here is too early for a valid character to exist
/*
 * Note: It doesn't need set the text in the character because there is no pawn before restarting the players
 */
bool AGameMode_Root::AssignChaser()
{
    AGameState_Root* GameState_Root = GetGameState<AGameState_Root>();
    if (!GameState_Root) return false;
    return GameState_Root->ChooseInitialChaser();
   
}

//Manual push to refresh all text above the players head
//::Note:: Also manually makes every players input mode become GameOnly just incase it wasn't previously..This fixes loading in without focus and requiring holding mouse down to control the camera 
void AGameMode_Root::UpdateAllPlayerText()
{
    AGameState_Root* GameState_Root = GetGameState<AGameState_Root>();
    if (!GameState_Root) return;
    GameState_Root->UpdateAllPlayerText();
}