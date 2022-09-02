// Fill out your copyright notice in the Description page of Project Settings.


#include "HWDetails.h"

#include "GenericPlatform/GenericPlatformApplicationMisc.h"
#include "GenericPlatform/GenericPlatformDriver.h"

FString UHWDetails::GetCPUBrand()
{
	return FPlatformMisc::GetCPUBrand();
}

FString UHWDetails::GetGPUBrand()
{
	return FPlatformMisc::GetPrimaryGPUBrand();
}

FString UHWDetails::GetRHI()
{
	FGPUDriverInfo DriverInfo;
	return DriverInfo.RHIName;
}

int32 UHWDetails::GetCPUCores()
{
	return FPlatformMisc::NumberOfCores();
}

int32 UHWDetails::GetCPUThreads()
{
	if(PLATFORM_WINDOWS)
	{
		return FWindowsPlatformMisc::NumberOfCoresIncludingHyperthreads();
	}
}

int32 UHWDetails::GetRAMCapacity()
{
	return FGenericPlatformMemory::GetPhysicalGBRam();
}

FString UHWDetails::GetOSVersion()
{
	return FPlatformMisc::GetOSVersion();
}
