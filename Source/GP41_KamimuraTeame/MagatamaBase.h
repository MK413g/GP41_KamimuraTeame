// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include"PlayerBase.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagatamaBase.generated.h"



USTRUCT(BlueprintType)
struct  FMagatamaState :public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	//Damage
	UPROPERTY(EditAnyWhere, AdvancedDisplay)float MaxDamage;//shot max damage
	UPROPERTY(EditAnyWhere, AdvancedDisplay)float MinDamage;//shot min damage 

//	UPROPERTY(EditAnyWhere, AdvancedDisplay)float MinKnockBackPower;
//	UPROPERTY(EditAnyWhere, AdvancedDisplay)float MaxKnockBackPower;

	//Rotation
	UPROPERTY(EditAnyWhere, AdvancedDisplay)float RotationMaxSpeed;//rotation max speed
	UPROPERTY(EditAnyWhere, AdvancedDisplay)float RotationMinSpeed;//rotetion min speed
	UPROPERTY(EditAnyWhere, AdvancedDisplay)float RotationAcceleration;//acceleration
	//Shot
	UPROPERTY(EditAnyWhere, AdvancedDisplay)float ShotToAngle;//to~from
	UPROPERTY(EditAnyWhere, AdvancedDisplay)float ShotFromAngle;//to~from
	UPROPERTY(EditAnyWhere, AdvancedDisplay)float ShotInitialMaxSpeed;
	UPROPERTY(EditAnyWhere, AdvancedDisplay)float ShotInitialMinSpeed;
	UPROPERTY(EditAnyWhere, AdvancedDisplay)float ShotGravity;
	UPROPERTY(EditAnyWhere, AdvancedDisplay)uint8 ShotBouns;

};



UENUM(BlueprintType)
enum class E_MagatamaState:uint8
{
	Wait,
	Rote,
	Shot,
	Drop,
	Null
};


UCLASS(Blueprintable)
class GP41_KAMIMURATEAME_API AMagatamaBase : public AActor
{
	GENERATED_BODY()
private:
	//�X�e�[�g�}�V���p�̃f���Q�[�g
	DECLARE_DELEGATE_TwoParams(FMagatamaDelegate, AActor*, USceneComponent*);
	template<typename T>
	void AddStateMap(T* self, E_MagatamaState state, typename TMemFunPtrType<false, T, void(AActor*,USceneComponent*)>::Type func)
	{
		if (stateMap.Contains(state)) { return; }		//�o�^�ς�
		FMagatamaDelegate tmp;
		tmp.BindUObject(self, func);
		stateMap.Add(state, tmp);
	}
public:	
	// Sets default values for this actor's properties
	AMagatamaBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	//�X�e�[�g
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		E_MagatamaState state;

	UPROPERTY(BlueprintReadWrite, Category = RotationSpeed)
		float speedRate;	//�v���C���[�̍ő呬�x(1)�̎��̍ő�p���x�̊���

	//�R���|�[�l���g
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UProjectileMovementComponent* projectilemovement;

	UPROPERTY(EditDefaultsOnly, Category = State)
		UDataTable* PlayerStateDataTabel;
	UPROPERTY(EditDefaultsOnly, Category = State)
		UDataTable* MagatamaStateDataTabel;
	//�u���[�v�����g�֐�
	//�ݒ�֐�
	UFUNCTION(BlueprintCallable)
		void SetupPlayerUse(FVector PlayerPos,USceneComponent* com);
	UFUNCTION(BlueprintCallable)
		void SetupShot(FVector targetvec);
	UFUNCTION(BlueprintCallable)
		void ResetWait();
	UFUNCTION(BlueprintCallable)
		bool GetShotAngle(AActor* player)const;
	UFUNCTION(BlueprintCallable)
		void Update(AActor* playeractor, USceneComponent* com);
	UFUNCTION(BlueprintCallable)
		float GetDamage(float enemyHp)const;
	UFUNCTION(BlueprintCallable)
		FVector GetNockBackForce()const;
	UFUNCTION(BlueprintCallable)
		void SetState(FMagatamaState stat, FPState pstate);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void Start();
	virtual void Start_Implementation(){}

	//�֐�
	void AngleRotation(float len);
	void RoteUpdate(AActor* playeractor, USceneComponent* com);
	void AngleUpRotation(USceneComponent* com);
	void SetupDrop();

protected:
	TMap<E_MagatamaState, FMagatamaDelegate> stateMap;
	float roteangle;
	float timecout = 0;//�o�ߎ���
	float pichangle;
	uint8 shotboounscount;
	FTransform center;
	float distance;
	FVector shotvec=FVector::ZeroVector;
	//stat
	//�_���[�W
	float MaxDamage = 50.f;
	float MinDamage = 10.f;
	float MinKnockBackPower = 100;
	float MaxKnockBackPower = 300;
	//���x�ϐ�
	float maxBaseSpeed;//�ő呬�x
	float minBaseSpeed;//�ŏ����x
	float RoteMaxDistance;
	float RoteMinDistance;
	float Acceleration;//�����x
	//���˕ϐ�
	float ShotInitialMaxSpeed = 0;
	float ShotInitialMinSpeed = 0;
	float ShotGravity = 0.5f;
	float ShotToAngle = 0;
	float ShotFromAngle = 30;
	float initialVelocity;//�����x
	float ShotMaxSpeed;
	uint8 ShotBouns = 1;

	//Playre�|�C���^�[
	APlayerBase* base=nullptr;
	float playerheight=0;
};