// Fill out your copyright notice in the Description page of Project Settings.

#include "MovesetComponent.h"

#include "AttackMotion.h"

DEFINE_LOG_CATEGORY_STATIC(LogMoveset, Warning, Warning)

UMovesetComponent::UMovesetComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMovesetComponent::SetSkeletalMeshComponent(USkeletalMeshComponent* Skeletal)
{
	SkeletalMeshComponent = IsValid(Skeletal) && IsValid(Skeletal->GetAnimInstance()) ? Skeletal : nullptr;
	MotionCache.Empty();
	ResetCurrentMotion();
}

void UMovesetComponent::AttackInput(UAttackInputTypeDataAsset* AttackType)
{
	if (SetNextMotionByAttackType(AttackType))
	{
		UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();
		UAnimMontage* Montage = CurrentMotion->GetAnim();
		CurrentMotion->PreStartAnim();
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_SetEndDelegate(MontageEnded, Montage);
		if (AnimStarted.IsBound())
			AnimStarted.Broadcast();
	}
}

bool UMovesetComponent::CanPlayNextMotion() const
{
	return IsValid(CurrentMotion) && (CurrentMotion->IsReadyToInterrupt() || !AttackMotionIsPlaying());
}

bool UMovesetComponent::AttackMotionIsPlaying() const
{
	if (!IsValid(SkeletalMeshComponent) || !IsValid(CurrentMotion) || !IsValid(CurrentMotion->GetAnim()))
		return false;
	return SkeletalMeshComponent->GetAnimInstance()->Montage_IsPlaying(CurrentMotion->GetAnim());
}

UAttackMotion* UMovesetComponent::GetCurrentMotion() const
{
	return CurrentMotion;
}

UAttackMotion* UMovesetComponent::GetPrevMotion() const
{
	return PrevMotion;
}

void UMovesetComponent::BeginPlay()
{
	Super::BeginPlay();
	MontageEnded.BindUObject(this, &UMovesetComponent::OnMontageEnded);
}

void UMovesetComponent::ResetCurrentMotion()
{
	UAttackMotion* HeadMotion = nullptr;
	if (IsValid(HeadAttackMotionClass))
		HeadMotion = FindOrCreateMotion(HeadAttackMotionClass);
	else
		UE_LOG(LogMoveset, Warning, TEXT("Head Attack Motion Class of %s.%s didn't set!"), *GetOwner()->GetName(), *GetName());
	SetCurrentMotion(HeadMotion);
	PrevMotion = CurrentMotion;
}

UAttackMotion* UMovesetComponent::FindOrCreateMotion(TSubclassOf<UAttackMotion> MotionClass)
{
	if (UAttackMotion** Motion = MotionCache.Find(MotionClass))
		return *Motion;

	UAttackMotion* Motion = NewObject<UAttackMotion>(this, MotionClass);
	Motion->SetMovesetComponent(this);
	MotionCache.Add(MotionClass, Motion);
	return Motion;
}

void UMovesetComponent::SetCurrentMotion(UAttackMotion* Motion)
{
	PrevMotion = CurrentMotion;
	CurrentMotion = Motion;
	if (PrevMotion)
		PrevMotion->AttackMotionReadyToInterrupt.RemoveDynamic(this, &UMovesetComponent::OnCurrentAttackMotionReadyToInterrupt);
	if (CurrentMotion)
		CurrentMotion->AttackMotionReadyToInterrupt.AddDynamic(this, &UMovesetComponent::OnCurrentAttackMotionReadyToInterrupt);
}

bool UMovesetComponent::SetNextMotionByAttackType(UAttackInputTypeDataAsset* AttackType)
{
	if (!IsValid(SkeletalMeshComponent))
	{
		UE_LOG(LogMoveset, Warning, TEXT("SkeletalMesh or AnimInstance for %s.%s didn't set!"), *GetOwner()->GetName(), *GetName());
		return false;
	}
	if (!CanPlayNextMotion())
		return false;
	TSubclassOf<UAttackMotion> MotionClass = CurrentMotion->AttackInput(AttackType);
	if (!IsValid(MotionClass))
		return false;
	UAttackMotion* NewMotion = FindOrCreateMotion(MotionClass);
	if (!IsValid(NewMotion->GetAnim()))
	{
		UE_LOG(LogMoveset, Warning, TEXT("Animation for %s didn't set!"), *NewMotion->GetName());
		return false;
	}
	SetCurrentMotion(NewMotion);
	return true;
}

void UMovesetComponent::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!IsValid(CurrentMotion) || !IsValid(PrevMotion))
		return;

	if (bInterrupted)
	{
		if (AnimInterrupted.IsBound() && PrevMotion->GetAnim() == Montage)
			AnimInterrupted.Broadcast();
	}
	else if (CurrentMotion->GetAnim() == Montage)
	{
		if (AnimCompleted.IsBound())
			AnimCompleted.Broadcast();
		ResetCurrentMotion();
	}
}

void UMovesetComponent::OnCurrentAttackMotionReadyToInterrupt()
{
	if (AttackMotionReadyToInterrupt.IsBound())
		AttackMotionReadyToInterrupt.Broadcast();
}
