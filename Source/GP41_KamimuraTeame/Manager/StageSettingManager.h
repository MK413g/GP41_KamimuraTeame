// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include"GameStageInstance.h"
#include "Engine/DataTable.h"

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "StageSettingManager.generated.h"


UENUM(BlueprintType)
enum class E_FiledState :uint8 
{
	Normal,
	Fire,
	Ice,
	Thunder,
	Water
};

USTRUCT(BlueprintType)
struct  FLevelState 
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnyWhere, AdvancedDisplay) TSubclassOf<APawn> EnemyBP;
	UPROPERTY(EditAnyWhere, AdvancedDisplay) FTransform EnemyTransform;
	UPROPERTY(EditAnyWhere, AdvancedDisplay) E_FiledState Filed;

};


USTRUCT(BlueprintType)
struct  FStageManegerState :public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnyWhere, AdvancedDisplay)
		TArray<FLevelState> StageState;
};

UCLASS()
class GP41_KAMIMURATEAME_API AStageSettingManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AStageSettingManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void Init();
	virtual void Init_Implementation() {}
	UFUNCTION(BlueprintCallable)
		bool GetDataTableState();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = State)
		UDataTable* StateDataTabel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = State)
		FLevelState state;
	UFUNCTION(BlueprintPure)
		void BreakFiled(const FLevelState& InTarget,E_FiledState& Value);
	UFUNCTION(BlueprintCallable)
		APawn* CreateEnemy();
	
private:
	TMap<E_LevelState, FName> stagename;
	void ErrorMessege();
};
