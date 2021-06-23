// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"

#include "Blueprint/UserWidget.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!ensure(HUDOverlayAsset != nullptr)) return;

	HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayAsset);
	HUDOverlay->AddToViewport();
	HUDOverlay->SetVisibility(ESlateVisibility::Visible);

	if (!ensure(WEneyHealthBar != nullptr)) return;
	
	EnemyHealthBar = CreateWidget<UUserWidget>(this, WEneyHealthBar);
	if (!ensure(EnemyHealthBar != nullptr)) return;

	EnemyHealthBar->AddToViewport();
	EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);

	FVector2D Alignment(0.f, 0.f);
	EnemyHealthBar->SetAlignmentInViewport(Alignment);

	if (!ensure(WPauseMenu != nullptr)) return;

	PauseMenu = CreateWidget<UUserWidget>(this, WPauseMenu);
	if (!ensure(PauseMenu != nullptr)) return;

	PauseMenu->AddToViewport();
	PauseMenu->SetVisibility(ESlateVisibility::Hidden);

	bPauseMenuVisible = false;
}

void AMainPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (EnemyHealthBar)
	{
		FVector2D PositionInViewport;
		ProjectWorldLocationToScreen(EnemyLocation, PositionInViewport);
		PositionInViewport.Y -= 100.f;

		FVector2D SizeInViewport(300.f, 25.f);

		EnemyHealthBar->SetPositionInViewport(PositionInViewport);
		EnemyHealthBar->SetDesiredSizeInViewport(SizeInViewport);
	}

}

void AMainPlayerController::DisplayEnemyHealthBar()
{
	if (!ensure(EnemyHealthBar != nullptr)) return;

	bEnemyHealthBarVisible = true;
	EnemyHealthBar->SetVisibility(ESlateVisibility::Visible);
}

void AMainPlayerController::RemoveEneyHealthBar()
{
	if (!ensure(EnemyHealthBar != nullptr)) return;

	bEnemyHealthBarVisible = false;
	EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
}

void AMainPlayerController::DisplayPauseMenu_Implementation()
{
	if (!ensure(PauseMenu != nullptr)) return;

	bPauseMenuVisible = true;
	PauseMenu->SetVisibility(ESlateVisibility::Visible);

	FInputModeGameAndUI InputModeGameAndUI;

	SetInputMode(InputModeGameAndUI);
	bShowMouseCursor = true;
}

void AMainPlayerController::RemovePauseMenu_Implementation()
{
	if (!ensure(PauseMenu != nullptr)) return;

	GameModeOnly();

	bPauseMenuVisible = false;
	
	bShowMouseCursor = false;
}

void AMainPlayerController::TogglePauseMenu()
{
	if (bPauseMenuVisible)
	{
		RemovePauseMenu();
	}
	else
	{
		DisplayPauseMenu();
	}


}

void AMainPlayerController::GameModeOnly()
{
	FInputModeGameOnly InputModeGameOnly;

	SetInputMode(InputModeGameOnly);
}

