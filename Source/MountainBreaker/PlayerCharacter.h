// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "PhysicsEngine/RadialForceComponent.h"

#include "HammerActor.h"

#include "PlayerCharacter.generated.h"

UCLASS()
class MOUNTAINBREAKER_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	public:
		// Sets default values for this character's properties
		APlayerCharacter();

	protected:
		// Called when the game starts or when spawned
		virtual void BeginPlay() override;

	public:	
		// Called every frame
		virtual void Tick(float DeltaTime) override;

		// Called to bind functionality to input
		virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

		UPROPERTY(Editanywhere, BlueprintReadOnly)
		bool IsAttacking = false;
		UPROPERTY()
		bool StartAttacking = false;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UPlayerReplicator* PlayerReplicator = nullptr;

		

	private:
		UFUNCTION()
			void OnOverlapHeadTriggerBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

		//Movement
		void MoveRight(float Value);
		void Attack();
		void ResetAttack();
		void FireHammerForce();

		//Components
		UPROPERTY(EditAnywhere)
		USphereComponent* HeadTrigger;
		UPROPERTY(EditAnywhere)
		URadialForceComponent* HeadRadialForce = nullptr;

		//Camera
		UPROPERTY(EditAnywhere)
		UCameraComponent* MainCamera = nullptr;

		//Weapon
		UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AHammerActor> HammerClass;
		UPROPERTY()
		AHammerActor* Hammer = nullptr;

		//Timers
		UPROPERTY()
		FTimerHandle StartAttackTimer;
		UPROPERTY()
		FTimerHandle HammerImpactTimer;
		UPROPERTY(EditAnywhere)
		float AttackDuration = 0.7f;

		//Default states
		FTransform CameraStartTransform;

		//Server Replication
		UFUNCTION(Server, Reliable, WithValidation)
		void Server_Attack();

		UPROPERTY(Replicated)
		bool ReplicatedIsAttacking = false;
};
