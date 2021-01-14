// Fill out your copyright notice in the Description page of Project Settings.


#include "HammerActor.h"

// Sets default values
AHammerActor::AHammerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	RadialForce = CreateDefaultSubobject< URadialForceComponent>(TEXT("RadiaForce"));
	RadialForce->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AHammerActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHammerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHammerActor::FireForce()
{
	UE_LOG(LogTemp, Warning, TEXT("Fore Fire!!"));
	RadialForce->FireImpulse();
}

