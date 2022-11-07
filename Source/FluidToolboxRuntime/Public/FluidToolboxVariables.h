// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/GameInstance.h"
#include "Engine/LatentActionManager.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "FluidToolboxVariables.generated.h"

//Fluid Toolbox Variables Actor Tags
USTRUCT()
struct FFluidToolboxVariablesActorTags
{
	GENERATED_BODY()

	TArray<FName> Tags;
};

// Fluid Toolbox Variables Blueprint Function Library (Clean Wrapper For Subsystem For Blueprint Users)
UCLASS()
class FLUIDTOOLBOXRUNTIME_API UFluidToolboxVariablesBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

//Fluid Toolbox Variables Blueprint Function Library : Bool Tools
public:
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetRememberBoolByName(const FName Name, bool& Value_Out, bool& bFound_Out);
	UFUNCTION(BlueprintCallable, Category = "Fluid Toolbox Variables")
		static void SetRememberBoolByName(const FName Name, const bool Value, bool& Value_Out);
	UFUNCTION(BlueprintCallable, Category = "Fluid Toolbox Variables")
		static void ToggleRememberBoolByName(const FName Name, bool& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetRandomBoolByWeight(TArray<bool> Values, TArray<float> Weights, const int MinCount, const int MaxCount, TArray<bool>& Values_Out, bool& ValueFirst_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetBoolByChance(const bool Value, const int Chance, const int RollAmount, TArray<bool>& Values_Out, bool& ValueFirst_Out, bool& bHasValues_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetBoolByChanceMulti(const TArray<bool> Values, const TArray<int> Chances, const TArray<int> RollAmounts, TArray<bool>& Values_Out, bool& ValueFirst_Out, bool& bHasValues_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeBoolArrays2(const TArray<bool> A, const TArray<bool> B, TArray<bool>& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeBoolArrays3(const TArray<bool> A, const TArray<bool> B, const TArray<bool> C, TArray<bool>& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeBoolArrays4(const TArray<bool> A, const TArray<bool> B, const TArray<bool> C, const TArray<bool> D, TArray<bool>& Value_Out);

//Fluid Toolbox Variables Blueprint Function Library : Byte Tools
public:
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetRememberByteByName(const FName Name, uint8& Value_Out, bool& bFound_Out);
	UFUNCTION(BlueprintCallable, Category = "Fluid Toolbox Variables")
		static void SetRememberByteByName(const FName Name, const uint8 Value, uint8& Value_Out);
	UFUNCTION(BlueprintCallable, Category = "Fluid Toolbox Variables")
		static void IncrementRememberByteByName(const FName Name, const int Amount, uint8& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetRandomByteByWeight(TArray<uint8> Values, TArray<float> Weights, const int MinCount, const int MaxCount, TArray<uint8>& Values_Out, uint8& ValueFirst_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetByteByChance(const uint8 Value, const int Chance, const int RollAmount, TArray<uint8>& Values_Out, uint8& ValueFirst_Out, bool& bHasValues_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetByteByChanceMulti(const TArray<uint8> Values, const TArray<int> Chances, const TArray<int> RollAmounts, TArray<uint8>& Values_Out, uint8& ValueFirst_Out, bool& bHasValues_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void SortBytes(TArray<uint8> Values, const bool bDescending, TArray<uint8>& Values_Out, uint8& ValueFirst_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeByteArrays2(const TArray<uint8> A, const TArray<uint8> B, TArray<uint8>& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeByteArrays3(const TArray<uint8> A, const TArray<uint8> B, const TArray<uint8> C, TArray<uint8>& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeByteArrays4(const TArray<uint8> A, const TArray<uint8> B, const TArray<uint8> C, const TArray<uint8> D, TArray<uint8>& Value_Out);
	
//Fluid Toolbox Variables Blueprint Function Library : Int Tools
public:
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetRememberIntByName(const FName Name, int& Value_Out, bool& bFound_Out);
	UFUNCTION(BlueprintCallable, Category = "Fluid Toolbox Variables")
		static void SetRememberIntByName(const FName Name, const int Value, int& Value_Out);
	UFUNCTION(BlueprintCallable, Category = "Fluid Toolbox Variables")
		static void IncrementRememberIntByName(const FName Name, const int Amount, int& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetRandomIntByWeight(TArray<int> Values, TArray<float> Weights, const int MinCount, const int MaxCount, TArray<int>& Values_Out, int& ValueFirst_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetIntByChance(const int Value, const int Chance, const int RollAmount, TArray<int>& Values_Out, int& ValueFirst_Out, bool& bHasValues_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetIntByChanceMulti(const TArray<int> Values, const TArray<int> Chances, const TArray<int> RollAmounts, TArray<int>& Values_Out, int& ValueFirst_Out, bool& bHasValues_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void SortIntS(TArray<int> Values, const bool bDescending, TArray<int>& Values_Out, int& ValueFirst_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeIntArrays2(const TArray<int> A, const TArray<int> B, TArray<int>& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeIntArrays3(const TArray<int> A, const TArray<int> B, const TArray<int> C, TArray<int>& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeIntArrays4(const TArray<int> A, const TArray<int> B, const TArray<int> C, const TArray<int> D, TArray<int>& Value_Out);

//Fluid Toolbox Variables Blueprint Function Library : Int 64 Tools
public:
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetRememberInt64ByName(const FName Name, int64& Value_Out, bool& bFound_Out);
	UFUNCTION(BlueprintCallable, Category = "Fluid Toolbox Variables")
		static void SetRememberInt64ByName(const FName Name, int64 Value, int64& Value_Out);
	UFUNCTION(BlueprintCallable, Category = "Fluid Toolbox Variables")
		static void IncrementRememberInt64ByName(const FName Name, const int64 Amount, int64& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetRandomInt64ByWeight(TArray<int64> Values, TArray<float> Weights, const int MinCount, const int MaxCount, TArray<int64>& Values_Out, int64& ValueFirst_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetInt64ByChance(const int64 Value, const int Chance, const int RollAmount, TArray<int64>& Values_Out, int64& ValueFirst_Out, bool& bHasValues_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetInt64ByChanceMulti(const TArray<int64> Values, const TArray<int> Chances, const TArray<int> RollAmounts, TArray<int64>& Values_Out, int64& ValueFirst_Out, bool& bHasValues_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void SortInt64S(TArray<int64> Values, const bool bDescending, TArray<int64>& Values_Out, int64& ValueFirst_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeInt64Arrays2(const TArray<int64> A, const TArray<int64> B, TArray<int64>& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeInt64Arrays3(const TArray<int64> A, const TArray<int64> B, const TArray<int64> C, TArray<int64>& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeInt64Arrays4(const TArray<int64> A, const TArray<int64> B, const TArray<int64> C, const TArray<int64> D, TArray<int64>& Value_Out);
	
//Fluid Toolbox Variables Blueprint Function Library : Float Tools
public:
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetRememberFloatByName(const FName Name, float& Value_Out, bool& bFound_Out);
	UFUNCTION(BlueprintCallable, Category = "Fluid Toolbox Variables")
		static void SetRememberFloatByName(const FName Name, float Value, float& Value_Out);
	UFUNCTION(BlueprintCallable, Category = "Fluid Toolbox Variables")
		static void IncrementRememberFloatByName(const FName Name, const float Amount, float& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetRandomFloatByWeight(TArray<float> Values, TArray<float> Weights, const int MinCount, const int MaxCount, TArray<float>& Values_Out, float& ValueFirst_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetFloatByChance(const float Value, const int Chance, const int RollAmount, TArray<float>& Values_Out, float& ValueFirst_Out, bool& bHasValues_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetFloatByChanceMulti(const TArray<float> Values, const TArray<int> Chances, const TArray<int> RollAmounts, TArray<float>& Values_Out, float& ValueFirst_Out, bool& bHasValues_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void SortFloats(TArray<float> Values, bool bDescending, TArray<float>& Values_Out, float& ValueFirst_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeFloatArrays2(const TArray<float> A, const TArray<float> B, TArray<float>& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeFloatArrays3(const TArray<float> A, const TArray<float> B, const TArray<float> C, TArray<float>& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeFloatArrays4(const TArray<float> A, const TArray<float> B, const TArray<float> C, const TArray<float> D, TArray<float>& Value_Out);

//Fluid Toolbox Variables Blueprint Function Library : Double Tools
public:
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetRememberDoubleByName(const FName Name, double& Value_Out, bool& bFound_Out);
	UFUNCTION(BlueprintCallable, Category = "Fluid Toolbox Variables")
		static void SetRememberDoubleByName(const FName Name, double Value, double& Value_Out);
	UFUNCTION(BlueprintCallable, Category = "Fluid Toolbox Variables")
		static void IncrementRememberDoubleByName(const FName Name, const double Amount, double& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetRandomDoubleByWeight(TArray<double> Values, TArray<float> Weights, const int MinCount, const int MaxCount, TArray<double>& Values_Out, double& ValueFirst_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetDoubleByChance(const double Value, const int Chance, const int RollAmount, TArray<double>& Values_Out, double& ValueFirst_Out, bool& bHasValues_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetDoubleByChanceMulti(const TArray<double> Values, const TArray<int> Chances, const TArray<int> RollAmounts, TArray<double>& Values_Out, double& ValueFirst_Out, bool& bHasValues_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void SortDoubles(TArray<double> Values, bool bDescending, TArray<double>& Values_Out, double& ValueFirst_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeDoubleArrays2(const TArray<double> A, const TArray<double> B, TArray<double>& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeDoubleArrays3(const TArray<double> A, const TArray<double> B, const TArray<double> C, TArray<double>& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeDoubleArrays4(const TArray<double> A, const TArray<double> B, const TArray<double> C, const TArray<double> D, TArray<double>& Value_Out);
	
//Fluid Toolbox Variables Blueprint Function Library : Name Tools
public:
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetRememberNameByName(const FName Name, FName& Value_Out, bool& bFound_Out);
	UFUNCTION(BlueprintCallable, Category = "Fluid Toolbox Variables")
		static void SetRememberNameByName(const FName Name, const FName Value, FName& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetRandomNameByWeight(TArray<FName> Values, TArray<float> Weights, const int MinCount, const int MaxCount, TArray<FName>& Values_Out, FName& ValueFirst_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetNameByChance(const FName Value, const int Chance, const int RollAmount, TArray<FName>& Values_Out, FName& ValueFirst_Out, bool& bHasValues_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetNameByChanceMulti(const TArray<FName> Values, const TArray<int> Chances, const TArray<int> RollAmounts, TArray<FName>& Values_Out, FName& ValueFirst_Out, bool& bHasValues_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeNameArrays2(const TArray<FName> A, const TArray<FName> B, TArray<FName>& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeNameArrays3(const TArray<FName> A, const TArray<FName> B, const TArray<FName> C, TArray<FName>& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeNameArrays4(const TArray<FName> A, const TArray<FName> B, const TArray<FName> C, const TArray<FName> D, TArray<FName>& Value_Out);
	
//Fluid Toolbox Variables Blueprint Function Library : String Tools
public:
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetRememberStringByName(const FName Name, FString& Value_Out, bool& bFound_Out);
	UFUNCTION(BlueprintCallable, Category = "Fluid Toolbox Variables")
		static void SetRememberStringByName(const FName Name, const FString Value, FString& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetRandomStringByWeight(TArray<FString> Values, TArray<float> Weights, const int MinCount, const int MaxCount, TArray<FString>& Values_Out, FString& ValueFirst_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetStringByChance(const FString Value, const int Chance, const int RollAmount, TArray<FString>& Values_Out, FString& ValueFirst_Out, bool& bHasValues_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetStringByChanceMulti(const TArray<FString> Values, const TArray<int> Chances, const TArray<int> RollAmounts, TArray<FString>& Values_Out, FString& ValueFirst_Out, bool& bHasValues_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeStringArrays2(const TArray<FString> A, const TArray<FString> B, TArray<FString>& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeStringArrays3(const TArray<FString> A, const TArray<FString> B, const TArray<FString> C, TArray<FString>& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeStringArrays4(const TArray<FString> A, const TArray<FString> B, const TArray<FString> C, const TArray<FString> D, TArray<FString>& Value_Out);

//Fluid Toolbox Variables Blueprint Function Library : Text Tools
public:
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetRememberTextByName(const FName Name, FText& Value_Out, bool& bFound_Out);
	UFUNCTION(BlueprintCallable, Category = "Fluid Toolbox Variables")
		static void SetRememberTextByName(const FName Name, const FText Value, FText& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetRandomTextByWeight(TArray<FText> Values, TArray<float> Weights, const int MinCount, const int MaxCount, TArray<FText>& Values_Out, FText& ValueFirst_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetTextByChance(const FText Value, const int Chance, const int RollAmount, TArray<FText>& Values_Out, FText& ValueFirst_Out, bool& bHasValues_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetTextByChanceMulti(const TArray<FText> Values, const TArray<int> Chances, const TArray<int> RollAmounts, TArray<FText>& Values_Out, FText& ValueFirst_Out, bool& bHasValues_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeTextArrays2(const TArray<FText> A, const TArray<FText> B, TArray<FText>& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeTextArrays3(const TArray<FText> A, const TArray<FText> B, const TArray<FText> C, TArray<FText>& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeTextArrays4(const TArray<FText> A, const TArray<FText> B, const TArray<FText> C, const TArray<FText> D, TArray<FText>& Value_Out);

//Fluid Toolbox Variables Blueprint Function Library : Vector Tools
public:
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetRememberVectorByName(const FName Name, FVector& Value_Out, bool& bFound_Out);
	UFUNCTION(BlueprintCallable, Category = "Fluid Toolbox Variables")
		static void SetRememberVectorByName(const FName Name, FVector Value, FVector& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetRandomVectorByWeight(TArray<FVector> Values, TArray<float> Weights, const int MinCount, const int MaxCount, TArray<FVector>& Values_Out, FVector& ValueFirst_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetVectorByChance(const FVector Value, const int Chance, const int RollAmount, TArray<FVector>& Values_Out, FVector& ValueFirst_Out, bool& bHasValues_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetVectorByChanceMulti(const TArray<FVector> Values, const TArray<int> Chances, const TArray<int> RollAmounts, TArray<FVector>& Values_Out, FVector& ValueFirst_Out, bool& bHasValues_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeVectorArrays2(const TArray<FVector> A, const TArray<FVector> B, TArray<FVector>& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeVectorArrays3(const TArray<FVector> A, const TArray<FVector> B, const TArray<FVector> C, TArray<FVector>& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeVectorArrays4(const TArray<FVector> A, const TArray<FVector> B, const TArray<FVector> C, const TArray<FVector> D, TArray<FVector>& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetVectorsAverageLocation(TArray<FVector> Values, FVector& Value_Out);

//Fluid Toolbox Variables Blueprint Function Library : Rotator Tools
public:
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetRememberRotatorByName(const FName Name, FRotator& Value_Out, bool& bFound_Out);
	UFUNCTION(BlueprintCallable, Category = "Fluid Toolbox Variables")
		static void SetRememberRotatorByName(const FName Name, const FRotator Value, FRotator& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetRandomRotatorByWeight(TArray<FRotator> Values, TArray<float> Weights, const int MinCount, const int MaxCount, TArray<FRotator>& Values_Out, FRotator& ValueFirst_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetRotatorByChance(const FRotator Value, const int Chance, const int RollAmount, TArray<FRotator>& Values_Out, FRotator& ValueFirst_Out, bool& bHasValues_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetRotatorByChanceMulti(const TArray<FRotator> Values, const TArray<int> Chances, const TArray<int> RollAmounts, TArray<FRotator>& Values_Out, FRotator& ValueFirst_Out, bool& bHasValues_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeRotatorArrays2(const TArray<FRotator> A, const TArray<FRotator> B, TArray<FRotator>& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeRotatorArrays3(const TArray<FRotator> A, const TArray<FRotator> B, const TArray<FRotator> C, TArray<FRotator>& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeRotatorArrays4(const TArray<FRotator> A, const TArray<FRotator> B, const TArray<FRotator> C, const TArray<FRotator> D, TArray<FRotator>& Value_Out);

//Fluid Toolbox Variables Blueprint Function Library : Transform Tools
public:
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetRememberTransformByName(const FName Name, FTransform& Value_Out, bool& bFound_Out);
	UFUNCTION(BlueprintCallable, Category = "Fluid Toolbox Variables")
		static void SetRememberTransformByName(const FName Name, const FTransform Value, FTransform& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetRandomTransformByWeight(TArray<FTransform> Values, const TArray<float> Weights, const int MinCount, const int MaxCount, TArray<FTransform>& Values_Out, FTransform& ValueFirst_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetTransformByChance(const FTransform Value, const int Chance, const int RollAmount, TArray<FTransform>& Values_Out, FTransform& ValueFirst_Out, bool& bHasValues_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetTransformByChanceMulti(const TArray<FTransform> Values, const TArray<int> Chances, const TArray<int> RollAmounts, TArray<FTransform>& Values_Out, FTransform& ValueFirst_Out, bool& bHasValues_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeTransformArrays2(const TArray<FTransform> A, const TArray<FTransform> B, TArray<FTransform>& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeTransformArrays3(const TArray<FTransform> A, const TArray<FTransform> B, const TArray<FTransform> C, TArray<FTransform>& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeTransformArrays4(const TArray<FTransform> A, const TArray<FTransform> B, const TArray<FTransform> C, const TArray<FTransform> D, TArray<FTransform>& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetTransformsAverageLocation(TArray<FTransform> Values, FVector& Value_Out);

//Fluid Toolbox Variables Blueprint Function Library : UObject Tools
public:
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetRememberObjectByName(const FName Name, UObject*& Value_Out, bool& bValueValid_Out, bool& bFound_Out);
	UFUNCTION(BlueprintCallable, Category = "Fluid Toolbox Variables")
		static void SetRememberObjectByName(const FName Name, UObject* Value,UObject*& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetRandomObjectByWeight(TArray<UObject*> Values, TArray<float> Weights, const int MinCount, const int MaxCount, TArray<UObject*>& Values_Out, UObject*& ValueFirst_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetObjectByChance(UObject* Value, const int Chance, const int RollAmount, TArray<UObject*>& Values_Out, UObject*& ValueFirst_Out, bool& bHasValues_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetObjectByChanceMulti(TArray<UObject*> Values, const TArray<int> Chances, const TArray<int> RollAmounts, TArray<UObject*>& Values_Out, UObject*& ValueFirst_Out, bool& bHasValues_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeObjectArrays2(const TArray<UObject*> A, const TArray<UObject*> B, TArray<UObject*>& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeObjectArrays3(const TArray<UObject*> A, const TArray<UObject*> B, const TArray<UObject*> C, TArray<UObject*>& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeObjectArrays4(const TArray<UObject*> A, const TArray<UObject*> B, const TArray<UObject*> C, const TArray<UObject*> D, TArray<UObject*>& Value_Out);

	
//Fluid Toolbox Variables Blueprint Function Library : Actor Tools
public:
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetRememberActorByName(const FName Name, AActor*& Value_Out, bool& bValueValid_Out, bool& bFound_Out);
	UFUNCTION(BlueprintCallable, Category = "Fluid Toolbox Variables")
		static void SetRememberActorByName(const FName Name, AActor* Value, AActor*& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetRandomActorByWeight(TArray<AActor*> Values, TArray<float> Weights, const int MinCount, const int MaxCount, TArray<AActor*>& Values_Out, AActor*& ValueFirst_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetActorByChance(AActor* Value, const int Chance, const int RollAmount, TArray<AActor*>& Values_Out, AActor*& ValueFirst_Out, bool& bHasValues_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetActorByChanceMulti(TArray<AActor*> Values, const TArray<int> Chances, const TArray<int> RollAmounts, TArray<AActor*>& Values_Out, AActor*& ValueFirst_Out, bool& bHasValues_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeActorArrays2(const TArray<AActor*> A, const TArray<AActor*> B, TArray<AActor*>& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeActorArrays3(const TArray<AActor*> A, const TArray<AActor*> B, const TArray<AActor*> C, TArray<AActor*>& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeActorArrays4(const TArray<AActor*> A, const TArray<AActor*> B, const TArray<AActor*> C, const TArray<AActor*> D, TArray<AActor*>& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetActorsAverageLocation(TArray<AActor*> Values, FVector& Value_Out);
	UFUNCTION(BlueprintCallable, Category = "Fluid Toolbox Variables")
		static void AddActorTag(AActor* Actor, const FName Tag, bool& bDidAddTag_Out);
	UFUNCTION(BlueprintCallable, Category = "Fluid Toolbox Variables")
        static void AddActorTags(AActor* Actor, const TArray<FName> Tags, bool& bDidAddTag_Out);
	UFUNCTION(BlueprintCallable, Category = "Fluid Toolbox Variables")
		static void RemoveActorTag(AActor* Actor, const FName Tag, bool& bDidRemoveTag_Out);
	UFUNCTION(BlueprintCallable, Category = "Fluid Toolbox Variables")
        static void RemoveActorTags(AActor* Actor, const TArray<FName> Tags, bool& bDidRemoveTag_Out);
	UFUNCTION(BlueprintCallable, Category = "Fluid Toolbox Variables")
		static void RemoveActorTagsThatContainsString(AActor* Actor, const FString String, bool& bDidRemoveTag_Out);
	UFUNCTION(BlueprintCallable, Category = "Fluid Toolbox Variables")
		static void GetActorTagThatContainsString(AActor* Actor, const FString String, TArray<FName>& Tags_Out, FName& TagAtFirstIndex_out, bool& bHasTags_Out);

//Fluid Toolbox Variables Blueprint Function Library : UClass Tools
public:
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetRememberActorUClassByName(const FName Name, TSubclassOf<AActor>& Value_Out, bool& bValueValid_Out, bool& bFound_Out);
	UFUNCTION(BlueprintCallable, Category = "Fluid Toolbox Variables")
		static void SetRememberActorUClassByName(const FName Name, TSubclassOf<AActor> Value, TSubclassOf<AActor>& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetRandomActorUClassByWeight(TArray<TSubclassOf<AActor>> Values, TArray<float> Weights, const int MinCount, const int MaxCount, TArray<TSubclassOf<AActor>>& Values_Out, TSubclassOf<AActor>& ValueFirst_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetActorUClassByChance(const TSubclassOf<AActor> Value, const int Chance, const int RollAmount, TArray<TSubclassOf<AActor>>& Values_Out, TSubclassOf<AActor>& ValueFirst_Out, bool& bHasValues_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetActorUClassByChanceMulti(const TArray<TSubclassOf<AActor>> Values, const TArray<int> Chances, const TArray<int> RollAmounts, TArray<TSubclassOf<AActor>>& Values_Out, TSubclassOf<AActor>& ValueFirst_Out, bool& bHasValues_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeActorUClassArrays2(const TArray<TSubclassOf<AActor>> A, const TArray<TSubclassOf<AActor>> B, TArray<TSubclassOf<AActor>>& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeActorUClassArrays3(const TArray<TSubclassOf<AActor>> A, const TArray<TSubclassOf<AActor>> B, const TArray<TSubclassOf<AActor>> C, TArray<TSubclassOf<AActor>>& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeActorUClassArrays4(const TArray<TSubclassOf<AActor>> A, const TArray<TSubclassOf<AActor>> B, const TArray<TSubclassOf<AActor>> C, const TArray<TSubclassOf<AActor>> D, TArray<TSubclassOf<AActor>>& Value_Out);
	UFUNCTION(BlueprintCallable, Category = "Fluid Toolbox Variables")
	    static void SpawnActorFromActorUClass(TSubclassOf<AActor> ActorClass, const FTransform Transform, ESpawnActorCollisionHandlingMethod SpawnActorCollisionHandlingMethod, bool bDebugActorClassName, AActor*& Actor_Out);
    
//Fluid Toolbox Variables Blueprint Function Library : UClass Tools
public:
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetRememberUClassByName(const FName Name, UClass*& Value_Out, bool& bValueValid_Out, bool& bFound_Out);
	UFUNCTION(BlueprintCallable, Category = "Fluid Toolbox Variables")
		static void SetRememberUClassByName(const FName Name, UClass* Value, UClass*& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetRandomUClassByWeight(TArray<UClass*> Values, TArray<float> Weights, const int MinCount, const int MaxCount, TArray<UClass*>& Values_Out, UClass*& ValueFirst_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetUClassByChance(UClass* Value, const int Chance, const int RollAmount, TArray<UClass*>& Values_Out, UClass*& ValueFirst_Out, bool& bHasValues_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetUClassByChanceMulti(TArray<UClass*> Values, const TArray<int> Chances, const TArray<int> RollAmounts, TArray<UClass*>& Values_Out, UClass*& ValueFirst_Out, bool& bHasValues_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeUClassArrays2(const TArray<UClass*> A, const TArray<UClass*> B, TArray<UClass*>& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeUClassArrays3(const TArray<UClass*> A, const TArray<UClass*> B, const TArray<UClass*> C, TArray<UClass*>& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeUClassArrays4(const TArray<UClass*> A, const TArray<UClass*> B, const TArray<UClass*> C, const TArray<UClass*> D, TArray<UClass*>& Value_Out);

//Fluid Toolbox Variables Blueprint Function Library : Scene Component Tools
public:
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetRememberSceneComponentByName(const FName Name, USceneComponent*& Value_Out, bool& bValueValid_Out, bool& bFound_Out);
	UFUNCTION(BlueprintCallable, Category = "Fluid Toolbox Variables")
		static void SetRememberSceneComponentByName(const FName Name, USceneComponent* Value, USceneComponent*& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetRandomSceneComponentByWeight(TArray<USceneComponent*> Values, TArray<float> Weights, const int MinCount, const int MaxCount, TArray<USceneComponent*>& Values_Out, USceneComponent*& ValueFirst_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetSceneComponentByChance(USceneComponent* Value, const int Chance, const int RollAmount, TArray<USceneComponent*>& Values_Out, USceneComponent*& ValueFirst_Out, bool& bHasValues_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetSceneComponentByChanceMulti(TArray<USceneComponent*> Values, const TArray<int> Chances, const TArray<int> RollAmounts, TArray<USceneComponent*>& Values_Out, USceneComponent*& ValueFirst_Out, bool& bHasValues_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeSceneComponentArrays2(const TArray<USceneComponent*> A, const TArray<USceneComponent*> B, TArray<USceneComponent*>& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeSceneComponentArrays3(const TArray<USceneComponent*> A, const TArray<USceneComponent*> B, const TArray<USceneComponent*> C, TArray<USceneComponent*>& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeSceneComponentArrays4(const TArray<USceneComponent*> A, const TArray<USceneComponent*> B, const TArray<USceneComponent*> C, const TArray<USceneComponent*> D, TArray<USceneComponent*>& Value_Out);

//Fluid Toolbox Variables Blueprint Function Library : User Widgets Tools
public:
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetRememberUserWidgetByName(const FName Name, UUserWidget*& Value_Out, bool& bValueValid_Out, bool& bFound_Out);
	UFUNCTION(BlueprintCallable, Category = "Fluid Toolbox Variables")
		static void SetRememberUserWidgetByName(const FName Name, UUserWidget* Value,UUserWidget*& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetRandomUserWidgetByWeight(TArray<UUserWidget*> Values, TArray<float> Weights, const int MinCount, const int MaxCount, TArray<UUserWidget*>& Values_Out, UUserWidget*& ValueFirst_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetUserWidgetByChance(UUserWidget* Value, const int Chance, const int RollAmount, TArray<UUserWidget*>& Values_Out, UUserWidget*& ValueFirst_Out, bool& bHasValues_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void GetUserWidgetByChanceMulti(TArray<UUserWidget*> Values, const TArray<int> Chances, const TArray<int> RollAmounts, TArray<UUserWidget*>& Values_Out, UUserWidget*& ValueFirst_Out, bool& bHasValues_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeUserWidgetArrays2(const TArray<UUserWidget*> A, const TArray<UUserWidget*> B, TArray<UUserWidget*>& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeUserWidgetArrays3(const TArray<UUserWidget*> A, const TArray<UUserWidget*> B, const TArray<UUserWidget*> C, TArray<UUserWidget*>& Value_Out);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fluid Toolbox Variables")
		static void MergeUserWidgetArrays4(const TArray<UUserWidget*> A, const TArray<UUserWidget*> B, const TArray<UUserWidget*> C, const TArray<UUserWidget*> D, TArray<UUserWidget*>& Value_Out);
};

// Fluid Toolbox Variables Subsystem
UCLASS()
class FLUIDTOOLBOXRUNTIME_API UFluidToolboxVariablesSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	//Begin | End
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	TMap<FName, bool> BoolValues;
	TMap<FName, uint8> ByteValues;
	TMap<FName, int> IntValues;
	TMap<FName, int64> Int64Values;
	TMap<FName, float> FloatValues;
	TMap<FName, FName> NameValues;
	TMap<FName, FString> StringValues;
	TMap<FName, FText> TextValues;
	TMap<FName, FVector> VectorValues;
	TMap<FName, FRotator> RotatorValues;
	TMap<FName, FTransform> TransformValues;
	TMap<FName, UObject*> UObjectValues;
	TMap<FName, AActor*> ActorValues;
	TMap<AActor*, FFluidToolboxVariablesActorTags> ActorTags;
	TMap<FName, TSubclassOf<AActor>> ActorUClassValues;
	TMap<FName, USceneComponent*> SceneComponentValues;
	TMap<FName, UUserWidget*> UserWidgetValues;
	TMap<FName, UClass*> UClassValues;
};
