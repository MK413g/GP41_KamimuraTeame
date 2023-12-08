// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameStageInstance.generated.h"

/**
 * 
 */

//ステージ種類
UENUM(BlueprintType)
enum class E_LevelState :uint8
{
	Title=0,
	Stage1=1,
	Stage2=2,
	Stage3=3,
	Stage4=4,
	Stage5=5,
	End=6,
	//ステージを増やすとき用
	Stage6=10 UMETA(Hidden),
	Stage7=11 UMETA(Hidden),
	Stage8=12 UMETA(Hidden),
	Null=255
};


UCLASS()
class GP41_KAMIMURATEAME_API UGameStageInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure)
	static UGameStageInstance* GetInstance();
	UFUNCTION(BlueprintCallable)
	void StageInit();//タイトルレベルで呼び出し
	UFUNCTION(BlueprintCallable)
	void SetStageState(E_LevelState state);//レベルステータスを呼び出し
	UFUNCTION(BlueprintCallable)
	bool SetNextStage();
	UFUNCTION(BlueprintPure)
	E_LevelState GetLevelState();
	UFUNCTION(BlueprintCallable, meta = (ExpandEnumAsExecs = "EnumOutput"))
		void StageEnumOutput(E_LevelState nowstate, E_LevelState& EnumOutput);
private:
	E_LevelState m_state=E_LevelState::Null;
};
