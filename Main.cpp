// Fill out your copyright notice in the Description page of Project Settings.


#include "Main.h"
#include "Engine/World.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"


/*
Options:

*** If you want your character to always move in the direction of the camera. Add the following to all movement functions inplace of CharacterRotation***

const FRotator Rotation = Controller->GetControlRotation();

*/

// Sets default values
AMain::AMain()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 600.f; // Camera follow distance.
	CameraBoom->bUsePawnControlRotation = true; //Rotates the arm based on controller.

	// Set size for collision capsule.
	GetCapsuleComponent()->SetCapsuleSize(35.f, 90.f);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Set our turn rates for input.
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Dont rotate when the controller rotates.
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	// Configure character movement.
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 1240.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 450.0f;
	GetCharacterMovement()->AirControl = 0.0f;

	/**
	 Player character attributes:
	*/
	MaxHealth = 100.f;
	Health = 60.f;
	HealthPercent = (Health / MaxHealth) * 100.f;
	HealthRecoveryRate = 10.f;

	MaxAbilityResource = 200.f;
	AbilityResource = 100.f;
	AbilityResourceRecoveryRate = 20.f;
	AbilityCost = 20.f;
	AbilityResourcePercent = (AbilityResource / MaxAbilityResource) * 100.f;

	Coins = 0;

	DefaultSpeed = 450.f;
	SprintSpeed = 765.f;
	MountSpeed = 900.f;

	bMounted = false;
	bSprinting = false;

	bRightMouseButtonDown = false;
	bLeftMouseButtonDown = false;

	MovementStatus = EMovementStatus::EMS_Normal;
	AbilityResourceStatus = EAbilityResourceStatus::EARS_Normal;
	HealthStatus = EHealthStatus::EHS_Normal;
}

// Called when the game starts or when spawned
void AMain::BeginPlay()
{
	Super::BeginPlay();
	
	UKismetSystemLibrary::DrawDebugSphere(this, GetActorLocation() + FVector(0, 0, 75.f), 25.f, 12, FLinearColor::Red, 5.f, 2.f);

}

// Called every frame
void AMain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float DeltaHealth = HealthRecoveryRate * (DeltaTime / 5.f);
	float DeltaAbilityResource = AbilityResourceRecoveryRate * (DeltaTime / 5.f);

	// Health status enums.
	switch (HealthStatus)
	{
	case EHealthStatus::EHS_Normal:
		if (HealthPercent > 25.f && HealthPercent <= 50.f)
		{
			SetHealthStatus(EHealthStatus::EHS_HalfHealth);
		}
		else if (HealthPercent <= 25.f)
		{
			SetHealthStatus(EHealthStatus::EHS_LowHealth);
		}
		break;
	case EHealthStatus::EHS_HalfHealth:
		if (HealthPercent <= 25.f)
		{
			SetHealthStatus(EHealthStatus::EHS_LowHealth);
		}
		else if (HealthPercent > 50.f)
		{
			SetHealthStatus(EHealthStatus::EHS_Normal);
		}
		break;
	case EHealthStatus::EHS_LowHealth:
		if (HealthPercent > 25.f && HealthPercent <= 50.f)
		{
			SetHealthStatus(EHealthStatus::EHS_HalfHealth);
		}
		else if (HealthPercent > 50.f)
		{
			SetHealthStatus(EHealthStatus::EHS_Normal);
		}
		break;
	case EHealthStatus::EHS_Max:
		break;
	default:
		break;
	}

	// Secondary resource status enums.
	switch (AbilityResourceStatus)
	{
	case EAbilityResourceStatus::EARS_Normal:
		if (AbilityResourcePercent <= 25.f)
		{
			SetAbilityResourceStatus(EAbilityResourceStatus::EARS_LowResources);
		}
		else
		{
			if (AbilityResourcePercent <= 100.f)
			{
				AbilityResource += DeltaAbilityResource;
			}
			else
			{
				SetAbilityResourceStatus(EAbilityResourceStatus::EARS_Max);
			}
		}
		break;
	case EAbilityResourceStatus::EARS_LowResources:
		if (AbilityResource <= AbilityCost)
		{
			SetAbilityResourceStatus(EAbilityResourceStatus::EARS_Recovering);
		}
		else if (AbilityResourcePercent >= 25.f)
		{
			SetAbilityResourceStatus(EAbilityResourceStatus::EARS_Normal);
		}
		break;
	case EAbilityResourceStatus::EARS_Recovering:
			/*
			* 
			*  This will active once the ability system is built
			* 
			if (AbilityResource >= AbilityCost && AbilityResourcePercent <= 25.f)
			{
				SetAbilityResourceStatus(EAbilityResourceStatus::EARS_LowResources);
			}
			else if (AbilityResource >= AbilityCost && AbilityResourcePercent >= 25.f)
			{
				SetAbilityResourceStatus(EAbilityResourceStatus::EARS_Normal);
			}
			*/
		if (AbilityResourcePercent <= 100.f)
		{
			AbilityResource += DeltaAbilityResource;
		}
		else
		{
			SetAbilityResourceStatus(EAbilityResourceStatus::EARS_Max);
		}
		break;
	case EAbilityResourceStatus::EARS_Max:
		if (AbilityResourcePercent != 100.f && AbilityResourcePercent >= 25.f)
		{
			SetAbilityResourceStatus(EAbilityResourceStatus::EARS_Normal);
		}
		else if (AbilityResourcePercent != 100.f && AbilityResourcePercent <= 25.f)
		{
			SetAbilityResourceStatus(EAbilityResourceStatus::EARS_LowResources);
		}
		break;
	default:
		break;
	}
}

