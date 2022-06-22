// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObjectPool.generated.h"

class APooledObject;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SUNRISEPROJECT_API UObjectPool : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UObjectPool();

	UFUNCTION(BlueprintCallable, Category="Pool Object")
		APooledObject* SpawnPooledObject();

	UPROPERTY(EditAnywhere, Category="Pool Object")
		TSubclassOf<APooledObject> PooledObjectClass;

	UPROPERTY(EditAnywhere, Category="Pool Object")
		int32 PoolSize = 20;

	UPROPERTY(EditAnywhere, Category="Pool Object")
		float PooledObjectLifeSpan = 0.f;

	UFUNCTION()
		void OnPoolObjectDespawn(APooledObject* PoolActor);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	TArray<APooledObject*> ObjectsPool;
	TArray<int32> SpawnedPoolIndex;
	
};
