// Fill out your copyright notice in the Description page of Project Settings.


#include "FluidToolboxVariables.h"

//Used Default C++ Arrays And Maps In Places To Make Things Easier
#include <map>
#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Variables Blueprint Function Library : Bool Tools

void UFluidToolboxVariablesBPLibrary::GetRememberBoolByName(const FName Name, bool& Value_Out, bool& bFound_Out)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	UFluidToolboxVariablesSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxVariablesSubsystem>();

	bFound_Out = false;
	Value_Out = false;

	if (Subsystem->BoolValues.Contains(Name))
	{
		bFound_Out = true;
		Value_Out = Subsystem->BoolValues[Name];
	}
}

void UFluidToolboxVariablesBPLibrary::SetRememberBoolByName(const FName Name,const bool Value, bool& Value_Out)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	UFluidToolboxVariablesSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxVariablesSubsystem>();

	Value_Out = Value;

	if (Subsystem->BoolValues.Contains(Name))
		Subsystem->BoolValues[Name] = Value;
	else
		Subsystem->BoolValues.Add(Name, Value);
}

void UFluidToolboxVariablesBPLibrary::ToggleRememberBoolByName(const FName Name, bool& Value_Out)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	UFluidToolboxVariablesSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxVariablesSubsystem>();
	
	if (Subsystem->BoolValues.Contains(Name))
		Value_Out = Subsystem->BoolValues[Name] = !Subsystem->BoolValues[Name];
	else
		Subsystem->BoolValues.Add(Name,0);
}

void UFluidToolboxVariablesBPLibrary::GetRandomBoolByWeight(TArray<bool> Values, TArray<float> Weights, int MinCount, int MaxCount, TArray<bool>& Values_Out, bool& ValueFirst_Out)
{
	//Force Empty
	Values_Out.Empty();

	//Convert To Standard C++ Value Pair | Get Max Weight
	float MaxWeight = 0;
	std::map<float, bool> Cumulative;
	for (int i = 0; i < Values.Num() && i < Weights.Num(); i++)
	{
		Cumulative.insert(std::make_pair(Weights[i], Values[i]));
		if (Weights[i] > MaxWeight)
			MaxWeight = Weights[i];
	}

	//Do Get values
	float Linear = 0;
	const int Count = FMath::RandRange(MinCount,MaxCount);
	for (int i = 0; i < Count; i++)
	{
		Linear = rand() * MaxWeight / RAND_MAX;
		Values_Out.Add(Cumulative.upper_bound(Linear)->second);
	}

	//Set First Value
	if (Values_Out.Num() != 0)
		ValueFirst_Out = Values_Out[0];
}

void UFluidToolboxVariablesBPLibrary::GetBoolByChance(const bool Value, const int Chance, const int RollAmount, TArray<bool>& Values_Out, bool& ValueFirst_Out, bool& bHasValues_Out)
{
	//Force Empty
	Values_Out.Empty();
	bHasValues_Out = false;

	//Do Chances
	int ChanceRole = 0;
	for (int i = 0; i < RollAmount; i++)
	{
		ChanceRole = FMath::RandRange(0,100);
		if (ChanceRole <= Chance)
			Values_Out.Add(Value);
	}
	
	//Set First Value
	if (Values_Out.Num() != 0)
	{
		ValueFirst_Out = Values_Out[0];
		bHasValues_Out = true;
	}
}

void UFluidToolboxVariablesBPLibrary::GetBoolByChanceMulti(const TArray<bool> Values, const TArray<int> Chances, const TArray<int> RollAmounts, TArray<bool>& Values_Out, bool& ValueFirst_Out, bool& bHasValues_Out)
{
	//Force Empty
	Values_Out.Empty();
	bHasValues_Out = false;

	//Return If Arrays Are Not Same Length
	if (Values.Num() != Chances.Num() || Values.Num() != RollAmounts.Num() || Chances.Num() != RollAmounts.Num())
		return;
	
	//Do Chances
	int ChanceRole = 0;
	for (int i = 0; i < Values.Num(); i++)
		for (int j = 0; j < RollAmounts[i]; j++)
		{
			ChanceRole = FMath::RandRange(0,100);
			if (ChanceRole <= Chances[i])
				Values_Out.Add(Values[i]);
		}
	
	//Set First Value
	if (Values_Out.Num() != 0)
	{
		ValueFirst_Out = Values_Out[0];
		bHasValues_Out = true;
	}
}

void UFluidToolboxVariablesBPLibrary::MergeBoolArrays2(const TArray<bool> A, const TArray<bool> B, TArray<bool>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
}

void UFluidToolboxVariablesBPLibrary::MergeBoolArrays3(const TArray<bool> A, const TArray<bool> B, const TArray<bool> C, TArray<bool>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
	Value_Out.Append(C);
}

void UFluidToolboxVariablesBPLibrary::MergeBoolArrays4(const TArray<bool> A, const TArray<bool> B, const TArray<bool> C, const TArray<bool> D, TArray<bool>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
	Value_Out.Append(C);
	Value_Out.Append(D);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Variables Blueprint Function Library : Byte Tools

void UFluidToolboxVariablesBPLibrary::GetRememberByteByName(const FName Name, uint8& Value_Out, bool& bFound_Out)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	UFluidToolboxVariablesSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxVariablesSubsystem>();

	bFound_Out = false;
	Value_Out =  0;

	if (Subsystem->ByteValues.Contains(Name))
	{
		bFound_Out = true;
		Value_Out = Subsystem->ByteValues[Name];
	}
}

void UFluidToolboxVariablesBPLibrary::SetRememberByteByName(const FName Name, const uint8 Value, uint8& Value_Out)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	UFluidToolboxVariablesSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxVariablesSubsystem>();

	Value_Out = Value;

	if (Subsystem->ByteValues.Contains(Name))
		Subsystem->ByteValues[Name] = Value;
	else
		Subsystem->ByteValues.Add(Name, Value);
}

void UFluidToolboxVariablesBPLibrary::IncrementRememberByteByName(const FName Name, const int Amount, uint8& Value_Out)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	UFluidToolboxVariablesSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxVariablesSubsystem>();
	
	Value_Out =  0;

	if (!Subsystem->ByteValues.Contains(Name))
		Subsystem->ByteValues.Add(Name,0);
	
	Subsystem->ByteValues[Name] += Amount;
	Value_Out = Subsystem->ByteValues[Name];
}

void UFluidToolboxVariablesBPLibrary::GetRandomByteByWeight(TArray<uint8> Values, TArray<float> Weights, const int MinCount, const int MaxCount, TArray<uint8>& Values_Out, uint8& ValueFirst_Out)
{
	//Force Empty
	Values_Out.Empty();

	//Convert To Standard C++ Value Pair | Get Max Weight
	float MaxWeight = 0;
	std::map<float, uint8> Cumulative;
	for (int i = 0; i < Values.Num() && i < Weights.Num(); i++)
	{
		Cumulative.insert(std::make_pair(Weights[i], Values[i]));
		if (Weights[i] > MaxWeight)
			MaxWeight = Weights[i];
	}

	//Do Get values
	float Linear = 0;
	const int Count = FMath::RandRange(MinCount,MaxCount);
	for (int i = 0; i < Count; i++)
	{
		Linear = rand() * MaxWeight / RAND_MAX;
		Values_Out.Add(Cumulative.upper_bound(Linear)->second);
	}

	//Set First Value
	if (Values_Out.Num() != 0)
		ValueFirst_Out = Values_Out[0];
}

void UFluidToolboxVariablesBPLibrary::GetByteByChance(const uint8 Value, const int Chance, const int RollAmount, TArray<uint8>& Values_Out, uint8& ValueFirst_Out, bool& bHasValues_Out)
{
	//Force Empty
	Values_Out.Empty();
	bHasValues_Out = false;

	//Do Chances
	int ChanceRole = 0;
	for (int i = 0; i < RollAmount; i++)
	{
		ChanceRole = FMath::RandRange(0,100);
		if (ChanceRole <= Chance)
			Values_Out.Add(Value);
	}
	
	//Set First Value
	if (Values_Out.Num() != 0)
	{
		ValueFirst_Out = Values_Out[0];
		bHasValues_Out = true;
	}
}

void UFluidToolboxVariablesBPLibrary::GetByteByChanceMulti(const TArray<uint8> Values, const TArray<int> Chances, const TArray<int> RollAmounts, TArray<uint8>& Values_Out, uint8& ValueFirst_Out, bool& bHasValues_Out)
{
	//Force Empty
	Values_Out.Empty();
	bHasValues_Out = false;

	//Return If Arrays Are Not Same Length
	if (Values.Num() != Chances.Num() || Values.Num() != RollAmounts.Num() || Chances.Num() != RollAmounts.Num())
		return;
	
	//Do Chances
	int ChanceRole = 0;
	for (int i = 0; i < Values.Num(); i++)
		for (int j = 0; j < RollAmounts[i]; j++)
		{
			ChanceRole = FMath::RandRange(0,100);
			if (ChanceRole <= Chances[i])
				Values_Out.Add(Values[i]);
		}
	
	//Set First Value
	if (Values_Out.Num() != 0)
	{
		ValueFirst_Out = Values_Out[0];
		bHasValues_Out = true;
	}
}

void UFluidToolboxVariablesBPLibrary::SortBytes(TArray<uint8> Values, const bool bDescending, TArray<uint8>& Values_Out, uint8& ValueFirst_Out)
{
	//Force Empty
	Values_Out.Empty();

	int32 Num = Values.Num();
	bool bDidSwap;

	for (int a = 0; a < (Num - 1); a++)
	{
		bDidSwap = false;

		//Hight To Low
		if (bDescending)
		{
			for (int k = 0; k < Num - a - 1; k++)
			{
				if (Values[k] < Values[k + 1])
				{
					uint8 z;
					z = Values[k];
					Values[k] = Values[k + 1];
					Values[k + 1] = z;
					bDidSwap = true;
				}
			}
		}

		//Low To High
		if (!bDescending)
		{
			for (int k = 0; k < Num - a - 1; k++)
			{
				if (Values[k] > Values[k + 1])
				{
					uint8 z;
					z = Values[k];
					Values[k] = Values[k + 1];
					Values[k + 1] = z;
					bDidSwap = true;
				}
			}
		}

		if (bDidSwap == false)
			break;
	}

	//Set Values | First Value
	Values_Out = Values;
	if (Values_Out.Num() != 0)
		ValueFirst_Out = Values_Out[0];
}

void UFluidToolboxVariablesBPLibrary::MergeByteArrays2(const TArray<uint8> A, const TArray<uint8> B, TArray<uint8>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
}

void UFluidToolboxVariablesBPLibrary::MergeByteArrays3(const TArray<uint8> A, const TArray<uint8> B, const TArray<uint8> C, TArray<uint8>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
	Value_Out.Append(C);
}

void UFluidToolboxVariablesBPLibrary::MergeByteArrays4(const TArray<uint8> A, const TArray<uint8> B, const TArray<uint8> C, const TArray<uint8> D, TArray<uint8>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
	Value_Out.Append(C);
	Value_Out.Append(D);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Variables Blueprint Function Library : Int Tools

void UFluidToolboxVariablesBPLibrary::GetRememberIntByName(const FName Name, int& Value_Out, bool& bFound_Out)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	UFluidToolboxVariablesSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxVariablesSubsystem>();

	bFound_Out = false;
	Value_Out = 0;

	if (Subsystem->IntValues.Contains(Name))
	{
		bFound_Out = true;
		Value_Out = Subsystem->IntValues[Name];
	}
}

void UFluidToolboxVariablesBPLibrary::SetRememberIntByName(const FName Name, int Value, int& Value_Out)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	UFluidToolboxVariablesSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxVariablesSubsystem>();

	Value_Out = Value;

	if (Subsystem->IntValues.Contains(Name))
		Subsystem->IntValues[Name] = Value;
	else
		Subsystem->IntValues.Add(Name, Value);
}

void UFluidToolboxVariablesBPLibrary::IncrementRememberIntByName(const FName Name, int Amount, int& Value_Out)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	UFluidToolboxVariablesSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxVariablesSubsystem>();
	
	Value_Out =  0;

	if (!Subsystem->IntValues.Contains(Name))
		Subsystem->IntValues.Add(Name,0);
	
	Subsystem->IntValues[Name] += Amount;
	Value_Out = Subsystem->IntValues[Name];
}

