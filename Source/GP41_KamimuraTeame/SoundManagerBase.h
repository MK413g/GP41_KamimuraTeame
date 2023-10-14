// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SoundManagerBase.generated.h"


UENUM(BlueprintType)
enum class ESoundManagerType : uint8
{
	BGM,
	SE,
	NONE
};

UCLASS()
class GP41_KAMIMURATEAME_API ASoundManagerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASoundManagerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	static bool CheckSetting(USoundBase* sound);
public:	
	UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = "pichmul, starttime,con,owningActor,blsUISound"))
		static void PlaySound2D(USoundBase* sound, ESoundManagerType type, float volumemul=1.f, float pichmul=1.f, float starttime=0.f, USoundConcurrency* con=nullptr, AActor* owningActor=nullptr,bool blsUISound=true);
	UFUNCTION(BlueprintCallable,meta=(AdvancedDisplay="pichmul,starttime,attenuaton,con"))
		static void PlaySoundLocation(USoundBase* sound, ESoundManagerType type, FVector  location = FVector::ZeroVector, float volumemul = 1.f, float pichmul = 1.f, float starttime = 0.f, USoundAttenuation* attenuaton = nullptr , USoundConcurrency* con = nullptr);

	// Called every frame
	static float VolumeAllRate;
	static float VolumeBGMRate;
	static float VolumeSERate;
};
