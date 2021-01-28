// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_Root.h"

#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/DemoNetDriver.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "MultiplayerWorld/Custom/Player/PlayerState_Root.h"

// Sets default values //Bind Dynamics for Objects before calling BP children of this base class
ACharacter_Root::ACharacter_Root()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
	
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	bReplicates = true;
	
	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 300.f;
	//CameraBoom->SetRelativeRotation(FRotator(-500.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level
	CameraBoom->bUsePawnControlRotation = true;
	
	// Create a camera...
	ThirdPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	ThirdPersonCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	ThirdPersonCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	//CreateTextRender...
	TextTag = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TeamTagText"));
	TextTag->SetupAttachment(RootComponent);

	

}

// Called when the game starts or when spawned
void ACharacter_Root::BeginPlay()
{
	Super::BeginPlay();
	
}



// Called to bind functionality to input
void ACharacter_Root::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis(TEXT("Forward"), this, &ACharacter_Root::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("Right"), this, &ACharacter_Root::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ACharacter_Root::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ACharacter_Root::LookUpAtRate);
	
}

/*void ACharacter_Root::MoveForward(const float MoveValue)
{
	const FVector MoveVector = FVector(MoveValue * MoveSpeed * GetWorld()->DeltaTimeSeconds, 0, 0);
	AddMovementInput(MoveVector);
}

void ACharacter_Root::MoveRight(const float MoveValue)
{
	const FVector MoveVector = FVector(0, MoveValue * MoveSpeed * GetWorld()->DeltaTimeSeconds, 0);
	AddMovementInput(MoveVector);
}*/

void ACharacter_Root::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ACharacter_Root::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ACharacter_Root::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ACharacter_Root::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}





void ACharacter_Root::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	//Manually grab the tag value from the PlayerState
	EPlayer_TagType PT = Cast<APlayerState_Root>(GetPlayerState())->Player_Tag;
	if(PT == Chaser)
	{
		//UE_LOG(LogTemp, Warning, TEXT("PS -- OnRep, Chaser"));
		SetTeamText(FText::FromString("Chaser"));
	}
	else if(PT == Runner)
	{
		//UE_LOG(LogTemp, Warning, TEXT("PS -- OnRep, Runner"));
		SetTeamText(FText::FromString("Runner"));
	}
}

void ACharacter_Root::SetTeamText(const FText& InText)
{
	TextTag->SetText(InText);
}