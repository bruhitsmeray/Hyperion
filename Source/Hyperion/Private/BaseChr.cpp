// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseChr.h"
#include "Vitals.h"

#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

bool bAllowCheats = false;

int DashMode; // 0 = walking, 1 = sprinting, 2 = dashing
float DashAxis;

bool IsWalkingV;
bool IsWalkingH;

FVector GrappleLocation;
float GrappleDistance = 3072.0f;

// Sets default values
ABaseChr::ABaseChr()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanJump = true;
	GetCharacterMovement()->MaxWalkSpeedCrouched = BaseCrouchSpeed;
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	GetCapsuleComponent()->SetCapsuleHalfHeight(90.0f);

	JumpMaxCount = 2;
	JumpMaxHoldTime = 0.1f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
	Camera->bUsePawnControlRotation = true;
	Camera->SetWorldLocation(FVector(0,0,70));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Camera);
	SpringArm->TargetArmLength = 0.0f;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraRotationLagSpeed = 12.0f;

	InnerLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("InnerLight"));
	InnerLight->SetupAttachment(SpringArm);
	InnerLight->AttenuationRadius = 2500.0f;
	InnerLight->InnerConeAngle = 16.0f;
	InnerLight->OuterConeAngle = 24.0f;
	InnerLight->SetVisibility(false);

	OuterLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("OuterLight"));
	OuterLight->SetupAttachment(SpringArm);
	OuterLight->Intensity = 2500.0f;
	OuterLight->AttenuationRadius = 2500.0f;
	OuterLight->InnerConeAngle = 32.0f;
	OuterLight->OuterConeAngle = 48.0f;
	OuterLight->SetVisibility(false);
	
	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));
}

// Called when the game starts or when spawned
void ABaseChr::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABaseChr::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickGrapple();
}

// Called to bind functionality to input
void ABaseChr::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(MoveForwardBind, this, &ABaseChr::VerticalMove);
	PlayerInputComponent->BindAxis(MoveSideBind, this, &ABaseChr::HorizontalMove);
	PlayerInputComponent->BindAxis(VerticalLookBind, this, &ABaseChr::VerticalLook);
	PlayerInputComponent->BindAxis(VerticalLookOnControllerBind, this, &ABaseChr::VerticalLookOnController);
	PlayerInputComponent->BindAxis(HorizontalLookBind, this, &ABaseChr::HorizontalLook);
	PlayerInputComponent->BindAxis(HorizontalLookOnControllerBind, this, &ABaseChr::HorizontalLookOnController);

	PlayerInputComponent->BindAction(GrappleBind, IE_Pressed, this, &ABaseChr::BeginGrapple);
	PlayerInputComponent->BindAction(GrappleBind, IE_Released, this, &ABaseChr::StopGrapple);
}

bool ABaseChr::IsMoving()
{
	if((InputComponent->GetAxisValue(MoveForwardBind) > 0 || InputComponent->GetAxisValue(MoveForwardBind) < 0)
		|| (InputComponent->GetAxisValue(MoveSideBind) > 0 || InputComponent->GetAxisValue(MoveSideBind) < 0))
		{
			return true;
		}
	return false;
}

void ABaseChr::VerticalMove(float Value)
{
	if(Value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Value);
		IsWalkingV = true;
	} else {
		IsWalkingV = false;
	}
}

void ABaseChr::HorizontalMove(float Value)
{
	if(Value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Value);
		IsWalkingH = true;
	} else {
		IsWalkingH = false;
	}
}

void ABaseChr::VerticalLook(float Axis)
{
	AddControllerPitchInput(Axis * Sensitivity);
	GrabLocation();
}

void ABaseChr::VerticalLookOnController(float Axis)
{
	AddControllerPitchInput(Axis * SensitivityY * GetWorld()->GetDeltaSeconds());
	GrabLocation();
}

void ABaseChr::HorizontalLook(float Axis)
{
	AddControllerYawInput(Axis * Sensitivity);
	GrabLocation();
}

void ABaseChr::HorizontalLookOnController(float Axis)
{
	AddControllerYawInput(Axis * SensitivityZ * GetWorld()->GetDeltaSeconds());
	GrabLocation();
}

void ABaseChr::DevCam(int Mode)
{
	if(bAllowCheats)
	{
		if(!Mode && !bDevWalk || Mode == 1)
		{
			bDevWalk = true;
			GetCharacterMovement()->SetMovementMode(MOVE_Flying);
			SetActorEnableCollision(false);
			UCSL_Window::PrintToConsole("BaseChr", "Warning", "DevCam is now ON.");
		} else if(!Mode && bDevWalk || Mode == 0) {
			bDevWalk = false;
			GetCharacterMovement()->SetMovementMode(MOVE_Walking);
			SetActorEnableCollision(true);
			UCSL_Window::PrintToConsole("BaseChr", "Warning", "DevCam is now OFF.");
		}	
	} else {
		UCSL_Window::PrintToConsole("BaseChr", "Warning", "DevCam needs AllowCheats set to 1. Please set AllowCheats to 1 and try again!");
	}
}

