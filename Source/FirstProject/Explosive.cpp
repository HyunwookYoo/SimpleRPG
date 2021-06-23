// Fill out your copyright notice in the Description page of Project Settings.


#include "Explosive.h"

#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Enemy.h"

AExplosive::AExplosive()
{
	Damage = 15.f;
}

void AExplosive::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (!ensure(OtherActor != nullptr)) return;

	AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);
	AEnemy* Enemy = Cast<AEnemy>(OtherActor);
	
	if (Main || Enemy)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OverlapParticles, GetActorLocation());
		UGameplayStatics::PlaySound2D(this, OverlapSound);
		
		UGameplayStatics::ApplyDamage(OtherActor, Damage, nullptr, this, DamageTypeClass);

		Destroy();
	}
}

void AExplosive::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	UE_LOG(LogTemp, Warning, TEXT("Explosive::OnOverlapEnd()"));
}
