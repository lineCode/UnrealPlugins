// Fill out your copyright notice in the Description page of Project Settings.


#include "FluidInventorySubsystem.h"

#include <map>
#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Inventory Item Event

bool UFluidInventoryItemEvent::OnUse_Implementation(AActor* OwnerActor, int InventoryIndex, int InventoryItemIndex)
{
	return false;
}

bool UFluidInventoryItemEvent::OnPassConditions_Implementation(AActor* OwnerActor, int InventoryIndex, int InventoryItemIndex)
{
	return false;
}

FName UFluidInventoryItemEvent::OnGetUIText_Implementation()
{
	return FName();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Inventory Subsystem : Initialize	

void UFluidInventorySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	//Initialize Once
	if (bIsAlreadyInitialized) return;
	InitializeInventories();
	InitializeItems();
	bIsAlreadyInitialized = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Inventory Subsystem : Data Set

void UFluidInventorySubsystem::RegisterDataSet(UFluidInventoryDataSet* DataSet)
{
	//Return If Data Set Is Not Valid
	if (DataSet == nullptr)
		return;
	
	//Add Data Set
	RegistredDataSets.AddUnique(DataSet);

	//Sort Data Sets By Priority (Bubble Sort, Lower Is Better)
	{
		int Iterations = RegistredDataSets.Num() - 1;
		while (Iterations > 0)
		{
			for (int32 x = 0; x < RegistredDataSets.Num() - 1; x++)
				if ( RegistredDataSets [x]->Priority > RegistredDataSets [x + 1]->Priority)
					RegistredDataSets.Swap(x, x + 1);
			Iterations--;
		}
	}
}

void UFluidInventorySubsystem::RegisterDataSets(TArray<UFluidInventoryDataSet*> DataSets, bool bPrintDataSetNamesByPriority)
{
	//Register Data Sets
	for (int i = 0; i < DataSets.Num(); i++)
		RegisterDataSet(DataSets[i]);

	//Print Data Set Names By Priority
	if (bPrintDataSetNamesByPriority)
		PrintDataSetNamesByPriority();
}

void UFluidInventorySubsystem::PrintDataSetNamesByPriority()
{
	//Print Data Sets By Priority (We Loop Backwards Since Print Is Displayed Backwards)
	for (int i = RegistredDataSets.Num(); i--; )
		GEngine->AddOnScreenDebugMessage(-1,5,FColor::Red,RegistredDataSets[i]->Name.ToString());
}

FFluidInventoryArchetype UFluidInventorySubsystem::GetInventoryArchetypeCopy(FName ID)
{
	auto I = GetInventoryArchetypeRef(ID);
	if (I) return *I;
	return FFluidInventoryArchetype();
}

FFluidInventoryItemArchetype UFluidInventorySubsystem::GetItemArchetypeCopy(FName ID)
{
	auto I = GetItemArchetypeRef(ID);
	if (I) return *I;
	return FFluidInventoryItemArchetype();
}

FFluidInventoryItemArchetypeList UFluidInventorySubsystem::GetItemArchetypeListCopy(FName ID)
{
	auto I = GetItemArchetypeListRef(ID);
	if (I) return *I;
	return FFluidInventoryItemArchetypeList();
}

FFluidInventoryArchetype* UFluidInventorySubsystem::GetInventoryArchetypeRef(FName ID)
{
	//Get Item List (Pointer)
	for (int i = 0; i < RegistredDataSets.Num(); i++)
		if (RegistredDataSets[i]->InventoryArchetypes.Contains(ID))
			return &RegistredDataSets[i]->InventoryArchetypes[ID];
	return nullptr;
}

FFluidInventoryItemArchetype* UFluidInventorySubsystem::GetItemArchetypeRef(FName ID)
{
	//Get Item Archetype (Pointer)
	for (int i = 0; i < RegistredDataSets.Num(); i++)
		if (RegistredDataSets[i]->ItemArchetypes.Contains(ID))
			return &RegistredDataSets[i]->ItemArchetypes[ID];
	return nullptr;
}

FFluidInventoryItemArchetypeList* UFluidInventorySubsystem::GetItemArchetypeListRef(FName ID)
{
	//Get Item List (Pointer)
	for (int i = 0; i < RegistredDataSets.Num(); i++)
		if (RegistredDataSets[i]->ItemArchetypeLists.Contains(ID))
			return &RegistredDataSets[i]->ItemArchetypeLists[ID];
	return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Inventory Subsystem : Inventories

void UFluidInventorySubsystem::InitializeInventories()
{
	//Return If Already Initalized
	if (bInventoriesInitialized) return;
	
	// Initialize With All Possible Inventories
	for (int i = 0; i < FLUID_INVENTORY_INVENTORIES_MAX; ++i)
		InventoriesAvailable.push(i);
	bInventoriesInitialized = true;
}

void UFluidInventorySubsystem::CreateInventory(const FName InventoryArchetypeID, int& Inventory_Out)
{
	//Return If We Reached Max Inventory Count
	if (InventoryCount >= FLUID_INVENTORY_INVENTORIES_MAX || GetInventoryArchetypeRef(InventoryArchetypeID) == nullptr)
	{
		Inventory_Out = -1;
		return;
	}

	//Get Inventory Archetype
	const auto InventoryArchetype = GetInventoryArchetypeRef(InventoryArchetypeID);
	if (InventoryArchetype == nullptr)
		return;
	
	//Get Inventory From Available Que | Remove It From Available Que
	Inventory_Out = InventoriesAvailable.front();
	InventoriesAvailable.pop();
	++InventoryCount;
	
	//Set Inventory Data
	InventoryCurrencyAmounts[Inventory_Out] = 0;
	InventorySignatures[Inventory_Out] = true;
	InventoryTags[Inventory_Out] = InventoryArchetype->Tags;
	InventoryItemLimitWithTags[Inventory_Out] = InventoryArchetype->ItemLimitWithTags;
	InventoryItemMaxCounts[Inventory_Out] = InventoryArchetype->MaxItemCount;
}

void UFluidInventorySubsystem::DestroyInventory(const int Inventory)
{
	//Return If Inventory Is Not Valid
	if (Inventory < 0 || Inventory >= FLUID_INVENTORY_INVENTORIES_MAX)
		return;
	
	//Add Inventory To Available Que
	InventoriesAvailable.push(Inventory);
	--InventoryCount;
	
	//Set Inventory Data Null
	InventoryCurrencyAmounts[Inventory] = 0;
	InventoryTags[Inventory].Empty();
	InventoryItemLimitWithTags[Inventory].Empty();
	InventorySignatures[Inventory] = false;
	InventoryItemMaxCounts[Inventory] = 0;

	//Destroy Items
	DestroyItemsByInventory(Inventory);
}

void UFluidInventorySubsystem::DestroyInventoryWithTag(const FName Tag, const bool bDestroyMulti, const bool bDestroyItems)
{
	//Destroy Inventory(s) With Tag
	for (int i = 0; i < FLUID_INVENTORY_INVENTORIES_MAX; i++)
		if (InventorySignatures[i])
			if (Tag == "" || InventoryTags[i].Contains(Tag))
			{
				DestroyInventory(i);
				
				//Only Destroy One Inventory
				if (!bDestroyMulti)
					return;
			}
}

void UFluidInventorySubsystem::DestroyInventoryWithTags(const TArray<FName> Tags, const bool bDestroyMulti, const bool bDestroyItems)
{
	//Get Inventories On (Signatures Equals True)
	for (int i = 0; i < FLUID_INVENTORY_INVENTORIES_MAX; i++)
	{
		if (InventorySignatures[i])
		{
			//Only Add If Inventory Has All Tags
			bool bHasAllTags = true;
			for (int j = 0; j < Tags.Num(); j++)
				if (!InventoryTags[i].Contains(Tags[j]))
					bHasAllTags = false;
			if (bHasAllTags) DestroyInventory(i);
		}
	}
}

void UFluidInventorySubsystem::GetInventories(TArray<int>& Inventories_Out)
{
	//Make Sure Empty
	Inventories_Out.Empty();

	//Get Inventories On (Signatures Equals True)
	for (int i = 0; i < FLUID_INVENTORY_INVENTORIES_MAX; i++)
		if (InventorySignatures[i])
			Inventories_Out.Add(i);
}

void UFluidInventorySubsystem::GetInventoriesWithTag(const FName Tag, TArray<int>& Inventories_Out)
{
	//Make Sure Empty
	Inventories_Out.Empty();

	//Get Inventories On (Signatures Equals True)
	for (int i = 0; i < FLUID_INVENTORY_INVENTORIES_MAX; i++)
		if (InventorySignatures[i])
			if (Tag == "" || InventoryTags[i].Contains(Tag))
				Inventories_Out.Add(i);
}

void UFluidInventorySubsystem::GetInventoriesWithTags(const TArray<FName> Tags, TArray<int>& Inventories_Out)
{
	//Make Sure Empty
	Inventories_Out.Empty();

	//Get Inventories On (Signatures Equals True)
	for (int i = 0; i < FLUID_INVENTORY_INVENTORIES_MAX; i++)
	{
		if (InventorySignatures[i])
		{
			//Only Add If Inventory Has All Tags
			bool bHasAllTags = true;
			for (int j = 0; j < Tags.Num(); j++)
				if (!InventoryTags[i].Contains(Tags[j]))
					bHasAllTags = false;
			if (bHasAllTags) Inventories_Out.Add(i);
		}
	}
}

void UFluidInventorySubsystem::AddInventoryTag(const int Inventory, const FName Tag)
{
	//Return If Inventory Is Not Valid
	if (Inventory < 0 || Inventory >= FLUID_INVENTORY_INVENTORIES_MAX)
		return;

	//Add Inventory Tag
	InventoryTags[Inventory].AddUnique(Tag);
}

void UFluidInventorySubsystem::AddInventoryTags(const int Inventory, const TArray<FName> Tags)
{
	//Return If Inventory Is Not Valid
	if (Inventory < 0 || Inventory >= FLUID_INVENTORY_INVENTORIES_MAX)
		return;

	//Add Inventory Tags
	for (int i = 0; i < Tags.Num(); i++)
		InventoryTags[Inventory].AddUnique(Tags[i]);
}

void UFluidInventorySubsystem::RemoveInventoryTag(const int Inventory, const FName Tag)
{
	//Return If Inventory Is Not Valid
	if (Inventory < 0 || Inventory >= FLUID_INVENTORY_INVENTORIES_MAX)
		return;

	//Remove Inventory Tag
	InventoryTags[Inventory].Remove(Tag);
}

void UFluidInventorySubsystem::RemoveInventoryTags(const int Inventory, const TArray<FName> Tags)
{
	//Return If Inventory Is Not Valid
	if (Inventory < 0 || Inventory >= FLUID_INVENTORY_INVENTORIES_MAX)
		return;

	//Remove Inventory Tags
	for (int i = 0; i < Tags.Num(); i++)
		InventoryTags[Inventory].Remove(Tags[i]);
}

void UFluidInventorySubsystem::RemoveInventoryTagsAll(const int Inventory)
{
	//Return If Inventory Is Not Valid
	if (Inventory < 0 || Inventory >= FLUID_INVENTORY_INVENTORIES_MAX)
		return;

	//Remove All Inventory Tags
	InventoryTags[Inventory].Empty();
}

void UFluidInventorySubsystem::ToggleInventoryTag(const int Inventory, const FName Tag)
{
	//Return If Inventory Is Not Valid
	if (Inventory < 0 || Inventory >= FLUID_INVENTORY_INVENTORIES_MAX)
		return;

	//Toggle Inventory Tag
	if (InventoryTags[Inventory].Contains(Tag))
		InventoryTags[Inventory].Remove(Tag);
	else
		InventoryTags[Inventory].Add(Tag);
}

void UFluidInventorySubsystem::ToggleInventoryTags(const int Inventory, const TArray<FName> Tags)
{
	//Return If Inventory Is Not Valid
	if (Inventory < 0 || Inventory >= FLUID_INVENTORY_INVENTORIES_MAX)
		return;

	//Toggle Inventory Tags
	for (int i = 0; i < Tags.Num(); i++)
		if (InventoryTags[Inventory].Contains(Tags[i]))
			InventoryTags[Inventory].Remove(Tags[i]);
		else
			InventoryTags[Inventory].Add(Tags[i]);
}

void UFluidInventorySubsystem::HasInventoryTag(const int Inventory, const FName Tag, bool& bHasTag)
{
	//Return If Inventory Is Not Valid
	bHasTag = false;
	if (Inventory < 0 || Inventory >= FLUID_INVENTORY_INVENTORIES_MAX)
		return;
	
	bHasTag = InventoryTags[Inventory].Contains(Tag);
}

void UFluidInventorySubsystem::HasInventoryTags(const int Inventory, const TArray<FName> Tags, bool& bHasTags)
{
	//Return If Inventory Is Not Valid
	bHasTags = false;
	if (Inventory < 0 || Inventory >= FLUID_INVENTORY_INVENTORIES_MAX)
		return;

	//Get If Has Tags
	bHasTags = true;
	for (int i = 0; i < Tags.Num(); i++)
		if (!InventoryTags[Inventory].Contains(Tags[i]))
		{
			bHasTags = false;
			return;
		}
}

void UFluidInventorySubsystem::AddInventoryActorBinding(const int Inventory, AActor* Actor)
{
	//Return If Inventory Is Not Valid
	if (Inventory < 0 || Inventory >= FLUID_INVENTORY_INVENTORIES_MAX)
		return;

	//Add Actor Binding
	InventoryActorBindings[Inventory].AddUnique(Actor);
}

void UFluidInventorySubsystem::RemoveInventoryActorBinding(const int Inventory, AActor* Actor)
{
	//Return If Inventory Is Not Valid
	if (Inventory < 0 || Inventory >= FLUID_INVENTORY_INVENTORIES_MAX)
		return;
	
	//Remove Actor Binding
	InventoryActorBindings[Inventory].Remove(Actor);
}

void UFluidInventorySubsystem::RemoveInventoryActorBindings(const int Inventory)
{
	//Return If Inventory Is Not Valid
	if (Inventory < 0 || Inventory >= FLUID_INVENTORY_INVENTORIES_MAX)
		return;

	//Remove Actor Binding
	InventoryActorBindings[Inventory].Empty();
}

void UFluidInventorySubsystem::GetInventoryActorBindings(const int Inventory, TArray<AActor*>& Actors_Out)
{
	//Return If Inventory Is Not Valid
	if (Inventory < 0 || Inventory >= FLUID_INVENTORY_INVENTORIES_MAX)
		return;

	Actors_Out = InventoryActorBindings[Inventory];
}

void UFluidInventorySubsystem::GetInventoryCurrencyAmount(const int Inventory, int& CurrencyAmount_Out)
{
	//Return If Inventory Is Not Valid
	if (Inventory < 0 || Inventory >= FLUID_INVENTORY_INVENTORIES_MAX)
	{
		CurrencyAmount_Out = 0;
		return;
	}

	//Return Currency Amount
	CurrencyAmount_Out = InventoryCurrencyAmounts[Inventory];
}

void UFluidInventorySubsystem::SetInventoryCurrencyAmount(const int Inventory, const int CurrencyAmount)
{
	//Return If Inventory Is Not Valid
	if (Inventory < 0 || Inventory >= FLUID_INVENTORY_INVENTORIES_MAX || InventorySignatures[Inventory] == false)
		return;

	//Set Currency Amount
	InventoryCurrencyAmounts[Inventory] = CurrencyAmount;
}

void UFluidInventorySubsystem::AddRemoveInventoryCurrencyAmount(const int Inventory, const int CurrencyAmount)
{
	//Return If Inventory Is Not Valid
	if (Inventory < 0 || Inventory >= FLUID_INVENTORY_INVENTORIES_MAX || InventorySignatures[Inventory] == false)
		return;

	//Set Currency Amount
	InventoryCurrencyAmounts[Inventory] += CurrencyAmount;
}

void UFluidInventorySubsystem::TransferInventoryCurrencyAmount(const int InventoryA, const int InventoryB, const int Amount, const bool bMustHaveEnough)
{
	//Return If Inventory A Is Not Valid
	if (InventoryA < 0 || InventoryA >= FLUID_INVENTORY_INVENTORIES_MAX || InventorySignatures[InventoryA] == false)
		return;

	//Return If Inventory B Is Not Valid
	if (InventoryB < 0 || InventoryB >= FLUID_INVENTORY_INVENTORIES_MAX || InventorySignatures[InventoryB] == false)
		return;

	//Return If Inventory B Has Max Item Count
	if (InventoryItemCounts[InventoryB] >= InventoryItemMaxCounts[InventoryB])
		return;

	//Return If Not Enough Currency
	if (bMustHaveEnough && InventoryCurrencyAmounts[InventoryA] - Amount < 0)
		return;

	//Transfer Currency
	InventoryCurrencyAmounts[InventoryA] -= Amount;
	InventoryCurrencyAmounts[InventoryB] += Amount;

	//Make Sure Currency Amount Is Not Negative
	if (InventoryCurrencyAmounts[InventoryA] < 0)
		InventoryCurrencyAmounts[InventoryA] = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Inventory Subsystem : Items

void UFluidInventorySubsystem::InitializeItems()
{
	//Return If Already Initialized
	if (bItemsInitialized) return;
	
	// Initialize With All Possible Items
	for (int i = 0; i < FLUID_INVENTORY_ITEMS_MAX; ++i)
		ItemsAvailable.push(i);
	bItemsInitialized = true;
}

void UFluidInventorySubsystem::CreateItem(const int Inventory, const FName ItemArchetypeID, int& Item_Out)
{
	//Null By Default
	Item_Out = -1;
	
	//Return If Inventory Is Not Valid Or If We Reached Max Items Count
	if (Inventory < 0 || Inventory >= FLUID_INVENTORY_INVENTORIES_MAX || ItemCount >= FLUID_INVENTORY_ITEMS_MAX)
		return;
	
	//Return If Item Archetype Is Not Valid
	const auto ItemArchetype = GetItemArchetypeRef(ItemArchetypeID);
	if (ItemArchetype == nullptr || !ItemArchetype->Socket.bIsAddableToInventory)
		return;
	
	//Add Item (Stacking)
	for (int i = 0; i < FLUID_INVENTORY_ITEMS_MAX; i++)
		if (ItemSignatures[i] && ItemIDs[i] == ItemArchetypeID && ItemInventories[i] == Inventory && ItemStackAmounts[i] < ItemArchetype->Stats.MaxStackAmount)
		{
			ItemStackAmounts[i] = ItemStackAmounts[i] + 1;
			Item_Out = i;
			return;
		}

	//Add Item (No Stacking) (Based On Inventory Item Count)
	if (InventoryItemCounts[Inventory] < InventoryItemMaxCounts[Inventory])
	{
		//Limit Item Amount If Inventory Max Item With Tags Contains Item Archetype ID
		for (int i = 0; i < ItemArchetype->Tags.Num(); i++)
		{
			if (InventoryItemLimitWithTags[Inventory].Contains(ItemArchetype->Tags[i]))
			{
				int ItemAmount = 0;
				const auto Limit = InventoryItemLimitWithTags[Inventory][ItemArchetype->Tags[i]];
				for (int j = 0; j < FLUID_INVENTORY_ITEMS_MAX; j++)
					if (ItemSignatures[j] && ItemInventories[j] == Inventory && ItemTags[j].Contains(ItemArchetype->Tags[i]))
						ItemAmount ++;
				if (ItemAmount >= Limit)
					return;
			}
		}
		
		//Get Item From Available Que | Remove It From Available Que
		Item_Out = ItemsAvailable.front();
		ItemsAvailable.pop();
		++ItemCount;
		
		//Set Item Data
		ItemIDs[Item_Out] = ItemArchetypeID;
		ItemTags[Item_Out] = ItemArchetype->Tags;
		ItemSignatures[Item_Out] = true;
		ItemInventories[Item_Out] = Inventory;
		ItemStackAmounts[Item_Out] = 1;

		//Increase Inventory Item Count
		InventoryItemCounts[Inventory]++;
	}
}

void UFluidInventorySubsystem::CreateItems(const int Inventory, const TArray<FName> ItemArchetypeIDs, TArray<int>& Items_Out)
{
	//Return If Inventory Is Not Valid Or If We Reached Max Items Count
	if (Inventory < 0 || Inventory >= FLUID_INVENTORY_INVENTORIES_MAX || ItemCount >= FLUID_INVENTORY_ITEMS_MAX)
		return;
	
	//Empty By Default
	Items_Out.Empty();

	//Create Items
	int ItemOut = 0;
	for (int i = 0; i < ItemArchetypeIDs.Num(); i++)
	{
		CreateItem(Inventory,ItemArchetypeIDs[i],ItemOut);
		if (ItemOut != -1)
			Items_Out.Add(ItemOut);
	}
}

void UFluidInventorySubsystem::DestroyItem(const int Item)
{
	//Return If Item Is Not Valid
	if (Item < 0 || Item >= FLUID_INVENTORY_ITEMS_MAX)
		return;
	
	//Add Item To Available Que
	ItemsAvailable.push(Item);
	--ItemCount;
	
	//Decrease Inventory Item Count
	InventoryItemCounts[ItemInventories[Item]]--;
	
	//Destroy Item Data
	ItemIDs[Item] = "";
	ItemTags[Item].Empty();
	ItemSignatures[Item] = false;
	ItemInventories[Item] = -1;
}

void UFluidInventorySubsystem::DestroyItemsByInventory(const int Inventory)
{
	//Destroy Items In Inventory
	for (int i = 0; i < FLUID_INVENTORY_ITEMS_MAX; i++)
		if (ItemInventories[i] == Inventory)
			DestroyItem(i);
}

void UFluidInventorySubsystem::GetItems(TArray<int>& Items_Out, TArray<int>& Inventories_Out)
{
	//Force Start Off Empty
	Items_Out.Empty();
	Inventories_Out.Empty();

	//Get Items | Inventory Owners
	for (int i = 0; i < FLUID_INVENTORY_ITEMS_MAX; i++)
		if (ItemSignatures[i])
		{
			Items_Out.Add(i);
			Inventories_Out.Add(ItemInventories[i]);
		}
}

void UFluidInventorySubsystem::GetItemsWithTag(const FName Tag, TArray<int>& Items_Out, TArray<int>& Inventories_Out)
{
	//Force Start Off Empty
	Items_Out.Empty();
	Inventories_Out.Empty();

	//Get Items On (Signatures Equals True)
	for (int i = 0; i < FLUID_INVENTORY_ITEMS_MAX; i++)
		if (ItemSignatures[i])
		{
			//Only Add If IItem Has All Tags
			if (ItemTags[i].Contains(Tag))
			{
				Items_Out.Add(i);
				Inventories_Out.Add(ItemInventories[i]);
			}
		}
}

void UFluidInventorySubsystem::GetItemsWithTags(const TArray<FName> Tags, TArray<int>& Items_Out, TArray<int>& Inventories_Out)
{
	//Force Start Off Empty
	Items_Out.Empty();
	Inventories_Out.Empty();

	//Get Items On (Signatures Equals True)
	for (int i = 0; i < FLUID_INVENTORY_ITEMS_MAX; i++)
	{
		if (ItemSignatures[i])
		{
			//Only Add If Item Has All Tags
			bool bHasAllTags = true;
			for (int j = 0; j < Tags.Num(); j++)
				if (!ItemTags[i].Contains(Tags[j]))
					bHasAllTags = false;
			if (bHasAllTags)
			{
				Items_Out.Add(i);
				Inventories_Out.Add(ItemInventories[i]);
			}
		}
	}
}

void UFluidInventorySubsystem::GetItemsByInventory(const int Inventory, TArray<int>& Items_Out, int& Inventory_Out)
{
	//Force Start Off Empty
	Items_Out.Empty();
	Inventory_Out = Inventory;
	
	//Get Items By Inventory
	for (int i = 0; i < FLUID_INVENTORY_ITEMS_MAX; i++)
		if (ItemSignatures[i] && ItemInventories[i] == Inventory)
			Items_Out.Add(i);
	
}

void UFluidInventorySubsystem::GetItemsByInventoryWithTag(const FName Tag, const int Inventory, TArray<int>& Items_Out, int& Inventory_Out)
{
	//Force Start Off Empty
	Items_Out.Empty();
	Inventory_Out = Inventory;

	//Get Items By Inventory
	for (int i = 0; i < FLUID_INVENTORY_ITEMS_MAX; i++)
		if (ItemSignatures[i] && ItemInventories[i] == Inventory && ItemTags[i].Contains(Tag))
			Items_Out.Add(i);
}

void UFluidInventorySubsystem::GetItemsByInventoryWithTags(const TArray<FName> Tags, const int Inventory, TArray<int>& Items_Out, int& Inventory_Out)
{
	//Force Start Off Empty
	Items_Out.Empty();
	Inventory_Out = Inventory;
	
	//Get Items By Inventory
	for (int i = 0; i < FLUID_INVENTORY_ITEMS_MAX; i++)
		if (ItemSignatures[i] && ItemInventories[i] == Inventory)
		{
			//Only Add If Item Has All Tags
			bool bHasAllTags = true;
			for (int j = 0; j < Tags.Num(); j++)
				if (!ItemTags[i].Contains(Tags[j]))
					bHasAllTags = false;
			if (bHasAllTags)
				Items_Out.Add(i);
		}
}

void UFluidInventorySubsystem::AddItemTag(const int Item, const FName Tag)
{
	//Return If Item Is Not Valid
	if (Item < 0 || Item >= FLUID_INVENTORY_ITEMS_MAX)
		return;

	//Add Item Tag
	ItemTags[Item].AddUnique(Tag);
}

void UFluidInventorySubsystem::AddItemTags(const int Item, const TArray<FName> Tags)
{
	//Return If Item Is Not Valid
	if (Item < 0 || Item >= FLUID_INVENTORY_ITEMS_MAX)
		return;

	//Add Item Tags
	for (int i = 0; i < Tags.Num(); i++)
		ItemTags[Item].AddUnique(Tags[i]);
}

void UFluidInventorySubsystem::RemoveItemTag(const int Item, const FName Tag)
{
	//Return If Item Is Not Valid
	if (Item < 0 || Item >= FLUID_INVENTORY_ITEMS_MAX)
		return;

	//Remove Item Tag
	ItemTags[Item].Remove(Tag);
}

void UFluidInventorySubsystem::RemoveItemTags(const int Item, const TArray<FName> Tags)
{
	//Return If Item Is Not Valid
	if (Item < 0 || Item >= FLUID_INVENTORY_ITEMS_MAX)
		return;
	
	//Remove Item Tags
	for (int i = 0; i < Tags.Num(); i++)
		ItemTags[Item].Remove(Tags[i]);
}

void UFluidInventorySubsystem::RemoveItemTagsAll(const int Item)
{
	//Return If Item Is Not Valid
	if (Item < 0 || Item >= FLUID_INVENTORY_ITEMS_MAX)
		return;
	
	//Remove All Item Tags
	ItemTags[Item].Empty();
}

void UFluidInventorySubsystem::ToggleItemTag(const int Item, const FName Tag)
{
	//Return If Item Is Not Valid
	if (Item < 0 || Item >= FLUID_INVENTORY_ITEMS_MAX)
		return;
	
	//Toggle Item Tag
	if (ItemTags[Item].Contains(Tag))
		ItemTags[Item].Remove(Tag);
	else
		ItemTags[Item].Add(Tag);
}

void UFluidInventorySubsystem::ToggleItemTags(const int Item, const TArray<FName> Tags)
{
	//Return If Item Is Not Valid
	if (Item < 0 || Item >= FLUID_INVENTORY_ITEMS_MAX)
		return;
	
	//Toggle Item Tags
	for (int i = 0; i < Tags.Num(); i++)
		if (ItemTags[Item].Contains(Tags[i]))
			ItemTags[Item].Remove(Tags[i]);
		else
			ItemTags[Item].Add(Tags[i]);
}

void UFluidInventorySubsystem::HasItemTag(const int Item, const FName Tag, bool& bHasTag)
{
	//Return If Inventory Is Not Valid
	bHasTag = false;
	if (Item < 0 || Item >= FLUID_INVENTORY_ITEMS_MAX)
		return;
	
	//Set Has Tag
	bHasTag = ItemTags[Item].Contains(Tag);
}

void UFluidInventorySubsystem::HasItemTags(const int Item, const TArray<FName> Tags, bool& bHasTags)
{
	//Return If Inventory Is Not Valid
	bHasTags = false;
	if (Item < 0 || Item >= FLUID_INVENTORY_ITEMS_MAX)
		return;

	//Get If Has Tags
	bHasTags = true;
	for (int i = 0; i < Tags.Num(); i++)
		if (!ItemTags[Item].Contains(Tags[i]))
		{
			bHasTags = false;
			return;
		}
}

void UFluidInventorySubsystem::GetItemDataCopy(const int Item, FFluidInventoryItemArchetype& ItemArchetype_Out, TArray<FName>& ItemTags_Out, int& ItemStackAmount_Out)
{
	//Return If Item Is Not Valid
	if (Item < 0 || Item >= FLUID_INVENTORY_ITEMS_MAX)
		return;
	
	ItemArchetype_Out = *GetItemArchetypeRef(ItemIDs[Item]);
	ItemTags_Out = ItemTags[Item];
	ItemStackAmount_Out = ItemStackAmounts[Item];
}

void UFluidInventorySubsystem::GetItemHasAnyEvents(const int Item, bool& bHasEvents_Out)
{
	//Return If Item Is Not Valid
	bHasEvents_Out = false;
	if (Item < 0 || Item >= FLUID_INVENTORY_ITEMS_MAX)
		return;
	
	//Get If Items Has Any Events
	const auto ItemArchetype = GetItemArchetypeRef(ItemIDs[Item]);
	bHasEvents_Out =  ItemArchetype != nullptr && ItemArchetype->ItemEvents.Num() > 0;
}

void UFluidInventorySubsystem::GetItemCustomStatAsInt(const int Item, const FName StatName, int& Value_Out)
{
	//Not Valid Stats
	Value_Out = -1;
	const auto ItemArchetype = GetItemArchetypeRef(ItemIDs[Item]);
	if (ItemArchetype == nullptr)
		return;

	//Get Stat
	if (ItemArchetype->Stats.CustomStats.Contains(StatName))
		Value_Out = FCString::Atoi(*ItemArchetype->Stats.CustomStats[StatName].ToString());
}

void UFluidInventorySubsystem::GetItemCustomStatAsFloat(const int Item, const FName StatName, float& Value_Out)
{
	//Not Valid Stats
	Value_Out = -1;
	const auto ItemArchetype = GetItemArchetypeRef(ItemIDs[Item]);
	if (ItemArchetype == nullptr)
		return;

	//Get Stat
	if (ItemArchetype->Stats.CustomStats.Contains(StatName))
		Value_Out = FCString::Atof(*ItemArchetype->Stats.CustomStats[StatName].ToString());
}

void UFluidInventorySubsystem::GetItemCustomStatAsBool(const int Item, const FName StatName, bool& Value_Out)
{
	//Not Valid Stats
	Value_Out = false;
	const auto ItemArchetype = GetItemArchetypeRef(ItemIDs[Item]);
	if (ItemArchetype == nullptr)
		return;

	//Get Stat
	if (ItemArchetype->Stats.CustomStats.Contains(StatName))
		Value_Out = ItemArchetype->Stats.CustomStats[StatName].ToString().ToBool();
}

void UFluidInventorySubsystem::GetItemCustomStatAsName(const int Item, const FName StatName, FName& Value_Out)
{
	//Not Valid Stats
	Value_Out = "";
	const auto ItemArchetype = GetItemArchetypeRef(ItemIDs[Item]);
	if (ItemArchetype == nullptr)
		return;

	//Get Stat
	if (ItemArchetype->Stats.CustomStats.Contains(StatName))
		Value_Out = ItemArchetype->Stats.CustomStats[StatName];
}

void UFluidInventorySubsystem::GetItemCustomStatAsString(const int Item, const FName StatName, FString& Value_Out)
{
	//Not Valid Stats
	Value_Out = "";
	const auto ItemArchetype = GetItemArchetypeRef(ItemIDs[Item]);
	if (ItemArchetype == nullptr)
		return;

	//Get Stat
	if (ItemArchetype->Stats.CustomStats.Contains(StatName))
		Value_Out = ItemArchetype->Stats.CustomStats[StatName].ToString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Inventory Subsystem : Pickups

void UFluidInventorySubsystem::CreatePickup(AActor* Actor, const FName ItemArchetypeID, const bool bIsNoneItemPickup, bool& bDidCreatePickup_Out)
{
	//Null Out By Default
	bDidCreatePickup_Out = false;

	//Not Valid
	if (Actor == nullptr || GetItemArchetypeRef(ItemArchetypeID) == nullptr && !bIsNoneItemPickup || PickupActors.Contains(Actor))
		return;

	//Add Pickup
	PickupActors.Add(Actor);
	PickupItemArchetypesIDs.Add(bIsNoneItemPickup ? "" : ItemArchetypeID);
	PickupPickupables.Add(true);
	bDidCreatePickup_Out = true;
}

void UFluidInventorySubsystem::DestroyPickup(AActor* Actor, bool& bDidDestroy_Out)
{
	//Null Out By Default
	bDidDestroy_Out = false;
	
	//Not Valid
	const int Index = PickupActors.Find(Actor);
	if (Actor == nullptr || Index == INDEX_NONE)
		return;

	//Destroy Pickup
	PickupActors.RemoveAt(Index);
	PickupItemArchetypesIDs.RemoveAt(Index);
	PickupPickupables.RemoveAt(Index);
	Actor->K2_DestroyActor();
	bDidDestroy_Out = true;
}

void UFluidInventorySubsystem::SetPickupIsPickupable(AActor* Actor, bool bIsPickupable, bool& bIsPickupable_Out)
{
	//Pass Is Pickupable
	bIsPickupable_Out = bIsPickupable;

	//Not Valid
	const int Index = PickupActors.Find(Actor);
	if (Actor == nullptr || Index == INDEX_NONE)
		return;

	//Set Is Pickupable
	PickupPickupables[Index] = bIsPickupable;
}

void UFluidInventorySubsystem::GetPickupClosest(const FVector Location,const FVector Direction, const float MaxPickupDistance, const float MinDot, const bool bOnlyAddableToInventoryOrNoneItemPickup, AActor*& PickupActor_Out, FName& PickupItemArchetypeID_Out, float& PickupDistance_Out, bool& bFoundPickup_Out)
{
	//Null Out By Default
	PickupActor_Out = nullptr;
	PickupItemArchetypeID_Out = "";
	PickupDistance_Out = -1;
	bFoundPickup_Out = false;

	//Get Closest Pickup Actor
	float ClosestPickupDistance = -1;
	float ClosestPickupDot = -1;
	float NewPickupDistance = -1;
	float NewPickupDot = -1;
	FVector DotOffset = FVector::ZeroVector;
	
	for (int i = 0; i < PickupActors.Num(); i++)
	{
		//Ignore If Not Pickupable
		if (!PickupPickupables[i])
			continue;
		
		//Ignore If Not Addable To Inventory (Item Archetype) And Is Not A None Item Pickup
		if (bOnlyAddableToInventoryOrNoneItemPickup && PickupItemArchetypesIDs[i] != "")
		{
			const auto ItemArchetype = GetItemArchetypeRef(PickupItemArchetypesIDs[i]);
			if (ItemArchetype == nullptr || !ItemArchetype->Socket.bIsAddableToInventory)
				continue;
		}
		
		//Get Distance
		NewPickupDistance = FVector::Dist(Location,PickupActors[i]->GetActorLocation());

		//Get Dot
		DotOffset =  PickupActors[i]->GetActorLocation() - Location;
		DotOffset = DotOffset.GetSafeNormal();
		NewPickupDot = FVector::DotProduct(Direction, DotOffset);

		//Set Closest Pickup
		if (ClosestPickupDistance == - 1 && NewPickupDistance < MaxPickupDistance && NewPickupDot >= MinDot || NewPickupDistance < ClosestPickupDistance && NewPickupDistance < MaxPickupDistance && NewPickupDot > ClosestPickupDot && NewPickupDot >= MinDot)
		{
			PickupActor_Out = PickupActors[i];
			ClosestPickupDistance = NewPickupDistance;
			ClosestPickupDot = NewPickupDot;
		}
	}

	//Return Data If Closest Pickup Is Valid
	if (ClosestPickupDistance != -1)
	{
		const int Index = PickupActors.Find(PickupActor_Out);
		PickupItemArchetypeID_Out = PickupItemArchetypesIDs[Index];
		PickupDistance_Out = ClosestPickupDistance;
		bFoundPickup_Out = true;
	}
}

void UFluidInventorySubsystem::GetPickupClosestSphereTraced(const FVector Location, const FVector Direction, const float MaxPickupDistance, const float MinDot, const ETraceTypeQuery TraceTypeQuery, const bool bTraceComplex, const bool bOnlyAddableToInventoryOrNoneItemPickup, AActor*& PickupActor_Out, FName& PickupItemArchetypeID_Out, float& PickupDistance_Out, bool& bFoundPickup_Out)
{
	//Null Out By Default
	PickupActor_Out = nullptr;
	PickupItemArchetypeID_Out = "";
	PickupDistance_Out = -1;
	bFoundPickup_Out = false;
	
	//Do Hit Trace
	TArray<AActor*> ActorsToIgnoire;
	TArray<FHitResult> HitResults;
	const bool bDidHit = UKismetSystemLibrary::SphereTraceMulti(GetWorld(),Location,Location,MaxPickupDistance,TraceTypeQuery, bTraceComplex, ActorsToIgnoire, EDrawDebugTrace::None, HitResults, false, FLinearColor::Red, FLinearColor::Red, 0);

	//Get Closest Pickup Actor
	int PickupIndex = INDEX_NONE;
	float ClosestPickupDistance = -1;
	float ClosestPickupDot = -1;
	float NewPickupDot = -1;
	FVector DotOffset = FVector::ZeroVector;
	
	for (int i = 0; i < HitResults.Num(); i++)
	{
		//Get Dot
		DotOffset =  PickupActors[i]->GetActorLocation() - Location;
		DotOffset = DotOffset.GetSafeNormal();
		NewPickupDot = FVector::DotProduct(Direction, DotOffset);

		PickupIndex = PickupActors.Find(HitResults[i].GetActor());
		if (PickupIndex != INDEX_NONE)
		{
			//Ignore If Not Pickupable
			if (!PickupPickupables[PickupIndex])
				continue;
			
			if (ClosestPickupDistance == -1 && NewPickupDot >= ClosestPickupDot || HitResults[i].Distance < ClosestPickupDistance && NewPickupDot >= ClosestPickupDot)
			{
				PickupActor_Out = HitResults[i].GetActor();
				ClosestPickupDistance = HitResults[i].Distance;
				ClosestPickupDot = NewPickupDot;
			}
		}
	}

	//Return Data If Closest Pickup Is Valid
	if (ClosestPickupDistance != -1)
	{
		//Get Index
		PickupIndex = PickupActors.Find(PickupActor_Out);
		
		//Ignore If Not Addable To Inventory (Item Archetype) And Is Not A None Item Pickup
		if (bOnlyAddableToInventoryOrNoneItemPickup && PickupItemArchetypesIDs[PickupIndex] != "")
		{
			const auto ItemArchetype = GetItemArchetypeRef(PickupItemArchetypesIDs[PickupIndex]);
			if (ItemArchetype == nullptr || !ItemArchetype->Socket.bIsAddableToInventory)
				return;
		}

		//Set Return Data
		PickupItemArchetypeID_Out = PickupItemArchetypesIDs[PickupIndex];
		PickupDistance_Out = ClosestPickupDistance;
		bFoundPickup_Out = true;
	}
}

void UFluidInventorySubsystem::GetPickupClosestLineTraced(const FVector Location, const FVector Direction, const float MaxPickupDistance, const ETraceTypeQuery TraceTypeQuery, const bool bTraceComplex, const bool bOnlyAddableToInventoryOrNoneItemPickup, AActor*& PickupActor_Out, FName& PickupItemArchetypeID_Out, float& PickupDistance_Out, bool& bFoundPickup_Out)
{
	//Null Out By Default
	PickupActor_Out = nullptr;
	PickupItemArchetypeID_Out = "";
	PickupDistance_Out = -1;
	bFoundPickup_Out = false;

	//Do Hit Trace
	TArray<AActor*> ActorsToIgnoire;
	FHitResult HitResult;
	const bool bDidHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(),Location,Location + Direction * MaxPickupDistance,TraceTypeQuery, bTraceComplex, ActorsToIgnoire, EDrawDebugTrace::None, HitResult, false, FLinearColor::Red, FLinearColor::Red, 0);
	
	//Return Data If Hit Is Valid
	const int PickupIndex = PickupActors.Find(HitResult.GetActor());
	if (bDidHit && PickupIndex != INDEX_NONE)
	{
		//Ignore If Not Pickupable
		if (!PickupPickupables[PickupIndex])
			return;
		
		//Ignore If Not Addable To Inventory (Item Archetype) And Is Not A None Item Pickup
		if (bOnlyAddableToInventoryOrNoneItemPickup && PickupItemArchetypesIDs[PickupIndex] != "")
		{
			const auto ItemArchetype = GetItemArchetypeRef(PickupItemArchetypesIDs[PickupIndex]);
			if (ItemArchetype == nullptr || !ItemArchetype->Socket.bIsAddableToInventory)
				return;
		}

		//Set Return Data
		PickupActor_Out = PickupActors[PickupIndex];
		PickupItemArchetypeID_Out = PickupItemArchetypesIDs[PickupIndex];
		PickupDistance_Out = HitResult.Distance;
		bFoundPickup_Out = true;
	}
}

void UFluidInventorySubsystem::PickupPickup(const int Inventory, AActor* PickupActor, bool& bDidPickup, int& Item_Out)
{
	//Null Out By Default
	bDidPickup = false;

	//Not A Pickup
	if (PickupActor == nullptr || !PickupActors.Contains(PickupActor))
		return;

	//GetPickup
	const int PickupIndex = PickupActors.Find(PickupActor);
	const FName PickupItemArchetypeID = PickupItemArchetypesIDs[PickupIndex];
	
	//Create Item
	CreateItem(Inventory,PickupItemArchetypeID,Item_Out);
	
	//Destroy Pickup If Valid Item Pickup Or None Item Pickup
	if (Item_Out != -1 || PickupItemArchetypeID == "")
	{
		bool bDidDestroy = false;
		DestroyPickup(PickupActor,bDidDestroy);
		bDidPickup = true;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Inventory Subsystem : Miscellaneous

void UFluidInventorySubsystem::SpawnItemArchetypeActor(const FName ItemArchetypeID, const FTransform Transform, ESpawnActorCollisionHandlingMethod SpawnActorCollisionHandlingMethod, AActor*& ActorSpawned_Out, bool& bDidSpawn_Out)
{
	//Null Out By Default
	ActorSpawned_Out = nullptr;
	bDidSpawn_Out = false;

	//Get Item Archetype
	const auto ItemArchetype = GetItemArchetypeRef(ItemArchetypeID);
	
	//Not Valid
	if (ItemArchetype == nullptr || ItemArchetype->Actor == nullptr)
		return;

	//Spawn Actor
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = SpawnActorCollisionHandlingMethod;
	ActorSpawned_Out = GetWorld()->SpawnActor<AActor>(ItemArchetype->Actor,Transform,SpawnInfo);
	bDidSpawn_Out = ActorSpawned_Out != nullptr;
}

void UFluidInventorySubsystem::SpawnItemArchetypeActors(const TArray<FName> ItemArchetypeIDs, const FTransform Transform, ESpawnActorCollisionHandlingMethod SpawnActorCollisionHandlingMethod, TArray<AActor*>& ActorsSpawned_Out, bool& bDidSpawn_Out)
{
	//Null Out By Default
	ActorsSpawned_Out.Empty();
	bDidSpawn_Out = false;

	AActor* SpawnedActor = nullptr;
	bool bDidSpawnActor = false;
	for (int i = 0; i < ItemArchetypeIDs.Num(); i++)
	{
		SpawnItemArchetypeActor(ItemArchetypeIDs[i],Transform,SpawnActorCollisionHandlingMethod,SpawnedActor,bDidSpawnActor);
		if (bDidSpawnActor)
		{
			ActorsSpawned_Out.Add(SpawnedActor);
			bDidSpawn_Out = true;
		}
	}
}

void UFluidInventorySubsystem::GetItemArchetypeIDsFromList(FName ItemArchetypeListID, TArray<FName>& ItemArchetypeIDs_Out, FName& ItemArchetypeIDFirst_Out, int& ItemArchetypeIDsCount_Out, bool& bHasItemArchetypeIDs)
{
	//Force Clean Slate
	ItemArchetypeIDs_Out.Empty();
	ItemArchetypeIDFirst_Out = "";
	bHasItemArchetypeIDs = false;
	
	//Get Item Archetype List | Return If Not Found
	const auto ItemArchetypeList = GetItemArchetypeListRef(ItemArchetypeListID);
	if (ItemArchetypeList == nullptr) return;
	
	//Absolute
	{
		const auto Items = ItemArchetypeList->AbsoluteItems;

		for (int i = 0; i < Items.Num(); i++)
			for (int j = 0; j < Items[i].Quantity; j++)
				ItemArchetypeIDs_Out.Add(Items[i].ItemArchetypeID);
	}

	//Chance 
	{
		//Get Data
		const auto Items = ItemArchetypeList->ChanceItems;
		
		//Get Item Archetypes By Chance
		for (int i = 0; i < Items.Num(); i++)
			for (int j = 0; j < FMath::RandRange(Items[i].QuantityMin,Items[i].QuantityMax) ; j++)
				if (FMath::RandRange(0,100) <= Items[i].Chance)
					ItemArchetypeIDs_Out.Add(Items[i].ItemArchetypeID);
	}

	//Weighted
	{
		//Get Data
		const auto Items = ItemArchetypeList->WeightedItems;

		//Chance To Get None
		if (FMath::RandRange(0,100) < ItemArchetypeList->WeightedItemsNoneChance)
			return;
	
		//Get Some
		{
			//Convert To Standard C++ Value Pair | Get Max Weight
			float MaxWeight = 0;
			std::map<float, FName> Cumulative;
			for (int i = 0; i < Items.Num(); i++)
			{
				Cumulative.insert(std::make_pair(Items[i].Weight, Items[i].ItemArchetypeID));
				if (Items[i].Weight > MaxWeight)
					MaxWeight = Items[i].Weight;
			}

			//Get Item Archetypes By Weight
			float Linear = 0;
			const int Count = FMath::RandRange(ItemArchetypeList->WeightedItemsQuantityMin,ItemArchetypeList->WeightedItemsQuantityMax);
			for (int i = 0; i < Count; i++)
			{
				Linear = rand() * MaxWeight / RAND_MAX;
				ItemArchetypeIDs_Out.Add(Cumulative.upper_bound(Linear)->second);
			}
		}
	}

	//Set First Item And Has Items
	if (ItemArchetypeIDs_Out.Num() > 0)
	{
		ItemArchetypeIDFirst_Out = ItemArchetypeIDs_Out[0];
		bHasItemArchetypeIDs = true;
	}
}