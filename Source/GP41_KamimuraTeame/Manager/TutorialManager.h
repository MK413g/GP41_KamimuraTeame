// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine/DataTable.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TutorialManager.generated.h"

UENUM(BlueprintType)
enum class E_TutorialType : uint8
{
	FirstWait			= 0			UMETA(ToolTip = "�ŏ��̃i���[�V�����҂�����"),
	PlayerState			= 12		UMETA(ToolTip = "�v���C���[�̃X�e�[�^�X"),
	EnemyState			= 13		UMETA(ToolTip = "�G�̃X�e�[�^�X"),
	MiniMap				= 14		UMETA(ToolTip = "�~�j�}�b�v"),
	Move				= 1			UMETA(ToolTip = "�ړ�"),
	RunMove				= 2			UMETA(ToolTip = "�_�b�V��"),
	Junp				= 3			UMETA(ToolTip = "�W�����v"),
	Perspective			= 4			UMETA(ToolTip = "���_�ړ�"),
	LockOn				= 5			UMETA(ToolTip = "���b�N�I��"),
	CollectMagatama		= 6			UMETA(ToolTip = "���ʂ��W�߂�"),
	RotationMagatama	= 7			UMETA(ToolTip = "���ʂ�������]"),
	AttackMagatama		= 8			UMETA(ToolTip = "���ʂ�������]�œG�ɂԂ���"),
	ShotMagatama		= 9			UMETA(ToolTip = "���ʂ�ł��Ԃ���"),
	EndWait				= 10		UMETA(ToolTip = "�Ō�̃i���[�V�����҂�����"),
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