void ABaseChr::SetAllowCheats(int Mode)
{
	if(Mode == 1)
	{
		bAllowCheats = true;
		UCSL_Window::PrintToConsole("BaseChr", "Notice", FString::Printf(TEXT("AllowCheats changed to %i"), Mode));
	} else if(Mode == 0) {
		bAllowCheats = false;
		UCSL_Window::PrintToConsole("BaseChr", "Notice", FString::Printf(TEXT("AllowCheats changed to %i"), Mode));
	}
}

void ABaseChr::TraceGrab(bool& ReturnValue, FVector& ReturnLocation)
{
	if(UWorld* World = GetWorld())
	{
		FVector CamLoc;
		FRotator CamRot;
		FHitResult HitResult;

		GetController()->GetPlayerViewPoint(CamLoc, CamRot);

		FVector Start = CamLoc;
		FVector End = (CamRot.Vector() * GrabDistance) + Start;

		FCollisionQueryParams TraceParams;
		bool bHit = World->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, TraceParams);

		if(bHit)
		{
			UCSL_Window::PrintToConsole("BaseChr", "Warning", "A hit has been registered.");
			if(IsValid(HitResult.GetComponent()) && HitResult.GetComponent()->IsSimulatingPhysics())
			{
				ReturnValue = bHit;
				HitComponent = HitResult.GetComponent();
				ReturnLocation = HitResult.GetComponent()->GetComponentLocation();
			} else {
				UCSL_Window::PrintToConsole("BaseChr", "Warning", "Component not available or does not simulate physics.");
			}
		} else {
			UCSL_Window::PrintToConsole("BaseChr", "Warning", "No hit registered.");
		}
	} else {
		UCSL_Window::PrintToConsole("BaseChr", "Warning", "World is not available.");
	}
}

void ABaseChr::GrabLocation()
{
	PhysicsHandle->SetTargetLocation(Camera->GetComponentLocation() + (Camera->GetForwardVector() * GrabDistance));
	if(bIsHolding && IsValid(HitComponent))
	{
		HitComponent->SetWorldRotation(GetControlRotation(), false, nullptr);
	}
}

void ABaseChr::StopGrab(UPrimitiveComponent* GrabbedObject)
{
	if(IsValid(GrabbedObject))
	{
		GrabbedObject->SetAngularDamping(0.0f);
		PhysicsHandle->ReleaseComponent();
		bIsHolding = false;
	}
}

void ABaseChr::BeginGrapple()
{
	TraceGrapple(GrappleDistance);
	if(bAllowGrapplingHook && !bIsHookSuppressed)
	{
		bIsGrappling = true;
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		SetGrappleVisibility(true);
	}
}

void ABaseChr::TickGrapple()
{
	if(bAllowGrapplingHook && !bIsHookSuppressed && bIsGrappling)
	{
		UCableComponent* Cable = FindComponentByClass<UCableComponent>();
		if(IsValid(Cable))
		{
			GetCharacterMovement()->AddForce(UKismetMathLibrary::Normal(UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), GrappleLocation) + ((GetInputAxisValue(MoveSideBind) * GetActorRightVector()) * 0.5f)) * 300000.0f);
			Cable->EndLocation = UKismetMathLibrary::InverseTransformLocation(GetActorTransform(), GrappleLocation);
			if((UKismetMathLibrary::Vector_Distance(GrappleLocation, GetActorLocation()) <= 256.0f) || (UKismetMathLibrary::Vector_Distance(GrappleLocation, GetActorLocation()) >= 3072.0f))
			{
				StopGrapple();
			}
		}
	}
}

bool ABaseChr::TraceGrapple(float Distance)
{
	if(UWorld* World = GetWorld())
	{
		FVector CamLoc;
		FRotator CamRot;
		FHitResult HitResult;

		GetController()->GetPlayerViewPoint(CamLoc, CamRot);
		
		FVector Start = CamLoc;
		FVector End = (CamRot.Vector() * Distance) + Start;

		FCollisionQueryParams TraceParams;
		
		bool bHit = World->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, TraceParams);
		if(bHit && !HitResult.GetComponent()->IsSimulatingPhysics())
		{
			UCSL_Window::PrintToConsole("BaseChr", "Warning", "A hit has been registered.");
			GrappleLocation = HitResult.Location;
			if(UKismetMathLibrary::Vector_Distance(GrappleLocation, GetActorLocation()) > 3072.0f)
			{
				bIsHookSuppressed = true;
				UCSL_Window::PrintToConsole("BaseChr", "Warnings", "The registered hit is too far away from the player.");
				return false;
			}
			bIsHookSuppressed = false;
			return HitResult.GetComponent()->IsSimulatingPhysics(); // What is this doing? I'll experiment with this line a bit and get rid of it if it does nothing.
		}
		bIsHookSuppressed = true;
		UCSL_Window::PrintToConsole("BaseChr", "Warning", "No hit registered.");
		return false;
	}
	return false;
}

