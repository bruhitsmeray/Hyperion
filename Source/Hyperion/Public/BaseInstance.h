// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BaseInstance.generated.h"

/**
 * 
 */
UCLASS()
class HYPERION_API UBaseInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "General")
		bool bUseOnlineFeatures;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "General")
		bool bUseAchievements;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "General")
		bool bUseFriendsList;

	UFUNCTION(Exec, BlueprintCallable, Category = "Client|Online|Login")
		void Login();
		void OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);

	UFUNCTION(Exec, BlueprintCallable, Category = "Client|Friends")
		void GetAllFriends();
		void OnGetAllFriendsComplete(int32 LocalUserNumber, bool bWasSuccessful, const FString& ListName, const FString& ErrorStr);

	UFUNCTION(Exec, BlueprintCallable, Category = "Client|UI")
		void ShowFriendsUI();
	UFUNCTION(Exec, BlueprintCallable, Category = "Client|UI")
		void ShowAchievementsUI();
};
