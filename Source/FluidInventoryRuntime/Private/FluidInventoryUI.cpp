#include "FluidInventoryUI.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Inventory Grid Widget

void UFluidInventoryItemSetWidget::Setup(UFluidInventorySubsystem* FluidInventorySubsystem, UWrapBox* WrapBoxToUse, TSubclassOf<UFluidInventoryItemSetItemWidget> InventoryItemSetItemWidgetClass)
{
	WrapBox = WrapBoxToUse;
	FluidInventorySubsystemInternal = FluidInventorySubsystem;
	InventoryItemSetItemWidgetClassInternal = InventoryItemSetItemWidgetClass;
}

void UFluidInventoryItemSetWidget::Update(int LocalPlayerIndex,bool bCaptureKeyboardWadsKeys,bool bCaptureKeyboardDirectionKeys, bool bCaptureGamePadLeftStick, bool bCaptureGamePadDPad, bool bIsInputEnabled)
{
	//Get Item Width From Class
	float ItemHeight = 0;
	float ItemWidth = 0;
	const auto ItemWidget = WidgetTree->ConstructWidget<UFluidInventoryItemSetItemWidget>(InventoryItemSetItemWidgetClassInternal);
	ItemWidget->GetWidthHeight(ItemWidth,ItemHeight);
	ItemWidget->ConditionalBeginDestroy();

	//Set Default
	int CursorInputX = 0;
	int CursorInputY = 0;
	
	//Cursor Start
	{
		//Get Cursor Position Max
		CursorPositionMax.X =  (int)((WrapBox->GetDesiredSize().X / ItemWidth) - 1);
		CursorPositionMax.Y =  (int)((WrapBox->GetDesiredSize().Y / ItemHeight) - 1);
	
		//Pre Clamp Cursor Position (Fix For Cursor Position Staring Out Of Bounds)
		CursorPosition.X = FMath::Clamp(CursorPosition.X,0.0f,CursorPositionMax.X);
		CursorPosition.Y = FMath::Clamp(CursorPosition.Y,0.0f,CursorPositionMax.Y);
	}

	//Cursor Input
	{
		//Get Player Controller
		LocalPlayerIndex = FMath::Clamp(LocalPlayerIndex, 0, 7);
		const auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(),LocalPlayerIndex);
		if (PlayerController != nullptr || !bIsInputEnabled)
		{
			//WADS
			if (bCaptureKeyboardWadsKeys)
			{
				if (PlayerController->WasInputKeyJustPressed(EKeys::W)) CursorInputY = -1;
				if (PlayerController->WasInputKeyJustPressed(EKeys::S)) CursorInputY = 1;
				if (PlayerController->WasInputKeyJustPressed(EKeys::A)) CursorInputX = -1;
				if (PlayerController->WasInputKeyJustPressed(EKeys::D)) CursorInputX = 1;
			}
	
			//Arrow
			if (bCaptureKeyboardDirectionKeys)
			{
				if (PlayerController->WasInputKeyJustPressed(EKeys::Up)) CursorInputY = -1;
				if (PlayerController->WasInputKeyJustPressed(EKeys::Down)) CursorInputY = 1;
				if (PlayerController->WasInputKeyJustPressed(EKeys::Left)) CursorInputX = -1;
				if (PlayerController->WasInputKeyJustPressed(EKeys::Right)) CursorInputX = 1;
			}

			//GamePad Left Stick
			if (bCaptureGamePadLeftStick)
			{
				if (PlayerController->WasInputKeyJustPressed(EKeys::Gamepad_LeftStick_Up)) CursorInputY = -1;
				if (PlayerController->WasInputKeyJustPressed(EKeys::Gamepad_LeftStick_Down)) CursorInputY = 1;
				if (PlayerController->WasInputKeyJustPressed(EKeys::Gamepad_LeftStick_Left)) CursorInputX = -1;
				if (PlayerController->WasInputKeyJustPressed(EKeys::Gamepad_LeftStick_Right)) CursorInputX = 1;
			}
			
			//GamePad DPad
			if (bCaptureGamePadDPad)
			{
				if (PlayerController->WasInputKeyJustPressed(EKeys::Gamepad_DPad_Up)) CursorInputY = -1;
				if (PlayerController->WasInputKeyJustPressed(EKeys::Gamepad_DPad_Down)) CursorInputY = 1;
				if (PlayerController->WasInputKeyJustPressed(EKeys::Gamepad_DPad_Left)) CursorInputX = -1;
				if (PlayerController->WasInputKeyJustPressed(EKeys::Gamepad_DPad_Right)) CursorInputX = 1;
			}
		}
	}

	//Cursor End
	{
		//Cursor Input
		CursorPosition.X += CursorInputX;
		CursorPosition.Y += CursorInputY;
		CursorPosition.X = FMath::Clamp(CursorPosition.X,0.0f,CursorPositionMax.X);
		CursorPosition.Y = FMath::Clamp(CursorPosition.Y,0.0f,CursorPositionMax.Y);

		//Do Cursor Input
		if (CursorPosition != CursorPositionPrevious)
		{
			CursorPosition1D = (CursorPositionMax.X * CursorPosition.Y) + CursorPosition.X;
			CursorPosition1D += CursorPosition.Y;
			CursorPosition1DPrevious = CursorPosition1D;
			CursorPositionPrevious = CursorPosition;
		}	
	}
	
	//Update Item Widgets
	for (int i = 0; i < ItemSetItemWidgets.Num(); i++)
		ItemSetItemWidgets[i]->OnUpdate();
}

