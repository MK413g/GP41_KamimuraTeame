// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBase.h"
#include"../Magatama/MagatamaBase.h"
#include "Kismet/GameplayStatics.h"

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

	
	stunflg = false;
	stunCuntTime = 0;
	runflg = false;
	lockonflg = false;
	cuntwiatstaminatime = 0;
	//キーバインド
	//InputComponent->BindAction("Attention", EInputEvent::IE_Pressed, this, &APlayerBase::LockOnEnemy);
	
}

// Called every frame
void APlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateStun(DeltaTime);
	LockOnUpdate(DeltaTime);
	UpdateRun(DeltaTime);
	NockBackUpdate();
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
	JumpPower = state.JumpPower;
	MoveSpeed = state.MoveSpeed;
	MoveBreaking = state.MoveBraking;
	JumpDownPower = state.JumpDownPower;
	MoveChangeSpeed = state.MoveChangeSpeed;
	MoveRunSpeed = state.RunMoveSpeed;
	runstamina = 1;
	waitstaminatime = state.StaminaWaitRegenerationTime;
	SetInit(MoveSpeed,JumpPower,MoveBreaking,state.MoveBrakingFrictionFactor);
}

bool APlayerBase::SetDataTable()
{
	if (StateDataTabel == nullptr)return false;

	// データテーブルの読み込み
	for (FName Key : (*StateDataTabel).GetRowNames())
	{
		if (Key.ToString() == LoadDataName) {
			// 情報の保存
			FPState* RowData = StateDataTabel->FindRow<FPState>(Key, FString());
			InitSetState(*RowData);
			return true;
		}
	}

	return false;
}

