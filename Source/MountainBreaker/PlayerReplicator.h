// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerReplicator.generated.h"

USTRUCT()
struct FMainPlayerState
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	int Test;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MOUNTAINBREAKER_API UPlayerReplicator : public UActorComponent
{
	GENERATED_BODY()

	public:	
		UPlayerReplicator();

	protected:
		virtual void BeginPlay() override;

	public:	
		virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	private:
		void ClientTick(float DeltaTime);

		void SimulatedProxy_OnRep_ServerState();
		void AutonomousProxy_OnRep_ServerState();

		UFUNCTION(Server, Reliable, WithValidation)
		void Server_SendState();
		UFUNCTION()
		void OnRep_ServerState();

		UPROPERTY(ReplicatedUsing = OnRep_ServerState)
		FMainPlayerState ServerState;
};
