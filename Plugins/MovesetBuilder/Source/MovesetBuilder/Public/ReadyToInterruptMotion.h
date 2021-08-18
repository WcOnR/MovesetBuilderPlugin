// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "CoreMinimal.h"

#include "ReadyToInterruptMotion.generated.h"

/**
 *
 */
UCLASS()
class MOVESETBUILDER_API UReadyToInterruptMotion : public UAnimNotify
{
	GENERATED_BODY()

	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
