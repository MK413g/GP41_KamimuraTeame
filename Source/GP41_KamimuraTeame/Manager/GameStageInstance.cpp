// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStageInstance.h"

UGameStageInstance* UGameStageInstance::GetInstance()
{
	UGameStageInstance* instance = nullptr;

	if (GEngine)
	{
		FWorldContext* context = GEngine->GetWorldContextFromGameViewport(GEngine->GameViewport);
		instance = Cast<UGameStageInstance>(context->OwningGameInstance);
	}

	return instance;
}

void UGameStageInstance::StageInit()
{
	m_state = E_LevelState::Title;
}

void UGameStageInstance::SetStageState(E_LevelState state)
{
	m_state = state;
}

bool UGameStageInstance::SetNextStage()
{
	m_state = (E_LevelState)((int)m_state + 1);

	if (m_state == E_LevelState::End) {
		//èIóπ
		return true;
	}
	return false;
}

E_LevelState UGameStageInstance::GetLevelState()
{
	return m_state;
}

void UGameStageInstance::StageEnumOutput(E_LevelState nowstate, E_LevelState& EnumOutput)
{
	EnumOutput = nowstate;
}
