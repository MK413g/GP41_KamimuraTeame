// Fill out your copyright notice in the Description page of Project Settings.


#include "StageSettingManager.h"

void AStageSettingManager::BreakFiled(const FLevelState& InTarget, E_FiledState& Value) 
{
	Value = InTarget.Filed;
}

// Sets default values
AStageSettingManager::AStageSettingManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	stagename.Add(E_LevelState::Stage1, "Stage1");
	stagename.Add(E_LevelState::Stage2, "Stage2");
	stagename.Add(E_LevelState::Stage3, "Stage3");
	stagename.Add(E_LevelState::Stage4, "Stage4");
	stagename.Add(E_LevelState::Stage5, "Stage5");

	stagename.Add(E_LevelState::Stage6, "Stage6");
	stagename.Add(E_LevelState::Stage7, "Stage7");
	stagename.Add(E_LevelState::Stage8, "Stage8");
}

// Called when the game starts or when spawned
void AStageSettingManager::BeginPlay()
{
	Super::BeginPlay();

	Init();
}

// Called every frame
void AStageSettingManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AStageSettingManager::GetDataTableState()
{
	//データテーブルからデータ取得する
	if (StateDataTabel == nullptr) {
		ErrorMessege();
		return false;
	}

	//ゲームインスタンス所得
	UGameStageInstance* instance = UGameStageInstance::GetInstance();
	int levelstat = (int)instance->GetLevelState();
	if (levelstat <= 0 || levelstat > (int)E_LevelState::End) {
		//デバック
		UE_LOG(LogTemp, Warning, TEXT("Debug:LevelType Stage1"));
		instance->SetStageState(E_LevelState::Stage1);
		if (GetDataTableState()) {
			ErrorMessege();
		}
	}

	if (instance == nullptr || levelstat <= 0 || levelstat >= (int)E_LevelState::End) {
		return false;
	}

	// データテーブルの読み込み
	for (FName Key : (*StateDataTabel).GetRowNames())
	{
		//現在のステージ数からデータを取得
		if (stagename[instance->GetLevelState()] == Key) {
			FStageManegerState* RowData = StateDataTabel->FindRow<FStageManegerState>(Key, FString());
			int stagenum = RowData->StageState.Num();
			if (stagenum > 0) {
				//FStageStateのFLevelState配列よりランダムでステージのデータを取得する
				int nowstage=FMath::RandRange(0, stagenum - 1);
				state = RowData->StageState[nowstage];
				return true;
			}
		}
	}

	ErrorMessege();
	return false;
}

APawn* AStageSettingManager::CreateEnemy()
{
	if (state.EnemyBP == NULL) {
		ErrorMessege();
		return NULL;
	}
	APawn* ret= GetWorld()->SpawnActor<APawn>(state.EnemyBP, state.EnemyTransform);
	if (ret != nullptr) {
		ret->SpawnDefaultController();
	}
	else {
		ErrorMessege();
	}
	return ret;
}

void AStageSettingManager::ErrorMessege()
{
	UE_LOG(LogTemp, Error, TEXT("Error:AStageSetting"));
}

