// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"

#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HeadTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("Head Trigger"));
	HeadTrigger->SetupAttachment(RootComponent);

	HeadRadialForce = CreateDefaultSubobject<URadialForceComponent>(TEXT("Head Radial Force"));
	HeadRadialForce->SetupAttachment(HeadTrigger);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//Setup Camera
	MainCamera = FindComponentByClass<UCameraComponent>();
	if (!MainCamera) { return; }

	CameraStartTransform = MainCamera->GetComponentToWorld();

	//Setup Hammer
	Hammer = GetWorld()->SpawnActor<AHammerActor>(HammerClass);
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
	if (!IsAttacking)
	{
		GetWorldTimerManager().SetTimer(HammerImpactTimer, this, &APlayerCharacter::FireHammerForce, 0.2f);
		GetWorldTimerManager().SetTimer(StartAttackTimer, this, &APlayerCharacter::ResetAttack, AttackDuration);
		IsAttacking = true;
	}
}

void APlayerCharacter::ResetAttack()
{
	IsAttacking = false;
}

void APlayerCharacter::FireHammerForce()
{
	if (!Hammer) { return; }
	
	Hammer->FireForce();
}



