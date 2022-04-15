// Copyright Epic Games, Inc. All Rights Reserved.


#include "FPSProjectGameModeBase.h"

void AFPSProjectGameModeBase::StartPlay()
{
	Super::StartPlay();
	//check()：若其中条件为false，则会停止执行
	check(GEngine!=nullptr);
	//显示调试消息5秒
	//参数中的-1“键”值类型参数能防止该消息被更新或刷新；5.0f表示显示5秒
	GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Yellow,TEXT("Hello world,this is FPSGameMode!"));
}
