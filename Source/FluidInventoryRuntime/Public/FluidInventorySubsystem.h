// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <queue>
#include <bitset>
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "FluidInventorySubsystem.generated.h"

//Fluid Inventory Inventories | Items Max
const int FLUID_INVENTORY_INVENTORIES_MAX = 10000;
const int FLUID_INVENTORY_ITEMS_MAX = 10000;

//Fluid Inventory Inventory Signature
using FluidInventoryInventorySignature = std::bitset<FLUID_INVENTORY_INVENTORIES_MAX>;
using FluidInventoryItemSignature = std::bitset<FLUID_INVENTORY_ITEMS_MAX>;

//Fluid Inventory Item Archetype List Editor Filter
UENUM(BlueprintType)
enum class EFluidInventoryItemArchetypeListEditorFilter : uint8
{
	All,
	Absolute,
	Chance,
	Weighted
};

//Fluid Inventory Item Archetype List Absolute Item
USTRUCT(BlueprintType)
struct FLUIDINVENTORYRUNTIME_API FFluidInventoryItemArchetypeListAbsoluteItem
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName ItemArchetypeID;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin=0))
		int Quantity;
};

//Fluid Inventory Item Archetype List Chance Item
USTRUCT(BlueprintType)
struct FLUIDINVENTORYRUNTIME_API FFluidInventoryItemArchetypeListChanceItem
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName ItemArchetypeID;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin=0))
		int QuantityMin;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin=0))
		int QuantityMax;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin=0, ClampMax=100))
		int Chance;
};

//Fluid Inventory Item Archetype List Weighted Item
USTRUCT(BlueprintType)
struct FLUIDINVENTORYRUNTIME_API FFluidInventoryItemArchetypeListWeightedItem
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName ItemArchetypeID;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		int Weight;
};

//Fluid Inventory Item Archetype List
USTRUCT(BlueprintType)
struct FLUIDINVENTORYRUNTIME_API FFluidInventoryItemArchetypeList
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(DisplayName = "Filter (Editor Only)"))
		EFluidInventoryItemArchetypeListEditorFilter EditorFilter;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(DisplayName = "Absolute Items", EditCondition="EditorFilter == EFluidInventoryItemArchetypeListEditorFilter::All || EditorFilter == EFluidInventoryItemArchetypeListEditorFilter::Absolute", EditConditionHides, TitleProperty = "ItemArchetypeID"))
		TArray<FFluidInventoryItemArchetypeListAbsoluteItem> AbsoluteItems;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(DisplayName = "Chance Items", EditCondition="EditorFilter == EFluidInventoryItemArchetypeListEditorFilter::All || EditorFilter == EFluidInventoryItemArchetypeListEditorFilter::Chance", EditConditionHides, TitleProperty = "ItemArchetypeID"))
		TArray<FFluidInventoryItemArchetypeListChanceItem> ChanceItems;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(DisplayName = "Weighted Items", EditCondition="EditorFilter == EFluidInventoryItemArchetypeListEditorFilter::All || EditorFilter == EFluidInventoryItemArchetypeListEditorFilter::Weighted", EditConditionHides,TitleProperty = "ItemArchetypeID"))
		TArray<FFluidInventoryItemArchetypeListWeightedItem> WeightedItems;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(DisplayName = "Weighted Items Quantity Min", EditCondition="EditorFilter == EFluidInventoryItemArchetypeListEditorFilter::All || EditorFilter == EFluidInventoryItemArchetypeListEditorFilter::Weighted", EditConditionHides, ClampMin=0))
		int WeightedItemsQuantityMin;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(DisplayName = "Weighted Items Quantity Max", EditCondition="EditorFilter == EFluidInventoryItemArchetypeListEditorFilter::All || EditorFilter == EFluidInventoryItemArchetypeListEditorFilter::Weighted", EditConditionHides, ClampMin=0))
		int WeightedItemsQuantityMax;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(DisplayName = "Weighted Items No Item Chance", EditCondition="EditorFilter == EFluidInventoryItemArchetypeListEditorFilter::All || EditorFilter == EFluidInventoryItemArchetypeListEditorFilter::Weighted", EditConditionHides, ClampMin=0,  ClampMax=100))
		int WeightedItemsNoneChance;
};

//Fluid Inventory Item UI
USTRUCT(BlueprintType)
struct FLUIDINVENTORYRUNTIME_API FFluidInventoryItemArchetypeUI
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName Name;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName Description;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UTexture2D* Icon;
};

//Fluid Inventory Item Archetype Stats
USTRUCT(BlueprintType)
struct FLUIDINVENTORYRUNTIME_API FFluidInventoryItemArchetypeStats
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (ClampMin = 1))
		int MaxStackAmount;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		int Worth;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TMap<FName,FName> CustomStats;
};

