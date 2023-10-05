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
	//ステートマシン用のデリゲート
	DECLARE_DELEGATE_TwoParams(FMagatamaDelegate, AActor*, USceneComponent*);
	template<typename T>
	void AddStateMap(T* self, E_MagatamaState state, typename TMemFunPtrType<false, T, void(AActor*,USceneComponent*)>::Type func)
	{
		if (stateMap.Contains(state)) { return; }		//登録済み
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
	//ステート
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		E_MagatamaState state;

	//ダメージ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		float MaxDamage = 50.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		float MinDamage = 10.f;

	//速度変数
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=RotationSpeed)
		float maxBaseSpeed;//最大速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RotationSpeed)
		float minBaseSpeed;//最小速度
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = RotationSpeed)
		float speedRate;	//プレイヤーの最大速度(1)の時の最大角速度の割合
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RotationSpeed)
		float initialVelocity;//初速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RotationSpeed)
		float speed;//加速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RotationSpeed)
		float RoteMaxDistance;//加速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RotationSpeed)
		float RoteMinDistance;//加速度

	//発射変数
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

	//ラグ変数
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Lag)
		bool isLagLocation;//ラグの処理を行うか
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Lag)
		float LocationLgSpeed;//速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Lag)
		float LagMaxDistance;//最大ラグ距離

	//コンポーネント
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UProjectileMovementComponent* projectilemovement;

	//ブループリント関数
	//設定関数
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

	//関数
	void AngleRotation(float len);
	void RoteUpdate(AActor* playeractor, USceneComponent* com);
	void AngleUpRotation(USceneComponent* com);
	void LagUpdate();

protected:
	TMap<E_MagatamaState, FMagatamaDelegate> stateMap;
	float roteangle;
	float timecout = 0;//経過時間
	float pichangle;
	uint8 shotboounscount;
	FTransform center;
	float distance;
};
