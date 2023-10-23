// Fill out your copyright notice in the Description page of Project Settings.


#include "MUesBlueprintFunctionLibrary.h"

float UMUesBlueprintFunctionLibrary::RateRangeFloat(float val, float min, float max)
{
	if (val > 1.f) { val = 1.f; }
	if (val < 0.f) { val = 0.f; }
	return min * (1.f - val) + max * val;
}
