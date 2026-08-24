// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "Tank.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEBLASTER_API ATank : public ABasePawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATank();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Inputs
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputMappingContext* DefaultMappingContext;

	// MoveInput
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere)
	float Speed = 300.0f;

	void MoveInput(const FInputActionValue& Value);

	// TurnInput

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* TurnAction;

	UPROPERTY(EditAnywhere)
	float TurnRate = 50.0f;

	void TurnInput(const FInputActionValue& Value);

	// FireInput

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* FireAction;


	// Camera Components
	UPROPERTY(VisibleAnywhere)
	USceneComponent* RootComp;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* CameraComp;

	APlayerController* PlayerController;
	bool IsAliveTank = true;


	void HandleDestruction();
	void SetPlayerEnabled(bool Enabled);
};
