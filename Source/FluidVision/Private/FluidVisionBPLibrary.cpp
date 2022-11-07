// Copyright Epic Games, Inc. All Rights Reserved.

#include "FluidVisionBPLibrary.h"
#include "FluidVision.h"

TMap<TSubclassOf<AFluidVisionEffectActor>, AFluidVisionEffectActor*> UFluidVisionBPLibrary::EffectActors;
AFluidVisionEffectActor* UFluidVisionBPLibrary::CurrentEffectActor;
APostProcessVolume* UFluidVisionBPLibrary::PostProcessVolume;

UFluidVisionBPLibrary::UFluidVisionBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

void UFluidVisionBPLibrary::InitVision()
{
	if (PostProcessVolume != nullptr)
		return;

	PostProcessVolume = nullptr;
	PostProcessVolume = (APostProcessVolume*)GEngine->GetCurrentPlayWorld()->SpawnActor(APostProcessVolume::StaticClass());
	PostProcessVolume->bUnbound = true;
}

void UFluidVisionBPLibrary::CleanupVision()
{
	PostProcessVolume = nullptr;
	CurrentEffectActor = nullptr;
	EffectActors.Empty();
}

void UFluidVisionBPLibrary::ToggleVisionEffect(TSubclassOf<AFluidVisionEffectActor> VisionEffectClass)
{
	if (CurrentEffectActor != nullptr && CurrentEffectActor->GetClass() == VisionEffectClass)
		RemoveVisionEffect(VisionEffectClass);
	else
		SetVisionEffect(VisionEffectClass);
}


void UFluidVisionBPLibrary::SetVisionEffect(TSubclassOf<AFluidVisionEffectActor> VisionEffectClass)
{
	if (PostProcessVolume == nullptr || CurrentEffectActor != nullptr && VisionEffectClass == CurrentEffectActor->GetClass())
		return;

	if (!EffectActors.Contains(VisionEffectClass))
		EffectActors.Add(VisionEffectClass, (AFluidVisionEffectActor*)GEngine->GetCurrentPlayWorld()->SpawnActor(VisionEffectClass));

	if (CurrentEffectActor != nullptr)
		CurrentEffectActor->bIsActive = false;

	CurrentEffectActor = EffectActors[VisionEffectClass];
	CurrentEffectActor->bIsActive = true;
	CurrentEffectActor->OnEffectStart(PostProcessVolume);
}

void UFluidVisionBPLibrary::RemoveVisionEffect(TSubclassOf<AFluidVisionEffectActor> VisionEffectClass)
{
	if (PostProcessVolume == nullptr)
		return;

	if (CurrentEffectActor && VisionEffectClass == CurrentEffectActor->GetClass())
	{
		CurrentEffectActor->OnEffectEnd(PostProcessVolume);
		CurrentEffectActor->bIsActive = false;
		CurrentEffectActor = nullptr;
	}
	
}

void UFluidVisionBPLibrary::CreatePostProcessVolume()
{
}


void AFluidVisionEffectActor::OnEffectStart_Implementation(APostProcessVolume* PostProcessVolume)
{
}

void AFluidVisionEffectActor::OnEffectEnd_Implementation(APostProcessVolume* PostProcessVolume)
{
}