void UFluidToolboxVariablesBPLibrary::GetRandomIntByWeight(TArray<int> Values, TArray<float> Weights, const int MinCount, const int MaxCount, TArray<int>& Values_Out, int& ValueFirst_Out)
{
	//Force Empty
	Values_Out.Empty();

	//Convert To Standard C++ Value Pair | Get Max Weight
	float MaxWeight = 0;
	std::map<float, int> Cumulative;
	for (int i = 0; i < Values.Num() && i < Weights.Num(); i++)
	{
		Cumulative.insert(std::make_pair(Weights[i], Values[i]));
		if (Weights[i] > MaxWeight)
			MaxWeight = Weights[i];
	}

	//Do Get values
	float Linear = 0;
	const int Count = FMath::RandRange(MinCount,MaxCount);
	for (int i = 0; i < Count; i++)
	{
		Linear = rand() * MaxWeight / RAND_MAX;
		Values_Out.Add(Cumulative.upper_bound(Linear)->second);
	}

	//Set First Value
	if (Values_Out.Num() != 0)
		ValueFirst_Out = Values_Out[0];
}

void UFluidToolboxVariablesBPLibrary::GetIntByChance(const int Value, const int Chance, const int RollAmount, TArray<int>& Values_Out, int& ValueFirst_Out, bool& bHasValues_Out)
{
	//Force Empty
	Values_Out.Empty();
	bHasValues_Out = false;

	//Do Chances
	int ChanceRole = 0;
	for (int i = 0; i < RollAmount; i++)
	{
		ChanceRole = FMath::RandRange(0,100);
		if (ChanceRole <= Chance)
			Values_Out.Add(Value);
	}
	
	//Set First Value
	if (Values_Out.Num() != 0)
	{
		ValueFirst_Out = Values_Out[0];
		bHasValues_Out = true;
	}
}

void UFluidToolboxVariablesBPLibrary::GetIntByChanceMulti(const TArray<int> Values, const TArray<int> Chances, const TArray<int> RollAmounts, TArray<int>& Values_Out, int& ValueFirst_Out, bool& bHasValues_Out)
{
	//Force Empty
	Values_Out.Empty();
	bHasValues_Out = false;

	//Return If Arrays Are Not Same Length
	if (Values.Num() != Chances.Num() || Values.Num() != RollAmounts.Num() || Chances.Num() != RollAmounts.Num())
		return;
	
	//Do Chances
	int ChanceRole = 0;
	for (int i = 0; i < Values.Num(); i++)
		for (int j = 0; j < RollAmounts[i]; j++)
		{
			ChanceRole = FMath::RandRange(0,100);
			if (ChanceRole <= Chances[i])
				Values_Out.Add(Values[i]);
		}
	
	//Set First Value
	if (Values_Out.Num() != 0)
	{
		ValueFirst_Out = Values_Out[0];
		bHasValues_Out = true;
	}
}

void UFluidToolboxVariablesBPLibrary::SortIntS(TArray<int> Values, const bool bDescending, TArray<int>& Values_Out, int& ValueFirst_Out)
{
	//Force Empty
	Values_Out.Empty();

	int32 Num = Values.Num();
	bool bDidSwap;

	for (int a = 0; a < (Num - 1); a++)
	{
		bDidSwap = false;

		//Hight To Low
		if (bDescending)
		{
			for (int k = 0; k < Num - a - 1; k++)
			{
				if (Values[k] < Values[k + 1])
				{
					int z;
					z = Values[k];
					Values[k] = Values[k + 1];
					Values[k + 1] = z;
					bDidSwap = true;
				}
			}
		}

		//Low To High
		if (!bDescending)
		{
			for (int k = 0; k < Num - a - 1; k++)
			{
				if (Values[k] > Values[k + 1])
				{
					int z;
					z = Values[k];
					Values[k] = Values[k + 1];
					Values[k + 1] = z;
					bDidSwap = true;
				}
			}
		}

		if (bDidSwap == false)
			break;
	}

	//Set Values | First Value
	Values_Out = Values;
	if (Values_Out.Num() != 0)
		ValueFirst_Out = Values_Out[0];
}

void UFluidToolboxVariablesBPLibrary::MergeIntArrays2(const TArray<int> A, const TArray<int> B, TArray<int>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
}

void UFluidToolboxVariablesBPLibrary::MergeIntArrays3(const TArray<int> A, const TArray<int> B, const TArray<int> C, TArray<int>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
	Value_Out.Append(C);
}

void UFluidToolboxVariablesBPLibrary::MergeIntArrays4(const TArray<int> A, const TArray<int> B, const TArray<int> C, const TArray<int> D, TArray<int>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
	Value_Out.Append(C);
	Value_Out.Append(D);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Variables Blueprint Function Library : Int 64 Tools

void UFluidToolboxVariablesBPLibrary::GetRememberInt64ByName(const FName Name, int64& Value_Out, bool& bFound_Out)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	UFluidToolboxVariablesSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxVariablesSubsystem>();

	bFound_Out = false;
	Value_Out = 0;

	if (Subsystem->Int64Values.Contains(Name))
	{
		bFound_Out = true;
		Value_Out = Subsystem->Int64Values[Name];
	}
}

void UFluidToolboxVariablesBPLibrary::SetRememberInt64ByName(const FName Name, int64 Value, int64& Value_Out)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	UFluidToolboxVariablesSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxVariablesSubsystem>();

	Value_Out = Value;

	if (Subsystem->Int64Values.Contains(Name))
		Subsystem->Int64Values[Name] = Value;
	else
		Subsystem->Int64Values.Add(Name, Value);
}

void UFluidToolboxVariablesBPLibrary::IncrementRememberInt64ByName(const FName Name, int64 Amount, int64& Value_Out)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	UFluidToolboxVariablesSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxVariablesSubsystem>();
	
	Value_Out =  0;

	if (!Subsystem->Int64Values.Contains(Name))
		Subsystem->Int64Values.Add(Name,0);
	
	Subsystem->Int64Values[Name] += Amount;
	Value_Out = Subsystem->Int64Values[Name];
}

void UFluidToolboxVariablesBPLibrary::GetRandomInt64ByWeight(TArray<int64> Values, TArray<float> Weights, const int MinCount, const int MaxCount, TArray<int64>& Values_Out, int64& ValueFirst_Out)
{
	//Force Empty
	Values_Out.Empty();

	//Convert To Standard C++ Value Pair | Get Max Weight
	float MaxWeight = 0;
	std::map<float, int64> Cumulative;
	for (int i = 0; i < Values.Num() && i < Weights.Num(); i++)
	{
		Cumulative.insert(std::make_pair(Weights[i], Values[i]));
		if (Weights[i] > MaxWeight)
			MaxWeight = Weights[i];
	}

	//Do Get values
	float Linear = 0;
	const int Count = FMath::RandRange(MinCount,MaxCount);
	for (int i = 0; i < Count; i++)
	{
		Linear = rand() * MaxWeight / RAND_MAX;
		Values_Out.Add(Cumulative.upper_bound(Linear)->second);
	}

	//Set First Value
	if (Values_Out.Num() != 0)
		ValueFirst_Out = Values_Out[0];
}

void UFluidToolboxVariablesBPLibrary::GetInt64ByChance(const int64 Value, const int Chance, const int RollAmount, TArray<int64>& Values_Out, int64& ValueFirst_Out, bool& bHasValues_Out)
{
	//Force Empty
	Values_Out.Empty();
	bHasValues_Out = false;

	//Do Chances
	int ChanceRole = 0;
	for (int i = 0; i < RollAmount; i++)
	{
		ChanceRole = FMath::RandRange(0,100);
		if (ChanceRole <= Chance)
			Values_Out.Add(Value);
	}
	
	//Set First Value
	if (Values_Out.Num() != 0)
	{
		ValueFirst_Out = Values_Out[0];
		bHasValues_Out = true;
	}
}

void UFluidToolboxVariablesBPLibrary::GetInt64ByChanceMulti(const TArray<int64> Values, const TArray<int> Chances, const TArray<int> RollAmounts, TArray<int64>& Values_Out, int64& ValueFirst_Out, bool& bHasValues_Out)
{
	//Force Empty
	Values_Out.Empty();
	bHasValues_Out = false;

	//Return If Arrays Are Not Same Length
	if (Values.Num() != Chances.Num() || Values.Num() != RollAmounts.Num() || Chances.Num() != RollAmounts.Num())
		return;
	
	//Do Chances
	int ChanceRole = 0;
	for (int i = 0; i < Values.Num(); i++)
		for (int j = 0; j < RollAmounts[i]; j++)
		{
			ChanceRole = FMath::RandRange(0,100);
			if (ChanceRole <= Chances[i])
				Values_Out.Add(Values[i]);
		}
	
	//Set First Value
	if (Values_Out.Num() != 0)
	{
		ValueFirst_Out = Values_Out[0];
		bHasValues_Out = true;
	}
}

void UFluidToolboxVariablesBPLibrary::SortInt64S(TArray<int64> Values, const bool bDescending, TArray<int64>& Values_Out, int64& ValueFirst_Out)
{
	//Force Empty
	Values_Out.Empty();

	int32 Num = Values.Num();
	bool bDidSwap;

	for (int a = 0; a < (Num - 1); a++)
	{
		bDidSwap = false;

		//Hight To Low
		if (bDescending)
		{
			for (int k = 0; k < Num - a - 1; k++)
			{
				if (Values[k] < Values[k + 1])
				{
					int64 z;
					z = Values[k];
					Values[k] = Values[k + 1];
					Values[k + 1] = z;
					bDidSwap = true;
				}
			}
		}

		//Low To High
		if (!bDescending)
		{
			for (int k = 0; k < Num - a - 1; k++)
			{
				if (Values[k] > Values[k + 1])
				{
					int64 z;
					z = Values[k];
					Values[k] = Values[k + 1];
					Values[k + 1] = z;
					bDidSwap = true;
				}
			}
		}

		if (bDidSwap == false)
			break;
	}

	//Set Values | First Value
	Values_Out = Values;
	if (Values_Out.Num() != 0)
		ValueFirst_Out = Values_Out[0];
}

void UFluidToolboxVariablesBPLibrary::MergeInt64Arrays2(const TArray<int64> A, const TArray<int64> B, TArray<int64>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
}

void UFluidToolboxVariablesBPLibrary::MergeInt64Arrays3(const TArray<int64> A, const TArray<int64> B, const TArray<int64> C, TArray<int64>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
	Value_Out.Append(C);
}

void UFluidToolboxVariablesBPLibrary::MergeInt64Arrays4(const TArray<int64> A, const TArray<int64> B, const TArray<int64> C, const TArray<int64> D, TArray<int64>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
	Value_Out.Append(C);
	Value_Out.Append(D);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Variables Blueprint Function Library : Float Tools

void UFluidToolboxVariablesBPLibrary::GetRememberFloatByName(const FName Name, float& Value_Out, bool& bFound_Out)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	UFluidToolboxVariablesSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxVariablesSubsystem>();

	bFound_Out = false;
	Value_Out = 0;

	if (Subsystem->FloatValues.Contains(Name))
	{
		bFound_Out = true;
		Value_Out = Subsystem->FloatValues[Name];
	}
}

void UFluidToolboxVariablesBPLibrary::SetRememberFloatByName(const FName Name, const float Value, float& Value_Out)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	UFluidToolboxVariablesSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxVariablesSubsystem>();

	Value_Out = Value;

	if (Subsystem->FloatValues.Contains(Name))
		Subsystem->FloatValues[Name] = Value;
	else
		Subsystem->FloatValues.Add(Name, Value);
}

void UFluidToolboxVariablesBPLibrary::IncrementRememberFloatByName(const FName Name, const float Amount, float& Value_Out)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	UFluidToolboxVariablesSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxVariablesSubsystem>();
	
	Value_Out =  0;

	if (!Subsystem->FloatValues.Contains(Name))
		Subsystem->FloatValues.Add(Name,0);
	
	Subsystem->FloatValues[Name] += Amount;
	Value_Out = Subsystem->FloatValues[Name];
}

