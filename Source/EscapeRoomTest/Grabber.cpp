// Zihe Zhang Lab

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	CheckPhysicsHandleReady();

	FindInputComponent();
	
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/*Debug use: for tracer visual*/
	/*GetCurrentViewportLocationAndRotation();
	DrawDebugLine(
		GetWorld(),
		CurrentViewportLocation,
		GetLineTraceEnd(),
		FColor(0, 255, 0),
		false,
		0.0f,
		0,
		3.0f
	);*/
	
	// If the physic handle is attach.
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("Physics Handle null on TickComponent"));
		return;
	}
	
	if (PhysicsHandle->GrabbedComponent)
	{
		// Move the object we are holding.
		PhysicsHandle->SetTargetLocation(GetLineTraceEnd());
	}

}

void UGrabber::Grab() {

	/*UE_LOG(LogTemp, Warning, TEXT("Grab action called"));*/
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();

	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("Physics Handle null on Grab"));
		return;
	}

	if (HitResult.GetActor()) {
		PhysicsHandle->GrabComponentAtLocation(ComponentToGrab, NAME_None, GetLineTraceEnd());
	}
	
}

void UGrabber::GrabReleased() {

	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("Physics Handle null on Grab"));
		return;
	}
	/*UE_LOG(LogTemp, Warning, TEXT("Grab action released"));*/
	PhysicsHandle->ReleaseComponent();
}

void UGrabber::CheckPhysicsHandleReady() {
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("The Object %s has no PhysicsHandle attached"), *GetOwner()->GetName());
		return;
	}
	
}

void UGrabber::FindInputComponent() {
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::GrabReleased);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("The Object %s has no InputComponent attached"), *GetOwner()->GetName());
		return;
	}
}

void UGrabber::GetCurrentViewportLocationAndRotation() {

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT CurrentViewportLocation, OUT CurrentViewportRotation);
}

FVector UGrabber::GetLineTraceEnd() {
	GetCurrentViewportLocationAndRotation();
	return CurrentViewportLocation + CurrentViewportRotation.Vector() * Reach;
}


FHitResult UGrabber::GetFirstPhysicsBodyInReach(){
	
	GetCurrentViewportLocationAndRotation();

	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		CurrentViewportLocation,
		GetLineTraceEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);
	/*if (Hit.GetActor()) {
		UE_LOG(LogTemp, Warning, TEXT("Now we are hitting the %s"), *Hit.GetActor()->GetName());
	}*/

	return Hit;
}
