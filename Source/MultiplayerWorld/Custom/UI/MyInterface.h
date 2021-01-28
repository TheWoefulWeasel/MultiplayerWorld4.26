// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MyInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * IMyInterface has no parents so when Game Instance inherits from this, it will not pick up any other parents which may be linked to other inherited classes in the Game Instance hierarchy. (Diamond Inheritance)
 */
class MULTIPLAYERWORLD_API IMyInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void HostAServer() = 0;
	virtual void JoinAServer(uint32 AddressIndex) = 0;
	virtual void LoadTheMainMenu() = 0;
	virtual void RefreshServerList() = 0;
};
