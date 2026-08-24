// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleBlasterGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "Tower.h"
#include "BattleBlasterGameInstance.h"

void ABattleBlasterGameMode::BeginPlay()
{
	Super::BeginPlay();


	TowerCount = GetTowerCount(Towers);
	UE_LOG(LogTemp, Display, TEXT("%d tower(s) left."), TowerCount);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (PlayerPawn) 
	{
		Tank = Cast<ATank>(PlayerPawn);
		if (!Tank)
		{
			UE_LOG(LogTemp, Display, TEXT("GameMode: Failed to find the tank actor!"));
		}
	}

	int32 LoopIndex = 0;
	
	while (LoopIndex < TowerCount)
	{
		AActor* TowerActor = Towers[LoopIndex];
		if (TowerActor)
		{
			ATower* Tower = Cast<ATower>(TowerActor);
			if (Tower && Tank)
			{
				Tower->Tank = Tank;
			}
		}

		LoopIndex++;
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (PlayerController)
	{
		ScreenMessageWidget = CreateWidget<UScreenMessage>(PlayerController, ScreenMessageClass);

		if (ScreenMessageWidget)
		{
			ScreenMessageWidget->AddToPlayerScreen();
			ScreenMessageWidget->SetMessageText("Ready!");
		}
	}

	CountdownSeconds = CountdownDelay;

	GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &ABattleBlasterGameMode::OnCountdownTimerTimeout, 1.0f, true);
}

void ABattleBlasterGameMode::OnCountdownTimerTimeout()
{
	CountdownSeconds -= 1;


	if (CountdownSeconds > 0)
	{
		//Ready
		ScreenMessageWidget->SetMessageText(FString::FromInt(CountdownSeconds));
	}
	else if (CountdownSeconds == 0)
	{
		//Start
		ScreenMessageWidget->SetMessageText("Start!");
	}
	else
	{
		//Hide
		ScreenMessageWidget->SetVisibility(ESlateVisibility::Hidden);
		Tank->SetPlayerEnabled(true);
		GetWorldTimerManager().ClearTimer(CountdownTimerHandle);
	}
} 

void ABattleBlasterGameMode::ActorDied(AActor* DeadActor)
{
	bool IsGameOver = false;

	if (DeadActor == Tank)
	{
		Tank->HandleDestruction();
		IsGameOver = true;
	}
	else 
	{
		ATower* DeadTower = Cast<ATower>(DeadActor);
		if(DeadTower)
		{
			
			DeadTower->HandleDestruction();

			TowerCount = GetTowerCount(Towers);

			if (TowerCount == 0)
			{
				IsGameOver = true;
				IsVictory = true;
			}
		}
	}

	if (IsGameOver)
	{
		FString GameOverString = IsVictory ? "Victory!" : "Defeat!";

		ScreenMessageWidget->SetVisibility(ESlateVisibility::Visible);
		ScreenMessageWidget->SetMessageText(GameOverString);

		//GameOver timer
		FTimerHandle GameOverTimerHandle;
		GetWorldTimerManager().SetTimer(GameOverTimerHandle, this, &ABattleBlasterGameMode::OnGameOverTimerTimeout, GameOverDelay, false);
	}
}

int32 ABattleBlasterGameMode::GetTowerCount(TArray<AActor*>& TargetArray)
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATower::StaticClass(), TargetArray);
	return TargetArray.Num();
}

void ABattleBlasterGameMode::OnGameOverTimerTimeout()
{
	UGameInstance* GameInstance = GetGameInstance();
	
	if (GameInstance)
	{
		UBattleBlasterGameInstance* BattleBlasterGameInstance = Cast<UBattleBlasterGameInstance>(GameInstance);
		if (BattleBlasterGameInstance)
		{
			if (IsVictory)
			{
				//Load next level
				BattleBlasterGameInstance->LoadNextLevel();
			}
			else
			{
				//Reload current level
				BattleBlasterGameInstance->RestartCurrentLevel();
			}
		}
	}
}