void ABaseChr::StopGrapple()
{
	if(bAllowGrapplingHook)
	{
		bIsGrappling = false;
		bIsHookSuppressed = true;
		GetCharacterMovement()->SetMovementMode(MOVE_Falling);
		SetGrappleVisibility(false);	
	}
}

void ABaseChr::SetGrappleVisibility(bool Visibility)
{
	UCableComponent* Cable = FindComponentByClass<UCableComponent>();
	if(IsValid(Cable))
	{
		Cable->SetVisibility(Visibility);
	} else {
		UCSL_Window::PrintToConsole("BaseChr", "Warning", "No cable component found! Please add a cable component to the Player Character blueprint in order to use the Grapple functions!");
	}
}

void ABaseChr::BeginSprint()
{
	if(IsMoving() && !bIsCrouching)
	{
		GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed * BaseWalkSpeedMultiplier;
		UCSL_Window::PrintToConsole("BaseChr", "Warning", FString::Printf(TEXT("MaxWalkSpeed is now at: %f (Sprinting/Dashing)."), GetCharacterMovement()->MaxWalkSpeed));
	}
}

void ABaseChr::StopSprint()
{
	if (GetCharacterMovement()->MaxWalkSpeed > BaseWalkSpeed)
	{
		GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
		UCSL_Window::PrintToConsole("BaseChr", "Warning", FString::Printf(TEXT("MaxWalkSpeed is now at: %f (Walking)."), GetCharacterMovement()->MaxWalkSpeed));
	}
}

void ABaseChr::BeginDash(float AxisV, float AxisH, int Cost)
{
	if(bAllowDash && IsMoving())
	{
		DashVertically(AxisV, Cost);
		DashHorizontally(AxisH, Cost);
	}
}

void ABaseChr::DashVertically(float AxisV, int Value)
{
	if(AxisV == 1.0f && IsWalkingV)
	{
		DashAxis = 5000.0f;
		if(!bIsCrouching || !bIsHolding || !bIsGrappling)
		{
			float LaunchX = GetActorForwardVector().X * DashAxis;
			float LaunchY = GetActorForwardVector().Y * DashAxis;
			float LaunchZ = GetActorForwardVector().Z;
			LaunchCharacter(UKismetMathLibrary::MakeVector(LaunchX, LaunchY, LaunchZ), false, false);
		}
	} else if(AxisV == -1.0f && IsWalkingV) {
		DashAxis = -5000.0f;
		if(!bIsCrouching || !bIsHolding || !bIsGrappling)
		{
			float LaunchX = GetActorForwardVector().X * DashAxis;
			float LaunchY = GetActorForwardVector().Y * DashAxis;
			float LaunchZ = GetActorForwardVector().Z;
			LaunchCharacter(UKismetMathLibrary::MakeVector(LaunchX, LaunchY, LaunchZ), false, false);
		}
	}
    UVitals::RemoveAuxPower(Value);
	UCSL_Window::PrintToConsole("BaseChr", "Warning", "DashVertically function called.");
}

void ABaseChr::DashHorizontally(float AxisH, int Value)
{
	if(AxisH == 1.0f && IsWalkingH)
	{
		DashAxis = 5000.0f;
		if(!bIsCrouching || !bIsHolding || !bIsGrappling)
		{
			float LaunchX = GetActorRightVector().X * DashAxis;
			float LaunchY = GetActorRightVector().Y * DashAxis;
			float LaunchZ = GetActorRightVector().Z;
			LaunchCharacter(UKismetMathLibrary::MakeVector(LaunchX, LaunchY, LaunchZ), false, false);
		}
	} else if(AxisH == -1.0f && IsWalkingH) {
		DashAxis = -5000.0f;
		if(!bIsCrouching || !bIsHolding || !bIsGrappling)
		{
			float LaunchX = GetActorRightVector().X * DashAxis;
			float LaunchY = GetActorRightVector().Y * DashAxis;
			float LaunchZ = GetActorRightVector().Z;
			LaunchCharacter(UKismetMathLibrary::MakeVector(LaunchX, LaunchY, LaunchZ), false, false);
		}
	}
	UVitals::RemoveAuxPower(Value);
	UCSL_Window::PrintToConsole("BaseChr", "Warning", "DashHorizontally function called");
}

