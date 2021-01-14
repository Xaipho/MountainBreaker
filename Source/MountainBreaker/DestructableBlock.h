// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DestructibleActor.h"
#include "DestructableBlock.generated.h"

/**
 * 
 */
UCLASS()
class MOUNTAINBREAKER_API ADestructableBlock : public ADestructibleActor
{
	GENERATED_BODY()

		ADestructableBlock();

		virtual void Tick(float DeltaTime) override;

	private:
		void RepairMesh();
		FTimerHandle RepairTimer;
		UDestructibleComponent* DestructibleComponent;
};
