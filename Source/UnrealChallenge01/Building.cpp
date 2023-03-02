// Fill out your copyright notice in the Description page of Project Settings.


#include "Building.h"

// Sets default values
ABuilding::ABuilding()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT COMPONENT"));
	this->MeshComponent = static_cast<USceneComponent*>(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurVisibleComponent")));
	this->MeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABuilding::BeginPlay()
{
	Super::BeginPlay();

	this->ECurrentState = EBuildingState::Inactive;
	if(this->Source)
	{
		this->InputInv1(this->InputInv1() + 1);
		this->InputInv2(this->InputInv2() + 1);
	}
}

// Called every frame
void ABuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(CheckProductionPossibility())
	{
		this->StartProduction();
	}

	if(this->CurrentState() == Active)
	{
		TimeElapsed += DeltaTime;
	}
	
}

bool ABuilding::CheckProductionPossibility() const
{
	if(this->CurrentState() == Active)		{ return false; }
	if(this->InputInv1() < 1)				{ return false; }
	if(this->InputInv2() < 1)				{ return false; }
	if(this->OutType() == SewMachine)		{ return true; }
	if(this->OutputInv() >= 3)				{ return false; }

	return true;
}

void ABuilding::StartProduction()
{
	//UE_LOG(LogTemp, Display, TEXT("Creating %d in %f seconds"), static_cast<int>(this->EOutType), ProductionTime);
	this->ECurrentState = EBuildingState::Active;
	this->GetWorld()->GetTimerManager().SetTimer(ProductionTimerHandle,
												this,
												&ABuilding::Produce,
												ProductionTime,
												false);
}


void ABuilding::Produce()
{
	//UE_LOG(LogTemp, Display, TEXT("Created %d!"), static_cast<int>(EOutType));
	TimeElapsed = 0;
	this->OutputInv(this->OutputInv() + 1);
	if (!Source)
	{
		this->InputInv1(this->InputInv1() - 1);
		this->InputInv2(this->InputInv2() - 1);
	}
	ECurrentState = Inactive;
}