void UFluidToolboxVariablesBPLibrary::GetRandomFloatByWeight(TArray<float> Values, TArray<float> Weights, const int MinCount, const int MaxCount, TArray<float>& Values_Out, float& ValueFirst_Out)
{
	//Force Empty
	Values_Out.Empty();

	//Convert To Standard C++ Value Pair | Get Max Weight
	float MaxWeight = 0;
	std::map<float, float> Cumulative;
	for (int i = 0; i < Values.Num() && i < Weights.Num(); i++)
	{
		Cumulative.insert(std::make_pair(Weights[i], Values[i]));
		if (Weights[i] > MaxWeight)
			MaxWeight = Weights[i];
	}

	//Do Get values
	float Linear = 0;
	const int Count = FMath::RandRange(MinCount,MaxCount);
	for (int i = 0; i < Count; i++)
	{
		Linear = rand() * MaxWeight / RAND_MAX;
		Values_Out.Add(Cumulative.upper_bound(Linear)->second);
	}

	//Set First Value
	if (Values_Out.Num() != 0)
		ValueFirst_Out = Values_Out[0];
}

void UFluidToolboxVariablesBPLibrary::GetFloatByChance(const float Value, const int Chance, const int RollAmount, TArray<float>& Values_Out, float& ValueFirst_Out, bool& bHasValues_Out)
{
	//Force Empty
	Values_Out.Empty();
	bHasValues_Out = false;

	//Do Chances
	int ChanceRole = 0;
	for (int i = 0; i < RollAmount; i++)
	{
		ChanceRole = FMath::RandRange(0,100);
		if (ChanceRole <= Chance)
			Values_Out.Add(Value);
	}
	
	//Set First Value
	if (Values_Out.Num() != 0)
	{
		ValueFirst_Out = Values_Out[0];
		bHasValues_Out = true;
	}
}

void UFluidToolboxVariablesBPLibrary::GetFloatByChanceMulti(const TArray<float> Values, const TArray<int> Chances, const TArray<int> RollAmounts, TArray<float>& Values_Out, float& ValueFirst_Out, bool& bHasValues_Out)
{
	//Force Empty
	Values_Out.Empty();
	bHasValues_Out = false;

	//Return If Arrays Are Not Same Length
	if (Values.Num() != Chances.Num() || Values.Num() != RollAmounts.Num() || Chances.Num() != RollAmounts.Num())
		return;
	
	//Do Chances
	int ChanceRole = 0;
	for (int i = 0; i < Values.Num(); i++)
		for (int j = 0; j < RollAmounts[i]; j++)
		{
			ChanceRole = FMath::RandRange(0,100);
			if (ChanceRole <= Chances[i])
				Values_Out.Add(Values[i]);
		}
	
	//Set First Value
	if (Values_Out.Num() != 0)
	{
		ValueFirst_Out = Values_Out[0];
		bHasValues_Out = true;
	}
}

void UFluidToolboxVariablesBPLibrary::SortFloats(TArray<float> Values, const bool bDescending, TArray<float>& Values_Out, float& ValueFirst_Out)
{
	//Force Empty
	Values_Out.Empty();

	int32 Num = Values.Num();
	bool bDidSwap;

	for (int a = 0; a < (Num - 1); a++)
	{
		bDidSwap = false;

		//Hight To Low
		if (bDescending)
		{
			for (int k = 0; k < Num - a - 1; k++)
			{
				if (Values[k] < Values[k + 1])
				{
					float z;
					z = Values[k];
					Values[k] = Values[k + 1];
					Values[k + 1] = z;
					bDidSwap = true;
				}
			}
		}

		//Low To High
		if (!bDescending)
		{
			for (int k = 0; k < Num - a - 1; k++)
			{
				if (Values[k] > Values[k + 1])
				{
					float z;
					z = Values[k];
					Values[k] = Values[k + 1];
					Values[k + 1] = z;
					bDidSwap = true;
				}
			}
		}

		if (bDidSwap == false)
			break;
	}

	//Set Values | First Value
	Values_Out = Values;
	if (Values_Out.Num() != 0)
		ValueFirst_Out = Values_Out[0];
}

void UFluidToolboxVariablesBPLibrary::MergeFloatArrays2(const TArray<float> A, const TArray<float> B, TArray<float>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
}

void UFluidToolboxVariablesBPLibrary::MergeFloatArrays3(const TArray<float> A, const TArray<float> B, const TArray<float> C, TArray<float>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
	Value_Out.Append(C);
}

void UFluidToolboxVariablesBPLibrary::MergeFloatArrays4(const TArray<float> A, const TArray<float> B, const TArray<float> C, const TArray<float> D, TArray<float>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
	Value_Out.Append(C);
	Value_Out.Append(D);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Variables Blueprint Function Library : Double Tools

void UFluidToolboxVariablesBPLibrary::GetRememberDoubleByName(const FName Name, double& Value_Out, bool& bFound_Out)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	UFluidToolboxVariablesSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxVariablesSubsystem>();

	bFound_Out = false;
	Value_Out = 0;

	if (Subsystem->FloatValues.Contains(Name))
	{
		bFound_Out = true;
		Value_Out = Subsystem->FloatValues[Name];
	}
}

void UFluidToolboxVariablesBPLibrary::SetRememberDoubleByName(const FName Name, const double Value, double& Value_Out)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	UFluidToolboxVariablesSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxVariablesSubsystem>();

	Value_Out = Value;

	if (Subsystem->FloatValues.Contains(Name))
		Subsystem->FloatValues[Name] = Value;
	else
		Subsystem->FloatValues.Add(Name, Value);
}

void UFluidToolboxVariablesBPLibrary::GetRandomDoubleByWeight(TArray<double> Values, TArray<float> Weights, const int MinCount, const int MaxCount, TArray<double>& Values_Out, double& ValueFirst_Out)
{
	//Force Empty
	Values_Out.Empty();

	//Convert To Standard C++ Value Pair | Get Max Weight
	float MaxWeight = 0;
	std::map<float, double> Cumulative;
	for (int i = 0; i < Values.Num() && i < Weights.Num(); i++)
	{
		Cumulative.insert(std::make_pair(Weights[i], Values[i]));
		if (Weights[i] > MaxWeight)
			MaxWeight = Weights[i];
	}

	//Do Get values
	float Linear = 0;
	const int Count = FMath::RandRange(MinCount,MaxCount);
	for (int i = 0; i < Count; i++)
	{
		Linear = rand() * MaxWeight / RAND_MAX;
		Values_Out.Add(Cumulative.upper_bound(Linear)->second);
	}

	//Set First Value
	if (Values_Out.Num() != 0)
		ValueFirst_Out = Values_Out[0];
}

void UFluidToolboxVariablesBPLibrary::GetDoubleByChance(const double Value, const int Chance, const int RollAmount, TArray<double>& Values_Out, double& ValueFirst_Out, bool& bHasValues_Out)
{
	//Force Empty
	Values_Out.Empty();
	bHasValues_Out = false;

	//Do Chances
	int ChanceRole = 0;
	for (int i = 0; i < RollAmount; i++)
	{
		ChanceRole = FMath::RandRange(0,100);
		if (ChanceRole <= Chance)
			Values_Out.Add(Value);
	}
	
	//Set First Value
	if (Values_Out.Num() != 0)
	{
		ValueFirst_Out = Values_Out[0];
		bHasValues_Out = true;
	}
}

void UFluidToolboxVariablesBPLibrary::GetDoubleByChanceMulti(const TArray<double> Values, const TArray<int> Chances, const TArray<int> RollAmounts, TArray<double>& Values_Out, double& ValueFirst_Out, bool& bHasValues_Out)
{
	//Force Empty
	Values_Out.Empty();
	bHasValues_Out = false;

	//Return If Arrays Are Not Same Length
	if (Values.Num() != Chances.Num() || Values.Num() != RollAmounts.Num() || Chances.Num() != RollAmounts.Num())
		return;
	
	//Do Chances
	int ChanceRole = 0;
	for (int i = 0; i < Values.Num(); i++)
		for (int j = 0; j < RollAmounts[i]; j++)
		{
			ChanceRole = FMath::RandRange(0,100);
			if (ChanceRole <= Chances[i])
				Values_Out.Add(Values[i]);
		}
	
	//Set First Value
	if (Values_Out.Num() != 0)
	{
		ValueFirst_Out = Values_Out[0];
		bHasValues_Out = true;
	}
}

void UFluidToolboxVariablesBPLibrary::IncrementRememberDoubleByName(const FName Name, const double Amount, double& Value_Out)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	UFluidToolboxVariablesSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxVariablesSubsystem>();
	
	Value_Out =  0;

	if (!Subsystem->FloatValues.Contains(Name))
		Subsystem->FloatValues.Add(Name,0);
	
	Subsystem->FloatValues[Name] += Amount;
	Value_Out = Subsystem->FloatValues[Name];
}

void UFluidToolboxVariablesBPLibrary::SortDoubles(TArray<double> Values, const bool bDescending, TArray<double>& Values_Out, double& ValueFirst_Out)
{
	//Force Empty
	Values_Out.Empty();

	int32 Num = Values.Num();
	bool bDidSwap;

	for (int a = 0; a < (Num - 1); a++)
	{
		bDidSwap = false;

		//Hight To Low
		if (bDescending)
		{
			for (int k = 0; k < Num - a - 1; k++)
			{
				if (Values[k] < Values[k + 1])
				{
					float z;
					z = Values[k];
					Values[k] = Values[k + 1];
					Values[k + 1] = z;
					bDidSwap = true;
				}
			}
		}

		//Low To High
		if (!bDescending)
		{
			for (int k = 0; k < Num - a - 1; k++)
			{
				if (Values[k] > Values[k + 1])
				{
					float z;
					z = Values[k];
					Values[k] = Values[k + 1];
					Values[k + 1] = z;
					bDidSwap = true;
				}
			}
		}

		if (bDidSwap == false)
			break;
	}

	//Set Values | First Value
	Values_Out = Values;
	if (Values_Out.Num() != 0)
		ValueFirst_Out = Values_Out[0];
}

void UFluidToolboxVariablesBPLibrary::MergeDoubleArrays2(const TArray<double> A, const TArray<double> B, TArray<double>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
}

void UFluidToolboxVariablesBPLibrary::MergeDoubleArrays3(const TArray<double> A, const TArray<double> B, const TArray<double> C, TArray<double>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
	Value_Out.Append(C);
}

void UFluidToolboxVariablesBPLibrary::MergeDoubleArrays4(const TArray<double> A, const TArray<double> B, const TArray<double> C, const TArray<double> D, TArray<double>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
	Value_Out.Append(C);
	Value_Out.Append(D);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Variables Blueprint Function Library : Name Tools

void UFluidToolboxVariablesBPLibrary::GetRememberNameByName(const FName Name, FName& Value_Out, bool& bFound_Out)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	UFluidToolboxVariablesSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxVariablesSubsystem>();

	bFound_Out = false;
	Value_Out = FName();

	if (Subsystem->NameValues.Contains(Name))
	{
		bFound_Out = true;
		Value_Out = Subsystem->NameValues[Name];
	}
}

void UFluidToolboxVariablesBPLibrary::SetRememberNameByName(const FName Name, const FName Value, FName& Value_Out)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	UFluidToolboxVariablesSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxVariablesSubsystem>();

	Value_Out = Value;

	if (Subsystem->NameValues.Contains(Name))
		Subsystem->NameValues[Name] = Value;
	else
		Subsystem->NameValues.Add(Name, Value);
}

void UFluidToolboxVariablesBPLibrary::GetRandomNameByWeight(TArray<FName> Values, TArray<float> Weights, const int MinCount, const int MaxCount, TArray<FName>& Values_Out, FName& ValueFirst_Out)
{
	//Force Empty
	Values_Out.Empty();

	//Convert To Standard C++ Value Pair | Get Max Weight
	float MaxWeight = 0;
	std::map<float, FName> Cumulative;
	for (int i = 0; i < Values.Num() && i < Weights.Num(); i++)
	{
		Cumulative.insert(std::make_pair(Weights[i], Values[i]));
		if (Weights[i] > MaxWeight)
			MaxWeight = Weights[i];
	}

	//Do Get values
	float Linear = 0;
	const int Count = FMath::RandRange(MinCount,MaxCount);
	for (int i = 0; i < Count; i++)
	{
		Linear = rand() * MaxWeight / RAND_MAX;
		Values_Out.Add(Cumulative.upper_bound(Linear)->second);
	}

	//Set First Value
	if (Values_Out.Num() != 0)
		ValueFirst_Out = Values_Out[0];
}

