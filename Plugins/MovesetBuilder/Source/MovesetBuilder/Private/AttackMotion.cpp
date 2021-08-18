// Fill out your copyright notice in the Description page of Project Settings.

#include "AttackMotion.h"

#include "Animation/AnimMontage.h"
#include "AttackInputTypeDataAsset.h"

void UAttackMotion::SetMovesetComponent(UMovesetComponent* Moveset)
{
	MovesetComponent = Moveset;
}

TSubclassOf<UAttackMotion> UAttackMotion::AttackInput_Implementation(UAttackInputTypeDataAsset* AttackType)
{
	return nullptr;
}

void UAttackMotion::PreStartAnim_Implementation()
{
	SetReadyToInterrupt(false);
}

bool UAttackMotion::IsReadyToInterrupt() const
{
	return bReadyToInterruptAnim;
}

void UAttackMotion::SetReadyToInterrupt(bool ReadyToInterrupt)
{
	if (bReadyToInterruptAnim != ReadyToInterrupt)
	{
		bReadyToInterruptAnim = ReadyToInterrupt;
		if (bReadyToInterruptAnim && AttackMotionReadyToInterrupt.IsBound())
		{
			AttackMotionReadyToInterrupt.Broadcast();
		}
	}
}

UAnimMontage* UAttackMotion::GetAnim() const
{
	return Anim;
}
