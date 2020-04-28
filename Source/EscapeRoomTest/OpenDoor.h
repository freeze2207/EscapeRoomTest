// Zihe Zhang Lab

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "Components/ActorComponent.h"
#include "Components/AudioComponent.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPEROOMTEST_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	float InitialYaw;
	float CurrentYaw;
	float DoorLastOpended = 0.0f;
	float DoorClosedDeplay = 0.5f;

	bool ShouldPlayDoorOpenSound = true;
	bool ShouldPlayDoorCloseSound = false;

	FRotator DoorRotation{ 0.0f, 0.0f, 0.0f };

	UPROPERTY(EditAnywhere)
		float OpenAngle = 90.0f;
	UPROPERTY(EditAnywhere)
		ATriggerVolume* PressurePlate = nullptr;
	UPROPERTY()
		UAudioComponent* AudioComponent = nullptr;
	/*UPROPERTY(EditAnywhere)
		AActor* ActorThatOpens;*/
	UPROPERTY(EditAnywhere)
		float OpenSpeed = 25.f;
	UPROPERTY(EditAnywhere)
		float CloseSpeed = 75.f;
	UPROPERTY(EditAnywhere)
		float MassRequiredToOpen = 70.f;

	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	void FindAudioComponent();
	void CheckPressurePlate() const;
	float TotalMassOverlappedInTrigger() const;
};
