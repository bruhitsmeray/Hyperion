// Fill out your copyright notice in the Description page of Project Settings.


#include "HWDetails.h"

#include "GenericPlatform/GenericPlatformApplicationMisc.h"
#include "GenericPlatform/GenericPlatformDriver.h"

FString UHWDetails::GetCPUBrand()
{
	return FWindowsPlatformMisc::GetCPUBrand();
}

FString UHWDetails::GetGPUBrand()
{
	FGPUDriverInfo DriverInfo;
	return DriverInfo.DeviceDescription;
}

FString UHWDetails::GetRHI()
{
	FGPUDriverInfo DriverInfo;
	return DriverInfo.RHIName;
}

int32 UHWDetails::GetCPUCores()
{
	if(PLATFORM_WINDOWS)
	{
		return FWindowsPlatformMisc::NumberOfCores();
	}
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

bool UHWDetails::IsWindows()
{
	if(PLATFORM_WINDOWS)
	{
		return true;
	}
}
