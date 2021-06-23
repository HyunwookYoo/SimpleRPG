// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "Components/SphereComponent.h"
#include "AIController.h"
#include "MainCharacter.h"
#include "DrawDebugHelpers.h"
#include "Components/BoxComponent.h"
#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "TimerManager.h"
#include "Components/CapsuleComponent.h"
#include "MainPlayerController.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AgroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AgroSphere"));
	AgroSphere->SetupAttachment(GetRootComponent());
	AgroSphere->InitSphereRadius(600.f);
	AgroSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);

	CombatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CombatSphere"));
	CombatSphere->SetupAttachment(GetRootComponent());
	CombatSphere->InitSphereRadius(90.f);

	CombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CombatCollision"));
	CombatCollision->SetupAttachment(GetMesh(), FName("EnemyAttackLeftSocket"));

	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	bOverlappingCombatSphere = false;

	MaxHealth = 100.f;
	Health = 75.f;
	Damage = 10.f;

	bAttacking = false;

	AttackMinTime = 0.5f;

	AttackMaxTime = 3.5f;

	EnemyMovementStatus = EEnemyMovementStatus::EMS_Idle;

	DeathDelay = 3.f;

	bHasValidTarget = false;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	AIController = Cast<AAIController>(GetController());

	AgroSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::AgroSphereOnOverlapBegin);
	AgroSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::AgroSphereOnOverlapEnd);

	CombatSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapBegin);
	CombatSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapEnd);

	CombatCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::ComBatOnOverlapBegin);
	CombatCollision->OnComponentEndOverlap.AddDynamic(this, &AEnemy::ComBatOnOverlapEnd);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!ensure(OtherActor != nullptr)) return;
	
	AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);
	if (Main && Alive())
	{
		MoveToTarget(Main);
	}
}

void AEnemy::AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!ensure(OtherActor != nullptr)) return;

	AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);
	if (Main)
	{
		bHasValidTarget = false;

		if (Main->GetCombatTarget() == this)
		{
			Main->SetCombatTarget(nullptr);
		}

		Main->SetHasCombatTarget(false);

		Main->UpdateCombatTarget();

		SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);
		if (!ensure(AIController != nullptr)) return;
		AIController->StopMovement();
	}
}

void AEnemy::CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!ensure(OtherActor != nullptr)) return;

	AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);
	if (Main && Alive())
	{
		bHasValidTarget = true;


		Main->SetCombatTarget(this);
		Main->SetHasCombatTarget(true);

		if (!ensure(Main->GetMainPlayerController() != nullptr)) return;
		Main->UpdateCombatTarget();

		CombatTarget = Main;
		bOverlappingCombatSphere = true;
		float AttackTime = FMath::RandRange(AttackMinTime, AttackMaxTime);
		GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
	}
}

void AEnemy::CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!ensure(OtherActor != nullptr)) return;

	AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);
	if (Main && OtherComp)
	{
		bOverlappingCombatSphere = false;
		MoveToTarget(Main);
		CombatTarget = nullptr;

		if (Main->GetCombatTarget() == this)
		{
			Main->SetCombatTarget(nullptr);
			Main->SetHasCombatTarget(false);
			Main->UpdateCombatTarget();
		}

		if (Main->GetMainPlayerController())
		{
			USkeletalMeshComponent* MainMesh = Cast<USkeletalMeshComponent>(OtherComp);
			if (MainMesh) Main->GetMainPlayerController()->RemoveEneyHealthBar();
		}
		
		GetWorldTimerManager().ClearTimer(AttackTimer);
	}
}

void AEnemy::MoveToTarget(AMainCharacter* Main)
{
	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveToTarget);

	if (!ensure(AIController != nullptr)) return;

	UE_LOG(LogTemp, Warning, TEXT("MoveToTarget"));

	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Main);
	MoveRequest.SetAcceptanceRadius(25.f);

	FNavPathSharedPtr NavPath;

	AIController->MoveTo(MoveRequest, &NavPath);

	/**
	TArray<FNavPathPoint> PathPoints = NavPath->GetPathPoints();
	for (auto& Point : PathPoints)
	{
		FVector Location = Point.Location;

		DrawDebugSphere(GetWorld(), Location, 25.f, 8, FColor::Green, false, 10.f, (uint8)5, .5f);
	}
	*/
}

void AEnemy::ComBatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!ensure(OtherActor != nullptr)) return;

	AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);
	if (Main)
	{
		if (!ensure(Main->GetHitParticle() != nullptr)) return;
		UGameplayStatics::SpawnEmitterAttached(Main->GetHitParticle(), GetMesh(), FName("TipSocket"));

		if (!ensure(Main->GetHitSound() != nullptr)) return;
		UGameplayStatics::PlaySound2D(this, Main->GetHitSound());

		if (!ensure(DamageTypeClass != nullptr)) return;
		UGameplayStatics::ApplyDamage(Main, Damage, AIController, this, DamageTypeClass);
	}
}

void AEnemy::ComBatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AEnemy::ActivateCollision()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	if (!ensure(SwingSound != nullptr)) return;
	UGameplayStatics::PlaySound2D(this, SwingSound);
}

void AEnemy::DeactivateCollision()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemy::Attack()
{
	if (!ensure(AIController != nullptr)) return;

	if (Alive() && bHasValidTarget)
	{
		AIController->StopMovement();
		SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Attacking);

		if (!bAttacking)
		{
			bAttacking = true;
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (!ensure(AnimInstance != nullptr)) return;

			AnimInstance->Montage_Play(CombatMontage, 1.35f);
			AnimInstance->Montage_JumpToSection(FName("Attack"), CombatMontage);
		}
	}
}

void AEnemy::AttackEnd()
{
	bAttacking = false;

	if (bOverlappingCombatSphere)
	{
		float AttackTime = FMath::RandRange(AttackMinTime, AttackMaxTime);
		GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
	}
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Health = FMath::Clamp(Health - DamageAmount, 0.f, MaxHealth);

	if (Health <= 0.f)
	{
		Die(DamageCauser);
	}

	return DamageAmount;
}

void AEnemy::Die(AActor* Causer)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!ensure(AnimInstance != nullptr)) return;
	if (!ensure(CombatMontage != nullptr)) return;

	AnimInstance->Montage_Play(CombatMontage);
	AnimInstance->Montage_JumpToSection(FName("Death"));

	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Dead);

	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AgroSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bAttacking = false;

	AMainCharacter* Main = Cast<AMainCharacter>(Causer);
	if (Main)
	{
		Main->UpdateCombatTarget();
	}
}

void AEnemy::DeathEnd()
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;

	GetWorldTimerManager().SetTimer(DeathTimer, this, &AEnemy::Disappear, DeathDelay);
}

bool AEnemy::Alive()
{
	return GetEnemyMovementStatus() != EEnemyMovementStatus::EMS_Dead;
}

void AEnemy::Disappear()
{
	Destroy();
}