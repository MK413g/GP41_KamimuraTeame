// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LevelStreamingDynamic.h"
#include"ManagerLevelLoad.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ManagerSubLevel.generated.h"

USTRUCT(BlueprintType)
struct  FSubLevelState:public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnyWhere, AdvancedDisplay, meta = (DisplayName = "LevelData"))
		TSoftObjectPtr <UWorld> level;
	// 出現位置
	UPROPERTY(EditAnyWhere, AdvancedDisplay, meta = (DisplayName = "SpawnLocation"))
		FVector Location;
	// 出現角度
	UPROPERTY(EditAnyWhere, AdvancedDisplay, meta = (DisplayName = "SpawnRotation"))
		FRotator Rotation;

};



UCLASS(Blueprintable)
class GP41_KAMIMURATEAME_API AManagerSubLevel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AManagerSubLevel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AManagerLevelLoad* loadmanager;
	UPROPERTY(EditAnywhere)
		UDataTable* LevelDataTabel;
	UFUNCTION(BlueprintCallable, Category = "Level")
		bool LoadSubLevel(FString name);
	UFUNCTION(BlueprintCallable, Category = "Level")
		bool ShowSubLevel(FString name,bool active);
protected:
	TMap<FString, FSubLevelState> levelstatus;
};
