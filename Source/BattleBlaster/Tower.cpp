// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"

void ATower::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle FireTimerHandle;
	GetWorldTimerManager().SetTimer(FireTimerHandle, this, &ATower::CheckFireCondition, FireRate, true);
}

void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Tank && Tank->IsAliveTank && IsInFireRange())
	{
		RotateTurret(Tank->GetActorLocation());
	}
	
}

void ATower::CheckFireCondition()
{
	if (Tank && Tank->IsAliveTank && IsInFireRange())
	{
		Fire();
	}
}

bool ATower::IsInFireRange()
{
	return	FVector::Dist(Tank->GetActorLocation(), GetActorLocation()) <= FireRange ? true : false;
}

void ATower::HandleDestruction()
{
	Super::HandleDestruction();

	Destroy();
}