// Called to bind functionality to input
void AMain::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	// Jump action.
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	// Movement speed actions.
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMain::SprintSpeedToggle);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMain::SprintSpeedToggle);
	PlayerInputComponent->BindAction("Mount", IE_Pressed, this, &AMain::MountSpeedToggle);
	PlayerInputComponent->BindAction("Mount", IE_Released, this, &AMain::MountSpeedToggle);

	// Mouse actions for applying values to track which mouse button is being pressed.
	PlayerInputComponent->BindAction("MouseButtonRotatePlayer", IE_Pressed, this, &AMain::RightMouseButtonToggle);
	PlayerInputComponent->BindAction("MouseButtonRotatePlayer", IE_Released, this, &AMain::RightMouseButtonToggle);
	PlayerInputComponent->BindAction("MouseButtonRotateCamera", IE_Pressed, this, &AMain::LeftMouseButtonToggle);
	PlayerInputComponent->BindAction("MouseButtonRotateCamera", IE_Released, this, &AMain::LeftMouseButtonToggle);

	// Mouse axis binding for the right mouse button.
	PlayerInputComponent->BindAxis("MouseMoveForward", this, &AMain::MouseMoveForward);

	// Standard axis bindings for WSAD keys.
	PlayerInputComponent->BindAxis("MoveForward", this, &AMain::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMain::MoveRight);

	// Mouse axis binding for rotating the camera using mouse X & Y axis.
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	// Camera control axis bindings to the arrow keys. 
	PlayerInputComponent->BindAxis("TurnRate", this, &AMain::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMain::LookUpAtRate);

}

/**

Movement Start

*/

void AMain::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{	
		if (bRightMouseButtonDown == false || bLeftMouseButtonDown == false)
		{
			const FRotator CharacterRotation = GetActorRotation();
			const FRotator YawRotation(0.f, CharacterRotation.Yaw, 0.f);
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X); // Find which way is forward.
			if (Value <= 1.0f && Value >= -1.0f)
			{
				AddMovementInput(Direction, Value);
			}
		}
	}
}

