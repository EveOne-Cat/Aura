// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

// 前向声明
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IHighlightInterface;

/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAuraPlayerController();
	virtual void PlayerTick(float DeltaTime) override;

private:
	// IMC
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	// IA
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	// 用于控制玩家移动的主函数
	void Move(const FInputActionValue& InputActionValue);

	// 检查鼠标下的Actor
	void CursorTrace();

	// 鼠标追踪到的Actor
	IHighlightInterface* LastActor;
	IHighlightInterface* ThisActor;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
};