//Fluid Inventory Item Archetype Socket
USTRUCT(BlueprintType)
struct FLUIDINVENTORYRUNTIME_API FFluidInventoryItemArchetypeSocket 
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName MainSocket;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName IdleSocket;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bIsAddableToInventory;
};

//Fluid Inventory Archetype
USTRUCT(BlueprintType)
struct FLUIDINVENTORYRUNTIME_API FFluidInventoryArchetype
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FName> Tags;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TMap<FName,int> ItemLimitWithTags;
	UPROPERTY(EditAnywhere, BlueprintReadOnly);
		int MaxItemCount;
};


//Fluid Inventory Item Archetype
USTRUCT(BlueprintType)
struct FLUIDINVENTORYRUNTIME_API FFluidInventoryItemArchetype
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FFluidInventoryItemArchetypeUI UI;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FFluidInventoryItemArchetypeStats Stats;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FFluidInventoryItemArchetypeSocket Socket;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FName> Tags;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TArray<TSubclassOf<class UFluidInventoryItemEvent>> ItemEvents;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<AActor> Actor;
};

//Fluid Inventory Data Set
UCLASS(BlueprintType)
class FLUIDINVENTORYRUNTIME_API UFluidInventoryDataSet : public UObject
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly)
		FName Name;
	UPROPERTY(EditDefaultsOnly, meta = (displayName = "Priority (lower is better)", ClampMin = "0"))
		float Priority;
	UPROPERTY(EditDefaultsOnly)
		TMap<FName,FFluidInventoryArchetype> InventoryArchetypes;
	UPROPERTY(EditDefaultsOnly)
		TMap<FName,FFluidInventoryItemArchetype> ItemArchetypes;
	UPROPERTY(EditDefaultsOnly)
		TMap<FName,FFluidInventoryItemArchetypeList> ItemArchetypeLists;
};

//Fluid Inventory Item Event
UCLASS(Blueprintable, BlueprintType, abstract)
class FLUIDINVENTORYRUNTIME_API UFluidInventoryItemEvent : public UObject
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool OnUse(AActor* OwnerActor, int Inventory, int Item);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool OnPassConditions(AActor* OwnerActor, int Inventory, int Item);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		FName OnGetUIText();
};

//Fluid Inventory Subsystem
UCLASS(BlueprintType)
class FLUIDINVENTORYRUNTIME_API UFluidInventorySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

//Fluid Inventory : Initialize	
public:	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	bool bIsAlreadyInitialized;

//Fluid Inventory Subsystem : Data Set
public:
	
	UFUNCTION(BlueprintCallable)
		void RegisterDataSet(UFluidInventoryDataSet* DataSet);
	UFUNCTION(BlueprintCallable)
		void RegisterDataSets(TArray<UFluidInventoryDataSet*>  DataSets, bool bPrintDataSetNamesByPriority);
	UFUNCTION(BlueprintCallable)
		void PrintDataSetNamesByPriority();
	UFUNCTION(BlueprintCallable)
		FFluidInventoryArchetype GetInventoryArchetypeCopy(FName ID);
	UFUNCTION(BlueprintCallable)
		FFluidInventoryItemArchetype GetItemArchetypeCopy(FName ID);
	UFUNCTION(BlueprintCallable)
		FFluidInventoryItemArchetypeList GetItemArchetypeListCopy(FName ID);
	FFluidInventoryArchetype* GetInventoryArchetypeRef(FName ID);
	FFluidInventoryItemArchetype* GetItemArchetypeRef(FName ID);
	FFluidInventoryItemArchetypeList* GetItemArchetypeListRef(FName ID);
	
	TArray<UFluidInventoryDataSet*> RegistredDataSets;
	
