// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseHUD.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Canvas.h"

void ABaseHUD::DrawHUD()
{
	Super::DrawHUD();
	DrawCrosshair(Canvas->SizeX, Canvas->SizeY);
}

void ABaseHUD::DrawCrosshair(int X, int Y)
{
	DrawRect(FLinearColor(255, 255, 255), X/2, Y/2, 2, 2);
}

void ABaseHUD::DrawWidget(TSubclassOf<UUserWidget> Class)
{
	UUserWidget* WidgetHUD = CreateWidget<UUserWidget>(GetGameInstance(), Class);
	if(Class)
	{
		WidgetHUD->AddToViewport();
	}
}
