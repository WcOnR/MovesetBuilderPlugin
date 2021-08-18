// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MovesetComponentProxy.generated.h"

class UMovesetComponent;

UINTERFACE(BlueprintType)
class UMovesetComponentProxy : public UInterface
{
	GENERATED_BODY()
};

class MOVESETBUILDER_API IMovesetComponentProxy
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UMovesetComponent* GetActiveMovesetComponent();
};
