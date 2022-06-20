// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpaceShipCharacter.generated.h"

UCLASS()
class SUNRISEPROJECT_API ASpaceShipCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASpaceShipCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* SpaceMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ASunriseProjectProjectile> ProjectileClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UCameraComponent* Camera;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Server function for spawning projectiles.*/
	UFUNCTION(Server, Reliable)
	void ServerFire();
	UFUNCTION(NetMulticast , Reliable)
	void MultiFire();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	float MaxHealth;

	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing=OnRep_CurrentHealth)
	float CurrentHealth;


	UPROPERTY()
	class ASpaceShipPlayerController* MyController;


	/** RepNotify for changes made to current health.*/
	UFUNCTION()
	void OnRep_CurrentHealth();
	/** Response to health being updated. Called on the server immediately after modification, and on clients in response to a RepNotify*/
	void OnHealthUpdate();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Die();

	/** Property replication */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
	void SetCurrentHealth(float NewHealth);

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetCurrentHealth(){return CurrentHealth;};

	/** Event for taking damage. Overridden from APawn.*/
	UFUNCTION(BlueprintCallable, Category = "Health")
	void ReceiveDamage(AActor* DamageCauser, float DamageTaken);
	
	UFUNCTION()
	bool CheckTeam(ASpaceShipCharacter* Ship1, ASpaceShipCharacter* Ship2);
	
private:

	//Input Functions
	void MoveForward(float Value);
	void MoveRight(float Value);


};
