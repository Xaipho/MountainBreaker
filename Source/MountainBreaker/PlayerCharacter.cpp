// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"

//Engine includes
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"

//Custom includes
#include "PlayerReplicator.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//bReplicates = true;

	HeadTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("Head Trigger"));
	HeadTrigger->SetupAttachment(RootComponent);

	HeadRadialForce = CreateDefaultSubobject<URadialForceComponent>(TEXT("Head Radial Force"));
	HeadRadialForce->SetupAttachment(HeadTrigger);

	PlayerReplicator = CreateDefaultSubobject<UPlayerReplicator>(TEXT("Replicator"));
	MainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
}

FString GetEnumText(ENetRole Role)
{
	switch (Role)
	{
	case ENetRole::ROLE_None:
	{
		return "None";
	}break;
	case ENetRole::ROLE_SimulatedProxy:
	{
		return "SimulatedProxy";
	}break;
	case ENetRole::ROLE_AutonomousProxy:
	{
		return "Autonomous Proxy";
	}break;
	case ENetRole::ROLE_Authority:
	{
		return "Authority";
	}break;

	default:
	{
		return "Error";
	}break;
	}
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//Setup Camera
	if (!MainCamera) { return; }
	CameraStartTransform = MainCamera->GetComponentToWorld();

	//Setup Hammer
	if (!HammerClass) { return; }
	Hammer = GetWorld()->SpawnActor<AHammerActor>(HammerClass);
	if (!Hammer) { return; }
	Hammer->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("HammerSocket"));
	Hammer->SetOwner(this);

	HeadTrigger->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapHeadTriggerBegin);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!MainCamera) { return; }
	CameraStartTransform.SetLocation(FVector(1000.f, GetActorLocation().Y, GetActorLocation().Z + 100));
	MainCamera->SetWorldTransform(CameraStartTransform);

	DrawDebugString(GetWorld(), FVector(0, 0, 100.f), GetEnumText(GetLocalRole()), this, FColor::White, DeltaTime);

	if (HasAuthority())
	{
		ReplicatedIsAttacking = IsAttacking;
	}
	else
	{
		IsAttacking = ReplicatedIsAttacking;
	}

	if (IsAttacking && !StartAttacking)
	{
		GetWorldTimerManager().SetTimer(HammerImpactTimer, this, &APlayerCharacter::FireHammerForce, 0.2f);
		GetWorldTimerManager().SetTimer(StartAttackTimer, this, &APlayerCharacter::ResetAttack, AttackDuration);
		StartAttacking = true;
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, &APlayerCharacter::Attack);
}

void APlayerCharacter::OnOverlapHeadTriggerBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HeadRadialForce) { return; }

	if (GetVelocity().Z > 0.f)
	{
		HeadRadialForce->FireImpulse();
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if (IsAttacking) { return; }

	//AddMovementInput(GetActorRightVector() * Value);
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void APlayerCharacter::Attack()
{
	IsAttacking = true;

	Server_Attack();
}

void APlayerCharacter::ResetAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("ResetAttack"));
	IsAttacking = false;
	StartAttacking = false;
}

void APlayerCharacter::FireHammerForce()
{
	if (!Hammer) { return; }
	
	Hammer->FireForce();
}

void APlayerCharacter::Server_Attack_Implementation()
{
	//if (!IsAttacking)
	//{
	//	GetWorldTimerManager().SetTimer(HammerImpactTimer, this, &APlayerCharacter::FireHammerForce, 0.2f);
	//	GetWorldTimerManager().SetTimer(StartAttackTimer, this, &APlayerCharacter::ResetAttack, AttackDuration);
		IsAttacking = true;
	//}
}

bool APlayerCharacter::Server_Attack_Validate()
{
	return true;
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlayerCharacter, ReplicatedIsAttacking);
}




