// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BaseHUD.generated.h"

/**
 * 
 */
UCLASS()
class HYPERION_API ABaseHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "General", meta = (GetOptions = "Skins"))
		FString HUD = "Built-in";

	UFUNCTION(BlueprintCallable, Category = "HUD")
		TArray<FString> Skins() const
		{
			return {
				"Off", // Disables the HUD completely.
				"Built-in" // This is the built-in HUD of the plugin.
				"Custom", // Custom is going to be whatever HUD you want to use in your own game.
			};
		}

protected:
	virtual void DrawHUD() override;
	UFUNCTION(BlueprintCallable, Category = "HUD")
		void DrawCrosshair(int X, int Y);
	UFUNCTION(BlueprintCallable, Category = "HUD")
		void DrawWidget(TSubclassOf<UUserWidget> Class);
};
