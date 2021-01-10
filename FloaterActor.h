// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Floater.generated.h"

UCLASS()
class RPGCOURSEPROJECT_API AFloater : public AActor
{
	GENERATED_BODY()
	
public:	

	// Welcome to the Pub!

	// Sets default values for this actor's properties
	AFloater();

	// ___Public Floats___

	// Amplitude - How much the unit will oscillate up and down.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Floater Variables")
	float A;

	// Period - How fast the unit will go up and down. 2 * PI / ABS(B)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Floater Variables")
	float B;

	// Phase Shift - (C / B)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Floater Variables")
	float C;

	// Vertical Shift - How high or low the unit will start in the world.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Floater Variables")
	float D;

	// ___Public Mesh___

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ActorMeshComponents")
	UStaticMeshComponent* StaticMesh;

	// ____Public FVectors____

	// Location used by SetActorLocation() when BeginPlay() is called.
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Floater Variables")
	FVector InitialLocation;

	// Location of the actor when dragged in from the editor.
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Floater Variables")
	FVector PlacedLocation;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Floater Variables")
	FVector WorldOrigin;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Floater Variables")
	FVector InitialDirection;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Floater Variables")
	FVector InitialForce;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Floater Variables")
	FVector InitialTorque;

	// ____Public Bools____

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Floater Variables")
	bool bShouldFloat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Floater Variables")
	bool bInitializeFloaterLocations;

	//____Public Functions___

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	// ___Private Floats___

	float RunningTime;

	float BaseZLocation;


protected:

	//____Protected Functions___

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
