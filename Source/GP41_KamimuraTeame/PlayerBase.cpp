// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBase.h"

// Sets default values
APlayerBase::APlayerBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerBase::BeginPlay()
{
	Super::BeginPlay();


	if (StateDataTabel == nullptr)return;

	// データテーブルの読み込み
	for (FName Key : (*StateDataTabel).GetRowNames())
	{
		if (Key.ToString() == LoadDataName) {
			// 情報の保存
			FPState* RowData = StateDataTabel->FindRow<FPState>(Key, FString());
			InitSetState(*RowData);
		}
	}
}

// Called every frame
void APlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APlayerBase::InitSetState(FPState state)
{
	Hp = state.HP;
	HpMax = Hp;
	Stamina = state.Stamina;
	StaminaMax = Stamina;
	StaminaRegeneration = state.StaminaRegeneration;
	RotationSpeed = state.RotationSpeed;
	Braking = state.JumpBraking;
}

bool APlayerBase::StaminaRegene(float axis)
{
	bool ret;
	ret = axis > 0.1f && Stamina > 0.f;
	if (axis < 0.1f) {
		Stamina += FApp::GetDeltaTime() * StaminaRegeneration;

		CheckVariable(StaminaMax, Stamina);
	}
	return ret;
}

void APlayerBase::CheckVariable(float max, float& variable)
{
	if (max < variable)variable = max;
}

float APlayerBase::GetRange(float rate)const
{
	return MinRange * (1.f - rate) + MaxRange * rate;
}

void APlayerBase::SetDamage(float damage) 
{
	Hp -= damage;
	if (Hp < 0)Hp = 0;
}

void APlayerBase::SetNockBack(FVector force, float power)
{
	NockbackPower = power;
	NockBackForce = force;
}

void APlayerBase::NockBackUpdate()
{
	if (NockbackPower < 10)return;
	FVector playerpos = GetActorLocation();
	float p = NockbackPower * 0.2;
	NockbackPower -= p;
	playerpos += NockBackForce * p;
	SetActorLocation(playerpos);

}
