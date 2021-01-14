// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestroyableBlock.generated.h"

UCLASS()
class MOUNTAINBREAKER_API ADestroyableBlock : public AActor
{
	GENERATED_BODY()
	
	public:	
		// Sets default values for this actor's properties
		ADestroyableBlock();

	protected:
		// Called when the game starts or when spawned
		virtual void BeginPlay() override;

	public:	
		// Called every frame
		virtual void Tick(float DeltaTime) override;

	private:
		UFUNCTION()
		void BlockHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

		UPROPERTY()
		class UStaticMeshComponent* Mesh;
};
