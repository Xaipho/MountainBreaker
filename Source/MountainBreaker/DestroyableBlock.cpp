// Fill out your copyright notice in the Description page of Project Settings.

#include "DestroyableBlock.h"

#include "Components/StaticMeshComponent.h"

#include "PlayerCharacter.h"

// Sets default values
ADestroyableBlock::ADestroyableBlock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADestroyableBlock::BeginPlay()
{
	Super::BeginPlay();
	
	Mesh = FindComponentByClass<UStaticMeshComponent>();

	Mesh->OnComponentHit.AddDynamic(this, &ADestroyableBlock::BlockHit);
}

// Called every frame
void ADestroyableBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADestroyableBlock::BlockHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);

	if (!Player) { return; }

	UE_LOG(LogTemp, Warning, TEXT("Collision with player"));
}

