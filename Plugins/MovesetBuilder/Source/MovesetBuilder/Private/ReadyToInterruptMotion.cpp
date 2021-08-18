// Fill out your copyright notice in the Description page of Project Settings.

#include "ReadyToInterruptMotion.h"

#include "AttackMotion.h"
#include "MovesetComponent.h"
#include "MovesetComponentProxy.h"

void UReadyToInterruptMotion::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	auto* Owner = MeshComp->GetOwner();
	if (!Owner)
		return;
	UMovesetComponent* MovesetComponent = nullptr;
	if (Owner->GetClass()->ImplementsInterface(UMovesetComponentProxy::StaticClass()))
	{
		MovesetComponent = IMovesetComponentProxy::Execute_GetActiveMovesetComponent(Owner);
	}
	else
	{
		MovesetComponent = Cast<UMovesetComponent>(Owner->GetComponentByClass(UMovesetComponent::StaticClass()));
	}
	if (IsValid(MovesetComponent))
	{
		UAttackMotion* AttackMotion = MovesetComponent->GetCurrentMotion();
		if (AttackMotion && AttackMotion->GetAnim() == Animation)
			AttackMotion->SetReadyToInterrupt(true);
	}
}
