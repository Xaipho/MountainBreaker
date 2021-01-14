// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/RadialForceComponent.h"

#include "HammerActor.generated.h"

UCLASS()
class MOUNTAINBREAKER_API AHammerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHammerActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void FireForce();

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root = nullptr;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh = nullptr;
	UPROPERTY(EditAnywhere)
	URadialForceComponent* RadialForce;


};
