// Zihe Zhang Lab

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPEROOMTEST_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
		float Reach = 70.0f;
	UPROPERTY()
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UPROPERTY()
	UInputComponent* InputComponent = nullptr;

	FVector CurrentViewportLocation;
	FRotator CurrentViewportRotation;

	void Grab();
	void GrabReleased();
	void CheckPhysicsHandleReady();
	void FindInputComponent();
	void GetCurrentViewportLocationAndRotation();
	FVector GetLineTraceEnd();
	FHitResult GetFirstPhysicsBodyInReach();

};
