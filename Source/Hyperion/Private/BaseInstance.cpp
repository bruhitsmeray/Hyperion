// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseInstance.h"
#include "CSL_Window.h"

#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlineExternalUIInterface.h"
#include "Interfaces/OnlineFriendsInterface.h"

IOnlineSubsystem* OnlineSubsystem;
bool bIsLoggedIn = false;
FString F_Name;

void UBaseInstance::Init()
{
	Super::Init();
	OnlineSubsystem = IOnlineSubsystem::Get();
}

void UBaseInstance::Login()
{
	if(bUseOnlineFeatures && OnlineSubsystem)
	{
		if(IOnlineIdentityPtr Identity  = OnlineSubsystem->GetIdentityInterface())
		{
			FOnlineAccountCredentials Credentials;
			Credentials.Id = FString("localhost:6300");
			Credentials.Token = FString("Context_1");
			Credentials.Type = FString("developer");

			Identity->OnLoginCompleteDelegates->AddUObject(this, &UBaseInstance::OnLoginComplete);
			Identity->Login(0, Credentials);
		}
	} else {
		UCSL_Window::PrintToConsole("Instance", "Warning", "Please check the UseOnlineFeatures box to be able to login.");
	}
}

void UBaseInstance::OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId,
	const FString& Error)
{
	bIsLoggedIn = true;
	if(OnlineSubsystem)
	{
		if(IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface())
		{
			Identity->ClearOnLoginCompleteDelegates(0, this);
		}
	}
}

void UBaseInstance::GetAllFriends()
{
	if(bUseOnlineFeatures && bUseFriendsList)
	{
		if(bIsLoggedIn && OnlineSubsystem)
		{
			if(IOnlineFriendsPtr FriendsPtr = OnlineSubsystem->GetFriendsInterface())
			{
				FriendsPtr->ReadFriendsList(0, FString(""), FOnReadFriendsListComplete::CreateUObject(this, &UBaseInstance::OnGetAllFriendsComplete));
			}
		}
	} else {
		UCSL_Window::PrintToConsole("Instance", "Warning", "Please check the UseOnlineFeatures and UseFriendsList boxes to be able to see the Friends list.");
	}
}

void UBaseInstance::OnGetAllFriendsComplete(int32 LocalUserNumber, bool bWasSuccessful, const FString& ListName,
	const FString& ErrorStr)
{
	if(bWasSuccessful && OnlineSubsystem)
	{
		if(IOnlineFriendsPtr FriendsPtr = OnlineSubsystem->GetFriendsInterface())
		{
			TArray<TSharedRef<FOnlineFriend>> FriendsList;
			if(FriendsPtr->GetFriendsList(0, ListName, FriendsList))
			{
				for(TSharedRef<FOnlineFriend> Friend : FriendsList)
				{
					F_Name = Friend.Get().GetRealName();
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%s"), *F_Name));
				}
			} else {
				UCSL_Window::PrintToConsole("Instance", "Warning", "Failed to retrieve the friends list.");
			}
		}
	}
}

void UBaseInstance::ShowFriendsUI()
{
	if(bUseFriendsList && bIsLoggedIn && OnlineSubsystem)
	{
		if(IOnlineExternalUIPtr UIPtr = OnlineSubsystem->GetExternalUIInterface())
		{
			UIPtr->ShowFriendsUI(0);
		}
	} else {
		UCSL_Window::PrintToConsole("Instance", "Warning", "Please check the UseOnlineFeatures and UseFriendsList boxes to be able to see the Friends UI.");
	}
}

void UBaseInstance::ShowAchievementsUI()
{
	if(bUseAchievements && bIsLoggedIn && OnlineSubsystem)
	{
		if(IOnlineExternalUIPtr UIPtr = OnlineSubsystem->GetExternalUIInterface())
		{
			UIPtr->ShowAchievementsUI(0);
		}
	} else {
		UCSL_Window::PrintToConsole("Instance", "Warning", "Please check the UseOnlineFeatures and UseAchievements boxes to be able to see the Achievements UI.");
	}
}