void UFluidToolboxVariablesBPLibrary::GetNameByChance(const FName Value, const int Chance, const int RollAmount, TArray<FName>& Values_Out, FName& ValueFirst_Out, bool& bHasValues_Out)
{
	//Force Empty
	Values_Out.Empty();
	bHasValues_Out = false;

	//Do Chances
	int ChanceRole = 0;
	for (int i = 0; i < RollAmount; i++)
	{
		ChanceRole = FMath::RandRange(0,100);
		if (ChanceRole <= Chance)
			Values_Out.Add(Value);
	}
	
	//Set First Value
	if (Values_Out.Num() != 0)
	{
		ValueFirst_Out = Values_Out[0];
		bHasValues_Out = true;
	}
}

void UFluidToolboxVariablesBPLibrary::GetNameByChanceMulti(const TArray<FName> Values, const TArray<int> Chances, const TArray<int> RollAmounts, TArray<FName>& Values_Out, FName& ValueFirst_Out, bool& bHasValues_Out)
{
	//Force Empty
	Values_Out.Empty();
	bHasValues_Out = false;

	//Return If Arrays Are Not Same Length
	if (Values.Num() != Chances.Num() || Values.Num() != RollAmounts.Num() || Chances.Num() != RollAmounts.Num())
		return;
	
	//Do Chances
	int ChanceRole = 0;
	for (int i = 0; i < Values.Num(); i++)
		for (int j = 0; j < RollAmounts[i]; j++)
		{
			ChanceRole = FMath::RandRange(0,100);
			if (ChanceRole <= Chances[i])
				Values_Out.Add(Values[i]);
		}
	
	//Set First Value
	if (Values_Out.Num() != 0)
	{
		ValueFirst_Out = Values_Out[0];
		bHasValues_Out = true;
	}
}

void UFluidToolboxVariablesBPLibrary::MergeNameArrays2(const TArray<FName> A, const TArray<FName> B, TArray<FName>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
}

void UFluidToolboxVariablesBPLibrary::MergeNameArrays3(const TArray<FName> A, const TArray<FName> B, const TArray<FName> C, TArray<FName>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
	Value_Out.Append(C);
}

void UFluidToolboxVariablesBPLibrary::MergeNameArrays4(const TArray<FName> A, const TArray<FName> B, const TArray<FName> C, const TArray<FName> D, TArray<FName>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
	Value_Out.Append(C);
	Value_Out.Append(D);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Variables Blueprint Function Library : String Tools

void UFluidToolboxVariablesBPLibrary::GetRememberStringByName(const FName Name, FString& Value_Out, bool& bFound_Out)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	UFluidToolboxVariablesSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxVariablesSubsystem>();

	bFound_Out = false;
	Value_Out = FString();

	if (Subsystem->StringValues.Contains(Name))
	{
		bFound_Out = true;
		Value_Out = Subsystem->StringValues[Name];
	}
}

void UFluidToolboxVariablesBPLibrary::SetRememberStringByName(const FName Name, const FString Value, FString& Value_Out)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	UFluidToolboxVariablesSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxVariablesSubsystem>();

	Value_Out = Value;

	if (Subsystem->StringValues.Contains(Name))
		Subsystem->StringValues[Name] = Value;
	else
		Subsystem->StringValues.Add(Name, Value);
}

void UFluidToolboxVariablesBPLibrary::GetRandomStringByWeight(TArray<FString> Values, TArray<float> Weights, int MinCount, int MaxCount, TArray<FString>& Values_Out, FString& ValueFirst_Out)
{
	//Force Empty
	Values_Out.Empty();

	//Convert To Standard C++ Value Pair | Get Max Weight
	float MaxWeight = 0;
	std::map<float, FString> Cumulative;
	for (int i = 0; i < Values.Num() && i < Weights.Num(); i++)
	{
		Cumulative.insert(std::make_pair(Weights[i], Values[i]));
		if (Weights[i] > MaxWeight)
			MaxWeight = Weights[i];
	}

	//Do Get values
	float Linear = 0;
	const int Count = FMath::RandRange(MinCount,MaxCount);
	for (int i = 0; i < Count; i++)
	{
		Linear = rand() * MaxWeight / RAND_MAX;
		Values_Out.Add(Cumulative.upper_bound(Linear)->second);
	}

	//Set First Value
	if (Values_Out.Num() != 0)
		ValueFirst_Out = Values_Out[0];
}

void UFluidToolboxVariablesBPLibrary::GetStringByChance(const FString Value, const int Chance, const int RollAmount, TArray<FString>& Values_Out, FString& ValueFirst_Out, bool& bHasValues_Out)
{
	//Force Empty
	Values_Out.Empty();
	bHasValues_Out = false;

	//Do Chances
	int ChanceRole = 0;
	for (int i = 0; i < RollAmount; i++)
	{
		ChanceRole = FMath::RandRange(0,100);
		if (ChanceRole <= Chance)
			Values_Out.Add(Value);
	}
	
	//Set First Value
	if (Values_Out.Num() != 0)
	{
		ValueFirst_Out = Values_Out[0];
		bHasValues_Out = true;
	}
}

void UFluidToolboxVariablesBPLibrary::GetStringByChanceMulti(const TArray<FString> Values, const TArray<int> Chances, const TArray<int> RollAmounts, TArray<FString>& Values_Out, FString& ValueFirst_Out, bool& bHasValues_Out)
{
	//Force Empty
	Values_Out.Empty();
	bHasValues_Out = false;

	//Return If Arrays Are Not Same Length
	if (Values.Num() != Chances.Num() || Values.Num() != RollAmounts.Num() || Chances.Num() != RollAmounts.Num())
		return;
	
	//Do Chances
	int ChanceRole = 0;
	for (int i = 0; i < Values.Num(); i++)
		for (int j = 0; j < RollAmounts[i]; j++)
		{
			ChanceRole = FMath::RandRange(0,100);
			if (ChanceRole <= Chances[i])
				Values_Out.Add(Values[i]);
		}
	
	//Set First Value
	if (Values_Out.Num() != 0)
	{
		ValueFirst_Out = Values_Out[0];
		bHasValues_Out = true;
	}
}

void UFluidToolboxVariablesBPLibrary::MergeStringArrays2(const TArray<FString> A, const TArray<FString> B, TArray<FString>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
}

void UFluidToolboxVariablesBPLibrary::MergeStringArrays3(const TArray<FString> A, const TArray<FString> B, const TArray<FString> C, TArray<FString>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
	Value_Out.Append(C);
}

void UFluidToolboxVariablesBPLibrary::MergeStringArrays4(const TArray<FString> A, const TArray<FString> B, const TArray<FString> C, const TArray<FString> D, TArray<FString>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
	Value_Out.Append(C);
	Value_Out.Append(D);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Variables Blueprint Function Library : Text Tools

void UFluidToolboxVariablesBPLibrary::GetRememberTextByName(const FName Name, FText& Value_Out, bool& bFound_Out)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	UFluidToolboxVariablesSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxVariablesSubsystem>();

	bFound_Out = false;
	Value_Out = FText();

	if (Subsystem->TextValues.Contains(Name))
	{
		bFound_Out = true;
		Value_Out = Subsystem->TextValues[Name];
	}
}

void UFluidToolboxVariablesBPLibrary::SetRememberTextByName(const FName Name, FText Value, FText& Value_Out)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	UFluidToolboxVariablesSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxVariablesSubsystem>();

	Value_Out = Value;

	if (Subsystem->TextValues.Contains(Name))
		Subsystem->TextValues[Name] = Value;
	else
		Subsystem->TextValues.Add(Name, Value);
}

void UFluidToolboxVariablesBPLibrary::GetRandomTextByWeight(TArray<FText> Values, TArray<float> Weights, int MinCount, int MaxCount, TArray<FText>& Values_Out, FText& ValueFirst_Out)
{
	//Force Empty
	Values_Out.Empty();

	//Convert To Standard C++ Value Pair | Get Max Weight
	float MaxWeight = 0;
	std::map<float, FText> Cumulative;
	for (int i = 0; i < Values.Num() && i < Weights.Num(); i++)
	{
		Cumulative.insert(std::make_pair(Weights[i], Values[i]));
		if (Weights[i] > MaxWeight)
			MaxWeight = Weights[i];
	}

	//Do Get values
	float Linear = 0;
	const int Count = FMath::RandRange(MinCount,MaxCount);
	for (int i = 0; i < Count; i++)
	{
		Linear = rand() * MaxWeight / RAND_MAX;
		Values_Out.Add(Cumulative.upper_bound(Linear)->second);
	}

	//Set First Value
	if (Values_Out.Num() != 0)
		ValueFirst_Out = Values_Out[0];
}

void UFluidToolboxVariablesBPLibrary::GetTextByChance(const FText Value, const int Chance, const int RollAmount, TArray<FText>& Values_Out, FText& ValueFirst_Out, bool& bHasValues_Out)
{
	//Force Empty
	Values_Out.Empty();
	bHasValues_Out = false;

	//Do Chances
	int ChanceRole = 0;
	for (int i = 0; i < RollAmount; i++)
	{
		ChanceRole = FMath::RandRange(0,100);
		if (ChanceRole <= Chance)
			Values_Out.Add(Value);
	}
	
	//Set First Value
	if (Values_Out.Num() != 0)
	{
		ValueFirst_Out = Values_Out[0];
		bHasValues_Out = true;
	}
}

void UFluidToolboxVariablesBPLibrary::GetTextByChanceMulti(const TArray<FText> Values, const TArray<int> Chances, const TArray<int> RollAmounts, TArray<FText>& Values_Out, FText& ValueFirst_Out, bool& bHasValues_Out)
{
	//Force Empty
	Values_Out.Empty();
	bHasValues_Out = false;

	//Return If Arrays Are Not Same Length
	if (Values.Num() != Chances.Num() || Values.Num() != RollAmounts.Num() || Chances.Num() != RollAmounts.Num())
		return;
	
	//Do Chances
	int ChanceRole = 0;
	for (int i = 0; i < Values.Num(); i++)
		for (int j = 0; j < RollAmounts[i]; j++)
		{
			ChanceRole = FMath::RandRange(0,100);
			if (ChanceRole <= Chances[i])
				Values_Out.Add(Values[i]);
		}
	
	//Set First Value
	if (Values_Out.Num() != 0)
	{
		ValueFirst_Out = Values_Out[0];
		bHasValues_Out = true;
	}
}

void UFluidToolboxVariablesBPLibrary::MergeTextArrays2(const TArray<FText> A, const TArray<FText> B, TArray<FText>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
}

void UFluidToolboxVariablesBPLibrary::MergeTextArrays3(const TArray<FText> A, const TArray<FText> B, const TArray<FText> C, TArray<FText>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
	Value_Out.Append(C);
}

void UFluidToolboxVariablesBPLibrary::MergeTextArrays4(const TArray<FText> A, const TArray<FText> B, const TArray<FText> C, const TArray<FText> D, TArray<FText>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
	Value_Out.Append(C);
	Value_Out.Append(D);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Variables Blueprint Function Library : Text Tools

void UFluidToolboxVariablesBPLibrary::GetRememberVectorByName(const FName Name, FVector& Value_Out, bool& bFound_Out)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	UFluidToolboxVariablesSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxVariablesSubsystem>();

	bFound_Out = false;
	Value_Out = FVector();

	if (Subsystem->VectorValues.Contains(Name))
	{
		bFound_Out = true;
		Value_Out = Subsystem->VectorValues[Name];
	}
}

void UFluidToolboxVariablesBPLibrary::SetRememberVectorByName(const FName Name, FVector Value, FVector& Value_Out)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	if (GameInstance == nullptr) return;
	UFluidToolboxVariablesSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxVariablesSubsystem>();
	if (Subsystem == nullptr) return;

	Value_Out = Value;

	if (Subsystem->VectorValues.Contains(Name))
		Subsystem->VectorValues[Name] = Value;
	else
		Subsystem->VectorValues.Add(Name, Value);
}

