// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "AttackMotion.generated.h"

class UAnimMontage;
class UAttackInputTypeDataAsset;
class UMovesetComponent;

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttackMotionNotify);

UCLASS(Abstract, Blueprintable)
class MOVESETBUILDER_API UAttackMotion : public UObject
{
	GENERATED_BODY()

public:
	void SetMovesetComponent(UMovesetComponent* Moveset);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TSubclassOf<UAttackMotion> AttackInput(UAttackInputTypeDataAsset* AttackType);
	virtual TSubclassOf<UAttackMotion> AttackInput_Implementation(UAttackInputTypeDataAsset* AttackType);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PreStartAnim();
	virtual void PreStartAnim_Implementation();

	UFUNCTION(BlueprintCallable)
	bool IsReadyToInterrupt() const;
	void SetReadyToInterrupt(bool ReadyToInterrupt);

	UFUNCTION(BlueprintCallable)
	UAnimMontage* GetAnim() const;

public:
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
	FAttackMotionNotify AttackMotionReadyToInterrupt;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* Anim;
	UPROPERTY()
	bool bReadyToInterruptAnim = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UMovesetComponent* MovesetComponent;
};
