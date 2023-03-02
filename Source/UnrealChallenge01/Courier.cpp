// Fill out your copyright notice in the Description page of Project Settings.


#include "Courier.h"

// Sets default values
ACourier::ACourier()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT COMPONENT"));
	this->MeshComponent = static_cast<USceneComponent*>(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurVisibleComponent")));
	this->MeshComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ACourier::BeginPlay()
{
	Super::BeginPlay();
	this->StartDestination = FVector(0.0f,0.0f,0.0f);
}

// Called every frame
void ACourier::Tick(float const DeltaTime)
{
	Super::Tick(DeltaTime);
	switch(this->CurrentState())
	{
	case Idle:
		this->GetOrder();
		break;

	case MovingToLoad:
		this->Move(this->StartDestination, this->LoadDestination(), DeltaTime);
		break;

	case MovingToUnload:
		this->Move(this->StartDestination, this->UnloadDestination(), DeltaTime);
		break;

	case Loading:
		this->LoadingUnloading();
		break;

	case Unloading:
		this->LoadingUnloading();
		break;

	default:
		break;

	}
}

FVector ACourier::GetBuildingLocation(EResourceType const Type)
{
	switch(Type)
	{
	case Coal:
		return this->BuildingList[0]->GetActorLocation();

	case Iron:
		return this->BuildingList[1]->GetActorLocation();

	case Wood:
		return this->BuildingList[2]->GetActorLocation();

	case SteelBeam:
		return this->BuildingList[3]->GetActorLocation();

	case SewMachine:
		return this->BuildingList[4]->GetActorLocation();

	case Gaia:

	default:
		return FVector(0.0f, 0.0f, 0.0f);
	}
}

int ACourier::GetBuildingIndex(EResourceType Type)
{
	switch(Type)
	{
	case Coal:
		return 0;

	case Iron:
		return 1;

	case Wood:
		return 2;

	case SteelBeam:
		return 3;

	case SewMachine:
		return 4;

	case Gaia:

	default:
		return -1;
	}
}

void ACourier::GetOrder()
{
	if(this->DecideRoute() == true)
	{
		this->CurrentState(MovingToLoad);
		this->LookAtDirection(this->LoadDestination());
		UE_LOG(LogTemp, Display, TEXT("Journey Begins"));
	}
}

void ACourier::Move(FVector const Start, FVector const End, float const DeltaTime)
{
	const float Distance = FVector::Distance(Start, End);
	const float Speed = this->CourierSpeed;
	RouteCompletion = TimeCompletion * Speed / Distance;

	const FVector NewLocation = FMath::Lerp(Start, End, RouteCompletion);
	TimeCompletion += DeltaTime;

	this->SetActorLocation(NewLocation);

	if(RouteCompletion >= 1)
	{
		this->SetActorLocation(End); //makeshift clamp
		this->StartDestination = this->GetActorLocation(); //reset
		this->RouteCompletion = 0; //reset
		this->TimeCompletion = 0; //reset
		if(this->CurrentState() == MovingToLoad)
		{
			this->CurrentState(Loading);
			this->LookAtDirection(this->UnloadDestination());
		}
		else if (this->CurrentState() == MovingToUnload)
		{
			this->CurrentState(Unloading);
		}
		UE_LOG(LogTemp, Display, TEXT("Journey End"));
	}
}

void ACourier::LoadingUnloading()
{
	if(this->MovingInventory == true) { return; }

	const float RandomTime = FMath::RandRange(LoadTimeMinMax.X, LoadTimeMinMax.Y);
	this->LoadTime(RandomTime);
	this->MovingInventory = true;

	if(this->CurrentState() == Loading)
	{
		UE_LOG(LogTemp, Display, TEXT("LOADING!"));
		this->GetWorld()->GetTimerManager().SetTimer(LoadUnloadTimerHandle,
												this,
													&ACourier::Load,
												RandomTime,
												false);
	}
	else if(this->CurrentState() == Unloading)
	{
		UE_LOG(LogTemp, Display, TEXT("UNLOADING!"));
		this->GetWorld()->GetTimerManager().SetTimer(LoadUnloadTimerHandle,
												this,
													&ACourier::Unload,
												RandomTime,
												false);
	}
}

void ACourier::Load()
{
	const int Index = GetBuildingIndex(this->InventoryType());
	int ResourceAmount = this->BuildingList[Index]->OutputInv();
	ResourceAmount = FMath::Clamp(ResourceAmount, 0, Max_Capacity);

	this->Inventory(ResourceAmount);
	this->BuildingList[Index]->OutputInv(this->BuildingList[Index]->OutputInv() - ResourceAmount);

	this->MovingInventory = false;
	this->CurrentState(MovingToUnload);

	UE_LOG(LogTemp, Display, TEXT("LOADED!"));
	UE_LOG(LogTemp, Display, TEXT("Journey Begins"));
}

void ACourier::Unload()
{
	const int Index = GetBuildingIndex(this->UnloadType());
	const int ResourceAmount = this->Inventory();
	int CurrentInventory;

	if(BuildingList[Index]->InType1() == this->InventoryType())
	{
		CurrentInventory = this->BuildingList[Index]->InputInv1();
		CurrentInventory += ResourceAmount;
		CurrentInventory = FMath::Clamp(CurrentInventory, 0, 3);
		this->BuildingList[Index]->InputInv1(CurrentInventory);
	}
	else if (BuildingList[Index]->InType2() == this->InventoryType())
	{
		CurrentInventory = this->BuildingList[Index]->InputInv2();
		CurrentInventory += ResourceAmount;
		CurrentInventory = FMath::Clamp(CurrentInventory, 0, 3);
		this->BuildingList[Index]->InputInv2(CurrentInventory);
	}
	this->Inventory(0);
	this->InventoryType(Gaia);

	this->MovingInventory = false;
	this->ClearRoute();

	this->CurrentState(Idle);

	UE_LOG(LogTemp, Display, TEXT("UNLOADED!"));
}

bool ACourier::DecideRoute()
{
	/* Building List: 0 = Coal, 1 = Iron, 2 = Wood, 3 = Steel Beam, 4 = Sew Machine*/

	//IF there is Steel Beam in Sew Machine, Wood Priority
	if(BuildingList[4]->InputInv1() > 0)
	{
		if(Friend->InventoryType() != Wood && BuildingList[4]->InputInv2() < 3 && BuildingList[2]->OutputInv() > 0)
		{
			AssignRoute(Wood, SewMachine);
			return true;
		}
	}

	// IF there is Steel 
	if(BuildingList[3]->OutputInv() > 0)
	{
		//IF other courier isn't transporting Steel Beams AND Sew Machine Maker has space for Steel Beams AND 
 		if(Friend->InventoryType() != SteelBeam && BuildingList[4]->InputInv1() < 3)
		{
			AssignRoute(SteelBeam, SewMachine);
			return true;
		}

		//IF other courier isn't transporting Wood AND Sew Machine Maker has space for Wood AND Wood Mine has Wood
		if(Friend->InventoryType() != Wood && BuildingList[4]->InputInv2() < 3 && BuildingList[2]->OutputInv() > 0)
		{
			AssignRoute(Wood, SewMachine);
			return true;
		}
	}

	//IF other courier isn't transporting Coal AND Steel Beam Maker has space for Coal AND Coal Mine has Coal
	if(Friend->InventoryType() != Coal && BuildingList[3]->InputInv1() < 3 && BuildingList[0]->OutputInv() > 0)
	{
		AssignRoute(Coal, SteelBeam);
		return true;
	}

	//IF other courier isn't transporting Iron AND Steel Beam Maker has space for Iron AND Iron Mine has Iron
	if(Friend->InventoryType() != Iron && BuildingList[3]->InputInv2() < 3 && BuildingList[1]->OutputInv() > 0)
	{
		AssignRoute(Iron, SteelBeam);
		return true;
	}

	//default without priority checks
	//IF other courier isn't transporting Wood AND Sew Machine Maker has space for Wood AND Wood Mine has Wood
	if(Friend->InventoryType() != Wood && BuildingList[4]->InputInv2() < 3 && BuildingList[2]->OutputInv() > 0)
	{
		AssignRoute(Wood, SewMachine);
		return true;
	}

	return false;
}

void ACourier::AssignRoute(EResourceType const Load, EResourceType const Unload)
{
	this->InventoryType(Load);
	this->LoadDestination(GetBuildingLocation(Load));

	this->UnloadType(Unload);
	this->UnloadDestination(GetBuildingLocation(Unload));
}

void ACourier::ClearRoute()
{
	this->InventoryType(Gaia);
	this->LoadDestination(FVector(0));

	this->UnloadType(Gaia);
	this->UnloadDestination(FVector(0));
}

void ACourier::LookAtDirection(FVector const Direction)
{
	const FVector WorldUp = FVector::UpVector;
    const FVector Forward = Direction - this->GetActorLocation();
    FRotator Rot = UKismetMathLibrary::MakeRotFromXZ(Forward, WorldUp);
	Rot += FRotator(0.0f, -90.0f, 0.0f); //offset
    this->SetActorRotation(Rot);
}









