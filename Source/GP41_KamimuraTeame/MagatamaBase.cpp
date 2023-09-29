// Fill out your copyright notice in the Description page of Project Settings.


#include "MagatamaBase.h"
#include "Logging/LogMacros.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "DrawDebugHelpers.h"

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
	if (projectilemovement) {
		projectilemovement->SetComponentTickEnabled(false);
		projectilemovement->ProjectileGravityScale = ShotGravity;
		projectilemovement->MaxSpeed = ShotMaxSpeed;
	}

	//ラグ変数初期化
	LocationLgSpeed = 10.f;
}

// Called every frame
void AMagatamaBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float AMagatamaBase::GetDamage() const
{
	if (state != E_MagatamaState::Shot && state != E_MagatamaState::Rote) { return 0; }

	float max = minBaseSpeed * (1.0f - speedRate) + maxBaseSpeed * speedRate;
	float ra = roteangle / max;
	float d = MinDamage * (1.f - speedRate) + MaxDamage * speedRate;
	UE_LOG(LogTemp, Log, TEXT("Damage:%s"), *FString::SanitizeFloat(d));
	return d;
}

void AMagatamaBase::SetupPlayerUse(FVector PlayerPos, USceneComponent* com)
{
	if (state!=E_MagatamaState::Wait) { return; }

	FVector pos=com->GetComponentLocation();
	center.SetLocation(PlayerPos);
	pichangle = (PlayerPos-pos).Rotation().Pitch;
	FRotator rote = (PlayerPos-pos).Rotation();
	center.SetRotation(rote.Quaternion());
	com->SetWorldLocation(pos);
	state = E_MagatamaState::Rote;
	timecout = 0;

	//距離を保存
	distance = (PlayerPos - pos).Size();
	distance = -distance;
	SetActorLocation(center.GetLocation() + center.GetRotation().Rotator().Vector()* distance);

}

void AMagatamaBase::SetupShot(USceneComponent* com, FVector targetvec)
{
	if (state != E_MagatamaState::Rote) { return; }

	state = E_MagatamaState::Shot;
	projectilemovement->SetComponentTickEnabled(true);
	float shotspeed= ShotInitialMinSpeed * (1.0f - speedRate) + ShotInitialMaxSpeed * speedRate;
	float max = minBaseSpeed * (1.0f - speedRate) + maxBaseSpeed * speedRate;
	float ra = roteangle / max;
	float s = shotspeed * ra;
	projectilemovement->InitialSpeed = shotspeed;
	projectilemovement->Velocity = targetvec * shotspeed;
	shotboounscount = 0;
}

void AMagatamaBase::ResetWait()
{
	if (state != E_MagatamaState::Shot) { return; }
	shotboounscount++;
	if (shotboounscount < ShotBouns+1) { return; }

	state = E_MagatamaState::Wait;
	projectilemovement->SetComponentTickEnabled(false);
}

bool AMagatamaBase::GetShotAngle(AActor* player)const
{
	if (player == nullptr) { return false; }

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
	UE_LOG(LogTemp, Log, TEXT("angle%s"), *FString::SanitizeFloat(angle));

	if (ShotToAngle > ShotFromAngle) {
		return ShotToAngle<angle || angle<ShotFromAngle;
	}
	else {
		return angle > ShotToAngle && ShotFromAngle > angle;
	}

	return true;
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

	SetActorLocation(center.GetLocation()+center.Rotator().Vector() * d);
}

void AMagatamaBase::LagUpdate()
{
	if (isLagLocation) {

	}
}

void AMagatamaBase::AngleUpRotation(USceneComponent* com)
{
	pichangle -= roteangle/10.0f;
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
	float v = speed * timecout;
	v += initialVelocity;

	float max = minBaseSpeed * (1.0f - speedRate) + maxBaseSpeed * speedRate;
	if (v > max) {
		v = max;
	}
	roteangle = v;
	FRotator rote(0,v,0);

	center.SetRotation( (center.Rotator() + rote).Quaternion());
}

