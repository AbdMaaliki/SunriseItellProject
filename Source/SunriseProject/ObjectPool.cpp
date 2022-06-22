// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPool.h"

#include "PooledObject.h"

// Sets default values for this component's properties
UObjectPool::UObjectPool()
{
}

// Called when the game starts
void UObjectPool::BeginPlay()
{
	if(IsValid(PooledObjectClass))
	{
		UWorld* const World = GetWorld();
		if(IsValid(World))
		{
			for(int32 i = 0; i < PoolSize; i++)
			{
				APooledObject* PoolableActor = World->SpawnActor<APooledObject>(PooledObjectClass, FVector().ZeroVector, FRotator().ZeroRotator);

				if(IsValid(PooledObjectClass))
				{
					PoolableActor->SetIsActive(false);
					PoolableActor->SetPoolIndex(i);
					PoolableActor->OnPooledObjectDespawn.AddDynamic(this, &UObjectPool::OnPoolObjectDespawn);
					ObjectsPool.Add(PoolableActor);
				}
			}
		}
	}
}

APooledObject* UObjectPool::SpawnPooledObject()
{
	for(APooledObject* PoolableActor : ObjectsPool)
	{
		if(IsValid(PoolableActor) && !PoolableActor->GetIsActive())
		{
			PoolableActor->TeleportTo(FVector(0,0,0), FRotator(0,0,0));
			PoolableActor->SetLifeSpan(PooledObjectLifeSpan);
			PoolableActor->SetIsActive(true);
			SpawnedPoolIndex.Add(PoolableActor->GetPoolIndex());

			return PoolableActor;
		}
	}

	if(SpawnedPoolIndex.Num() > 0)
	{
		int32 PooledObjectIndex = SpawnedPoolIndex[0];
		SpawnedPoolIndex.Remove(PooledObjectIndex);
		APooledObject* PoolableActor = ObjectsPool[PooledObjectIndex];

		if(IsValid(PoolableActor))
		{
			PoolableActor->SetIsActive(false);

			PoolableActor->TeleportTo(FVector(0,0,0), FRotator(0,0,0));
			PoolableActor->SetLifeSpan(PooledObjectLifeSpan);
			PoolableActor->SetIsActive(true);
			SpawnedPoolIndex.Add(PoolableActor->GetPoolIndex());

			return PoolableActor;
		}
	}

	return nullptr;
}

void UObjectPool::OnPoolObjectDespawn(APooledObject* PoolActor)
{
	SpawnedPoolIndex.Remove(PoolActor->GetPoolIndex());
}

