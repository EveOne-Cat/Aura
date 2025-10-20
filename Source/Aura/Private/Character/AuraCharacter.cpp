// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacter.h"

#include "Animation/MovieScene2DTransformSection.h"
#include "GameFramework/CharacterMovementComponent.h"

AAuraCharacter::AAuraCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);	// 转向速度
	GetCharacterMovement()->bConstrainToPlane = true;	// 限制移动在平面上
	GetCharacterMovement()->bSnapToPlaneAtStart = true;		// 限制移动在平面上

	// 默认参数设置
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}
