// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerReplicator.h"

#include "Engine/Engine.h"
#include "Net/UnrealNetwork.h"


UPlayerReplicator::UPlayerReplicator()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated(true);
}

//FString GetEnumText(ENetRole Role)
//{
//	switch (Role)
//	{
//	case ENetRole::ROLE_None:
//	{
//		return "None";
//	}break;
//	case ENetRole::ROLE_SimulatedProxy:
//	{
//		return "SimulatedProxy";
//	}break;
//	case ENetRole::ROLE_AutonomousProxy:
//	{
//		return "Autonomous Proxy";
//	}break;
//	case ENetRole::ROLE_Authority:
//	{
//		return "Authority";
//	}break;
//
//	default:
//	{
//		return "Error";
//	}break;
//	}
//}

void UPlayerReplicator::BeginPlay()
{
	Super::BeginPlay();

}

void UPlayerReplicator::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UPlayerReplicator, ServerState);
}


void UPlayerReplicator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//WE are client
	if (GetOwnerRole() == ROLE_AutonomousProxy)
	{
		//Server_SendMove(LastMove);
	}

	//We are the server and have a Pawn
	if (GetOwner()->GetRemoteRole() == ROLE_SimulatedProxy)
	{
		//UpdateServerState(LastMove);
	}

	if (GetOwnerRole() == ROLE_SimulatedProxy)
	{
		ClientTick(DeltaTime);
	}
}

void UPlayerReplicator::OnRep_ServerState()
{
	switch (GetOwnerRole())
	{
	case ROLE_AutonomousProxy:
	{
		AutonomousProxy_OnRep_ServerState();
	}break;

	case ROLE_SimulatedProxy:
	{
		SimulatedProxy_OnRep_ServerState();
	}break;

	default: break;
	}
}

void UPlayerReplicator::ClientTick(float DeltaTime)
{
	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("ClientTick"));
}

void UPlayerReplicator::SimulatedProxy_OnRep_ServerState()
{

}

void UPlayerReplicator::AutonomousProxy_OnRep_ServerState()
{
	//We are client
}

void UPlayerReplicator::Server_SendState_Implementation()
{

}

bool UPlayerReplicator::Server_SendState_Validate()
{
	return true;
}




