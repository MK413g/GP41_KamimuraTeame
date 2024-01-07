// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include"../Player/PlayerBase.h"
#include"../Manager/StageSettingManager.h"

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
	UPROPERTY(EditAnyWhere, AdvancedDisplay,meta=(ClampMin = "0.01", ClampMax = "1.0", UIMix="0.01",UIMax="1.0"))float HitStopTimeDilation;//hitStop
	UPROPERTY(EditAnyWhere, AdvancedDisplay,meta=(ClampMin = "0.01", ClampMax = "1.0", UIMix="0.01",UIMax="1.0"))float HitStopTime;//hitStop

//	UPROPERTY(EditAnyWhere, AdvancedDisplay)float MinKnockBackPower;
//	UPROPERTY(EditAnyWhere, AdvancedDisplay)float MaxKnockBackPower;

	//Rotation
	UPROPERTY(EditAnyWhere, AdvancedDisplay)float RotationMaxSpeed;//rotation max speed
	UPROPERTY(EditAnyWhere, AdvancedDisplay)float RotationMinSpeed;//rotetion min speed
	UPROPERTY(EditAnyWhere, AdvancedDisplay)float RotationAcceleration;//acceleration
	UPROPERTY(EditAnyWhere, AdvancedDisplay)float RotateionDamegeMax;
	UPROPERTY(EditAnyWhere, AdvancedDisplay)float RotateionDamegeMin;
	UPROPERTY(EditAnyWhere, AdvancedDisplay)float RotateionDamegeSpeed;

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

	UPROPERTY(BlueprintReadWrite, Category = RotationSpeed)
		float speedRate;	//プレイヤーの最大速度(1)の時の最大角速度の割合

	//コンポーネント
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UProjectileMovementComponent* projectilemovement;

	UPROPERTY(EditDefaultsOnly, Category = State)
		UDataTable* PlayerStateDataTabel;
	UPROPERTY(EditDefaultsOnly, Category = State)
		UDataTable* MagatamaStateDataTabel;
	//ブループリント関数
	//設定関数
	UFUNCTION(BlueprintCallable)
		bool SetupPlayerUse(FVector PlayerPos,USceneComponent* com);
	UFUNCTION(BlueprintCallable)
		bool SetupShot(FVector targetvec);
	UFUNCTION(BlueprintCallable)
		bool ResetWait(bool enemyhit);
	UFUNCTION(BlueprintCallable)
		bool GetShotAngle(AActor* player)const;
	UFUNCTION(BlueprintCallable)
		void Update(AActor* playeractor, USceneComponent* com);
	UFUNCTION(BlueprintPure)
		float GetDamage();
	UFUNCTION(BlueprintPure)
		float GetHitStopTime();
	UFUNCTION(BlueprintPure)
		float GetHitStopTimeDilation();
	UFUNCTION(BlueprintPure)
		FVector GetNockBackForce();
	UFUNCTION(BlueprintCallable)
		void SetState(FMagatamaState stat, FPState pstate);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void Start();
	virtual void Start_Implementation(){}
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void SettingFiled(E_FiledState filed);
	void SettingFiled_Implementation(E_FiledState filed){}

	UFUNCTION(BlueprintPure)
		bool GetHitEffect();
	UFUNCTION(BlueprintCallable)
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,FVector HitNoraml, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION(BlueprintPure)
		static bool GetHitStopFlg();
	UFUNCTION(BlueprintCallable)
		static void SetHitStopFlg(bool set);
	static bool HitStopFlg;
	//関数
	void AngleRotation(float len);
	void RoteUpdate(AActor* playeractor, USceneComponent* com);
	void AngleUpRotation(USceneComponent* com);
	void SetupDrop();
	void SetFiled(E_FiledState filed);
protected:
	TMap<E_MagatamaState, FMagatamaDelegate> stateMap;
	float roteangle;
	float timecout = 0;//経過時間
	float pichangle;
	uint8 shotboounscount;
	FTransform center;
	float distance;
	FVector shotvec=FVector::ZeroVector;
	//stat
	//ダメージ
	float MaxDamage = 50.f;
	float MinDamage = 10.f;
	float MinKnockBackPower = 100;
	float MaxKnockBackPower = 300;
	float HitStopTime=10.f;
	float HitStopTimeDilation=0.1f;
	//速度変数
	float maxBaseSpeed;//最大速度
	float minBaseSpeed;//最小速度
	float RoteMaxDistance;
	float RoteMinDistance;
	float RoteDamageMax;
	float RoteDamageMin;
	float RoteDamageSpeedRate;
	float Acceleration;//加速度
	//発射変数
	float ShotInitialMaxSpeed = 0;
	float ShotInitialMinSpeed = 0;
	float ShotGravity = 0.5f;
	float ShotToAngle = 0;
	float ShotFromAngle = 30;
	float initialVelocity;//初速度
	float ShotMaxSpeed;
	uint8 ShotBouns = 1;

	//Playreポインター
	APlayerBase* base=nullptr;
	float playerheight=0;
};
