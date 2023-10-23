// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MainGameEndManager.generated.h"


UENUM(BlueprintType)
enum class E_MainGameEndStat :uint8
{
	Null,
	Clear,
	Over,
	End UMETA(Hidden)
};

UCLASS()
class GP41_KAMIMURATEAME_API AMainGameEndManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMainGameEndManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	static E_MainGameEndStat stat;
	UFUNCTION(BlueprintCallable)
		static void SetOver();
	UFUNCTION(BlueprintCallable)
		static void SetClear();
	UFUNCTION(BlueprintCallable, meta = (ExpandEnumAsExecs = "DebugTestEnumOutput"))
		void ExecEnumOutput(E_MainGameEndStat st,E_MainGameEndStat& DebugTestEnumOutput);
	UFUNCTION(BlueprintPure)
		static E_MainGameEndStat GetStat();
	UFUNCTION(BlueprintPure)
		static bool GetGameEnd();
	UFUNCTION(BlueprintPure)
		static bool GetGameClearFlg ();
	UFUNCTION(BlueprintPure)
		static bool GetGameOverFlg();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void SetOverStat();
	virtual void SetOverStat_Implementation();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void SetClearStat();
	virtual void SetClearStat_Implementation();
	

	static bool gameover;
	static bool gameclear;

};
