// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

class UUserWidget;

/**
 * 
 */
UCLASS()
class FIRSTPROJECT_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	void DisplayEnemyHealthBar();
	void RemoveEneyHealthBar();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void DisplayPauseMenu();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
	void RemovePauseMenu();

	void TogglePauseMenu();

	void GameModeOnly();

	/** Setter */
	FORCEINLINE void SetEnemyLocation(FVector Location) { EnemyLocation = Location; }

	/** Getter */
	FORCEINLINE bool GetEnemyHealthBarVisible() { return bEnemyHealthBarVisible; }
	FORCEINLINE bool GetPauseMenuVisible() { return bPauseMenuVisible; }

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

private:

	/** Reference to the UMG asset in the editor */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> HUDOverlayAsset;

	/** Variable to hold the widget after creating it */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	UUserWidget* HUDOverlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> WEneyHealthBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	UUserWidget* EnemyHealthBar;

	bool bEnemyHealthBarVisible;

	FVector EnemyLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> WPauseMenu;

	/** Variable to hold the widget after creating it */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	UUserWidget* PauseMenu;

	bool bPauseMenuVisible;
};
