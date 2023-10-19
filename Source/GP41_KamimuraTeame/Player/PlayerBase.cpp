// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBase.h"
#include"../Magatama/MagatamaBase.h"

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
	MaxRadius = state.MaxRadius;
	MinRadius = state.MinRadius;
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
	return MinRadius * (1.f - rate) + MaxRadius * rate;
}

void APlayerBase::SetDamage_Implementation(float damage, FVector force, float power)
{
	Damage(damage, force, power);
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
	float p = NockbackPower * 0.2f;
	NockbackPower -= p;
	playerpos += NockBackForce * p;
	SetActorLocation(playerpos);

}

void APlayerBase::AddMagatama(AMagatamaBase* magatama) {
	hasMagatama.Add(magatama);
}

void APlayerBase::Damage(float damage, FVector force, float power)
{
	Hp -= damage;
	if (Hp < 0)Hp = 0;
	SetNockBack(force, power);

	if (Hp <= 0) {
		SetGameOver();
	}

	//勾玉のドロップ(MaxHPの5分の1で1つ落ちる)
	int drop = damage / (HpMax / 5.f);
	if (drop >= MagatamaNum) {
		drop = MagatamaNum;
	}
	for (int i = 0; i < drop; i++) {
		hasMagatama[0]->SetupDrop();
		hasMagatama.RemoveAt(0);
	}
	MagatamaNum -= drop;
}
void APlayerBase::DeleteMagatama(AMagatamaBase* magatama) {

	int32 index;
	if (hasMagatama.Find(magatama,index)) {
		hasMagatama.Remove(magatama);
	}
}