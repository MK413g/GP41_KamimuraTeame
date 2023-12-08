// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameEndManager.h"

E_MainGameEndStat AMainGameEndManager::stat = E_MainGameEndStat::Null;
bool AMainGameEndManager::gameclear = false;
bool AMainGameEndManager::gameover = false;

// Sets default values
AMainGameEndManager::AMainGameEndManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	gameover = false;
	gameclear = false;
	stat = E_MainGameEndStat::Null;

}

// Called when the game starts or when spawned
void AMainGameEndManager::BeginPlay()
{
	Super::BeginPlay();
	EnableInput(GetWorld()->GetFirstPlayerController());
	stat = E_MainGameEndStat::Null;
	gameover = false;
	gameclear = false;
}

void AMainGameEndManager::SetOver()
{
	if (stat == E_MainGameEndStat::Null) {
		stat = E_MainGameEndStat::Over;
	}
}

void AMainGameEndManager::SetClear()
{
	if (stat == E_MainGameEndStat::Null) {
		stat = E_MainGameEndStat::Clear;
	}
}

void AMainGameEndManager::ExecEnumOutput(E_MainGameEndStat st, E_MainGameEndStat& DebugTestEnumOutput)
{
	DebugTestEnumOutput = st;
}

E_MainGameEndStat AMainGameEndManager::GetStat()
{
	return stat;
}

bool AMainGameEndManager::GetGameEnd()
{
	return stat != E_MainGameEndStat::Null;
}

bool AMainGameEndManager::GetGameClearFlg()
{
	return gameclear;
}

bool AMainGameEndManager::GetGameOverFlg()
{
	return gameover;
}

void AMainGameEndManager::SetOverStat_Implementation()
{
	stat = E_MainGameEndStat::End;
	gameover = true;
}

void AMainGameEndManager::SetClearStat_Implementation()
{
	stat = E_MainGameEndStat::End;
	gameclear = true;
}

void AMainGameEndManager::Reset()
{
	stat = E_MainGameEndStat::Null;
	gameover = false;
	gameclear = false;
}