void AMain::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// Find which way is right or left.
		const FRotator CharacterRotation = GetActorRotation();
		const FRotator YawRotation(0.f, CharacterRotation.Yaw, 0.f);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y); // Find which way is left/right.
		if (Value <= 1.0f && Value >= -1.0f) // if the value is more than the entended speed, the movement will not execute.
		{
			AddMovementInput(Direction, Value);
		}
	}
}
/* 
Created for when both buttons on the mouse are pressed and held, the player unit will move forward and in the direction the camera is facing. 
In the UE project settings only the right mouse button will be set to apply a value.
*/

void AMain::MouseMoveForward(float Value)
{
	if (bRightMouseButtonDown == true && bLeftMouseButtonDown == true)
	{
		if ((Controller != nullptr) && (Value != 0.0f))
		{
			const FRotator CharacterRotation = GetActorRotation();
			const FRotator YawRotation(0.f, CharacterRotation.Yaw, 0.f);
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X); // Find which way is forward.
			if (Value <= 1.0f && Value >= -1.0f) // if the value is more than the entended speed, the movement will not execute.
			{
				AddMovementInput(Direction, Value);
			}
		}
	}
}

// Camera behavior based upon which mouse button is pressed. Right click should rotate player unit and left click rotates the camera.
void AMain::TurnAtRate(float Rate)
{
	if ((bRightMouseButtonDown == true && bLeftMouseButtonDown == true) || (bRightMouseButtonDown == true && bLeftMouseButtonDown == false))
	{
		AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
	else if (bLeftMouseButtonDown == true && bRightMouseButtonDown == false)
	{
		AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
	else
	{
		AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
}

// Camera behavior based upon which mouse button is pressed. Right click should rotate player unit and left click rotates the camera.
void AMain::LookUpAtRate(float Rate)
{
	if ((bRightMouseButtonDown == true && bLeftMouseButtonDown == true) || (bRightMouseButtonDown == true && bLeftMouseButtonDown == false))
	{
		AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
	else if (bLeftMouseButtonDown == true && bRightMouseButtonDown == false)
	{
		AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
	else
	{
		AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
}

/**

Movement End

*/

void AMain::DecrementHealth(float Value)
{
	Health -= Value;
	if (Health <= 0.f)
	{
		Health = 0.f;
		Dead();
		Health = MaxHealth;
	}
}

void AMain::IncrementCoins(int32 Value)
{
	Coins += Value;
}

void AMain::Dead()
{
	UE_LOG(LogTemp, Warning, TEXT("AMain::Dead() Triggered"));
}

void AMain::SetMovementStatus(EMovementStatus Status)
{
	UE_LOG(LogTemp, Warning, TEXT("AMain::SetMovementStatus() Triggered"));
	MovementStatus = Status;
	if (MovementStatus == EMovementStatus::EMS_Sprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;

	}
	else if (MovementStatus == EMovementStatus::EMS_Mounted)
	{
		GetCharacterMovement()->MaxWalkSpeed = MountSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
	}
}

/**
Boolean Toggles
*/

void AMain::MountSpeedToggle()
{
	if (bMounted == false)
	{
		bMounted = true;
	}
	else if (bMounted == true)
	{
		bMounted = false;
	}
}

// Values created based on which mouse button is being held/pressed. The release functions reset the value before the corresponding button was pressed.
void AMain::RightMouseButtonToggle()
{
	if (bRightMouseButtonDown == false)
	{
		bRightMouseButtonDown = true;
	}
	else if (bRightMouseButtonDown == true)
	{
		bRightMouseButtonDown = false;
	}
}

void AMain::LeftMouseButtonToggle()
{
	if (bLeftMouseButtonDown == false)
	{
		bLeftMouseButtonDown = true;
	}
	else if (bLeftMouseButtonDown == true)
	{
		bLeftMouseButtonDown = false;
	}

}

void AMain::SprintSpeedToggle()
{
	if (bSprinting == false)
	{
		bSprinting = true;
	}
	else if (bSprinting == true)
	{
		bSprinting = false;
	}
}

/**
End Boolean Toggles
*/