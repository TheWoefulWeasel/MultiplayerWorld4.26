// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState_Root.h"


#include "Net/UnrealNetwork.h"
#include "UserInteractable/PlayerController_Root.h"
#include "UserInteractable/Characters/Character_Root.h"


EPlayer_TagType APlayerState_Root::GetPlayerTag() const
{
    return Player_Tag;
}

void APlayerState_Root::SetPlayerTag(const EPlayer_TagType& TagValue) //Server setting the tag value
{
    UE_LOG(LogTemp, Warning, TEXT("Server has updated Tag value"));
    Player_Tag = TagValue;
    UpdateText();
}

bool APlayerState_Root::GetIsImmune()
{
    return bIsImmune;
}

void APlayerState_Root::SetIsImmune(bool immuneValue)
{
    this->bIsImmune = immuneValue;
}

void APlayerState_Root::OnRep_PlayerTag()
{
    UE_LOG(LogTemp, Warning, TEXT("OnRepPT"));
    UpdateText();
}

void APlayerState_Root::UpdateText()  //LOCAL CLIENT UPDATING
{
    UE_LOG(LogTemp, Warning, TEXT("UpdateText()"));
    
    APlayerController_Root* OwnerController = Cast<APlayerController_Root>(GetOwner()); //only works on server or local client
    if (OwnerController != NULL)
    {
        UE_LOG(LogTemp, Warning, TEXT("Valid Controller"));
        ACharacter_Root* Character = Cast<ACharacter_Root>(OwnerController->GetCharacter());
        if (Character != NULL)
        {
            UE_LOG(LogTemp, Warning, TEXT("Valid Character"));

            if(Player_Tag == Chaser)
            {
                UE_LOG(LogTemp, Warning, TEXT("PS -- OnRep, Chaser"));
                Character->SetTeamText(FText::FromString("Chaser"));
            }
            else if(Player_Tag == Runner)
            {
                UE_LOG(LogTemp, Warning, TEXT("PS -- OnRep, Runner"));
                Character->SetTeamText(FText::FromString("Runner"));
            }
            
        }
    }
}

void APlayerState_Root::SetPlayerToInputGameMode()
{
    APlayerController_Root* OwnerController = Cast<APlayerController_Root>(GetOwner()); //only works on server or local client
    if (OwnerController != NULL)
    {
        FInputModeGameOnly GameOnly;
        OwnerController->SetInputMode(GameOnly);
    }
}

APlayerState_Root::APlayerState_Root()
{
    bReplicates = true;
    
}

void APlayerState_Root::GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const
{
    Super::GetLifetimeReplicatedProps( OutLifetimeProps );

    DOREPLIFETIME( APlayerState_Root, Player_Tag );
}

