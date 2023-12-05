// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialManager.h"

// Sets default values
ATutorialManager::ATutorialManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	framebflg = true;
	type = E_TutorialType::FirstWait;
}

// Called when the game starts or when spawned
void ATutorialManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATutorialManager::NextType(E_TutorialType intype)
{
	if (intype == type) {
		if (GetNextTutorial()&&framebflg) {
			framebflg = false;
			type = (E_TutorialType)((int)type + 1);
			UpdateWidget(type);
		}
	}
}

bool ATutorialManager::GetActionTutorial(E_TutorialType actiontype)
{
	if ((int)actiontype == (int)type) {
	
		if (GetNext()) {
			return false;
		}

		if (actiontype == E_TutorialType::Junp) {
			if (GetInputTextTutorial() || !GetNextTutorial()) {
				return false;
			}
		}
		return !GetExplanation() && !GetInputTextTutorial() && GetNextTutorial();
	}


	//�W�����v�ƃe�L�X�g���肪���Ȃ��悤�ɂ���
	if (actiontype == E_TutorialType::Junp) {
		if (GetInputTextTutorial() || !GetNextTutorial()) {
			return false;
		}
	}

	return (int)actiontype <= (int)type;
}

E_TutorialType ATutorialManager::GetTutorialType()
{
	return type;
}

void ATutorialManager::NextFlg()
{
	framebflg = true;
}
