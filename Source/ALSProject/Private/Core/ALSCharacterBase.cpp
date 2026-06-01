// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/ALSCharacterBase.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"
#include "TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"

AALSCharacterBase::AALSCharacterBase()
{
	// 射线检测由 Timer 驱动，关闭 Tick 避免每帧空转
	PrimaryActorTick.bCanEverTick = true;
}

bool AALSCharacterBase::PerformLineTrace(FVector Start, FVector End, FHitResult& OutHit)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return false;
	}

	// 使用蓝图节点等价实现：UKismetSystemLibrary::LineTraceSingle
	// 将 ECollisionChannel 转换为 ETraceTypeQuery（Blueprint 的 Channel 节点内部使用的类型）
	const ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(TraceChannel);

	// 忽略自身以避免射线打到角色胶囊体
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	// 使用 LineTraceSingle：bTraceComplex 设置为 false（与之前实现一致）
	// 我们在此将 LineTrace 的内置调试关闭，改为根据 bDrawDebug 手动绘制彩色线段和球体以保持一致性
	const bool bHit = UKismetSystemLibrary::LineTraceSingle(
		World,
		Start,
		End,
		TraceType,
		false,                // bTraceComplex
		ActorsToIgnore,
		EDrawDebugTrace::None,
		OutHit,
		true                  // bIgnoreSelf
	);

	if (bDrawDebug)
	{
		const FVector DrawEnd = bHit ? OutHit.Location : End;
		const FColor LineColor = bHit ? FColor::Green : FColor::Red;
		DrawDebugLine(World, Start, DrawEnd, LineColor, false, LineTraceInterval, 0, 2.0f);
		if (bHit)
		{
			DrawDebugSphere(World, OutHit.Location, 8.0f, 12, FColor::Green, false, LineTraceInterval, 0, 1.0f);
		}
	}

	return bHit;
}
