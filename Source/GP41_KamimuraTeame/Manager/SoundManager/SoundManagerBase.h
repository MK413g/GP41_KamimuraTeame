// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include"Sound/SoundClass.h"
#include "Sound/SoundMix.h"

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

	UFUNCTION(BlueprintPure)
		static float GetBGMVoluem();	
	UFUNCTION(BlueprintPure)
		static float GetSEVoluem();
	UFUNCTION(BlueprintPure)
		static float GetALLVoluem();
	
	UFUNCTION(BlueprintCallable)
		static void SetBGMVoluem(float volume = 0);
	UFUNCTION(BlueprintCallable)
		static void SetSEVoluem(float volume = 0);
	UFUNCTION(BlueprintCallable)
		static void SetALLVoluem(float volume = 0);

//private:
	static bool CheckUClass(ESoundManagerType type);
	static bool CheckUClass();

	static USoundMix* mix;
	static USoundClass* baseclass;
	static USoundClass* bgmclass;
	static USoundClass* seclass;



	static float VolumeAllRate;
	static float VolumeBGMRate;
	static float VolumeSERate;
};
