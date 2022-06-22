// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SpacePawn.generated.h"

UCLASS()
class SUNRISEPROJECT_API ASpacePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASpacePawn();
	
	/** Property replication */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ASunriseProjectProjectile> ProjectileClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = "Movement", Replicated)
	float Speed = 200.f;

	UPROPERTY(EditAnywhere, Category = "Movement", Replicated)
	float TurnRate = 45.f;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UFUNCTION(Server, Reliable)void Server_CallMoveForward(float value);
	UFUNCTION(NetMulticast, Reliable) void MoveForward(float value);
	UFUNCTION(Server, Reliable)void Server_CallMoveRight(float value);
	UFUNCTION(NetMulticast, Reliable) void MoveRight(float value);
	UFUNCTION(Server, Reliable)void Server_CallTurn(float value);
	UFUNCTION(NetMulticast, Reliable) void Turn(float value);
	UFUNCTION(Server, Reliable)void Server_CallFire();
	UFUNCTION(NetMulticast, Reliable)	void Multi_Fire();

};
