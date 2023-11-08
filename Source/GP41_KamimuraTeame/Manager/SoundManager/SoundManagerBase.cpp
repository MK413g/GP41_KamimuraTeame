// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundManagerBase.h"
#include "Kismet/GameplayStatics.h"
#include"AudioDevice.h"
#include "Engine/World.h"

float ASoundManagerBase::VolumeAllRate = 1.f;
float ASoundManagerBase::VolumeBGMRate = 1.f;
float ASoundManagerBase::VolumeSERate = 1.f;

USoundClass* ASoundManagerBase::baseclass = nullptr;
USoundClass* ASoundManagerBase::bgmclass = nullptr;
USoundClass* ASoundManagerBase::seclass = nullptr;
USoundMix* ASoundManagerBase::mix = nullptr;

// Sets default values
ASoundManagerBase::ASoundManagerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	mix = nullptr;
	baseclass = nullptr;
	bgmclass = nullptr;
	seclass = nullptr;
}

// Called when the game starts or when spawned
void ASoundManagerBase::BeginPlay()
{
	Super::BeginPlay();
	//‰Šú‰»
	mix = nullptr;
	baseclass = nullptr;
	bgmclass = nullptr;
	seclass = nullptr;

}

bool ASoundManagerBase::CheckSetting(USoundBase* sound)
{
	if (!sound) {
		UE_LOG(LogTemp, Warning,TEXT("Warning:not setting USoundBaseClass"));
	}
	return !sound;
}

void ASoundManagerBase::PlaySound2D(USoundBase* sound, ESoundManagerType type, float volumemul, float pichmul, float starttime, USoundConcurrency* con, AActor* owningActor, bool blsUISound)
{
	if (CheckSetting(sound)) {
		return;
	}

	if (!GEngine->GameViewport) {
		return;
	}

	float vol = 1.f;
	switch (type)
	{
	case ESoundManagerType::BGM: vol = VolumeBGMRate; break;
	case ESoundManagerType::SE:  vol = VolumeSERate;  break;
	}

	UGameplayStatics::PlaySound2D(
		GEngine->GameViewport->GetWorld(),
		sound,
		volumemul * vol * VolumeAllRate,
		pichmul,
		starttime,
		con,
		owningActor,
		blsUISound
	);
}

void ASoundManagerBase::PlaySoundLocation(USoundBase* sound, ESoundManagerType type, FVector location, float volumemul, float pichmul, float starttime, USoundAttenuation* attenuaton, USoundConcurrency* con)
{
	if (CheckSetting(sound)) { return; }


	float vol;
	switch (type)
	{
	case ESoundManagerType::BGM: vol = VolumeBGMRate; break;
	case ESoundManagerType::SE:  vol = VolumeSERate;  break;
	case ESoundManagerType::NONE:vol = 1.f;			  break;
	}

	UGameplayStatics::PlaySoundAtLocation(
		GEngine->GameViewport->GetWorld(),
		sound,
		location,
		volumemul * vol * VolumeAllRate,
		pichmul,
		starttime,
		attenuaton,
		con
	);
}

float ASoundManagerBase::GetBGMVoluem()
{
	return VolumeBGMRate;
}

float ASoundManagerBase::GetSEVoluem()
{
	return VolumeSERate;
}

float ASoundManagerBase::GetALLVoluem()
{
	return VolumeAllRate;
}

void ASoundManagerBase::SetBGMVoluem(float volume)
{
	if (VolumeBGMRate != volume) {
		if (CheckUClass(ESoundManagerType::BGM)) {
			UWorld* World = GEngine->GameViewport->GetWorld();
			FAudioDevice* device = World->GetAudioDeviceRaw();
			device->SetSoundMixClassOverride(mix, bgmclass, VolumeBGMRate, 1, 0, true);
		}
	}

	VolumeBGMRate = volume;
}

void ASoundManagerBase::SetSEVoluem(float volume)
{
	if (VolumeSERate != volume) {
		if (CheckUClass(ESoundManagerType::SE)) {
			UWorld* World = GEngine->GameViewport->GetWorld();
			FAudioDevice* device = World->GetAudioDeviceRaw();
			device->SetSoundMixClassOverride(mix, bgmclass, VolumeSERate, 1, 0, true);
		}
	}
	VolumeSERate = volume;
}

void ASoundManagerBase::SetALLVoluem(float volume)
{
	VolumeAllRate = volume;
}

bool ASoundManagerBase::CheckUClass(ESoundManagerType type)
{
	if (mix == nullptr) {
		mix= LoadObject<USoundMix>(NULL, TEXT("/Game/Main/Sound/SMix_SoundBaseMix.SMix_SoundBaseMix"), NULL, LOAD_None, NULL);

		UWorld* World = GEngine->GameViewport->GetWorld();
		FAudioDevice* device = World->GetAudioDeviceRaw();
		device->PushSoundMixModifier(mix);
	}
	if (mix == NULL) {
		UWorld* World = GEngine->GameViewport->GetWorld();
		FAudioDevice* device = World->GetAudioDeviceRaw();
		device->PopSoundMixModifier(mix);
		mix = nullptr;
		UE_LOG(LogTemp, Warning, TEXT("not mix"));
		return false;
	}

	switch (type)
	{
	case ESoundManagerType::BGM:
		if (bgmclass != nullptr)break;
		bgmclass= LoadObject<USoundClass>(NULL, TEXT("/Game/Main/Sound/SC_BGM.SC_BGM"), NULL, LOAD_None, NULL);
		if (bgmclass == NULL) {
			UE_LOG(LogTemp, Warning, TEXT("not bgmclass"));
			bgmclass = nullptr;
			return false;
		}
		break;
	case ESoundManagerType::SE:
		if (seclass != nullptr)break;
		seclass = LoadObject<USoundClass>(NULL, TEXT("/Game/Main/Sound/SC_SE.SC_SE"), NULL, LOAD_None, NULL);
		if (seclass == NULL) {
			UE_LOG(LogTemp, Warning, TEXT("not seclass"));
			seclass = nullptr;
			return false;
		}
		break;
	default:
		break;
	}


	return true;
}

bool ASoundManagerBase::CheckUClass()
{
	if (mix == nullptr) {
		mix = LoadObject<USoundMix>(NULL, TEXT("/Game/Main/Sound/SMix_SoundBaseMix.SMix_SoundBaseMix"), NULL, LOAD_None, NULL);

		UWorld* World = GEngine->GameViewport->GetWorld();
		FAudioDevice* device = World->GetAudioDeviceRaw();
		device->PushSoundMixModifier(mix);
	}
	if (mix == NULL) {
		UWorld* World = GEngine->GameViewport->GetWorld();
		FAudioDevice* device = World->GetAudioDeviceRaw();
		device->PopSoundMixModifier(mix);
		mix = nullptr;
		UE_LOG(LogTemp, Warning, TEXT("not mix"));
		return false;
	}
	if (baseclass == nullptr) {
		baseclass = LoadObject<USoundClass>(NULL, TEXT("/Game/Main/Sound/SC_Base.SC_Base"), NULL, LOAD_None, NULL);
		if (baseclass == NULL) {
			UE_LOG(LogTemp, Warning, TEXT("not bgmclass"));
			baseclass = nullptr;
			return false;
		}
	}

	return true;
}
