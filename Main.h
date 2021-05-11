// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/CameraComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Character.h"
#include "Main.generated.h"

UENUM(BlueprintType)
enum class EMovementStatus : uint8
{
	EMS_Normal UMETA(DisplayName = "Normal"),
	EMS_Sprinting UMETA(DisplayName = "Sprinting"),
	EMS_Mounted UMETA(DisplayName = "Mounted"),
	EMS_Slowed UMETA(DisplayName = "Slowed"),
	EMS_Max UMETA(DisplayName = "DefaultMax")
};

UENUM(BlueprintType)
enum class EHealthStatus : uint8
{
	EHS_Normal UMETA(DisplayName = "Normal"),
	EHS_HalfHealth UMETA(DisplayName = "HalfHealth"),
	EHS_LowHealth UMETA(DisplayName = "LowHealth"),
	EHS_Max UMETA(DisplayName = "DefaultMax")
};

UENUM(BlueprintType)
enum class EAbilityResourceStatus : uint8
{
	EARS_Normal UMETA(DisplayName = "Normal"),
	EARS_LowResources UMETA(DisplayName = "Low Resources"),
	EARS_Recovering UMETA(DisplayName = "Recovering"),
	EARS_Max UMETA(DisplayName = "DefaultMax")
};

UCLASS()
class RPGCOURSEPROJECT_API AMain : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMain();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
	EMovementStatus MovementStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
	EAbilityResourceStatus AbilityResourceStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
	EHealthStatus HealthStatus;

	FORCEINLINE void SetAbilityResourceStatus(EAbilityResourceStatus Status) { AbilityResourceStatus = Status; }
	FORCEINLINE void SetHealthStatus(EHealthStatus Status) { HealthStatus = Status; }


	/* Set movement status and running speed */
	void SetMovementStatus(EMovementStatus Status);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Running")
	float DefaultSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Running")
	float SprintSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Running")
	float MountSpeed;

	bool bMounted;

	bool bSprinting;

	bool bRightMouseButtonDown;
	bool bLeftMouseButtonDown;

	void MountSpeedToggle();

	void SprintSpeedToggle();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;


	// Base turn rates to scale turning functions for the camera.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseTurnRate;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseLookUpRate;

	/**
	/*
	/*
	/* Player Character Attributes:
	/*
	/*
	*/

	float HealthPercent;

	float AbilityResourcePercent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float HealthRecoveryRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats")
	float MaxAbilityResource;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float AbilityResource;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	int32 Coins;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float AbilityResourceRecoveryRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float AbilityCost;

	void DecrementHealth(float Value);

	void IncrementCoins(int32 Value);

	void Dead();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called for forwards & backwards input.
	void MoveForward(float Value);

	// Called for forwards when both mouse buttons are pressed.
	void MouseMoveForward(float Value);

	// Called for turning right & left input.
	void MoveRight(float Value);

	/** Called via input to turn at a given rate. 
	* @param Rate, this is a normalized rate. (ex. 1.0 means 100% of desired turn rate.)
	*/
	void TurnAtRate(float Rate);

	/** Called via input to look up or down at a given rate.
	* @param Rate, this is a normalized rate. (ex. 1.0 means 100% of desired turn rate.)
	*/
	void LookUpAtRate(float Rate);

	void LeftMouseButtonToggle();
	void RightMouseButtonToggle();


	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
