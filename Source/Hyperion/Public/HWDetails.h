// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HWDetails.generated.h"

/**
 * 
 */
UCLASS()
class HYPERION_API UHWDetails : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(Exec, BlueprintPure, meta = (DisplayName = "Get CPU brand", Keywords = "CPU brand"), Category = "Hardware Info|CPU")
		static FString GetCPUBrand();
	UFUNCTION(Exec, BlueprintPure, meta = (DisplayName = "Get GPU brand", Keywords = "GPU brand"), Category = "Hardware Info|GPU")
		static FString GetGPUBrand();
	UFUNCTION(Exec, BlueprintPure, meta = (DisplayName = "Get RHI name", Keywords = "RHI name"), Category = "Hardware Info")
		static FString GetRHI();
	UFUNCTION(Exec, BlueprintPure, meta = (DisplayName = "Get CPU cores", Keywords = "CPU cores"), Category = "Hardware Info|CPU")
		static int32 GetCPUCores();
	UFUNCTION(Exec, BlueprintPure, meta = (DisplayName = "Get CPU threads", Keywords = "CPU threads"), Category = "Hardware Info|CPU")
		static int32 GetCPUThreads();
	UFUNCTION(Exec, BlueprintPure, meta = (DisplayName = "Get RAM capacity", Keywords = "RAM capacity"), Category = "Hardware Info|RAM")
		static int32 GetRAMCapacity();
	UFUNCTION(Exec, BlueprintPure, Category = "Hardware Info")
		static bool IsWindows();
};
