// Fill out your copyright notice in the Description page of Project Settings.

///REMEMBER THE GAME STATE IS ON SERVER & CLIENT

#include "GameState_Root.h"

#include "PlayerState_Root.h"
#include "Engine/DemoNetDriver.h"
#include "Engine/EngineTypes.h"
#include "MultiplayerWorld/Custom/World/GameMode_Root.h"

//Client receives the updated Timer Value
void AGameState_Root::OnRep_UpdateClientTimer()
{
    //push timer to client widgets
    UE_LOG(LogTemp, Warning, TEXT("Client Recieved Timer Rep: %f"), CurrentTimerValue);
    
}

void AGameState_Root::SetTimer(const float& GmTimer)
{
    
   UE_LOG(LogTemp,Warning,TEXT("InitTimerValue : %f"), GmTimer);
   CurrentTimerValue = GmTimer;
}

//Server decrementing a timer
void AGameState_Root::SetTimer()
{
   
    if(!AuthorityGameMode) return; //check to make sure only server runs this code
    //decrement the timer
    CurrentTimerValue--;
    UE_LOG(LogTemp,Warning,TEXT("Server Timer: %f"), CurrentTimerValue);
    if (CurrentTimerValue == 0.f)
    {
        UE_LOG(LogTemp, Warning, TEXT("TimerEnded"));
        //tell the gamemode to end the round (set up the next round with respawning chaser and resetting the timer)
        //we know we are on the server from the auth check. so can get instance of gamemode for callback
        AGameMode_Root* GM = Cast<AGameMode_Root>(AuthorityGameMode);
        //AGameMode_Root* GM = Cast<AGameMode_Root>(GetWorld()->GetAuthGameMode());
        GM->RoundEnd();
    }
}

/*void AGameState_Root::PopulateChaserList()
{
    for(FConstPlayerControllerIterator it = GetWorld()->GetPlayerControllerIterator(); it; it++)
    {
        APlayerController_Root* PC = Cast<APlayerController_Root>(it->Get());
      
        if(PC && PC->GetIsChaser())
        {
            //add to chaser list
            ChaserPlayers.Emplace(PC);
        }
    }
}*/

/*TArray<APlayerController_Root*> AGameState_Root::GetChaserList()
{
    ChaserPlayers.Empty();
    
    for(FConstPlayerControllerIterator it = GetWorld()->GetPlayerControllerIterator(); it; ++it)
    {
        APlayerController_Root* PC = Cast<APlayerController_Root>(it->Get());
      
        if(PC && PC->GetPlayerState<APlayerState_Root>()->GetPlayerTag())
        {
            //add to chaser list
            ChaserPlayers.Emplace(PC);
        }
    }

   
    
    return ChaserPlayers;
}*/

bool AGameState_Root::ChooseInitialChaser()
{

    /*
     * get list of playerstates
     * choose a random one
     * assign the EPlayer_TagType as Chaser
     * 
     */
    if(PlayerArray.Num() < 2) return false;
    
    int PlayerNum =  FMath::RandHelper(PlayerArray.Num());
    Cast<APlayerState_Root>(PlayerArray[PlayerNum])->SetPlayerTag(Chaser);
    return true;
}

void AGameState_Root::UpdateChaser(int ChaserNum)
{
    Cast<APlayerState_Root>(PlayerArray[ChaserNum])->SetPlayerTag(Chaser);
}

void AGameState_Root::UpdateAllPlayerText() //Manual push to refresh all player text
{
    for(auto Player : PlayerArray)
    {
        Cast<APlayerState_Root>(Player)->UpdateText();
        Cast<APlayerState_Root>(Player)->SetPlayerToInputGameMode();
    }
}

void AGameState_Root::UpdateImmunity(bool State)
{
    for(auto Player : PlayerArray)
    {
        Cast<APlayerState_Root>(Player)->SetIsImmune(State);
    }
}


void AGameState_Root::GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const
{
    Super::GetLifetimeReplicatedProps( OutLifetimeProps );

    DOREPLIFETIME(AGameState_Root, CurrentTimerValue );
}
