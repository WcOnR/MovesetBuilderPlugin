// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"

#include "MovesetComponent.generated.h"

class UAttackMotion;

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMovesetNotify);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MOVESETBUILDER_API UMovesetComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMovesetComponent();

	UFUNCTION(BlueprintCallable)
	void SetSkeletalMeshComponent(USkeletalMeshComponent* Skeletal);

	UFUNCTION(BlueprintCallable)
	void AttackInput(UAttackInputTypeDataAsset* AttackType);

	UFUNCTION(BlueprintCallable)
	bool CanPlayNextMotion() const;
	UFUNCTION(BlueprintCallable)
	bool AttackMotionIsPlaying() const;

	UFUNCTION(BlueprintCallable)
	UAttackMotion* GetCurrentMotion() const;
	UFUNCTION(BlueprintCallable)
	UAttackMotion* GetPrevMotion() const;

public:
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
	FMovesetNotify AnimStarted;
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
	FMovesetNotify AnimCompleted;
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
	FMovesetNotify AnimInterrupted;
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
	FMovesetNotify AttackMotionReadyToInterrupt;

protected:
	virtual void BeginPlay() override;

	void ResetCurrentMotion();
	UAttackMotion* FindOrCreateMotion(TSubclassOf<UAttackMotion> MotionClass);

	void SetCurrentMotion(UAttackMotion* Motion);
	bool SetNextMotionByAttackType(UAttackInputTypeDataAsset* AttackType);

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
	void OnCurrentAttackMotionReadyToInterrupt();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UAttackMotion> HeadAttackMotionClass;
	UPROPERTY()
	USkeletalMeshComponent* SkeletalMeshComponent;
	UPROPERTY()
	TMap<TSubclassOf<UAttackMotion>, UAttackMotion*> MotionCache;
	UPROPERTY()
	UAttackMotion* CurrentMotion;
	UPROPERTY()
	UAttackMotion* PrevMotion;

private:
	FOnMontageEnded MontageEnded;
};