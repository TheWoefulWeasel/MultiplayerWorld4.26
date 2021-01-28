// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/TextRenderComponent.h"
#include "GameFramework/Character.h"
#include "Character_Root.generated.h"

class UTextRenderComponent;
class UCameraComponent;
class USpringArmComponent;

UCLASS(Blueprintable)
class MULTIPLAYERWORLD_API ACharacter_Root : public ACharacter
{
	GENERATED_BODY()

	//Character Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* ThirdPersonCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Text, meta = (AllowPrivateAccess = "true"))
	UTextRenderComponent* TextTag;
	
	


protected:

	
	float MoveSpeed = 100.f;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(const float MoveValue);
	void MoveRight(const float MoveValue);

	/** 
	* Called via input to turn at a given rate. 
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void TurnAtRate(float Rate);

	/**
	* Called via input to turn look up/down at a given rate. 
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void LookUpAtRate(float Rate);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	// Sets default values for this character's properties
	ACharacter_Root();
	
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	//Replicate the characters PlayerState here because my method of spawning players leaves an uncertain call order for the character and playerstate classes which means without replicating here too..
	//The character could execute before being given updated values from the PlayerState
	virtual void OnRep_PlayerState() override;
	
	void SetTeamText(const FText& InText);
	
	//Getters for character components-- Called INLINE
	FORCEINLINE UCameraComponent* GetThirdPersonCameraComponent() const { return ThirdPersonCameraComponent; }
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UTextRenderComponent* GetTextTag() const { return TextTag; }
	
	
};
