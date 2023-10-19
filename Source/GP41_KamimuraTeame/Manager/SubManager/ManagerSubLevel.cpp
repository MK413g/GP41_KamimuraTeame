// Fill out your copyright notice in the Description page of Project Settings.


#include "ManagerSubLevel.h"

// Sets default values
AManagerSubLevel::AManagerSubLevel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AManagerSubLevel::BeginPlay()
{
	Super::BeginPlay();
	
	if (LevelDataTabel == nullptr)return;

	// データテーブルの読み込み
	for (FName Key : (*LevelDataTabel).GetRowNames())
	{
		// 情報の保存
		FSubLevelState* RowData = LevelDataTabel->FindRow<FSubLevelState>(Key, FString());
		levelstatus.Add(Key.ToString(), *RowData);
		loadmanager->LoadSubLevel(levelstatus[Key.ToString()]);
	}
	Start();
}

void AManagerSubLevel::Start_Implementation()
{
}

// Called every frame
void AManagerSubLevel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AManagerSubLevel::LoadSubLevel(FString name)
{
	if (!levelstatus.Contains(name)||loadmanager == nullptr)return false;
	loadmanager->LoadSubLevel(levelstatus[name]);
	return true;
}

bool AManagerSubLevel::ShowSubLevel(FString name,bool active)
{
	if (!levelstatus.Contains(name)|| loadmanager==nullptr)return false;
	loadmanager->ShowLevel(levelstatus[name].level.ToSoftObjectPath().ToString(),active);
	return true;
}

