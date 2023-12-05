// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerBase.generated.h"

class AMagatamaBase;

USTRUCT(BlueprintType)
struct  FPState :public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnyWhere, AdvancedDisplay)float HP;
	UPROPERTY(EditAnyWhere, AdvancedDisplay)float StaminaRegeneration;
	UPROPERTY(EditAnyWhere, AdvancedDisplay)float Stamina;
	UPROPERTY(EditAnyWhere, AdvancedDisplay)float RotationSpeed;
	UPROPERTY(EditAnyWhere, AdvancedDisplay)float JumpBraking;
	UPROPERTY(EditAnyWhere, AdvancedDisplay)float JumpPower;
	UPROPERTY(EditAnyWhere, AdvancedDisplay)float JumpDownPower;
	UPROPERTY(EditAnyWhere, AdvancedDisplay)float MoveSpeed;
	UPROPERTY(EditAnyWhere, AdvancedDisplay)float RunMoveSpeed;
	UPROPERTY(EditAnyWhere, AdvancedDisplay)float MoveBraking;
	UPROPERTY(EditAnyWhere, AdvancedDisplay)float MoveBrakingFrictionFactor;
	UPROPERTY(EditAnyWhere, AdvancedDisplay)float MoveChangeSpeed;
	//UPROPERTY(EditAnyWhere, AdvancedDisplay)float MoveAcceleration;
	//UPROPERTY(EditAnyWhere, AdvancedDisplay)float RunMoveStamina;

	UPROPERTY(EditAnyWhere, AdvancedDisplay)float MaxRadius;
	UPROPERTY(EditAnyWhere, AdvancedDisplay)float MinRadius;
};


UCLASS()
class GP41_KAMIMURATEAME_API APlayerBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UPROPERTY(BlueprintReadWrite, Category = State)
		float HpMax;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = State)
		float Hp;
	UPROPERTY(VisibleInstanceOnly,BlueprintReadWrite, Category = State)
		float Stamina;
	UPROPERTY(BlueprintReadWrite, Category = State)
		float StaminaMax;
	UPROPERTY(BlueprintReadWrite, Category = State)
		float StaminaRegeneration;
	UPROPERTY(BlueprintReadWrite, Category = State)
		float RotationSpeed;
	UPROPERTY(BlueprintReadWrite, Category = State)
		float Braking;
	UPROPERTY(BlueprintReadWrite, Category = State)
		float JumpPower;
	UPROPERTY(BlueprintReadWrite, Category = State)
		float JumpDownPower;
	UPROPERTY(BlueprintReadWrite, Category = State)
		float MaxRadius;
	UPROPERTY(BlueprintReadWrite, Category = State)
		float MinRadius;
	UPROPERTY(BlueprintReadWrite, Category = State)
		float MoveSpeed;
	UPROPERTY(BlueprintReadWrite, Category = State)
		float MoveRunSpeed;
	UPROPERTY(BlueprintReadWrite, Category = State)
		float MoveBreaking;
	UPROPERTY(BlueprintReadWrite, Category = State)
		float MoveChangeSpeed;
	UPROPERTY(BlueprintReadWrite, Category = State)
		uint8 MagatamaNum;
	UPROPERTY(BlueprintReadWrite, Category = State)
		bool ShotMagatama = true;
	
	UPROPERTY(BlueprintReadWrite, Category = State)
		bool stunflg;

	UPROPERTY(BlueprintReadWrite)
		bool lockonflg;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = State)
		UDataTable* StateDataTabel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = State)
		FString LoadDataName;



	UFUNCTION(BlueprintCallable)
		void InitSetState(FPState state);
	UFUNCTION(BlueprintCallable)
		bool SetDataTable();
	UFUNCTION(BlueprintCallable)
		bool StaminaRegene(float axis);
	UFUNCTION(BlueprintCallable)
		void CheckVariable(float max, float& variable);
	UFUNCTION(BlueprintPure)
		float GetRange(float rate)const;
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
		void SetDamage(float damage, FVector force = FVector::ZeroVector, float power = 0.f);
	virtual void SetDamage_Implementation(float damage, FVector force = FVector::ZeroVector, float power = 0.f);

	UFUNCTION(BlueprintCallable)
		void SetNockBack(FVector force,float power);
	UFUNCTION(BlueprintCallable)
		void NockBackUpdate();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void SetGameOver();
	virtual void SetGameOver_Implementation(){}
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void SetInit(float Mspeed, float Jpower,float MBreaking,float MoveBrakingFrictionFactor);
	virtual void SetInit_Implementation(float Mspeed, float Jpower,float MBreaking,float MoveBrakingFrictionFactor){}
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void SetStun(float time);
	virtual void SetStun_Implementation(float time);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void StunRecovery();
	virtual void StunRecovery_Implementation();
	UFUNCTION(BlueprintCallable)
		bool CheckMoveForward(float inputvalue,float speed,float& reinput);
	UFUNCTION(BlueprintCallable)
		bool CheckMoveRight(float inputvalue,float speed,float &reinput);
	UFUNCTION(BlueprintCallable)
		void HasMagatamaHidden();


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SettingStartRun();
	void SettingStartRun_Implementation();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SettingEndRun();
	void SettingEndRun_Implementation();
	UFUNCTION(BlueprintPure)
		bool GetRunFlg();
	UFUNCTION(BlueprintCallable)
	void LockOnEnemy();
	
	void AddMagatama(AMagatamaBase* magatama);
	void Damage(float damage, FVector force = FVector::ZeroVector, float power = 0.f);
	void DeleteMagatama(AMagatamaBase* matagata);

	//TickçXêV
	void UpdateStun(float deltatime);
	void LockOnUpdate(float deltatime);
	void UpdateRun(float deltatime);

private:
	bool GetEnemyActor(AActor** target);

	FVector nowlockAngle;
	FVector lockontargetAngle;
	float lockonCount;
	const float lockonTime = 0.1f;
	
	float runstamina;
	FVector2D runrate;
	bool runflg;

	const float oldinputrate = 0.3f;
	float oldinputforwardValue;
	float oldinputrightValue;
	float stunTime,stunCuntTime;
	float NockbackPower;
	FVector NockBackForce;
	TArray<AMagatamaBase*> hasMagatama;
};
