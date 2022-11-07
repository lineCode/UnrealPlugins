// Fill out your copyright notice in the Description page of Project Settings.


#include "FluidMenuItemBaseUserWidget.h"

void UFluidMenuItemBaseUserWidget::OnSetData_Implementation(UFluidMenuBaseUserWidget* ParentMenuWidget_in, FName Data_in, FName ActionID_in)
{
	ActionID = ActionID_in;
	ParentMenuWidget = ParentMenuWidget_in;
}

void UFluidMenuItemBaseUserWidget::OnSetIsSelected_Implementation(bool bIsSelected_in, bool bIsBeingInitalized_in)
{
	bIsSelected = bIsSelected_in;
	bIsBeingInitalized = bIsBeingInitalized_in;

}

void UFluidMenuItemBaseUserWidget::DoGetActionIDS_Implementation(FName& ID_out, FName& SubID_out)
{
	ID_out = ActionID;
}

void UFluidMenuItemBaseUserWidget::OnHorizontalInput_Implementation(int Axis)
{
}