void UFluidToolboxVariablesBPLibrary::GetRandomVectorByWeight(TArray<FVector> Values, TArray<float> Weights, int MinCount, int MaxCount, TArray<FVector>& Values_Out, FVector& ValueFirst_Out)
{
	//Force Empty
	Values_Out.Empty();

	//Convert To Standard C++ Value Pair | Get Max Weight
	float MaxWeight = 0;
	std::map<float, FVector> Cumulative;
	for (int i = 0; i < Values.Num() && i < Weights.Num(); i++)
	{
		Cumulative.insert(std::make_pair(Weights[i], Values[i]));
		if (Weights[i] > MaxWeight)
			MaxWeight = Weights[i];
	}

	//Do Get values
	float Linear = 0;
	const int Count = FMath::RandRange(MinCount,MaxCount);
	for (int i = 0; i < Count; i++)
	{
		Linear = rand() * MaxWeight / RAND_MAX;
		Values_Out.Add(Cumulative.upper_bound(Linear)->second);
	}

	//Set First Value
	if (Values_Out.Num() != 0)
		ValueFirst_Out = Values_Out[0];
}

void UFluidToolboxVariablesBPLibrary::GetVectorByChance(const FVector Value, const int Chance, const int RollAmount, TArray<FVector>& Values_Out, FVector& ValueFirst_Out, bool& bHasValues_Out)
{
	//Force Empty
	Values_Out.Empty();
	bHasValues_Out = false;

	//Do Chances
	int ChanceRole = 0;
	for (int i = 0; i < RollAmount; i++)
	{
		ChanceRole = FMath::RandRange(0,100);
		if (ChanceRole <= Chance)
			Values_Out.Add(Value);
	}
	
	//Set First Value
	if (Values_Out.Num() != 0)
	{
		ValueFirst_Out = Values_Out[0];
		bHasValues_Out = true;
	}
}

void UFluidToolboxVariablesBPLibrary::GetVectorByChanceMulti(const TArray<FVector> Values, const TArray<int> Chances, const TArray<int> RollAmounts, TArray<FVector>& Values_Out, FVector& ValueFirst_Out, bool& bHasValues_Out)
{
	//Force Empty
	Values_Out.Empty();
	bHasValues_Out = false;

	//Return If Arrays Are Not Same Length
	if (Values.Num() != Chances.Num() || Values.Num() != RollAmounts.Num() || Chances.Num() != RollAmounts.Num())
		return;
	
	//Do Chances
	int ChanceRole = 0;
	for (int i = 0; i < Values.Num(); i++)
		for (int j = 0; j < RollAmounts[i]; j++)
		{
			ChanceRole = FMath::RandRange(0,100);
			if (ChanceRole <= Chances[i])
				Values_Out.Add(Values[i]);
		}
	
	//Set First Value
	if (Values_Out.Num() != 0)
	{
		ValueFirst_Out = Values_Out[0];
		bHasValues_Out = true;
	}
}

void UFluidToolboxVariablesBPLibrary::MergeVectorArrays2(const TArray<FVector> A, const TArray<FVector> B, TArray<FVector>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
}

void UFluidToolboxVariablesBPLibrary::MergeVectorArrays3(const TArray<FVector> A, const TArray<FVector> B, const TArray<FVector> C, TArray<FVector>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
	Value_Out.Append(C);
}

void UFluidToolboxVariablesBPLibrary::MergeVectorArrays4(const TArray<FVector> A, const TArray<FVector> B, const TArray<FVector> C, const TArray<FVector> D, TArray<FVector>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
	Value_Out.Append(C);
	Value_Out.Append(D);
}

void UFluidToolboxVariablesBPLibrary::GetVectorsAverageLocation(TArray<FVector> Values, FVector& Value_Out)
{
	Value_Out = FVector(0, 0, 0);
	int C = 0;

	for (int i = 0; i < Values.Num(); i++)
	{
		C++;
		Value_Out += Values[i];
	}

	Value_Out /= C;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Variables Blueprint Function Library : Rotator Tools

void UFluidToolboxVariablesBPLibrary::GetRememberRotatorByName(const FName Name, FRotator& Value_Out, bool& bFound_Out)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	if (GameInstance == nullptr) return;
	const UFluidToolboxVariablesSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxVariablesSubsystem>();
	if (Subsystem == nullptr) return;

	bFound_Out = false;
	Value_Out = FRotator();

	if (Subsystem->RotatorValues.Contains(Name))
	{
		bFound_Out = true;
		Value_Out = Subsystem->RotatorValues[Name];
	}
}

void UFluidToolboxVariablesBPLibrary::SetRememberRotatorByName(const FName Name, FRotator Value, FRotator& Value_Out)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	if (GameInstance == nullptr) return;
	UFluidToolboxVariablesSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxVariablesSubsystem>();
	if (Subsystem == nullptr) return;

	Value_Out = Value;

	if (Subsystem->RotatorValues.Contains(Name))
		Subsystem->RotatorValues[Name] = Value;
	else
		Subsystem->RotatorValues.Add(Name, Value);
}

void UFluidToolboxVariablesBPLibrary::GetRandomRotatorByWeight(TArray<FRotator> Values, TArray<float> Weights, int MinCount, int MaxCount, TArray<FRotator>& Values_Out, FRotator& ValueFirst_Out)
{
	//Force Empty
	Values_Out.Empty();

	//Convert To Standard C++ Value Pair | Get Max Weight
	float MaxWeight = 0;
	std::map<float, FRotator> Cumulative;
	for (int i = 0; i < Values.Num() && i < Weights.Num(); i++)
	{
		Cumulative.insert(std::make_pair(Weights[i], Values[i]));
		if (Weights[i] > MaxWeight)
			MaxWeight = Weights[i];
	}

	//Do Get values
	float Linear = 0;
	const int Count = FMath::RandRange(MinCount,MaxCount);
	for (int i = 0; i < Count; i++)
	{
		Linear = rand() * MaxWeight / RAND_MAX;
		Values_Out.Add(Cumulative.upper_bound(Linear)->second);
	}

	//Set First Value
	if (Values_Out.Num() != 0)
		ValueFirst_Out = Values_Out[0];
}

void UFluidToolboxVariablesBPLibrary::GetRotatorByChance(const FRotator Value, const int Chance, const int RollAmount, TArray<FRotator>& Values_Out, FRotator& ValueFirst_Out, bool& bHasValues_Out)
{
	//Force Empty
	Values_Out.Empty();
	bHasValues_Out = false;

	//Do Chances
	int ChanceRole = 0;
	for (int i = 0; i < RollAmount; i++)
	{
		ChanceRole = FMath::RandRange(0,100);
		if (ChanceRole <= Chance)
			Values_Out.Add(Value);
	}
	
	//Set First Value
	if (Values_Out.Num() != 0)
	{
		ValueFirst_Out = Values_Out[0];
		bHasValues_Out = true;
	}
}

void UFluidToolboxVariablesBPLibrary::GetRotatorByChanceMulti(const TArray<FRotator> Values, const TArray<int> Chances, const TArray<int> RollAmounts, TArray<FRotator>& Values_Out, FRotator& ValueFirst_Out, bool& bHasValues_Out)
{
	//Force Empty
	Values_Out.Empty();
	bHasValues_Out = false;

	//Return If Arrays Are Not Same Length
	if (Values.Num() != Chances.Num() || Values.Num() != RollAmounts.Num() || Chances.Num() != RollAmounts.Num())
		return;
	
	//Do Chances
	int ChanceRole = 0;
	for (int i = 0; i < Values.Num(); i++)
		for (int j = 0; j < RollAmounts[i]; j++)
		{
			ChanceRole = FMath::RandRange(0,100);
			if (ChanceRole <= Chances[i])
				Values_Out.Add(Values[i]);
		}
	
	//Set First Value
	if (Values_Out.Num() != 0)
	{
		ValueFirst_Out = Values_Out[0];
		bHasValues_Out = true;
	}
}

void UFluidToolboxVariablesBPLibrary::MergeRotatorArrays2(const TArray<FRotator> A, const TArray<FRotator> B, TArray<FRotator>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
}

void UFluidToolboxVariablesBPLibrary::MergeRotatorArrays3(const TArray<FRotator> A, const TArray<FRotator> B, const TArray<FRotator> C, TArray<FRotator>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
	Value_Out.Append(C);
}

void UFluidToolboxVariablesBPLibrary::MergeRotatorArrays4(const TArray<FRotator> A, const TArray<FRotator> B, const TArray<FRotator> C, const TArray<FRotator> D, TArray<FRotator>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
	Value_Out.Append(C);
	Value_Out.Append(D);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Variables Blueprint Function Library : Transform Tools

void UFluidToolboxVariablesBPLibrary::GetRememberTransformByName(const FName Name, FTransform& Value_Out, bool& bFound_Out)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	if (GameInstance == nullptr) return;
	const UFluidToolboxVariablesSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxVariablesSubsystem>();
	if (Subsystem == nullptr) return;

	bFound_Out = false;
	Value_Out = FTransform();

	if (Subsystem->TransformValues.Contains(Name))
	{
		bFound_Out = true;
		Value_Out = Subsystem->TransformValues[Name];
	}
}

void UFluidToolboxVariablesBPLibrary::SetRememberTransformByName(const FName Name, FTransform Value, FTransform& Value_Out)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	if (GameInstance == nullptr) return;
	UFluidToolboxVariablesSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxVariablesSubsystem>();
	if (Subsystem == nullptr) return;

	Value_Out = Value;

	if (Subsystem->TransformValues.Contains(Name))
		Subsystem->TransformValues[Name] = Value;
	else
		Subsystem->TransformValues.Add(Name, Value);
}

void UFluidToolboxVariablesBPLibrary::GetRandomTransformByWeight(TArray<FTransform> Values, TArray<float> Weights, int MinCount, int MaxCount, TArray<FTransform>& Values_Out, FTransform& ValueFirst_Out)
{
	//Force Empty
	Values_Out.Empty();

	//Convert To Standard C++ Value Pair | Get Max Weight
	float MaxWeight = 0;
	std::map<float, FTransform> Cumulative;
	for (int i = 0; i < Values.Num() && i < Weights.Num(); i++)
	{
		Cumulative.insert(std::make_pair(Weights[i], Values[i]));
		if (Weights[i] > MaxWeight)
			MaxWeight = Weights[i];
	}

	//Do Get values
	float Linear = 0;
	const int Count = FMath::RandRange(MinCount,MaxCount);
	for (int i = 0; i < Count; i++)
	{
		Linear = rand() * MaxWeight / RAND_MAX;
		Values_Out.Add(Cumulative.upper_bound(Linear)->second);
	}

	//Set First Value
	if (Values_Out.Num() != 0)
		ValueFirst_Out = Values_Out[0];
}

void UFluidToolboxVariablesBPLibrary::GetTransformByChance(const FTransform Value, const int Chance, const int RollAmount, TArray<FTransform>& Values_Out, FTransform& ValueFirst_Out, bool& bHasValues_Out)
{
	//Force Empty
	Values_Out.Empty();
	bHasValues_Out = false;

	//Do Chances
	int ChanceRole = 0;
	for (int i = 0; i < RollAmount; i++)
	{
		ChanceRole = FMath::RandRange(0,100);
		if (ChanceRole <= Chance)
			Values_Out.Add(Value);
	}
	
	//Set First Value
	if (Values_Out.Num() != 0)
	{
		ValueFirst_Out = Values_Out[0];
		bHasValues_Out = true;
	}
}

void UFluidToolboxVariablesBPLibrary::GetTransformByChanceMulti(const TArray<FTransform> Values, const TArray<int> Chances, const TArray<int> RollAmounts, TArray<FTransform>& Values_Out, FTransform& ValueFirst_Out, bool& bHasValues_Out)
{
	//Force Empty
	Values_Out.Empty();
	bHasValues_Out = false;

	//Return If Arrays Are Not Same Length
	if (Values.Num() != Chances.Num() || Values.Num() != RollAmounts.Num() || Chances.Num() != RollAmounts.Num())
		return;
	
	//Do Chances
	int ChanceRole = 0;
	for (int i = 0; i < Values.Num(); i++)
		for (int j = 0; j < RollAmounts[i]; j++)
		{
			ChanceRole = FMath::RandRange(0,100);
			if (ChanceRole <= Chances[i])
				Values_Out.Add(Values[i]);
		}
	
	//Set First Value
	if (Values_Out.Num() != 0)
	{
		ValueFirst_Out = Values_Out[0];
		bHasValues_Out = true;
	}
}

void UFluidToolboxVariablesBPLibrary::MergeTransformArrays2(const TArray<FTransform> A, const TArray<FTransform> B, TArray<FTransform>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
}

void UFluidToolboxVariablesBPLibrary::MergeTransformArrays3(const TArray<FTransform> A, const TArray<FTransform> B, const TArray<FTransform> C, TArray<FTransform>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
	Value_Out.Append(C);
}

