// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundManagerBase.h"
#include "Kismet/GameplayStatics.h"

float ASoundManagerBase::VolumeAllRate = 1.f;
float ASoundManagerBase::VolumeBGMRate = 1.f;
float ASoundManagerBase::VolumeSERate = 1.f;

// Sets default values
ASoundManagerBase::ASoundManagerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ASoundManagerBase::BeginPlay()
{
	Super::BeginPlay();
	
}

bool ASoundManagerBase::CheckSetting(USoundBase* sound)
{
	if (!sound) {
		UE_LOG(LogTemp, Warning,TEXT("Warning:not setting USoundBaseClass"));
	}
	return !sound;
}

void ASoundManagerBase::PlaySound2D(USoundBase* sound, ESoundManagerType type, float volumemul,float pichmul,float starttime,USoundConcurrency* con,AActor* owningActor, bool blsUISound)
{
	if (CheckSetting(sound)) {
		return;
	}

	if (!GEngine->GameViewport) {
		return;
	}

	
	float vol=1.f; 
	switch (type)
	{
	case ESoundManagerType::BGM: vol = VolumeBGMRate; break;
	case ESoundManagerType::SE:  vol = VolumeSERate;  break;
	}
	//if (world == nullptr) {

	//	UGameplayStatics::PlaySound2D(
	//		GEngine->GetWorld(),
	//		sound,
	//		volumemul * vol * VolumeAllRate,
	//		pichmul,
	//		starttime,
	//		con,
	//		owningActor,
	//		blsUISound
	//	);
	//}
	//else {

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
//	}

}

void ASoundManagerBase::PlaySoundLocation(USoundBase* sound, ESoundManagerType type, FVector location, float volumemul, float pichmul, float starttime, USoundAttenuation* attenuaton,USoundConcurrency* con)
{
	if (CheckSetting(sound)) { return; }


	float vol;
	switch (type)
	{
	case ESoundManagerType::BGM: vol = VolumeBGMRate; break;
	case ESoundManagerType::SE:  vol = VolumeSERate;  break;
	case ESoundManagerType::NONE:vol = 1.f;			  break;
	}

//	if (world != nullptr) {
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
	//}
	//else {
	//	UGameplayStatics::PlaySoundAtLocation(
	//		GEngine->GetWorld(),
	//		sound,
	//		location,
	//		volumemul * vol * VolumeAllRate,
	//		pichmul,
	//		starttime,
	//		attenuato	n,
	//		con
	//	);

	//}
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
	VolumeBGMRate = volume;
}

void ASoundManagerBase::SetSEVoluem(float volume)
{
	VolumeSERate = volume;
}

void ASoundManagerBase::SetALLVoluem(float volume)
{
	VolumeAllRate = volume;
}
