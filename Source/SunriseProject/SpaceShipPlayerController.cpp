// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceShipPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Net/UnrealNetwork.h"
#include "SpaceShipCharacter.h"
#include "SpaceShipPlayerState.h"

// Replicated Properties
void ASpaceShipPlayerController::GetLifetimeReplicatedProps(TArray <FLifetimeProperty> & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    //Replicate Start location.
    DOREPLIFETIME(ASpaceShipPlayerController, StartLocation);
}

void ASpaceShipPlayerController::BeginPlay()
{   
    AddHUD();
    OnUpdateUIDelegate.Broadcast();
    
}


void ASpaceShipPlayerController::AddHUD_Implementation()
{
    if(IsLocalPlayerController() && PlayerWidgetClass)
    {
        Widget = CreateWidget<UUserWidget>(this, PlayerWidgetClass);
        if(Widget)
        {
            Widget->AddToViewport();    
        }
    }
}

void ASpaceShipPlayerController::CallUpdateUIDelegate()
{
    OnUpdateUIDelegate.Broadcast();
}


void ASpaceShipPlayerController::SetStartLocation(FVector Location)
{
    StartLocation = Location;
}

float ASpaceShipPlayerController::GetHealth() const
{
    ASpaceShipCharacter* SpaceShip = Cast<ASpaceShipCharacter>(GetPawn());
    if(SpaceShip)
    {
        return SpaceShip->GetCurrentHealth();
    }
    
    return 100.f;
}

