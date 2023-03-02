// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyEnums.h"
#include "Building.h"
#include "Math/Vector.h"
#include "Kismet/KismetMathLibrary.h"
#include "Courier.generated.h"

UCLASS()
class UNREALCHALLENGE01_API ACourier : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACourier();

	//Current Goal Set/Get
	UFUNCTION(BlueprintCallable)
	TEnumAsByte<EResourceType> InventoryType() const { return EInventoryType; }
	void InventoryType(TEnumAsByte<EResourceType> const ResourceType) {EInventoryType = ResourceType; }

	TEnumAsByte<EResourceType> UnloadType() const { return EUnloadType; }
	void UnloadType(TEnumAsByte<EResourceType> const ResourceType) {EUnloadType = ResourceType; }

	UFUNCTION(BlueprintCallable)
	int Inventory() const { return NInventory; }
	void Inventory(int const Input) {NInventory = Input; }

	TEnumAsByte<EVehicleState> CurrentState() const { return ECurrentState; }
	void CurrentState(TEnumAsByte<EVehicleState> const CurrentState) {ECurrentState = CurrentState; }

	FVector LoadDestination() const {return FLoadDestination;}
	void LoadDestination(FVector const Destination) {FLoadDestination = Destination; }

	FVector UnloadDestination() const {return FUnloadDestination;}
	void UnloadDestination(FVector const Destination) {FUnloadDestination = Destination; }

	UFUNCTION(BlueprintCallable)
	float LoadTime() const { return FLoadTime; }
	void LoadTime(float const Time) { FLoadTime = Time;} 

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere) USceneComponent* MeshComponent;
	UPROPERTY(EditAnywhere) float CourierSpeed;
	UPROPERTY(EditAnywhere) int Max_Capacity;
	UPROPERTY(EditAnywhere) TArray<ABuilding*> BuildingList;
	UPROPERTY(EditAnywhere) ACourier* Friend;
	UPROPERTY(EditAnywhere) FVector2f LoadTimeMinMax;

	//DO NOT EDIT IN SCENE
	UPROPERTY(VisibleAnywhere) TEnumAsByte<EVehicleState> ECurrentState;
	UPROPERTY(VisibleAnywhere) TEnumAsByte<EResourceType> EInventoryType;
	UPROPERTY(VisibleAnywhere) TEnumAsByte<EResourceType> EUnloadType;

	UPROPERTY(VisibleAnywhere) FVector StartDestination;
	UPROPERTY(VisibleAnywhere) FVector FLoadDestination;
	UPROPERTY(VisibleAnywhere) FVector FUnloadDestination;
	UPROPERTY(VisibleAnywhere) int NInventory = 0;
	UPROPERTY(VisibleAnywhere) float RouteCompletion = 0;
	UPROPERTY(VisibleAnywhere) float TimeCompletion = 0;
	UPROPERTY(VisibleAnywhere) float FLoadTime = 0;

	FVector GetBuildingLocation(EResourceType Type);
	int GetBuildingIndex(EResourceType Type);
	FTimerHandle LoadUnloadTimerHandle;
	bool MovingInventory = false;

	void GetOrder();
	bool DecideRoute();
	void AssignRoute(EResourceType const Load, EResourceType const Unload);
	void Move(FVector Start, FVector End, float DeltaTime);
	void Load();
	void LoadingUnloading();
	void Unload();
	void ClearRoute();
	void LookAtDirection(FVector const Direction);

};
