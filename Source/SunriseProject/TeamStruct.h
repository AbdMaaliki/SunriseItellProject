// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TeamStruct.generated.h"

USTRUCT(BlueprintType)
struct FTeamStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Teams")
	FString TeamName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Teams")
	int32 TeamKills;
};
