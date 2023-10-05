// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/ProjectileMovementComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagatamaBase.generated.h"

UENUM(BlueprintType)
enum class E_MagatamaState:uint8
{
	Wait,
	Rote,
	Shot,
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

	//�_���[�W
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		float MaxDamage = 50.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		float MinDamage = 10.f;

	//���x�ϐ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=RotationSpeed)
		float maxBaseSpeed;//�ő呬�x
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RotationSpeed)
		float minBaseSpeed;//�ŏ����x
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = RotationSpeed)
		float speedRate;	//�v���C���[�̍ő呬�x(1)�̎��̍ő�p���x�̊���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RotationSpeed)
		float initialVelocity;//�����x
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RotationSpeed)
		float speed;//�����x
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RotationSpeed)
		float RoteMaxDistance;//�����x
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RotationSpeed)
		float RoteMinDistance;//�����x

	//���˕ϐ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shot)
		float ShotMaxSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shot)
		float ShotInitialMaxSpeed=0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shot)
		float ShotInitialMinSpeed=0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shot)
		float ShotGravity=0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shot)
		float ShotToAngle=30;//[to]~[from]
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shot)
		float ShotFromAngle=30;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shot)
		uint8 ShotBouns = 1;

	//���O�ϐ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Lag)
		bool isLagLocation;//���O�̏������s����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Lag)
		float LocationLgSpeed;//���x
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Lag)
		float LagMaxDistance;//�ő僉�O����

	//�R���|�[�l���g
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UProjectileMovementComponent* projectilemovement;

	//�u���[�v�����g�֐�
	//�ݒ�֐�
	UFUNCTION(BlueprintCallable)
		void SetupPlayerUse(FVector PlayerPos,USceneComponent* com);
	UFUNCTION(BlueprintCallable)
		void SetupShot(USceneComponent* com,FVector targetvec);
	UFUNCTION(BlueprintCallable)
		void ResetWait();
	UFUNCTION(BlueprintCallable)
		bool GetShotAngle(AActor* player)const;
	UFUNCTION(BlueprintCallable)
		void Update(AActor* playeractor, USceneComponent* com);
	UFUNCTION(BlueprintCallable)
		float GetDamage(float enemyHp)const;

	//�֐�
	void AngleRotation(float len);
	void RoteUpdate(AActor* playeractor, USceneComponent* com);
	void AngleUpRotation(USceneComponent* com);
	void LagUpdate();

protected:
	TMap<E_MagatamaState, FMagatamaDelegate> stateMap;
	float roteangle;
	float timecout = 0;//�o�ߎ���
	float pichangle;
	uint8 shotboounscount;
	FTransform center;
	float distance;
};
