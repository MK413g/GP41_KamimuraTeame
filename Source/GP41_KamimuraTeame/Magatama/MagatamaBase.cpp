// Fill out your copyright notice in the Description page of Project Settings.


#include "MagatamaBase.h"
#include "Logging/LogMacros.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMagatamaBase::AMagatamaBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	state = E_MagatamaState::Wait;
	
	projectilemovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));

	AddStateMap(this, E_MagatamaState::Rote, &AMagatamaBase::RoteUpdate);
}
	
// Called when the game starts or when spawned
void AMagatamaBase::BeginPlay()
{
	Super::BeginPlay();
	EnableInput(GetWorld()->GetFirstPlayerController());

	//Playerを取得しておく
	APawn* p = UGameplayStatics::GetPlayerPawn(this->GetWorld(), 0);
	if (p) {
		APlayerBase* pl = Cast<APlayerBase>(p);
		if (pl) {
			base = pl;
			playerheight = pl->GetActorLocation().Z * 2.0f;
		}
	}
	initialVelocity = 0.0f;
	ShotMaxSpeed = 10000.0;

	if (PlayerStateDataTabel || MagatamaStateDataTabel ) {

		FPState* PST = nullptr;
		for (FName Key : (*PlayerStateDataTabel).GetRowNames())
		{
			// 情報の保存
			PST = PlayerStateDataTabel->FindRow<FPState>(Key, FString());
		}

		FMagatamaState* MST = nullptr;
		for (FName Key : (*MagatamaStateDataTabel).GetRowNames())
		{
			// 情報の保存
			MST = MagatamaStateDataTabel->FindRow<FMagatamaState>(Key, FString());
		}

		if (PST!=NULL || MST!=NULL) {
			SetState(*MST, *PST);
		}
	}

	if (projectilemovement) {
		projectilemovement->SetComponentTickEnabled(false);
		projectilemovement->ProjectileGravityScale = ShotGravity;
		projectilemovement->MaxSpeed = ShotMaxSpeed;
	}

	Start();
}

void AMagatamaBase::SetState(FMagatamaState stat, FPState pstate)
{
	RoteMaxDistance = pstate.MaxRadius;
	RoteMinDistance = pstate.MinRadius;

	MaxDamage = stat.MaxDamage;
	MinDamage = stat.MinDamage;
	maxBaseSpeed = stat.RotationMaxSpeed;
	minBaseSpeed = stat.RotationMinSpeed;

	Acceleration = stat.RotationAcceleration;

	ShotInitialMaxSpeed = stat.ShotInitialMaxSpeed;
	ShotInitialMinSpeed = stat.ShotInitialMinSpeed;
	ShotGravity = stat.ShotGravity;
	ShotToAngle = stat.ShotToAngle;
	ShotFromAngle = stat.ShotFromAngle;
	ShotBouns = stat.ShotBouns;

	/*RoteDamageMax = RoteDamageMin = stat.MaxDamage;
	RoteDamageSpeedRate = 1.f;*/

	RoteDamageMax = stat.RotateionDamegeMax;
	RoteDamageMin = stat.RotateionDamegeMin;
	RoteDamageSpeedRate = stat.RotateionDamegeSpeed;

}
// Called every frame
void AMagatamaBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (playerheight<GetActorLocation().Z) {
		projectilemovement->ProjectileGravityScale = 2.f;
	}
}


float AMagatamaBase::GetDamage()
{
	if (state != E_MagatamaState::Shot&&state != E_MagatamaState::Rote) 
	{
		return 0;
	}

	if (state == E_MagatamaState::Rote) {
		if (speedRate < RoteDamageSpeedRate) {
			return 0;
		}
		float damage = RoteDamageMin * (1.f - speedRate) + RoteDamageMax * speedRate;
	}

	float max = minBaseSpeed * (1.0f - speedRate) + maxBaseSpeed * speedRate;
	float d = MinDamage * (1.f - speedRate) + MaxDamage * speedRate;
	//UE_LOG(LogTemp, Log, TEXT("Damage:%s"), *FString::SanitizeFloat(d));
	return d;
}

FVector AMagatamaBase::GetNockBackForce()  
{
	if (state == E_MagatamaState::Rote || state == E_MagatamaState::Shot)
	{
		//ノックバック方向
		FVector vec;
		float shotspeed = MinKnockBackPower * (1.0f - speedRate) + MaxKnockBackPower * speedRate;
		switch (state)
		{
		case E_MagatamaState::Rote:
			if (base) {
				FVector pos = base->GetActorLocation() - GetActorLocation();
				//外積を求める
				FVector cross = FVector::CrossProduct(pos, FVector::UpVector);
				vec = cross * shotspeed;
			}
			break;

		case E_MagatamaState::Shot:
			vec = shotvec.GetSafeNormal() * shotspeed;
			break;
		default:
			break;
		}
		return vec;
	}

	return FVector::ZeroVector;
}

void AMagatamaBase::SetupDrop() {
	//プレイヤーとの接線を求め、現在の速度で物理運動をする
	//プレイヤーとのベクトルを求める
	if (base) {
		FVector pos = base->GetActorLocation() - GetActorLocation();
		//外積を求める
		FVector cross = FVector::CrossProduct(pos, FVector::UpVector);

		const float r = 5.f;
		float shotspeed = ShotInitialMinSpeed / r * (1.0f - speedRate) + ShotInitialMaxSpeed / r * speedRate;

		projectilemovement->InitialSpeed = shotspeed;
		projectilemovement->Velocity = cross * shotspeed;
	}

	projectilemovement->SetComponentTickEnabled(true);
	shotboounscount = 0;
	state = E_MagatamaState::Drop;
	if (ShotBouns <= 1) {
		shotboounscount = -1;
	}
}

