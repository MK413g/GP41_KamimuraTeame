// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ManagerLevelLoad.generated.h"
struct FSubLevelState;
class ULevelStreamingDynamic;
UCLASS()
class GP41_KAMIMURATEAME_API AManagerLevelLoad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AManagerLevelLoad();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void LoadSubLevel(FSubLevelState sublevel);//ロード
	virtual void AnLoadSubLevel(FString levelname, bool& bOutSuccess);//アンロード
	bool ShowLevel(FString _level,bool active);//表示する
protected:
	TMap<FString, ULevelStreamingDynamic*> levelmap;
};
