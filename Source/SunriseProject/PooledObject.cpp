// Fill out your copyright notice in the Description page of Project Settings.


#include "PooledObject.h"

// Sets default values
APooledObject::APooledObject()
{

}

void APooledObject::Deactivate()
{
	SetIsActive(false);
	GetWorldTimerManager().ClearAllTimersForObject(this);
	OnPooledObjectDespawn.Broadcast(this);
}

void APooledObject::SetIsActive(bool IsActive)
{
	bIsActive = IsActive;
	SetActorHiddenInGame(!bIsActive);
	GetWorldTimerManager().SetTimer(LifeSpanTimer, this, &APooledObject::Deactivate, false);
}

void APooledObject::SetLifeSpan(float LifeTime)
{
	LifeSpan = LifeTime;
}

void APooledObject::SetPoolIndex(int32 Index)
{
	PoolIndex = Index;
}

bool APooledObject::GetIsActive()
{
	return bIsActive;
}

int32 APooledObject::GetPoolIndex()
{
	return PoolIndex;
}

