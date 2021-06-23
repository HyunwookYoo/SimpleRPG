// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UParticleSystemComponent;
class UParticleSystem;
class USoundCue;

UCLASS()
class FIRSTPROJECT_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	FORCEINLINE void SetRotate(bool bIsRotate) { bRotate = bIsRotate; }

	void DeactivateIdleParticle();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Particles", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* OverlapParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sounds", meta = (AllowPrivateAccess = "true"))
	USoundCue* OverlapSound;

private:	
	/** Base shape collision */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item | Collision", meta = (AllowPrivateAccess = "true"))
	USphereComponent* CollisionVolume;

	/** Base mesh component */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item | Mesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Particles", meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* IdleParticlesComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | ItemProperties", meta = (AllowPrivateAccess = "true"))
	bool bRotate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | ItemProperties", meta = (AllowPrivateAccess = "true"))
	float RotationRate;
};