void UFluidInventoryItemSetWidget::Build(const int Inventory)
{
	//Return If Not Valid
	if (FluidInventorySubsystemInternal == nullptr || !FluidInventorySubsystemInternal->InventorySignatures[Inventory] || WrapBox == nullptr || !IsValid(InventoryItemSetItemWidgetClassInternal) || Inventory == -1 || Inventory >= FLUID_INVENTORY_INVENTORIES_MAX)
		return;

	//Inventory Out (Required By Function But We Don't Need It)
	int InventoryOut;
	
	//Get Items
	if (ItemTags.Num() != 0)
		FluidInventorySubsystemInternal->GetItemsByInventoryWithTags(ItemTags,Inventory,Items,InventoryOut);
	else
		FluidInventorySubsystemInternal->GetItemsByInventory(Inventory,Items,InventoryOut);
	
	//Empty Wrap Box
	WrapBox->ClearChildren();
	ItemSetItemWidgets.Empty();
	
	//Add Empty Items (Based On Max Inventory Count)
	for (int i = 0; i < FluidInventorySubsystemInternal->InventoryItemMaxCounts[Inventory]; i++)
	{
		const auto ItemWidget = WidgetTree->ConstructWidget<UFluidInventoryItemSetItemWidget>(InventoryItemSetItemWidgetClassInternal);
		WrapBox->AddChildToWrapBox(ItemWidget);
		ItemWidget->OnBuildEmpty();
		ItemSetItemWidgets.Add(ItemWidget);
		ItemSetItemWidgets[i]->Item = -1;
		ItemSetItemWidgets[i]->ItemSetWidget = this;
		ItemSetItemWidgets[i]->IndexInItemSet = i;
	}

	//Add Items
	for (int i = 0; i < Items.Num() && i < ItemSetItemWidgets.Num(); i++)
	{
		ItemSetItemWidgets[i]->Item = Items[i];
		ItemSetItemWidgets[i]->OnBuild();
	}
}

void UFluidInventoryItemSetWidget::GetHoveredItem(int& Item_Out)
{
	if (CursorPosition1D < Items.Num())
		Item_Out = Items[CursorPosition1D];
	else Item_Out = -1;
}

