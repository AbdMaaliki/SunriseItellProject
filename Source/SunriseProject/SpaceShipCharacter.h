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

	//Static mesh since space ship mesh doesn't have a skeleta mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* SpaceMesh;

	//Assignable spawn point for the projectile
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* ProjectileSpawnPoint;

	//Blueprint assignable projectile class
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ASunriseProjectProjectile> ProjectileClass;

	//Spring arm and camera components to change the camera
	//to how we see fit
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

	//Player's max health
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	float MaxHealth;

	//Player's replicated current health
	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing=OnRep_CurrentHealth)
	float CurrentHealth;

	//Refrence to player's controller
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

	//Called when health drops to zero
	//or when the game restarts
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Die();

	/** Property replication */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//Current health setter.
	//Only Server can set the value
	UFUNCTION(BlueprintCallable)
	void SetCurrentHealth(float NewHealth);

	//current health getter
	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetCurrentHealth(){return CurrentHealth;};

	/** Custome function that gets called when from
	/** other actors to damage player*/
	UFUNCTION(BlueprintCallable, Category = "Health")
	void ReceiveDamage(AActor* DamageCauser, float DamageTaken);
	
	//Checks if passed players are on the same team
	UFUNCTION()
	bool CheckTeam(ASpaceShipCharacter* Ship1, ASpaceShipCharacter* Ship2);

	//Calls server function to chabge meshh material
	UFUNCTION(BlueprintImplementableEvent)
	void AssignMaterial();
	
private:

	//Input Functions
	void MoveForward(float Value);
	void MoveRight(float Value);


};
