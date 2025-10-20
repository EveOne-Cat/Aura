// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/HighlightInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	// 网络同步
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false,CursorHit);
	if (!CursorHit.bBlockingHit) return;

	// 这个顺序保证了LastActor指向上一帧的情况
	LastActor = ThisActor;	
	ThisActor = Cast<IHighlightInterface>(CursorHit.GetActor());

	/**
	 * 多种边界情况需要处理
	 * 1：LastActor = NULL && ThisActor = NULL
	 *		- 不处理
	 * 2：LastActor = NULL && ThisActor != NULL
	 *		- 高亮ThisActor
	 * 3：LastActor ！= NULL && ThisActor = NULL
	 *		- 取消高亮LastActor
	 * 4：都valid，但是指向不同Actor
	 *		- 取消高亮LastActor，高亮ThisActor
	 * 5：都valid，指向同一个Actor
	 *		- 不处理
	 */

	if (LastActor == nullptr)
	{
		if (ThisActor != nullptr)
		{
			// 情况2
			ThisActor->HighlightActor();
		}
	}
	else
	{
		if (ThisActor == nullptr)
		{
			// 情况3
			LastActor->UnHighlightActor();
		}
		else
		{
			if (LastActor != ThisActor)
			{
				// 情况4
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
		}
	}
}


void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 用断言的方法保证只有设置了IMC才能游玩
	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(AuraContext, 0);

	// 光标样式
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::GrabHand;

	// 输入模式
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
 
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	// 绑定输入
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	// 获取单位方向向量，确定方向
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// 移动
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}