void UFluidInventoryItemSetWidget::GetHoveredItemPrevious(int& Item_Out)
{
	if (CursorPosition1DPrevious < Items.Num())
		Item_Out = Items[CursorPosition1DPrevious];
	else Item_Out = -1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Inventory Action Menu Widget

void UFluidInventoryActionMenuWidget::Setup(UFluidInventorySubsystem* FluidInventorySubsystem, UWrapBox* WrapBoxToUse, TSubclassOf<UFluidInventoryActionMenuItemWidget> ActionMenuItemWidgetClass)
{
	WrapBox = WrapBoxToUse;
	FluidInventorySubsystemInternal = FluidInventorySubsystem;
	ActionMenuItemWidgetClassInternal = ActionMenuItemWidgetClass;
}

void UFluidInventoryActionMenuWidget::Update(int LocalPlayerIndex,bool bCaptureKeyboardWadsKeys,bool bCaptureKeyboardDirectionKeys, bool bCaptureGamePadLeftStick, bool bCaptureGamePadDPad, bool bIsInputEnabled)
{
	//Cursor Input Start
	{
		//Get Cursor Position Max
		CursorPositionMax =  ActionMenuItemWidgets.Num() - 1;
	
		//Pre Clamp Cursor Position (Fix For Cursor Position Staring Out Of Bounds)
		CursorPosition = FMath::Clamp(CursorPosition,0,CursorPositionMax);
	}
	
	//Cursor Input
	int Input = 0;
	{
		//Get Player Controller
		LocalPlayerIndex = FMath::Clamp(LocalPlayerIndex, 0, 7);
		const auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(),LocalPlayerIndex);
		if (PlayerController != nullptr || !bIsInputEnabled)
		{
			//WADS
			if (bCaptureKeyboardWadsKeys)
			{
				if (PlayerController->WasInputKeyJustPressed(EKeys::W)) Input = -1;
				if (PlayerController->WasInputKeyJustPressed(EKeys::S)) Input = 1;
			}
	
			//Arrow
			if (bCaptureKeyboardDirectionKeys)
			{
				if (PlayerController->WasInputKeyJustPressed(EKeys::Up)) Input = -1;
				if (PlayerController->WasInputKeyJustPressed(EKeys::Down)) Input = 1;
			}

			//GamePad Left Stick
			if (bCaptureGamePadLeftStick)
			{
				if (PlayerController->WasInputKeyJustPressed(EKeys::Gamepad_LeftStick_Up)) Input = -1;
				if (PlayerController->WasInputKeyJustPressed(EKeys::Gamepad_LeftStick_Down)) Input = 1;
			}
			
			//GamePad DPad
			if (bCaptureGamePadDPad)
			{
				if (PlayerController->WasInputKeyJustPressed(EKeys::Gamepad_DPad_Up)) Input = -1;
				if (PlayerController->WasInputKeyJustPressed(EKeys::Gamepad_DPad_Down)) Input = 1;
			}

			CursorPosition += Input;
		}

		//Cursor Input End
		{
			//Pre Clamp Cursor Position (Fix For Cursor Position Staring Out Of Bounds)
			CursorPosition = FMath::Clamp(CursorPosition,0,CursorPositionMax);

			//Do Cursor Input
			if (CursorPosition != CursorPositionPrevious)
				CursorPositionPrevious = CursorPosition;
		}
	}
}

void UFluidInventoryActionMenuWidget::Build(const int Item)
{
	//Empty Wrap Box
	WrapBox->ClearChildren();
	ActionMenuItemWidgets.Empty();
	
	//Return If Not Valid
	if (FluidInventorySubsystemInternal == nullptr || Item < 0 || Item >= FLUID_INVENTORY_ITEMS_MAX)
		return;

	//Get Item Data
	FFluidInventoryItemArchetype ItemArchetype;
	TArray<FName> ItemTags;
	int StackAmount;
	FluidInventorySubsystemInternal->GetItemDataCopy(Item,ItemArchetype,ItemTags,StackAmount);
	
	//Add Action Items
	for (int i = 0; i < ItemArchetype.ItemEvents.Num(); i++)
	{
		const auto ItemWidget = WidgetTree->ConstructWidget<UFluidInventoryActionMenuItemWidget>(ActionMenuItemWidgetClassInternal);
		WrapBox->AddChildToWrapBox(ItemWidget);
		ActionMenuItemWidgets.Add(ItemWidget);
		ActionMenuItemWidgets[i]->OnBuild(ItemArchetype.ItemEvents[i].GetDefaultObject()->OnGetUIText());
		ActionMenuItemWidgets[i]->ActionMenuWidget = this;
		ActionMenuItemWidgets[i]->IndexActionMenu = i;
	}

	//Start With 0 Cursor Position
	CursorPosition = 0;
}
