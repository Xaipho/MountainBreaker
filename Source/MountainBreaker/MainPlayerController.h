// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"

#include "MainCameraActor.h"

#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MOUNTAINBREAKER_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

	public:
		AMainPlayerController();

		virtual void BeginPlay() override;
};
