// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include"../Manager/StageSettingManager.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CreateStartMagatama.generated.h"

UCLASS()
class GP41_KAMIMURATEAME_API ACreateStartMagatama : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACreateStartMagatama();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	FVector GetNonOverlapVector(float maxRange, int index, float repossessionRange, const TArray<FVector>& vectorArray);
	void GetNonOverlapVectorArray(float maxRange, float repossessionRange, int requestCount, TArray<FVector>& OutVecArray);
	USceneComponent* com;
public:	
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> CreateActor;
	UPROPERTY(EditAnywhere)
		uint8 CreateNum;
	UPROPERTY(EditAnywhere)
		float CreateHeight=0;
	UPROPERTY(EditAnywhere)
		float CreateRange=0;
	UPROPERTY(EditAnywhere)
		TMap<E_FiledState, TSubclassOf<UClass>> fildmap;
};
