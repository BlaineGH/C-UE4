
#pragma once

#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "ColliderMovementComponent.h"
#include "Collider.generated.h"

UCLASS()
class RPGCOURSEPROJECT_API ACollider : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACollider();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	UColliderMovementComponent* OurMovementComponent;

	virtual UPawnMovementComponent* GetMovementComponent() const override;

	// ___Mesh ForceInline___
	FORCEINLINE UStaticMeshComponent* GetMeshComponent(){return MeshComponent;}
	FORCEINLINE void SetMeshComponent(UStaticMeshComponent* Mesh){MeshComponent = Mesh;}

	// ___Sphere ForceInline___
	FORCEINLINE USphereComponent* GetSphereComponent(){return SphereComponent;}
	FORCEINLINE void SetShpereComponent(USphereComponent* Sphere){SphereComponent = Sphere;}

	// ___Camera ForceInline___
	FORCEINLINE UCameraComponent* GetCameraComponent(){return Camera;}
	FORCEINLINE void SetCameraComponent(UCameraComponent* InCamera){Camera = InCamera;}

	// ___SpringArm ForceInline___
	FORCEINLINE USpringArmComponent* GetSpringArmComponent(){return SpringArm;}
	FORCEINLINE void SetSpringArmComponent(USpringArmComponent* InSpringArm){SpringArm = InSpringArm;}

private:

	void MoveForward(float input);
	void MoveRight(float input);
	void PitchCamera(float AxisValue);
	void YawCamera(float AxisValue);

	FVector2D CameraInput;

};