void UFluidToolboxVariablesBPLibrary::MergeTransformArrays4(const TArray<FTransform> A, const TArray<FTransform> B, const TArray<FTransform> C, const TArray<FTransform> D, TArray<FTransform>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
	Value_Out.Append(C);
	Value_Out.Append(D);
}

void UFluidToolboxVariablesBPLibrary::GetTransformsAverageLocation(TArray<FTransform> Values, FVector& Value_Out)
{
	Value_Out = FVector(0, 0, 0);
	int C = 0;

	for (int i = 0; i < Values.Num(); i++)
	{
		C++;
		Value_Out += Values[i].GetLocation();
	}

	Value_Out /= C;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Variables Blueprint Function Library : Actor Tools

void UFluidToolboxVariablesBPLibrary::GetRememberActorByName(const FName Name, AActor*& Value_Out, bool& bValueValid_Out, bool& bFound_Out)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	if (GameInstance == nullptr) return;
	const UFluidToolboxVariablesSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxVariablesSubsystem>();
	if (Subsystem == nullptr) return;

	bFound_Out = false;
	Value_Out = nullptr;

	if (Subsystem->ActorValues.Contains(Name))
	{
		bFound_Out = true;
		Value_Out = Subsystem->ActorValues[Name];
	}

	bValueValid_Out = IsValid(Value_Out);
}

void UFluidToolboxVariablesBPLibrary::SetRememberActorByName(const FName Name, AActor* Value, AActor*& Value_Out)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	if (GameInstance == nullptr) return;
	UFluidToolboxVariablesSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxVariablesSubsystem>();
	if (Subsystem == nullptr) return;

	Value_Out = Value;

	if (Subsystem->ActorValues.Contains(Name))
		Subsystem->ActorValues[Name] = Value;
	else
		Subsystem->ActorValues.Add(Name, Value);
}

void UFluidToolboxVariablesBPLibrary::GetRandomActorByWeight(TArray<AActor*> Values, TArray<float> Weights, int MinCount, int MaxCount, TArray<AActor*>& Values_Out, AActor*& ValueFirst_Out)
{
	//Force Empty
	Values_Out.Empty();

	//Convert To Standard C++ Value Pair | Get Max Weight
	float MaxWeight = 0;
	std::map<float, AActor*> Cumulative;
	for (int i = 0; i < Values.Num() && i < Weights.Num(); i++)
	{
		Cumulative.insert(std::make_pair(Weights[i], Values[i]));
		if (Weights[i] > MaxWeight)
			MaxWeight = Weights[i];
	}

	//Do Get values
	float Linear = 0;
	const int Count = FMath::RandRange(MinCount,MaxCount);
	for (int i = 0; i < Count; i++)
	{
		Linear = rand() * MaxWeight / RAND_MAX;
		Values_Out.Add(Cumulative.upper_bound(Linear)->second);
	}

	//Set First Value
	if (Values_Out.Num() != 0)
		ValueFirst_Out = Values_Out[0];
}

void UFluidToolboxVariablesBPLibrary::GetActorByChance(AActor* Value, const int Chance, const int RollAmount, TArray<AActor*>& Values_Out, AActor*& ValueFirst_Out, bool& bHasValues_Out)
{
	//Force Empty
	Values_Out.Empty();
	bHasValues_Out = false;

	//Do Chances
	int ChanceRole = 0;
	for (int i = 0; i < RollAmount; i++)
	{
		ChanceRole = FMath::RandRange(0,100);
		if (ChanceRole <= Chance)
			Values_Out.Add(Value);
	}
	
	//Set First Value
	if (Values_Out.Num() != 0)
	{
		ValueFirst_Out = Values_Out[0];
		bHasValues_Out = true;
	}
}

void UFluidToolboxVariablesBPLibrary::GetActorByChanceMulti(const TArray<AActor*> Values, const TArray<int> Chances, const TArray<int> RollAmounts, TArray<AActor*>& Values_Out, AActor*& ValueFirst_Out, bool& bHasValues_Out)
{
	//Force Empty
	Values_Out.Empty();
	bHasValues_Out = false;

	//Return If Arrays Are Not Same Length
	if (Values.Num() != Chances.Num() || Values.Num() != RollAmounts.Num() || Chances.Num() != RollAmounts.Num())
		return;
	
	//Do Chances
	int ChanceRole = 0;
	for (int i = 0; i < Values.Num(); i++)
		for (int j = 0; j < RollAmounts[i]; j++)
		{
			ChanceRole = FMath::RandRange(0,100);
			if (ChanceRole <= Chances[i])
				Values_Out.Add(Values[i]);
		}
	
	//Set First Value
	if (Values_Out.Num() != 0)
	{
		ValueFirst_Out = Values_Out[0];
		bHasValues_Out = true;
	}
}

void UFluidToolboxVariablesBPLibrary::MergeActorArrays2(const TArray<AActor*> A, const TArray<AActor*> B, TArray<AActor*>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
}

void UFluidToolboxVariablesBPLibrary::MergeActorArrays3(const TArray<AActor*> A, const TArray<AActor*> B, const TArray<AActor*> C, TArray<AActor*>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
	Value_Out.Append(C);
}

void UFluidToolboxVariablesBPLibrary::MergeActorArrays4(const TArray<AActor*> A, const TArray<AActor*> B, const TArray<AActor*> C, const TArray<AActor*> D, TArray<AActor*>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
	Value_Out.Append(C);
	Value_Out.Append(D);
}

void UFluidToolboxVariablesBPLibrary::GetActorsAverageLocation(TArray<AActor*> Values, FVector& Value_Out)
{
	Value_Out = FVector(0, 0, 0);
	int C = 0;

	for (int i = 0; i < Values.Num(); i++)
	{
		C++;
		Value_Out += Values[i]->GetActorLocation();
	}

	Value_Out /= C;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Variables Blueprint Function Library : UObject Tools

void UFluidToolboxVariablesBPLibrary::GetRememberObjectByName(const FName Name, UObject*& Value_Out, bool& bValueValid_Out, bool& bFound_Out)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	if (GameInstance == nullptr) return;
	const UFluidToolboxVariablesSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxVariablesSubsystem>();
	if (Subsystem == nullptr) return;

	bFound_Out = false;
	Value_Out = nullptr;

	if (Subsystem->UObjectValues.Contains(Name))
	{
		bFound_Out = true;
		Value_Out = Subsystem->UObjectValues[Name];
	}

	bValueValid_Out =  IsValid(Value_Out);
}

void UFluidToolboxVariablesBPLibrary::SetRememberObjectByName(const FName Name, UObject* Value, UObject*& Value_Out)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	if (GameInstance == nullptr) return;
	UFluidToolboxVariablesSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxVariablesSubsystem>();
	if (Subsystem == nullptr) return;

	Value_Out = Value;

	if (Subsystem->UObjectValues.Contains(Name))
		Subsystem->UObjectValues[Name] = Value;
	else
		Subsystem->UObjectValues.Add(Name, Value);
}

void UFluidToolboxVariablesBPLibrary::GetRandomObjectByWeight(TArray<UObject*> Values, TArray<float> Weights, const int MinCount, const int MaxCount, TArray<UObject*>& Values_Out, UObject*& ValueFirst_Out)
{
	//Force Empty
	Values_Out.Empty();

	//Convert To Standard C++ Value Pair | Get Max Weight
	float MaxWeight = 0;
	std::map<float, UObject*> Cumulative;
	for (int i = 0; i < Values.Num() && i < Weights.Num(); i++)
	{
		Cumulative.insert(std::make_pair(Weights[i], Values[i]));
		if (Weights[i] > MaxWeight)
			MaxWeight = Weights[i];
	}

	//Do Get values
	float Linear = 0;
	const int Count = FMath::RandRange(MinCount,MaxCount);
	for (int i = 0; i < Count; i++)
	{
		Linear = rand() * MaxWeight / RAND_MAX;
		Values_Out.Add(Cumulative.upper_bound(Linear)->second);
	}

	//Set First Value
	if (Values_Out.Num() != 0)
		ValueFirst_Out = Values_Out[0];
}

void UFluidToolboxVariablesBPLibrary::GetObjectByChance(UObject* Value, const int Chance, const int RollAmount, TArray<UObject*>& Values_Out, UObject*& ValueFirst_Out, bool& bHasValues_Out)
{
	//Force Empty
	Values_Out.Empty();
	bHasValues_Out = false;

	//Do Chances
	int ChanceRole = 0;
	for (int i = 0; i < RollAmount; i++)
	{
		ChanceRole = FMath::RandRange(0,100);
		if (ChanceRole <= Chance)
			Values_Out.Add(Value);
	}
	
	//Set First Value
	if (Values_Out.Num() != 0)
	{
		ValueFirst_Out = Values_Out[0];
		bHasValues_Out = true;
	}
}

void UFluidToolboxVariablesBPLibrary::GetObjectByChanceMulti(TArray<UObject*> Values, const TArray<int> Chances, const TArray<int> RollAmounts, TArray<UObject*>& Values_Out, UObject*& ValueFirst_Out, bool& bHasValues_Out)
{
	//Force Empty
	Values_Out.Empty();
	bHasValues_Out = false;

	//Return If Arrays Are Not Same Length
	if (Values.Num() != Chances.Num() || Values.Num() != RollAmounts.Num() || Chances.Num() != RollAmounts.Num())
		return;
	
	//Do Chances
	int ChanceRole = 0;
	for (int i = 0; i < Values.Num(); i++)
		for (int j = 0; j < RollAmounts[i]; j++)
		{
			ChanceRole = FMath::RandRange(0,100);
			if (ChanceRole <= Chances[i])
				Values_Out.Add(Values[i]);
		}
	
	//Set First Value
	if (Values_Out.Num() != 0)
	{
		ValueFirst_Out = Values_Out[0];
		bHasValues_Out = true;
	}
}

void UFluidToolboxVariablesBPLibrary::MergeObjectArrays2(const TArray<UObject*> A, const TArray<UObject*> B, TArray<UObject*>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
}

void UFluidToolboxVariablesBPLibrary::MergeObjectArrays3(const TArray<UObject*> A, const TArray<UObject*> B, const TArray<UObject*> C, TArray<UObject*>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
	Value_Out.Append(C);
}

void UFluidToolboxVariablesBPLibrary::MergeObjectArrays4(const TArray<UObject*> A, const TArray<UObject*> B, const TArray<UObject*> C, const TArray<UObject*> D, TArray<UObject*>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
	Value_Out.Append(C);
	Value_Out.Append(D);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Variables Blueprint Function Library : Actor UClass Tools

void UFluidToolboxVariablesBPLibrary::GetRememberActorUClassByName(const FName Name, TSubclassOf<AActor>& Value_Out, bool& bValueValid_Out, bool& bFound_Out)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	if (GameInstance == nullptr) return;
	const UFluidToolboxVariablesSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxVariablesSubsystem>();
	if (Subsystem == nullptr) return;

	bFound_Out = false;
	Value_Out = nullptr;

	if (Subsystem->ActorUClassValues.Contains(Name))
	{
		bFound_Out = true;
		Value_Out = Subsystem->ActorUClassValues[Name];
	}

	bValueValid_Out = IsValid(Value_Out);
}

void UFluidToolboxVariablesBPLibrary::SetRememberActorUClassByName(const FName Name, TSubclassOf<AActor> Value, TSubclassOf<AActor>& Value_Out)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	if (GameInstance == nullptr) return;
	UFluidToolboxVariablesSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxVariablesSubsystem>();
	if (Subsystem == nullptr) return;

	Value_Out = Value;

	if (Subsystem->ActorUClassValues.Contains(Name))
		Subsystem->ActorUClassValues[Name] = Value;
	else
		Subsystem->ActorUClassValues.Add(Name, Value);
}

