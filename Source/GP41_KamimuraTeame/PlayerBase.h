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
		float MaxRadius;
	UPROPERTY(BlueprintReadWrite, Category = State)
		float MinRadius;
	UPROPERTY(BlueprintReadWrite, Category = State)
		uint8 MagatamaNum;
	UPROPERTY(BlueprintReadWrite, Category = State)
		bool ShotMagatama = true;
	
	UPROPERTY(BlueprintReadWrite, Category = State)
		bool GameOverFlg = false;
	

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = State)
		UDataTable* StateDataTabel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = State)
		FString LoadDataName;

	UFUNCTION(BlueprintCallable)
		void InitSetState(FPState state);
	UFUNCTION(BlueprintCallable)
		bool StaminaRegene(float axis);
	UFUNCTION(BlueprintCallable)
		void CheckVariable(float max, float& variable);
	UFUNCTION(BlueprintCallable)
		float GetRange(float rate)const;
	UFUNCTION(BlueprintCallable)
		void SetDamage(float damage, FVector force = FVector::ZeroVector, float power = 0.f);
	UFUNCTION(BlueprintCallable)
		void SetNockBack(FVector force,float power);
	UFUNCTION(BlueprintCallable)
		void NockBackUpdate();

	void AddMagatama(AMagatamaBase* magatama);
	void DeleteMagatama(AMagatamaBase* matagata);
private:
	float NockbackPower;
	FVector NockBackForce;
	TArray<AMagatamaBase*> hasMagatama;
};