bool APlayerBase::StaminaRegene(float axis)
{
	bool ret;
	ret = axis > 0.1f && Stamina > 0.f;
	if (axis < 0.1f && runrate.Size() <= 0.01f) {
		cuntwiatstaminatime += FApp::GetDeltaTime();
		//スタミナ待ち時間
		if (cuntwiatstaminatime >= waitstaminatime) {

			Stamina += FApp::GetDeltaTime() * StaminaRegeneration;
			CheckVariable(StaminaMax, Stamina);
		}
	}

	if (ret) {
		Stamina -= FApp::GetDeltaTime();
		cuntwiatstaminatime = 0;
		if (Stamina <= 0.0f) {
			Stamina = 0;
		}
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
	
	SetActorLocation(playerpos,true);

}

void APlayerBase::SetStun_Implementation(float time)
{
	stunTime = time;
	stunCuntTime = 0;
	stunflg = true;

	MagatamaNum = hasMagatama.Num();

	//勾玉をすべてドロップする
	for (int i = 0; i < MagatamaNum; i++) {
		if (hasMagatama[0] != NULL) {
			hasMagatama[0]->SetupDrop();
		}	hasMagatama.RemoveAt(0);

	}
	MagatamaNum = 0;
}

void APlayerBase::StunRecovery_Implementation()
{
	stunflg = false;
}

bool APlayerBase::CheckMoveForward(float inputvalue,float speed,float& reinput)
{
	if (runflg) {
		runrate.Y = inputvalue;
	}


	if (fabsf(inputvalue) < 0.05f) {
		return false;
	}

	if (oldinputforwardValue * inputvalue < 0) {
		//反対方向の入力時
		if (speed > MoveChangeSpeed) {
			reinput = oldinputforwardValue * oldinputrate;
			return false;
		}
	}
	reinput = inputvalue;
	oldinputforwardValue = inputvalue;
	return true;
}

bool APlayerBase::CheckMoveRight(float inputvalue, float speed,float& reinput)
{
	if (runflg) {
		runrate.X = inputvalue;
	}

	if (fabsf(inputvalue) < 0.05f) {
		return false;
	}

	if (oldinputrightValue * inputvalue < 0) {
		//反対方向の入力時
		if (speed > MoveChangeSpeed) {
			reinput = oldinputrightValue * oldinputrate;
			return false;
		}
	}
	reinput = inputvalue;
	oldinputrightValue = inputvalue;
	return true;
}

void APlayerBase::HasMagatamaHidden()
{
	for (int i = 0; i < MagatamaNum; i++) {
		if (hasMagatama[i] != NULL) {
			hasMagatama[i]->SetHidden(true);
		}
	}
}

void APlayerBase::SettingStartRun_Implementation()
{
	runflg = true;
}

void APlayerBase::SettingEndRun_Implementation()
{
	runflg = false;
	runrate.X = runrate.Y = 0.f;
}

bool APlayerBase::GetRunFlg()
{
	return runflg;
}

void APlayerBase::LockOnUpdate(float deltatime)
{
	if (!lockonflg) {
		return;
	}

	//カメラを回転させる
	FRotator rote;
//	rote.Pitch=lockontargetAngle
	
	lockonCount += deltatime;
	float rate = 1.f / lockonTime * lockonCount;
	if (rate >= 1.0f) {
		rate = 1.0f;
	}
	rote.Pitch = nowlockAngle.X * (1.f - rate) + lockontargetAngle.X*rate;
	rote.Yaw = nowlockAngle.Y * (1.f - rate) + lockontargetAngle.Y*rate;
	rote.Roll = nowlockAngle.Z * (1.f - rate) + lockontargetAngle.Z*rate;

	APlayerController* const PC = CastChecked<APlayerController>(GetController());
	PC->ClientSetRotation(rote);

	if (lockonCount >= lockonTime) {
		lockonflg = false;
		lockonCount = 0.0f;
		
		FRotator prote = this->GetActorRotation();
		prote.Yaw = rote.Yaw;
		//プレイヤーの向きを合わせる
		SetActorRotation(prote);
	}

}

void APlayerBase::UpdateRun(float deltatime)
{
	if (!runflg) {
		return;
	}

	if (Stamina <= 0.f) {
		Stamina = 0;
		SettingEndRun();
		runflg = true;
		return;
	}

	Stamina -= deltatime * runstamina * runrate.Size();
	cuntwiatstaminatime = 0;
}

void APlayerBase::LockOnEnemy()
{
	if (lockonflg) {
		return;
	}

	AActor* target=nullptr;
	if (!GetEnemyActor(&target)) 
	{//エラー 
		return;
	}
	FRotator rote = GetControlRotation();
	nowlockAngle.X = rote.Pitch < 0.0f ? rote.Pitch + 360.0f : rote.Pitch;
	nowlockAngle.Y = rote.Yaw < 0.0f ? rote.Yaw + 360.0f : rote.Yaw;
	nowlockAngle.Z = rote.Roll < 0.0f ? rote.Roll + 360.0f : rote.Roll;

	FVector targepos = target->GetActorLocation();
	targepos.Z = 0;
	FVector pos = this->GetActorLocation();
	pos.Z = 0;
	FVector pevec = targepos - pos;
	
	pevec.Z = 0;
	pevec.Normalize();
	FVector nowvec = FVector(1.0,0.f, 0.f);
	nowvec.Normalize();

	//XY軸面角度を求める
	float dot = FVector::DotProduct(nowvec, pevec);
	float deg = (180.f) / PI * FMath::Acos(dot);
	FVector cross = FVector::CrossProduct(nowvec, pevec);
	dot = FVector::DotProduct(cross, FVector::UpVector);	
	float angle = deg;
	if (dot > 0.f) {
		angle = deg;
	}
	else {
		angle = 360 - deg;
	}
	//回転させるときの方向を求める
	float ro = fabsf(fabsf(nowlockAngle.Y) - angle);
	if (ro >= 180) {
		angle += 360.0f;
	}
	//Yaw
	lockontargetAngle.X = nowlockAngle.X >= 180 ? 360 : 0;	
	lockontargetAngle.Y = angle;
	lockontargetAngle.Z = nowlockAngle.Z >= 180 ? 360 : 0;
	lockonflg = true;

	//UE_LOG(LogTemp,Log,TEXT("angle %s: nowrote%s"),*FString::SanitizeFloat(angle),*FString::SanitizeFloat(nowlockAngle.Y))
}

void APlayerBase::ChangeFiled(E_FiledState filed)
{

	TMap<E_FiledState, FString> map;
	map.Add(E_FiledState::Normal, "NormalState");
	map.Add(E_FiledState::Fire, "FireFiledState");
	map.Add(E_FiledState::Ice, "IceFiledState");
	map.Add(E_FiledState::Thunder, "ThunderFiledState");
	map.Add(E_FiledState::Water, "WaterFiledState");

	if (StateDataTabel == nullptr)return;

	// データテーブルの読み込み
	for (FName Key : (*StateDataTabel).GetRowNames())
	{
		if (Key.ToString() == map[filed]) {
			// 情報の保存
			FPState* RowData = StateDataTabel->FindRow<FPState>(Key, FString());
			InitSetState(*RowData);
			break;
		}
	}

	ChangeMagatama(filed);

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

	if (drop >= hasMagatama.Num()) {
		drop = hasMagatama.Num();
	}

	for (int i = 0; i < drop; i++) {
		if (hasMagatama[0] != NULL) {
			hasMagatama[0]->SetupDrop();
		}
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

void APlayerBase::UpdateStun(float deltatime)
{
	if (!stunflg) { return; }
	if (stunCuntTime >= stunTime) {
		StunRecovery();
	}
	stunCuntTime += deltatime;
}

bool APlayerBase::GetEnemyActor(AActor** target)
{
	TArray<AActor*> list;
	UGameplayStatics::GetAllActorsOfClassWithTag(this, AActor::StaticClass(), FName("Enemy"), list);

	if (list.Num() == 0) {
		UE_LOG(LogTemp, Error, TEXT("PlayerBase : No Find EnemyActor"));
		return false;
	}
	*target = list[0];
	return true;
}