void UFluidToolboxVariablesBPLibrary::GetRandomActorUClassByWeight(TArray<TSubclassOf<AActor>> Values, TArray<float> Weights, const int MinCount, const int MaxCount, TArray<TSubclassOf<AActor>>& Values_Out, TSubclassOf<AActor>& ValueFirst_Out)
{
	//Force Empty
	Values_Out.Empty();

	//Convert To Standard C++ Value Pair | Get Max Weight
	float MaxWeight = 0;
	std::map<float, TSubclassOf<AActor>> Cumulative;
	for (int i = 0; i < Values.Num() && i < Weights.Num(); i++)
	{
		Cumulative.insert(std::make_pair(Weights[i], Values[i]));
		if (Weights[i] > MaxWeight)
			MaxWeight = Weights[i];
	}

	//Do Get values
	float Linear = 0;
	const int Count = FMath::RandRange(MinCount,MaxCount);
	for (int i = 0; i < Count; i++)
	{
		Linear = rand() * MaxWeight / RAND_MAX;
		Values_Out.Add(Cumulative.upper_bound(Linear)->second);
	}

	//Set First Value
	if (Values_Out.Num() != 0)
		ValueFirst_Out = Values_Out[0];
}

void UFluidToolboxVariablesBPLibrary::GetActorUClassByChance(const TSubclassOf<AActor> Value, const int Chance, const int RollAmount, TArray<TSubclassOf<AActor>>& Values_Out, TSubclassOf<AActor>& ValueFirst_Out, bool& bHasValues_Out)
{
	//Force Empty
	Values_Out.Empty();
	bHasValues_Out = false;

	//Do Chances
	int ChanceRole = 0;
	for (int i = 0; i < RollAmount; i++)
	{
		ChanceRole = FMath::RandRange(0,100);
		if (ChanceRole <= Chance)
			Values_Out.Add(Value);
	}
	
	//Set First Value
	if (Values_Out.Num() != 0)
	{
		ValueFirst_Out = Values_Out[0];
		bHasValues_Out = true;
	}
}

void UFluidToolboxVariablesBPLibrary::GetActorUClassByChanceMulti(const TArray<TSubclassOf<AActor>> Values, const TArray<int> Chances, const TArray<int> RollAmounts, TArray<TSubclassOf<AActor>>& Values_Out, TSubclassOf<AActor>& ValueFirst_Out, bool& bHasValues_Out)
{
	//Force Empty
	Values_Out.Empty();
	bHasValues_Out = false;

	//Return If Arrays Are Not Same Length
	if (Values.Num() != Chances.Num() || Values.Num() != RollAmounts.Num() || Chances.Num() != RollAmounts.Num())
		return;
	
	//Do Chances
	int ChanceRole = 0;
	for (int i = 0; i < Values.Num(); i++)
		for (int j = 0; j < RollAmounts[i]; j++)
		{
			ChanceRole = FMath::RandRange(0,100);
			if (ChanceRole <= Chances[i])
				Values_Out.Add(Values[i]);
		}
	
	//Set First Value
	if (Values_Out.Num() != 0)
	{
		ValueFirst_Out = Values_Out[0];
		bHasValues_Out = true;
	}
}

void UFluidToolboxVariablesBPLibrary::MergeActorUClassArrays2(const TArray<TSubclassOf<AActor>> A, const TArray<TSubclassOf<AActor>> B, TArray<TSubclassOf<AActor>>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
}

void UFluidToolboxVariablesBPLibrary::MergeActorUClassArrays3(const TArray<TSubclassOf<AActor>> A, const TArray<TSubclassOf<AActor>> B, const TArray<TSubclassOf<AActor>> C, TArray<TSubclassOf<AActor>>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
	Value_Out.Append(C);
}

void UFluidToolboxVariablesBPLibrary::MergeActorUClassArrays4(const TArray<TSubclassOf<AActor>> A, const TArray<TSubclassOf<AActor>> B, const TArray<TSubclassOf<AActor>> C, const TArray<TSubclassOf<AActor>> D, TArray<TSubclassOf<AActor>>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
	Value_Out.Append(C);
	Value_Out.Append(D);
}

void UFluidToolboxVariablesBPLibrary::SpawnActorFromActorUClass(TSubclassOf<AActor> ActorClass, const FTransform Transform, ESpawnActorCollisionHandlingMethod SpawnActorCollisionHandlingMethod, bool bDebugActorClassName, AActor*& Actor_Out)
{
	//Return If Class In Null
	if (ActorClass == nullptr || GEngine == nullptr)
		return;

	//Get Subsystem
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	if (GameInstance == nullptr) return;
	const UFluidToolboxVariablesSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxVariablesSubsystem>();
	if (Subsystem == nullptr) return;
	
	Actor_Out = Subsystem->GetWorld()->SpawnActorDeferred<AActor>(ActorClass,Transform,nullptr,nullptr,SpawnActorCollisionHandlingMethod);

	if (bDebugActorClassName && Actor_Out != nullptr)
		GEngine->AddOnScreenDebugMessage(-1,1,FColor::Cyan,Actor_Out->GetClass()->GetName());
}

void UFluidToolboxVariablesBPLibrary::AddActorTag(AActor* Actor, const FName Tag, bool& bDidAddTag_Out)
{
	//Force Empty
	bDidAddTag_Out = false;

	//Get Subsystem
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	if (GameInstance == nullptr) return;
	UFluidToolboxVariablesSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxVariablesSubsystem>();
	if (Subsystem == nullptr) return;
	
	//Return If Actor Is Null
	if (Actor == nullptr)
		return;

	//Add Actor To Actor Tags
	if (!Subsystem->ActorTags.Contains(Actor))
		Subsystem->ActorTags.Add(Actor);

	//Add Actor Tag
	if (!Subsystem->ActorTags[Actor].Tags.Contains(Tag))
	{
		Subsystem->ActorTags[Actor].Tags.Add(Tag);
		bDidAddTag_Out = true;
	}
}

void UFluidToolboxVariablesBPLibrary::AddActorTags(AActor* Actor, const TArray<FName> Tags, bool& bDidAddTag_Out)
{
	//Force Empty
	bDidAddTag_Out = false;
	
    //Add Actor Tags
	bool bDidAddTag;
    for (int i = 0; i < Tags.Num(); i++)
    {
	    AddActorTag(Actor,Tags[i], bDidAddTag);
    	if (bDidAddTag) bDidAddTag_Out = true;
    }
}

void UFluidToolboxVariablesBPLibrary::RemoveActorTag(AActor* Actor, const FName Tag, bool& bDidRemoveTag_Out)
{
	//Force Empty
	bDidRemoveTag_Out = false;

	//Get Subsystem
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	if (GameInstance == nullptr) return;
	UFluidToolboxVariablesSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxVariablesSubsystem>();
	if (Subsystem == nullptr) return;
	
	//Return If Actor Is Null
	if (Actor == nullptr)
		return;

	//Add Actor To Actor Tags
	if (!Subsystem->ActorTags.Contains(Actor))
		Subsystem->ActorTags.Add(Actor);

	//Remove Actor Tag
	if (Subsystem->ActorTags[Actor].Tags.Contains(Tag))
	{
		Subsystem->ActorTags[Actor].Tags.Remove(Tag);
		bDidRemoveTag_Out = true;
	}
}

void UFluidToolboxVariablesBPLibrary::RemoveActorTags(AActor* Actor, const TArray<FName> Tags, bool& bDidRemoveTag_Out)
{
	//Force Empty
	bDidRemoveTag_Out = false;
	
	//Add Actor Tags
	bool bDidRemoveTag;
	for (int i = 0; i < Tags.Num(); i++)
	{
		RemoveActorTag(Actor,Tags[i], bDidRemoveTag);
		if (bDidRemoveTag) bDidRemoveTag_Out = true;
	}
}

void UFluidToolboxVariablesBPLibrary::RemoveActorTagsThatContainsString(AActor* Actor, const FString String, bool& bDidRemoveTag_Out)
{
	//Force Empty
	bDidRemoveTag_Out = false;
	
	//Get Subsystem
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	if (GameInstance == nullptr) return;
	UFluidToolboxVariablesSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxVariablesSubsystem>();
	if (Subsystem == nullptr) return;
	
	//Return If Actor Is Null
	if (Actor == nullptr)
		return;

	//Add Actor To Actor Tags
	if (!Subsystem->ActorTags.Contains(Actor))
		Subsystem->ActorTags.Add(Actor);

	//Remove Actor Tags
	for (int i = Subsystem->ActorTags[Actor].Tags.Num(); i--; )
		if (Subsystem->ActorTags[Actor].Tags[i].ToString().Contains(String))
		{
			Subsystem->ActorTags[Actor].Tags.RemoveAt(i);
			bDidRemoveTag_Out = true;
		}
}

void UFluidToolboxVariablesBPLibrary::GetActorTagThatContainsString(AActor* Actor, const FString String, TArray<FName>& Tags_Out, FName& TagAtFirstIndex_out, bool& bHasTags_Out)
{
	//Get Subsystem
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	if (GameInstance == nullptr) return;
	UFluidToolboxVariablesSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxVariablesSubsystem>();
	if (Subsystem == nullptr) return;
	
	//Return If Actor Is Null
	if (Actor == nullptr)
		return;

	//Add Actor To Actor Tags
	if (!Subsystem->ActorTags.Contains(Actor))
		Subsystem->ActorTags.Add(Actor);

	//Remove Actor Tag
	for (int i = 0; i < Subsystem->ActorTags[Actor].Tags.Num(); i++)
	{
		if (Subsystem->ActorTags[Actor].Tags[i].ToString().Contains(String))
		{
			TagAtFirstIndex_out = Tags_Out.Num() == 0 ? Subsystem->ActorTags[Actor].Tags[i] : TagAtFirstIndex_out;
			Tags_Out.Add(Subsystem->ActorTags[Actor].Tags[i]);
			bHasTags_Out = true;
			return;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Variables Blueprint Function Library : UClass Tools

void UFluidToolboxVariablesBPLibrary::GetRememberUClassByName(const FName Name, UClass*& Value_Out, bool& bValueValid_Out, bool& bFound_Out)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	if (GameInstance == nullptr) return;
	const UFluidToolboxVariablesSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxVariablesSubsystem>();
	if (Subsystem == nullptr) return;

	bFound_Out = false;
	Value_Out = nullptr;

	if (Subsystem->UClassValues.Contains(Name))
	{
		bFound_Out = true;
		Value_Out = Subsystem->UClassValues[Name];
	}

	bValueValid_Out =  IsValid(Value_Out);
}

void UFluidToolboxVariablesBPLibrary::SetRememberUClassByName(const FName Name, UClass* Value, UClass*& Value_Out)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	if (GameInstance == nullptr) return;
	UFluidToolboxVariablesSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxVariablesSubsystem>();
	if (Subsystem == nullptr) return;

	Value_Out = Value;

	if (Subsystem->UClassValues.Contains(Name))
		Subsystem->UClassValues[Name] = Value;
	else
		Subsystem->UClassValues.Add(Name, Value);
}

void UFluidToolboxVariablesBPLibrary::GetRandomUClassByWeight(TArray<UClass*> Values, TArray<float> Weights, const int MinCount, const int MaxCount, TArray<UClass*>& Values_Out, UClass*& ValueFirst_Out)
{
	//Force Empty
	Values_Out.Empty();

	//Convert To Standard C++ Value Pair | Get Max Weight
	float MaxWeight = 0;
	std::map<float, UClass*> Cumulative;
	for (int i = 0; i < Values.Num() && i < Weights.Num(); i++)
	{
		Cumulative.insert(std::make_pair(Weights[i], Values[i]));
		if (Weights[i] > MaxWeight)
			MaxWeight = Weights[i];
	}

	//Do Get values
	float Linear = 0;
	const int Count = FMath::RandRange(MinCount,MaxCount);
	for (int i = 0; i < Count; i++)
	{
		Linear = rand() * MaxWeight / RAND_MAX;
		Values_Out.Add(Cumulative.upper_bound(Linear)->second);
	}

	//Set First Value
	if (Values_Out.Num() != 0)
		ValueFirst_Out = Values_Out[0];
}

void UFluidToolboxVariablesBPLibrary::GetUClassByChance(UClass* Value, const int Chance, const int RollAmount, TArray<UClass*>& Values_Out, UClass*& ValueFirst_Out, bool& bHasValues_Out)
{
	//Force Empty
	Values_Out.Empty();
	bHasValues_Out = false;

	//Do Chances
	int ChanceRole = 0;
	for (int i = 0; i < RollAmount; i++)
	{
		ChanceRole = FMath::RandRange(0,100);
		if (ChanceRole <= Chance)
			Values_Out.Add(Value);
	}
	
	//Set First Value
	if (Values_Out.Num() != 0)
	{
		ValueFirst_Out = Values_Out[0];
		bHasValues_Out = true;
	}
}

void UFluidToolboxVariablesBPLibrary::GetUClassByChanceMulti(const TArray<UClass*> Values, const TArray<int> Chances, const TArray<int> RollAmounts, TArray<UClass*>& Values_Out, UClass*& ValueFirst_Out, bool& bHasValues_Out)
{
	//Force Empty
	Values_Out.Empty();
	bHasValues_Out = false;

	//Return If Arrays Are Not Same Length
	if (Values.Num() != Chances.Num() || Values.Num() != RollAmounts.Num() || Chances.Num() != RollAmounts.Num())
		return;
	
	//Do Chances
	int ChanceRole = 0;
	for (int i = 0; i < Values.Num(); i++)
		for (int j = 0; j < RollAmounts[i]; j++)
		{
			ChanceRole = FMath::RandRange(0,100);
			if (ChanceRole <= Chances[i])
				Values_Out.Add(Values[i]);
		}
	
	//Set First Value
	if (Values_Out.Num() != 0)
	{
		ValueFirst_Out = Values_Out[0];
		bHasValues_Out = true;
	}
}

void UFluidToolboxVariablesBPLibrary::MergeUClassArrays2(const TArray<UClass*> A, const TArray<UClass*> B, TArray<UClass*>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
}

void UFluidToolboxVariablesBPLibrary::MergeUClassArrays3(const TArray<UClass*> A, const TArray<UClass*> B, const TArray<UClass*> C, TArray<UClass*>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
	Value_Out.Append(C);
}

void UFluidToolboxVariablesBPLibrary::MergeUClassArrays4(const TArray<UClass*> A, const TArray<UClass*> B, const TArray<UClass*> C, const TArray<UClass*> D, TArray<UClass*>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
	Value_Out.Append(C);
	Value_Out.Append(D);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Variables Blueprint Function Library : Scene Component Tools

void UFluidToolboxVariablesBPLibrary::GetRememberSceneComponentByName(const FName Name, USceneComponent*& Value_Out, bool& bValueValid_Out, bool& bFound_Out)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	if (GameInstance == nullptr) return;
	const UFluidToolboxVariablesSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxVariablesSubsystem>();
	if (Subsystem == nullptr) return;

	bFound_Out = false;
	Value_Out = nullptr;

	if (Subsystem->SceneComponentValues.Contains(Name))
	{
		bFound_Out = true;
		Value_Out = Subsystem->SceneComponentValues[Name];
	}

	bValueValid_Out =  IsValid(Value_Out);
}

void UFluidToolboxVariablesBPLibrary::SetRememberSceneComponentByName(const FName Name, USceneComponent* Value, USceneComponent*& Value_Out)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	if (GameInstance == nullptr) return;
	UFluidToolboxVariablesSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxVariablesSubsystem>();
	if (Subsystem == nullptr) return;

	Value_Out = Value;

	if (Subsystem->SceneComponentValues.Contains(Name))
		Subsystem->SceneComponentValues[Name] = Value;
	else
		Subsystem->SceneComponentValues.Add(Name, Value);
}

