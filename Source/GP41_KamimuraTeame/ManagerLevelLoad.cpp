// Fill out your copyright notice in the Description page of Project Settings.


#include "ManagerLevelLoad.h"
#include"ManagerSubLevel.h"

// Sets default values
AManagerLevelLoad::AManagerLevelLoad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AManagerLevelLoad::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AManagerLevelLoad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AManagerLevelLoad::LoadSubLevel(FSubLevelState sublevel)
{
	//サブレベルの読み込みを行う
	FString levelname = sublevel.level.ToSoftObjectPath().ToString();
	bool use = false;
	if (!levelmap.Contains(levelname)) {
		
		ULevelStreamingDynamic* instace = ULevelStreamingDynamic::LoadLevelInstance(this, levelname,sublevel.Location, sublevel.Rotation, use);
		
		levelmap.Add(levelname,instace);
		
		levelmap[levelname]->SetShouldBeVisible(false);
		
		if (!use) {
			//UE_LOG(LogTemp, Log, TEXT("%sがありません"), *levelname);
		}
	}

	FLatentActionInfo	LatentAction;//読み込みアクション
	LatentAction.CallbackTarget = this;
	LatentAction.ExecutionFunction = "LoadCallBack";    // <- 完了時に呼び出される関数名を設定
	LatentAction.UUID = 1;
	LatentAction.Linkage = 0;
	UGameplayStatics::LoadStreamLevel(this, *levelname, true, true, LatentAction);
	//UGameplayStatics::LoadStreamLevel(this, _levelname, true, true, LatentAction);
}

void AManagerLevelLoad::AnLoadSubLevel(FString levelname, bool& bOutSuccess)
{
	if (!levelmap.Contains(levelname))return;
	bOutSuccess = false;
	UWorld* const World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::LogAndReturnNull);
	if (!levelmap[levelname])
	{
		return;
	}

	levelmap[levelname]->SetShouldBeLoaded(false);
	levelmap[levelname]->SetShouldBeVisible(false);
	levelmap[levelname]->SetIsRequestingUnloadAndRemoval(true);
	bOutSuccess = true;

	//Map内から消す
	levelmap.Remove(levelname);
}

bool AManagerLevelLoad::ShowLevel(FString _level,bool active)
{
	if (!levelmap.Contains(_level))return false;
	levelmap[_level]->SetShouldBeVisible(active);
	return true;
}

