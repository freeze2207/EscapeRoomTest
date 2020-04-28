// Zihe Zhang Lab

#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"


#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	OpenAngle += InitialYaw;

	CheckPressurePlate();
	
	FindAudioComponent();
	/*ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();*/
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TotalMassOverlappedInTrigger() >= MassRequiredToOpen) {
		OpenDoor(DeltaTime);
		DoorLastOpended = GetWorld()->GetTimeSeconds();
	}
	else if ( GetWorld() -> GetTimeSeconds() - DoorLastOpended > DoorClosedDeplay) {
		CloseDoor(DeltaTime);
	}

}

void UOpenDoor::OpenDoor(float DeltaTime) {
	
	CurrentYaw = FMath::FInterpConstantTo(CurrentYaw, OpenAngle, DeltaTime, OpenSpeed);
	/*UE_LOG(LogTemp, Warning, TEXT("Yaw is %f"), OpenDoor.Yaw);*/
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);
	ShouldPlayDoorCloseSound = true;
	if (!AudioComponent) {
		return;
	}
	if (ShouldPlayDoorOpenSound) {
		AudioComponent->Play();
		ShouldPlayDoorOpenSound = false;
	}
	
	
}

void UOpenDoor::CloseDoor(float DeltaTime) {
	CurrentYaw = FMath::FInterpConstantTo(CurrentYaw, InitialYaw, DeltaTime, CloseSpeed);
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);
	ShouldPlayDoorOpenSound = true;
	if (!AudioComponent) {
		return;
	}
	if (ShouldPlayDoorCloseSound && CurrentYaw == InitialYaw) {
		AudioComponent->Play();
		ShouldPlayDoorCloseSound = false;
	}
	
}

float UOpenDoor::TotalMassOverlappedInTrigger() const {

	float TotalMass = 0.0f;

	// Find All Overlapping Actors
	TArray<AActor*> OverlappingActors; 

	CheckPressurePlate();

	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	for (AActor* Actor : OverlappingActors) {
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	/*UE_LOG(LogTemp, Warning, TEXT("The total mass overlapping in trigger is: %f"), TotalMass);*/
	return TotalMass;
}

void  UOpenDoor::CheckPressurePlate() const{
	if (!PressurePlate) {
		UE_LOG(LogTemp, Warning, TEXT("The Actor %s has OpenDoor, but no PressurePlate"), *GetOwner()->GetName());
		return;
	}
}

void UOpenDoor::FindAudioComponent() {
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (!AudioComponent) {
		UE_LOG(LogTemp, Error, TEXT("%s has no AudioComponent attached"), *GetOwner()->GetName());
	}
}