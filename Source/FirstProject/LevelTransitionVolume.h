// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelTransitionVolume.generated.h"

class UBoxComponent;
class UBillboardComponent;

UCLASS()
class FIRSTPROJECT_API ALevelTransitionVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelTransitionVolume();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Transition", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* TransitionVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Particles", meta = (AllowPrivateAccess =  "true"))
	UParticleSystemComponent* IdleParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Transition", meta = (AllowPrivateAccess = "true"))
	FName TransitionLevelName;
};