void UFluidToolboxVariablesBPLibrary::GetRandomSceneComponentByWeight(TArray<USceneComponent*> Values, TArray<float> Weights, const int MinCount, const int MaxCount, TArray<USceneComponent*>& Values_Out, USceneComponent*& ValueFirst_Out)
{
	//Force Empty
	Values_Out.Empty();

	//Convert To Standard C++ Value Pair | Get Max Weight
	float MaxWeight = 0;
	std::map<float, USceneComponent*> Cumulative;
	for (int i = 0; i < Values.Num() && i < Weights.Num(); i++)
	{
		Cumulative.insert(std::make_pair(Weights[i], Values[i]));
		if (Weights[i] > MaxWeight)
			MaxWeight = Weights[i];
	}

	//Do Get values
	float Linear = 0;
	const int Count = FMath::RandRange(MinCount,MaxCount);
	for (int i = 0; i < Count; i++)
	{
		Linear = rand() * MaxWeight / RAND_MAX;
		Values_Out.Add(Cumulative.upper_bound(Linear)->second);
	}

	//Set First Value
	if (Values_Out.Num() != 0)
		ValueFirst_Out = Values_Out[0];
}

void UFluidToolboxVariablesBPLibrary::GetSceneComponentByChance(USceneComponent* Value, const int Chance, const int RollAmount, TArray<USceneComponent*>& Values_Out, USceneComponent*& ValueFirst_Out, bool& bHasValues_Out)
{
	//Force Empty
	Values_Out.Empty();
	bHasValues_Out = false;

	//Do Chances
	int ChanceRole = 0;
	for (int i = 0; i < RollAmount; i++)
	{
		ChanceRole = FMath::RandRange(0,100);
		if (ChanceRole <= Chance)
			Values_Out.Add(Value);
	}
	
	//Set First Value
	if (Values_Out.Num() != 0)
	{
		ValueFirst_Out = Values_Out[0];
		bHasValues_Out = true;
	}
}

void UFluidToolboxVariablesBPLibrary::GetSceneComponentByChanceMulti(TArray<USceneComponent*> Values, const TArray<int> Chances, const TArray<int> RollAmounts, TArray<USceneComponent*>& Values_Out, USceneComponent*& ValueFirst_Out, bool& bHasValues_Out)
{
	//Force Empty
	Values_Out.Empty();
	bHasValues_Out = false;

	//Return If Arrays Are Not Same Length
	if (Values.Num() != Chances.Num() || Values.Num() != RollAmounts.Num() || Chances.Num() != RollAmounts.Num())
		return;
	
	//Do Chances
	int ChanceRole = 0;
	for (int i = 0; i < Values.Num(); i++)
		for (int j = 0; j < RollAmounts[i]; j++)
		{
			ChanceRole = FMath::RandRange(0,100);
			if (ChanceRole <= Chances[i])
				Values_Out.Add(Values[i]);
		}
	
	//Set First Value
	if (Values_Out.Num() != 0)
	{
		ValueFirst_Out = Values_Out[0];
		bHasValues_Out = true;
	}
}

void UFluidToolboxVariablesBPLibrary::MergeSceneComponentArrays2(const TArray<USceneComponent*> A, const TArray<USceneComponent*> B, TArray<USceneComponent*>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
}

void UFluidToolboxVariablesBPLibrary::MergeSceneComponentArrays3(const TArray<USceneComponent*> A, const TArray<USceneComponent*> B, const TArray<USceneComponent*> C, TArray<USceneComponent*>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
	Value_Out.Append(C);
}

void UFluidToolboxVariablesBPLibrary::MergeSceneComponentArrays4(const TArray<USceneComponent*> A, const TArray<USceneComponent*> B, const TArray<USceneComponent*> C, const TArray<USceneComponent*> D, TArray<USceneComponent*>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
	Value_Out.Append(C);
	Value_Out.Append(D);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Variables Blueprint Function Library : User Widget Tools

void UFluidToolboxVariablesBPLibrary::GetRememberUserWidgetByName(const FName Name, UUserWidget*& Value_Out, bool& bValueValid_Out, bool& bFound_Out)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	if (GameInstance == nullptr) return;
	const UFluidToolboxVariablesSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxVariablesSubsystem>();
	if (Subsystem == nullptr) return;

	bFound_Out = false;
	Value_Out = nullptr;

	if (Subsystem->UserWidgetValues.Contains(Name))
	{
		bFound_Out = true;
		Value_Out = Subsystem->UserWidgetValues[Name];
	}

	bValueValid_Out =  IsValid(Value_Out);
}

void UFluidToolboxVariablesBPLibrary::SetRememberUserWidgetByName(const FName Name, UUserWidget* Value, UUserWidget*& Value_Out)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	if (GameInstance == nullptr) return;
	UFluidToolboxVariablesSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxVariablesSubsystem>();
	if (Subsystem == nullptr) return;

	Value_Out = Value;

	if (Subsystem->UserWidgetValues.Contains(Name))
		Subsystem->UserWidgetValues[Name] = Value;
	else
		Subsystem->UserWidgetValues.Add(Name, Value);
}

void UFluidToolboxVariablesBPLibrary::GetRandomUserWidgetByWeight(TArray<UUserWidget*> Values, TArray<float> Weights, const int MinCount, const int MaxCount, TArray<UUserWidget*>& Values_Out, UUserWidget*& ValueFirst_Out)
{
	//Force Empty
	Values_Out.Empty();

	//Convert To Standard C++ Value Pair | Get Max Weight
	float MaxWeight = 0;
	std::map<float, UUserWidget*> Cumulative;
	for (int i = 0; i < Values.Num() && i < Weights.Num(); i++)
	{
		Cumulative.insert(std::make_pair(Weights[i], Values[i]));
		if (Weights[i] > MaxWeight)
			MaxWeight = Weights[i];
	}

	//Do Get values
	float Linear = 0;
	const int Count = FMath::RandRange(MinCount,MaxCount);
	for (int i = 0; i < Count; i++)
	{
		Linear = rand() * MaxWeight / RAND_MAX;
		Values_Out.Add(Cumulative.upper_bound(Linear)->second);
	}

	//Set First Value
	if (Values_Out.Num() != 0)
		ValueFirst_Out = Values_Out[0];
}

void UFluidToolboxVariablesBPLibrary::GetUserWidgetByChance(UUserWidget* Value, const int Chance, const int RollAmount, TArray<UUserWidget*>& Values_Out, UUserWidget*& ValueFirst_Out, bool& bHasValues_Out)
{
	//Force Empty
	Values_Out.Empty();
	bHasValues_Out = false;

	//Do Chances
	int ChanceRole = 0;
	for (int i = 0; i < RollAmount; i++)
	{
		ChanceRole = FMath::RandRange(0,100);
		if (ChanceRole <= Chance)
			Values_Out.Add(Value);
	}
	
	//Set First Value
	if (Values_Out.Num() != 0)
	{
		ValueFirst_Out = Values_Out[0];
		bHasValues_Out = true;
	}
}

void UFluidToolboxVariablesBPLibrary::GetUserWidgetByChanceMulti(TArray<UUserWidget*> Values, const TArray<int> Chances, const TArray<int> RollAmounts, TArray<UUserWidget*>& Values_Out, UUserWidget*& ValueFirst_Out, bool& bHasValues_Out)
{
	//Force Empty
	Values_Out.Empty();
	bHasValues_Out = false;

	//Return If Arrays Are Not Same Length
	if (Values.Num() != Chances.Num() || Values.Num() != RollAmounts.Num() || Chances.Num() != RollAmounts.Num())
		return;
	
	//Do Chances
	int ChanceRole = 0;
	for (int i = 0; i < Values.Num(); i++)
		for (int j = 0; j < RollAmounts[i]; j++)
		{
			ChanceRole = FMath::RandRange(0,100);
			if (ChanceRole <= Chances[i])
				Values_Out.Add(Values[i]);
		}
	
	//Set First Value
	if (Values_Out.Num() != 0)
	{
		ValueFirst_Out = Values_Out[0];
		bHasValues_Out = true;
	}
}

void UFluidToolboxVariablesBPLibrary::MergeUserWidgetArrays2(const TArray<UUserWidget*> A, const TArray<UUserWidget*> B, TArray<UUserWidget*>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
}

void UFluidToolboxVariablesBPLibrary::MergeUserWidgetArrays3(const TArray<UUserWidget*> A, const TArray<UUserWidget*> B, const TArray<UUserWidget*> C, TArray<UUserWidget*>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
	Value_Out.Append(C);
}

void UFluidToolboxVariablesBPLibrary::MergeUserWidgetArrays4(const TArray<UUserWidget*> A, const TArray<UUserWidget*> B, const TArray<UUserWidget*> C, const TArray<UUserWidget*> D, TArray<UUserWidget*>& Value_Out)
{
	Value_Out.Empty();
	Value_Out.Append(A);
	Value_Out.Append(B);
	Value_Out.Append(C);
	Value_Out.Append(D);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Variables Subsystem : Begin | End

void UFluidToolboxVariablesSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
}

void UFluidToolboxVariablesSubsystem::Deinitialize()
{
	BoolValues.Empty();
	ByteValues.Empty();
	IntValues.Empty();
	FloatValues.Empty();
	NameValues.Empty();
	StringValues.Empty();
	TextValues.Empty();
	VectorValues.Empty();
	RotatorValues.Empty();
	TransformValues.Empty();
	ActorValues.Empty();
	ActorUClassValues.Empty();
	UClassValues.Empty();
}