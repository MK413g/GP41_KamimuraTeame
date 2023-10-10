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

	initialVelocity = 1.0f;
	ShotMaxSpeed = 10000.0;

	if (PlayerStateDataTabel != nullptr || MagatamaStateDataTabel != nullptr) {

		FPState* PST = nullptr;
		for (FName Key : (*PlayerStateDataTabel).GetRowNames())
		{
			// ���̕ۑ�
			PST = PlayerStateDataTabel->FindRow<FPState>(Key, FString());
		}

		FMagatamaState* MST = nullptr;
		for (FName Key : (*MagatamaStateDataTabel).GetRowNames())
		{
			// ���̕ۑ�
			MST = MagatamaStateDataTabel->FindRow<FMagatamaState>(Key, FString());
		}

		if (PST != nullptr || MST != nullptr) {
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
}
// Called every frame
void AMagatamaBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


float AMagatamaBase::GetDamage(float enemyHp) const
{
	if (state != E_MagatamaState::Shot && state != E_MagatamaState::Rote) { return 0; }

	float max = minBaseSpeed * (1.0f - speedRate) + maxBaseSpeed * speedRate;
	float d = MinDamage * (1.f - speedRate) + MaxDamage * speedRate;
	//UE_LOG(LogTemp, Log, TEXT("Damage:%s"), *FString::SanitizeFloat(d));

	enemyHp -= d;
	if (enemyHp < 0)return 0;

	return enemyHp;
}

void AMagatamaBase::SetupPlayerUse(FVector PlayerPos, USceneComponent* com)
{
	if (state!=E_MagatamaState::Wait) { return; }
	APawn* p = UGameplayStatics::GetPlayerPawn(this->GetWorld(), 0);
	if (p != nullptr) {
		APlayerBase* base = Cast<APlayerBase>(p);
		if (base != nullptr){
			base->MagatamaNum++;
			base->AddMagatama(this);
		}
	}

	FVector pos=com->GetComponentLocation();
	center.SetLocation(PlayerPos);
	pichangle = (PlayerPos-pos).Rotation().Pitch;
	FRotator rote = (PlayerPos-pos).Rotation();
	center.SetRotation(rote.Quaternion());
	com->SetWorldLocation(pos);
	state = E_MagatamaState::Rote;
	timecout = 0;

	//������ۑ�
	distance = (PlayerPos - pos).Size();
	distance = -distance;
	SetActorLocation(center.GetLocation() + center.GetRotation().Rotator().Vector()* distance);

}

void AMagatamaBase::SetupShot(FVector targetvec)
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

	APawn* p = UGameplayStatics::GetPlayerPawn(this->GetWorld(), 0);
	if (p != nullptr) {
		APlayerBase* base = Cast<APlayerBase>(p);
		if (base != nullptr) {
			base->MagatamaNum--;
			base->DeleteMagatama(this);
		}
	}
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
	if (state != E_MagatamaState::Rote) { return false; }
	if (player == nullptr) { return false; }
	APlayerBase* base = Cast<APlayerBase>(player);
	if (base == nullptr) { return false; }
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
	//root���v���C���[�̈ʒu�ɂ���
	center.SetLocation(playeractor->GetActorLocation());
	//�v���C���[�Ƃ̋��������߂�
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
	//��]����
	timecout += FApp::GetDeltaTime();

	//z��
	float r = len;
	//���x
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