//Fluid Inventory Subsystem : Inventories
public:

	UFUNCTION(BlueprintInternalUseOnly)
		void InitializeInventories();
	UFUNCTION(BlueprintCallable)
		void CreateInventory(const FName InventoryArchetypeID, int& Inventory_Out);
	UFUNCTION(BlueprintCallable)
		void DestroyInventory(const int Inventory);
	UFUNCTION(BlueprintCallable)
		void DestroyInventoryWithTag(const FName Tag, const bool bDestroyMulti,const bool bDestroyItems);
	UFUNCTION(BlueprintCallable)
		void DestroyInventoryWithTags(const TArray<FName> Tags, const bool bDestroyMulti,const bool bDestroyItems);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetInventories(TArray<int>& Inventories_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetInventoriesWithTag(const FName Tag, TArray<int>& Inventories_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetInventoriesWithTags(const TArray<FName> Tags, TArray<int>& Inventories_Out);
	UFUNCTION(BlueprintCallable)
		void AddInventoryTag(const int Inventory, const FName Tag);
	UFUNCTION(BlueprintCallable)
		void AddInventoryTags(const int Inventory, const TArray<FName> Tags);
	UFUNCTION(BlueprintCallable)
	    void RemoveInventoryTag(const int Inventory, const FName Tag);
	UFUNCTION(BlueprintCallable)
		void RemoveInventoryTags(const int Inventory, const TArray<FName> Tags);
	UFUNCTION(BlueprintCallable)
		void RemoveInventoryTagsAll(const int Inventory);
	UFUNCTION(BlueprintCallable)
		void ToggleInventoryTag(const int Inventory, const FName Tag);
	UFUNCTION(BlueprintCallable)
		void ToggleInventoryTags(const int Inventory, const TArray<FName> Tags);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void HasInventoryTag(const int Inventory, const FName Tag,bool& bHasTag);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void HasInventoryTags(const int Inventory, const TArray<FName> Tags,bool& bHasTags);
	UFUNCTION(BlueprintCallable)
		void AddInventoryActorBinding(const int Inventory, AActor* Actor);
	UFUNCTION(BlueprintCallable)
		void RemoveInventoryActorBinding(const int Inventory, AActor* Actor);
	UFUNCTION(BlueprintCallable)
		void RemoveInventoryActorBindings(const int Inventory);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetInventoryActorBindings(const int Inventory, TArray<AActor*>& Actors_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetInventoryCurrencyAmount(const int Inventory, int& CurrencyAmount_Out);
	UFUNCTION(BlueprintCallable)
		void SetInventoryCurrencyAmount(const int Inventory, const int CurrencyAmount);
	UFUNCTION(BlueprintCallable)
		void AddRemoveInventoryCurrencyAmount(const int Inventory, const int CurrencyAmount);
	UFUNCTION(BlueprintCallable)
		void TransferInventoryCurrencyAmount(const int InventoryA, const int InventoryB, const int Amount, bool const bMustHaveEnough);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetInventoryCount(int& InventoryCount_Out) { InventoryCount_Out = InventoryCount; };
	
	std::queue<int> InventoriesAvailable;
	FluidInventoryInventorySignature InventorySignatures;
	TArray<FName> InventoryTags[FLUID_INVENTORY_INVENTORIES_MAX];
	TMap<FName,int> InventoryItemLimitWithTags[FLUID_INVENTORY_INVENTORIES_MAX];
	TArray<AActor*> InventoryActorBindings[FLUID_INVENTORY_INVENTORIES_MAX];
	int InventoryItemMaxCounts[FLUID_INVENTORY_INVENTORIES_MAX];
	int InventoryItemCounts[FLUID_INVENTORY_INVENTORIES_MAX];
	int InventoryCurrencyAmounts[FLUID_INVENTORY_INVENTORIES_MAX];
	int InventoryCount;
	bool bInventoriesInitialized;
	
//Fluid Inventory Subsystem : Items
public:

	UFUNCTION(BlueprintInternalUseOnly)
		void InitializeItems();
	UFUNCTION(BlueprintCallable)
		void CreateItem(const int Inventory, const FName ItemArchetypeID, int& Item_Out);
	UFUNCTION(BlueprintCallable)
		void CreateItems(const int Inventory, const TArray<FName> ItemArchetypeIDs, TArray<int>& Items_Out);
	UFUNCTION(BlueprintCallable)
		void DestroyItem(const int Item);
	UFUNCTION(BlueprintCallable)
		void DestroyItemsByInventory(const int Inventory);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetItems(TArray<int>& Items_Out, TArray<int>& Inventories_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetItemsWithTag(const FName Tag, TArray<int>& Items_Out, TArray<int>& Inventories_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetItemsWithTags(const TArray<FName> Tags, TArray<int>& Items_Out, TArray<int>& Inventories_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetItemsByInventory(const int Inventory, TArray<int>& Items_Out, int& Inventory_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetItemsByInventoryWithTag(const FName Tag,const int Inventory, TArray<int>& Items_Out, int& Inventory_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetItemsByInventoryWithTags(const TArray<FName> Tags,const int Inventory, TArray<int>& Items_Out, int& Inventory_Out);
	UFUNCTION(BlueprintCallable)
		void AddItemTag(const int Item, const FName Tag);
	UFUNCTION(BlueprintCallable)
		void AddItemTags(const int Item, const TArray<FName> Tags);
	UFUNCTION(BlueprintCallable)
		void RemoveItemTag(const int Item, const FName Tag);
	UFUNCTION(BlueprintCallable)
		void RemoveItemTags(const int Item, const TArray<FName> Tags);
	UFUNCTION(BlueprintCallable)
		void RemoveItemTagsAll(const int Item);
	UFUNCTION(BlueprintCallable)
		void ToggleItemTag(const int Item, const FName Tag);
	UFUNCTION(BlueprintCallable)
		void ToggleItemTags(const int Item, const TArray<FName> Tags);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void HasItemTag(const int Item, const FName Tag, bool& bHasTag);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void HasItemTags(const int Item, const TArray<FName> Tags, bool& bHasTags);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetItemDataCopy(const int Item, FFluidInventoryItemArchetype& ItemArchetype_Out, TArray<FName>& ItemTags_Out, int& ItemStackAmount_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetItemHasAnyEvents(const int Item, bool& bHasEvents_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetItemCustomStatAsInt(const int Item, const FName StatName, int& Value_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetItemCustomStatAsFloat(const int Item, const FName StatName, float& Value_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetItemCustomStatAsBool(const int Item, const FName StatName, bool& Value_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetItemCustomStatAsName(const int Item, const FName StatName, FName& Value_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetItemCustomStatAsString(const int Item, const FName StatName, FString& Value_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetItemCount(int& ItemCount_Out) { ItemCount_Out = ItemCount; };
	
	std::queue<int> ItemsAvailable;
	FluidInventoryItemSignature ItemSignatures;
	TArray<FName> ItemTags[FLUID_INVENTORY_ITEMS_MAX];
	FName ItemIDs[FLUID_INVENTORY_ITEMS_MAX];
	int ItemStackAmounts[FLUID_INVENTORY_ITEMS_MAX];
	int ItemInventories[FLUID_INVENTORY_ITEMS_MAX];
	int ItemCount;
	bool bItemsInitialized;
	
//Fluid Inventory Subsystem : Pickups
public:

	UFUNCTION(BlueprintCallable)
		void CreatePickup(AActor* Actor, const FName ItemArchetypeID, const bool bIsNoneItemPickup, bool& bDidCreatePickup_Out);
	UFUNCTION(BlueprintCallable)
		void DestroyPickup(AActor* Actor, bool& bDidDestroy_Out);
	UFUNCTION(BlueprintCallable)
		void SetPickupIsPickupable(AActor* Actor, bool bIsPickupable, bool& bIsPickupable_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetPickupClosest(const FVector Location, const FVector Direction, const float MaxPickupDistance, const float MinDot, const bool bOnlyAddableToInventoryOrNoneItemPickup, AActor*& PickupActor_Out, FName& PickupItemArchetypeID_Out, float& PickupDistance_Out, bool& bFoundPickup_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetPickupClosestSphereTraced(const FVector Location,const FVector Direction, const float MaxPickupDistance, const float MinDot, const ETraceTypeQuery TraceTypeQuery, const bool bTraceComplex, const bool bOnlyAddableToInventoryOrNoneItemPickup, AActor*& PickupActor_Out, FName& PickupItemArchetypeID_Out, float& PickupDistance_Out, bool& bFoundPickup_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetPickupClosestLineTraced(const FVector Location,const FVector Direction, const float MaxPickupDistance, const ETraceTypeQuery TraceTypeQuery, const bool bTraceComplex,const bool bOnlyAddableToInventoryOrNoneItemPickup, AActor*& PickupActor_Out, FName& PickupItemArchetypeID_Out, float& PickupDistance_Out, bool& bFoundPickup_Out);
	UFUNCTION(BlueprintCallable)
		void PickupPickup(const int Inventory,AActor* PickupActor, bool& bDidPickup,int& Item_Out);

	TArray<AActor*> PickupActors;
	TArray<FName> PickupItemArchetypesIDs;
	TArray<bool> PickupPickupables;
	
//Fluid Inventory Subsystem : Miscellaneous
public:

	UFUNCTION(BlueprintCallable)
		void SpawnItemArchetypeActor(const FName ItemArchetypeID, const FTransform Transform, ESpawnActorCollisionHandlingMethod SpawnActorCollisionHandlingMethod, AActor*& ActorSpawned_Out, bool& bDidSpawn_Out);
	UFUNCTION(BlueprintCallable)
		void SpawnItemArchetypeActors(const TArray<FName> ItemArchetypeIDs, const FTransform Transform, ESpawnActorCollisionHandlingMethod SpawnActorCollisionHandlingMethod, TArray<AActor*>& ActorsSpawned_Out, bool& bDidSpawn_Out);
	UFUNCTION(BlueprintCallable)
		void GetItemArchetypeIDsFromList(FName ItemArchetypeListID, TArray<FName>& ItemArchetypeIDs_Out, FName& ItemArchetypeIDFirst_Out, int& ItemArchetypeIDsCount_Out, bool& bHasItemArchetypeIDs);
};