bool AMagatamaBase::SetupPlayerUse(FVector PlayerPos, USceneComponent* com)
{
	if (state!=E_MagatamaState::Wait) { return false; }
	if (base ) {
		base->MagatamaNum++;
		base->AddMagatama(this);
	}

	FVector pos=com->GetComponentLocation();
	center.SetLocation(PlayerPos);
	pichangle = (PlayerPos-pos).Rotation().Pitch;
	FRotator rote = (PlayerPos-pos).Rotation();
	center.SetRotation(rote.Quaternion());
	com->SetWorldLocation(pos);
	state = E_MagatamaState::Rote;
	timecout = 0;
	projectilemovement->ProjectileGravityScale = ShotGravity;

	//距離を保存
	distance = (PlayerPos - pos).Size();
	distance = -distance;
	SetActorLocation(center.GetLocation() + center.GetRotation().Rotator().Vector()* distance);
	return true;
}

bool AMagatamaBase::SetupShot(FVector targetvec)
{
	if (state != E_MagatamaState::Rote) { return false; }

	state = E_MagatamaState::Shot;
	projectilemovement->SetComponentTickEnabled(true);
	float shotspeed= ShotInitialMinSpeed * (1.0f - speedRate) + ShotInitialMaxSpeed * speedRate;
	shotvec = targetvec * shotspeed;
	projectilemovement->InitialSpeed = shotspeed;
	projectilemovement->Velocity = shotvec;
	shotboounscount = 0;

	if (base) {
		base->MagatamaNum--;
		base->DeleteMagatama(this);
	}
	return true;
}

bool AMagatamaBase::ResetWait(bool enemyhit)
{
	if (enemyhit) {
		if (state == E_MagatamaState::Rote && speedRate >= RoteDamageSpeedRate) {
			if (base != nullptr) {
				base->DeleteMagatama(this);
			}
			SetupDrop();
			return true;
		}

		//if (state == E_MagatamaState::Rote) {
		//	if (speedRate <= 0.5f) {
		//		speedRate = 0.5f;
		//	}

		//	if (base!=nullptr) {
		//		base->DeleteMagatama(this);
		//	}
		//	SetupDrop();
		//	return true;
		//}
	}


	if (state != E_MagatamaState::Shot&&state!=E_MagatamaState::Drop) { return false; }
	shotboounscount++;
	if (shotboounscount < ShotBouns + 1 ) { return false; }
	if (playerheight < GetActorLocation().Z) { return false; }

	state = E_MagatamaState::Wait;
	projectilemovement->SetComponentTickEnabled(false);
	return true;
}

bool AMagatamaBase::GetShotAngle(AActor* player)const
{
	if (state != E_MagatamaState::Rote) { return false; }
	if (!base) { return false; }
	if (!base->ShotMagatama) { return false; }

	FVector playervec = player->GetActorForwardVector();
	playervec.Normalize();

	FVector myvec = GetActorLocation() - player->GetActorLocation();
	myvec.Normalize();
	float angle = UKismetMathLibrary::DegAcos(FVector::DotProduct(playervec, myvec));
	float mainasu = FVector::DotProduct(FVector::CrossProduct(playervec, myvec), FVector::UpVector);

	if (mainasu < 0) {
		angle *= -1;
		angle+=360;
	}
	//UE_LOG(LogTemp, Log, TEXT("angle%s"), *FString::SanitizeFloat(angle));
	bool ret = false;
	if (ShotToAngle > ShotFromAngle) {
		ret = ShotToAngle < angle || angle < ShotFromAngle;
	}
	else {
		ret = angle > ShotToAngle && ShotFromAngle > angle;
	}

	if (ret) { base->ShotMagatama = false; }

	return ret;
}


void AMagatamaBase::Update(AActor* playeractor, USceneComponent* com)
{
	if (stateMap.Contains(state)) {
		stateMap[state].ExecuteIfBound(playeractor, com);
	}
}

void AMagatamaBase::RoteUpdate(AActor* playeractor, USceneComponent* com)
{
	//rootをプレイヤーの位置にする
	center.SetLocation(playeractor->GetActorLocation());
	//プレイヤーとの距離を求める
	FVector vec = playeractor->GetActorLocation() - com->GetComponentLocation();
	AngleRotation(vec.Size());
	AngleUpRotation(com);

	float dis = RoteMinDistance * (1.0f - speedRate) + RoteMaxDistance * speedRate;
	float max = minBaseSpeed * (1.0f - speedRate) + maxBaseSpeed * speedRate;
	float ra = roteangle / max;
	float d = distance * (1.f - ra) +( -dis) * ra;
	SetActorLocation(center.GetLocation() + center.Rotator().Vector() * d, true);
	SetActorLocation(center.GetLocation() + center.Rotator().Vector() * d);
}

void AMagatamaBase::AngleUpRotation(USceneComponent* com)
{
	pichangle -= roteangle/100.0f;
	if (pichangle < 0) {
		pichangle = 0;
	}
	FRotator rote = center.GetRotation().Rotator();
	rote.Pitch = pichangle;
	center.SetRotation(rote.Quaternion());
}

void AMagatamaBase::AngleRotation(float len)
{
	//回転処理
	timecout += FApp::GetDeltaTime();

	//z軸
	float r = len;
	//速度
	float v = Acceleration * timecout;
	v += initialVelocity;

	float max = minBaseSpeed * (1.0f - speedRate) + maxBaseSpeed * speedRate;
	if (v > max) {
		v = max;
	}
	roteangle = v;
	FRotator rote(0,v,0);

	center.SetRotation( (center.Rotator() + rote).Quaternion());
}

