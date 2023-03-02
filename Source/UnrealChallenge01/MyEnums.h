// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum EResourceType {
	Gaia = 0	UMETA(DisplayName = "Gaia"),
	Coal		UMETA(DisplayName = "Coal"),
	Iron		UMETA(DisplayName = "Iron"),
	Wood		UMETA(DisplayName = "Wood"),
	SteelBeam	UMETA(DisplayName = "Steel Beam"),
	SewMachine	UMETA(DisplayName = "Sewing Machine")
};

UENUM(BlueprintType)
enum EBuildingState
{
	Inactive = 0	UMETA(DisplayName = "Idle"),
	Active			UMETA(DisplayName = "Busy")
};

UENUM(BlueprintType)
enum EVehicleState {
	Idle = 0		UMETA(DisplayName = "Idle"),
	MovingToLoad	UMETA(DisplayName = "MovingToLoad"),
	Loading			UMETA(DisplayName = "Loading"),
	MovingToUnload	UMETA(DisplayName = "MovingToUnload"),
	Unloading		UMETA(DisplayName = "Unloading")
};
