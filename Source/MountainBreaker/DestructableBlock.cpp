// Fill out your copyright notice in the Description page of Project Settings.

#include "DestructableBlock.h"

#include "DestructibleComponent.h"
#include "DrawDebugHelpers.h"
#include "PhysXIncludes.h" 


ADestructableBlock::ADestructableBlock()
{
	PrimaryActorTick.bCanEverTick = true;

	DestructibleComponent = GetDestructibleComponent();
}

void ADestructableBlock::Tick(float DeltaTime)
{
    static bool start = false;
    uint32 ChunkCount = DestructibleComponent->ApexDestructibleActor->getNumVisibleChunks();

    if (ChunkCount > 1 && !start)
    {
        UE_LOG(LogTemp, Warning, TEXT("Destroy Mesh!!"));
        start = true;
        GetWorldTimerManager().SetTimer(RepairTimer, this, &ADestructableBlock::RepairMesh, 5.f);
    }
}

void ADestructableBlock::RepairMesh()
{
    UE_LOG(LogTemp, Warning, TEXT("Repair Mesh!!"));
    uint32 ChunkCount = DestructibleComponent->ApexDestructibleActor->getNumVisibleChunks();
    const uint16* ChunkIndices = DestructibleComponent->ApexDestructibleActor->getVisibleChunks();
    for (uint32 c = 0; c < ChunkCount; c++)
    {
        PxRigidDynamic* PActor = DestructibleComponent->ApexDestructibleActor->getChunkPhysXActor(ChunkIndices[c]);
   
        check(PActor);

        PxTransform pos = PActor->getGlobalPose();
        FVector ActorPOs = GetActorLocation();

        PActor->setKinematicTarget(pos);

        PxTransform Trans = PActor->getGlobalPose();
        PxVec3 PxLoc = Trans.p;
        FVector Location(PxLoc.x, PxLoc.y, PxLoc.z);

        DrawDebugPoint(GetWorld(), Location, 7.f, FColor::Red, false, 60.f);


        UE_LOG(LogTemp, Warning, TEXT("Number of Chunks: %i with Locations: %s"), ChunkCount, *Location.ToString());
    }
}

