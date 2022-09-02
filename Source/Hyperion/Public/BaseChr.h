// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Konsole.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

#include "BaseChr.generated.h"

UCLASS()
class HYPERION_API ABaseChr : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseChr();
	UPROPERTY(BlueprintReadWrite, Category = "Components")
		UCameraComponent* Camera;
	UPROPERTY(BlueprintReadWrite, Category = "Components")
		UPhysicsHandleComponent* PhysicsHandle;
	UPROPERTY(BlueprintReadWrite, Category = "Components")
		USpringArmComponent* SpringArm;
	UPROPERTY(BlueprintReadWrite, Category = "Components")
		USpotLightComponent* InnerLight;
	UPROPERTY(BlueprintReadWrite, Category = "Components")
		USpotLightComponent* OuterLight;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PhysicsInteraction")
		UPrimitiveComponent* HitComponent;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character|Bindings|Camera")
		FName VerticalLookBind = "MouseY";
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character|Bindings|Camera")
		FName VerticalLookOnControllerBind = "LookRate";
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character|Bindings|Camera")
		FName HorizontalLookBind = "MouseX";
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character|Bindings|Camera")
		FName HorizontalLookOnControllerBind = "TurnRate";
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character|Bindings|Movement")
		FName MoveForwardBind = "Vertical";
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character|Bindings|Movement")
		FName MoveSideBind = "Horizontal";
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character|Bindings|Abilities")
		FName GrappleBind = "Grapple";

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Character|Mouse|Sensitivity")
		float Sensitivity = 0.25f;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Character|Gamepad|Sensitivity")
		float SensitivityZ = 25.0f;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Character|Gamepad|Sensitivity")
		float SensitivityY = 45.0f;

	UFUNCTION(BlueprintPure, Category = "Character|Mouse|Sensitivity")
		float GetMouseSens() const
		{
			UKonsole::PrintToConsole("Character", "Warning", FString::Printf(TEXT("Sensitivity: %f"), Sensitivity));
			return Sensitivity;	
		}
	UFUNCTION(BlueprintPure, Category = "Character|Gamepad|Sensitivity")
		float GetSensZ() const
		{
			UKonsole::PrintToConsole("Character", "Warning", FString::Printf(TEXT("Z Sensitivity: %f"), SensitivityZ));
			return SensitivityZ;
		}
	UFUNCTION(BlueprintPure, Category = "Character|Gamepad|Sensitivity")
		float GetSensY() const
		{
			UKonsole::PrintToConsole("Character", "Warning", FString::Printf(TEXT("X Sensitivity: %f"), SensitivityY));
			return SensitivityY;
		}

public:
	bool IsMoving();
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Character|Movement|Speed")
		float BaseWalkSpeed = 250.0f;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Character|Movement|Speed")
		float BaseWalkSpeedMultiplier = 2.2f;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Character|Movement|Speed")
		float BaseCrouchSpeed = 75.0f;

	UFUNCTION(BlueprintCallable, Category = "Character|Movement")
		void VerticalMove(float Value);
	UFUNCTION(BlueprintCallable, Category = "Character|Movement")
		void HorizontalMove(float Value);
	UFUNCTION(BlueprintCallable, Category = "Character|Camera")
		void VerticalLook(float Axis);
	UFUNCTION(BlueprintCallable, Category = "Character|Camera")
		void VerticalLookOnController(float Axis);
	UFUNCTION(BlueprintCallable, Category = "Character|Camera")
		void HorizontalLook(float Axis);
	UFUNCTION(BlueprintCallable, Category = "Character|Camera")
		void HorizontalLookOnController(float Axis);
	
	bool bDevWalk = false;
	UFUNCTION(BlueprintCallable, Exec, Category = "Character|Cheats")
		void DevCam(int Mode); // 0 = disable, 1 = enable
	UFUNCTION(BlueprintCallable, Exec, Category = "Character|Cheats")
		void SetAllowCheats(int Mode); // 0 = disable, 1 = enable

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Preferences|Gameplay")
		bool bAllowDash = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Preferences|Gameplay")
		bool bAllowGrab = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Preferences|Gameplay")
		bool bAllowFlash = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Preferences|Gameplay")
		bool bAllowWallrun = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Preferences|Gameplay")
		bool bUseSmoothCrouch = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Preferences|Gameplay")
		bool bAllowGrapplingHook = false;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Status")
		bool bIsHookSuppressed;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Status")
		bool bIsCrouching = false;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Status")
		bool bIsHolding = false;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Status")
		bool bIsGrappling = false;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Status")
		bool bIsOnWall = false;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Status")
		bool bIsFlashOn = false;


protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character|Interaction")
		float GrabDistance = 256.0f;
		float OutlineRange = GrabDistance;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character|Interaction")
		float OutlineRadius = 128.0f;

	UFUNCTION(BlueprintCallable, Category = "PhysicsInteraction")
		void TraceGrab(bool& ReturnValue, FVector& ReturnLocation);
	UFUNCTION(BlueprintCallable, Category = "PhysicsInteraction")
		void GrabLocation();
	UFUNCTION(BlueprintCallable, Category = "PhysicsInteraction")
		void StopGrab(UPrimitiveComponent* GrabbedObject);
	
	UFUNCTION(BlueprintCallable, Category = "Abilities|GrapplingHook")
		void BeginGrapple();
	UFUNCTION(BlueprintCallable, Category = "Abilities|GrapplingHook")
		void TickGrapple();
	UFUNCTION(BlueprintCallable, Category = "Abilities|GrapplingHook")
		bool TraceGrapple(float Distance);
	UFUNCTION(BlueprintCallable, Category = "Abilities|GrapplingHook")
		void StopGrapple();
	UFUNCTION(BlueprintCallable, Category = "Abilities|GrapplingHook")
		void SetGrappleVisibility(bool Visibility);

	UFUNCTION(BlueprintCallable, Category = "Abilities|Sprint/Dash")
		void BeginSprint();
	UFUNCTION(BlueprintCallable, Category = "Abilities|Sprint/Dash")
		void StopSprint();
	UFUNCTION(BlueprintCallable, Category = "Abilities|Sprint/Dash")
		void BeginDash(float AxisV, float AxisH, int Cost);
	UFUNCTION(BlueprintCallable, Category = "Abilities|Sprint/Dash")
		void DashVertically(float AxisV, int Value);
	UFUNCTION(BlueprintCallable, Category = "Abilities|Sprint/Dash")
		void DashHorizontally(float AxisH, int Value);
};
