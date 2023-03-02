// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyEnums.h"
#include "Building.generated.h"

UCLASS()
class UNREALCHALLENGE01_API ABuilding : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuilding();

	UFUNCTION(BlueprintCallable)
	TEnumAsByte<EResourceType> OutType() const { return EOutType; }
	void OutType(TEnumAsByte<EResourceType> const OutType) {EOutType = OutType; }

	//OutputInv Set/Get
	UFUNCTION(BlueprintCallable)
	int OutputInv() const { return NOutputInv; }
	void OutputInv(int const OutputInv) {NOutputInv = OutputInv;}

	//InType1 Set/Get
	UFUNCTION(BlueprintCallable)
	TEnumAsByte<EResourceType> InType1() const { return EInType1; }
	void InType1(TEnumAsByte<EResourceType> const InType1) {EInType1 = InType1; }

	//InputInv1 Set/Get
	UFUNCTION(BlueprintCallable)
	int InputInv1() const { return NInputInv1; }
	void InputInv1(int const InputInv1) {NInputInv1 = InputInv1;}

	//InType1 Set/Get
	UFUNCTION(BlueprintCallable)
	TEnumAsByte<EResourceType> InType2() const { return EInType2; }
	void InType2(TEnumAsByte<EResourceType> const InType2) {EInType2 = InType2; }

	//InputInv2 Set/Get
	UFUNCTION(BlueprintCallable)
	int InputInv2() const { return NInputInv2; }
	void InputInv2(int const InputInv2) {NInputInv2 = InputInv2;}

	//CurrentState Set/Get
	UFUNCTION(BlueprintCallable)
	TEnumAsByte<EBuildingState> CurrentState() const { return ECurrentState; }
	void CurrentState(TEnumAsByte<EBuildingState> const CurrentState) { ECurrentState = CurrentState; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly) float TimeElapsed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float ProductionTime; //time to produce output

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	bool CheckProductionPossibility() const;

private:
	UPROPERTY(EditAnywhere) USceneComponent* MeshComponent;
	//configurable
	UPROPERTY(EditAnywhere) TEnumAsByte<EResourceType> EOutType;
	UPROPERTY(EditAnywhere) TEnumAsByte<EResourceType> EInType1;
	UPROPERTY(EditAnywhere) TEnumAsByte<EResourceType> EInType2;
	UPROPERTY(EditAnywhere) bool Source;
	UPROPERTY(VisibleAnywhere) int NOutputInv; //current output inventory
	UPROPERTY(VisibleAnywhere) int NInputInv1; //current input inventory
	UPROPERTY(VisibleAnywhere) int NInputInv2; //current input inventory
	UPROPERTY(VisibleAnywhere) TEnumAsByte<EBuildingState> ECurrentState;

	const int Max_Inventory = 3;
	FTimerHandle ProductionTimerHandle;

	//functions
	void StartProduction();
	void Produce();

};
