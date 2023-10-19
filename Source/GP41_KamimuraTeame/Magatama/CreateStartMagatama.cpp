// Fill out your copyright notice in the Description page of Project Settings.


#include "CreateStartMagatama.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
ACreateStartMagatama::ACreateStartMagatama()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    com = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneConponent"));
    RootComponent = com;
}

// Called when the game starts or when spawned
void ACreateStartMagatama::BeginPlay()
{
	Super::BeginPlay();
    TArray<FVector> list;
    GetNonOverlapVectorArray(CreateRange,31,CreateNum,list);
    FVector pos = GetActorLocation();
	for (int i = 0; i < CreateNum; i++) {
		GetWorld()->SpawnActor<AActor>(CreateActor, FTransform(FVector(pos.X+list[i].X, pos.Y+list[i].Y, CreateHeight)));
	}
}

// Called every frame
void ACreateStartMagatama::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



FVector ACreateStartMagatama::GetNonOverlapVector(float maxRange, int index, float repossessionRange, const TArray<FVector>& vectorArray)
{
    auto getRandomVector = [maxRange](FVector biasVector)
    {
        float xMin = FMath::Clamp((-maxRange) + biasVector.X, -maxRange, maxRange);
        float xMax = FMath::Clamp(maxRange + biasVector.X, -maxRange, maxRange);
        float yMin = FMath::Clamp((-maxRange) + biasVector.Y, -maxRange, maxRange);
        float yMax = FMath::Clamp(maxRange + biasVector.Y, -maxRange, maxRange);
        float xRange = FMath::RandRange(xMin, xMax);
        float yRange = FMath::RandRange(yMin, yMax);
        return FVector(xRange, yRange, 0);
    };
    float deg = FMath::RandRange(5.f, 30.f);
    float theta = FMath::DegreesToRadians(index * deg);
    int r = maxRange / 2;//中心点から最大範囲の中間の長さ
    FVector biasVector = FVector(FMath::Cos(theta) * r, FMath::Sin(theta) * r, 0);
    FVector retVector = getRandomVector(biasVector);
    const int safeTryCount = 10;
    int count = 0;
    bool isLoopEnd = false;
    //とり続けると設定値次第では無限LOOPしかねないのでセーフラインを設けておく
    while (!isLoopEnd && count < safeTryCount)
    {
        isLoopEnd = true;
        for (const auto& vec : vectorArray)
        {
            //再取得範囲内だったら再取得
            float xMax = vec.X + repossessionRange;
            float xMin = vec.X - repossessionRange;
            if (xMin <= retVector.X && retVector.X <= xMax)
            {
                retVector = getRandomVector(biasVector);
                isLoopEnd = false;
                break;
            }
            float yMax = vec.Y + repossessionRange;
            float yMin = vec.Y - repossessionRange;
            if (yMin <= retVector.Y && retVector.Y <= yMax)
            {
                retVector = getRandomVector(biasVector);
                isLoopEnd = false;
                break;
            }
        }
        count++;
    }
    return retVector;
}

void ACreateStartMagatama::GetNonOverlapVectorArray(float maxRange, float repossessionRange, int requestCount, TArray<FVector>& OutVecArray)
{
    OutVecArray.Empty();
    for (int i = 0; i < requestCount; i++)
    {
        FVector vector = GetNonOverlapVector(maxRange, i, repossessionRange, OutVecArray);
        OutVecArray.Add(vector);
    }
}