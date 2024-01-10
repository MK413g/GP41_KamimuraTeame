// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine/DataTable.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TutorialManager.generated.h"

UENUM(BlueprintType)
enum class E_TutorialType : uint8
{
	FirstWait			= 0			UMETA(ToolTip = "最初のナレーション待ち時間"),
	PlayerState			= 12		UMETA(ToolTip = "プレイヤーのステータス"),
	EnemyState			= 13		UMETA(ToolTip = "敵のステータス"),
	MiniMap				= 14		UMETA(ToolTip = "ミニマップ"),
	Move				= 1			UMETA(ToolTip = "移動"),
	RunMove				= 2			UMETA(ToolTip = "ダッシュ"),
	Junp				= 3			UMETA(ToolTip = "ジャンプ"),
	Perspective			= 4			UMETA(ToolTip = "視点移動"),
	LockOn				= 5			UMETA(ToolTip = "ロックオン"),
	CollectMagatama		= 6			UMETA(ToolTip = "勾玉を集める"),
	RotationMagatama	= 7			UMETA(ToolTip = "勾玉を高速回転"),
	AttackMagatama		= 8			UMETA(ToolTip = "勾玉を高速回転で敵にぶつける"),
	ShotMagatama		= 9			UMETA(ToolTip = "勾玉を打ちぶつける"),
	EndWait				= 10		UMETA(ToolTip = "最後のナレーション待ち時間"),
	Null				= 11
};


USTRUCT(BlueprintType)
struct FTutorialTextList :public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FString> textlist; 
};


UCLASS()
class GP41_KAMIMURATEAME_API ATutorialManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATutorialManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
	void NextType(E_TutorialType intype);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void UpdateWidget(E_TutorialType nexttype);
	virtual void UpdateWidget_Implementation(E_TutorialType nexttype) {}
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		bool GetNextTutorial();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		bool GetInputTextTutorial();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		bool GetExplanation();
	UFUNCTION(BlueprintPure)
		bool GetActionTutorial(E_TutorialType actiontype);
	UFUNCTION(BlueprintPure)
		E_TutorialType GetTutorialType();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void UpdateTutorial();
	void UpdateTutorial_Implementation(){}
	UFUNCTION(BlueprintCallable)
		void NextFlg();
	UFUNCTION(BlueprintCallable)
		void NextTypeFlg(E_TutorialType now);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		bool GetNext();
public:	
	E_TutorialType type;
	E_TutorialType nowtype;
	bool framebflg;
};
