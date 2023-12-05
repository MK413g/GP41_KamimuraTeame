// Fill out your copyright notice in the Description page of Project Settings.


#include "MUesBlueprintFunctionLibrary.h"

float UMUesBlueprintFunctionLibrary::RateRangeFloat(float val, float min, float max)
{
	if (val > 1.f) { val = 1.f; }
	if (val < 0.f) { val = 0.f; }
	return min * (1.f - val) + max * val;
}

FVector UMUesBlueprintFunctionLibrary::RateRangeVector(float val, FVector min, FVector max)
{
	if (val > 1.f) { val = 1.f; }
	if (val < 0.f) { val = 0.f; }
	FVector ret;
	ret.X = min.X * (1.f - val) + max.X * val;
	ret.Y = min.Y * (1.f - val) + max.Y * val;
	ret.Z = min.Z * (1.f - val) + max.Z * val;
	return ret;
}
