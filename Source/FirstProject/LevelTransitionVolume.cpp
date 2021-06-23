// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelTransitionVolume.h"

#include "Components/BoxComponent.h"
#include "Components/BillboardComponent.h"
#include "MainCharacter.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ALevelTransitionVolume::ALevelTransitionVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TransitionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TransitionVolume"));
	SetRootComponent(TransitionVolume);

	IdleParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("IdleParticle"));
	IdleParticle->SetupAttachment(TransitionVolume);

	TransitionLevelName = TEXT("SunTemple");
}

// Called when the game starts or when spawned
void ALevelTransitionVolume::BeginPlay()
{
	Super::BeginPlay();

	TransitionVolume->OnComponentBeginOverlap.AddDynamic(this, &ALevelTransitionVolume::OnOverlapBegin);
	
}

// Called every frame
void ALevelTransitionVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelTransitionVolume::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!ensure(OtherActor != nullptr)) return;

	AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);
	if (!Main) return;

	Main->SwitchLevel(TransitionLevelName);
}
