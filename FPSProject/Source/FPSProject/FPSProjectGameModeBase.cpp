// Copyright Epic Games, Inc. All Rights Reserved.


#include "FPSProjectGameModeBase.h"

void AFPSProjectGameModeBase::StartPlay()
{
	Super::StartPlay();
	//check()������������Ϊfalse�����ִֹͣ��
	check(GEngine!=nullptr);
	//��ʾ������Ϣ5��
	//�����е�-1������ֵ���Ͳ����ܷ�ֹ����Ϣ�����»�ˢ�£�5.0f��ʾ��ʾ5��
	GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Yellow,TEXT("Hello world,this is FPSGameMode!"));
}
