// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MUesBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class GP41_KAMIMURATEAME_API UMUesBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
		UFUNCTION(BlueprintCallable, BlueprintPure)
		static float RateRangeFloat(float val, float min = 0.f, float max = 1.f);


};
