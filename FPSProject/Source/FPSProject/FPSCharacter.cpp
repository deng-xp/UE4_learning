// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSCharacter.h"

// Sets default values
AFPSCharacter::AFPSCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//相机初始化
	FPSCamerComponent=CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	check(FPSCamerComponent!=nullptr);
	//附加到根组件（胶囊体组件）上(SetupAttachment参数应为USceneComponent类型，此处是判断类型，不满足便进行转换的含义）
	FPSCamerComponent->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));
	//设置相机位置
	FPSCamerComponent->SetRelativeLocation(FVector(0.0f,0.0f,50.0f+BaseEyeHeight));		//放在略高于眼睛的位置
	FPSCamerComponent->SetRelativeRotation(FRotator(0.0f,0.0f,0.0f));	//设置倾角
	//启用Pawn控制摄像机旋转
	FPSCamerComponent->bUsePawnControlRotation=true;

	//为所属玩家创建第一人称网格体组件
	FPSMesh=CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	check(FPSMesh!=nullptr);
	//仅所属玩家可看见此网格体（表示仅对拥有此角色的PlayerController可见）
	FPSMesh->SetOnlyOwnerSee(true);
	//将FPS网格体附加到FPS摄像机
	FPSMesh->SetupAttachment(FPSCamerComponent);
	//禁用某些环境下的阴影，实现只有单个网格体的感觉
	FPSMesh->bCastDynamicShadow=false;
	FPSMesh->CastShadow = false;

	//所属玩家看不到常规（第三人称）全身网格体
	GetMesh()->SetOwnerNoSee(true); 
	
}

// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(GEngine != nullptr);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("We are using FPSCharacter"));
}

// Called every frame
void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input （将功能与输入绑定）
void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//绑定移动输入
	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);
	//绑定摄像机视角变动
	PlayerInputComponent->BindAxis("Turn", this, &AFPSCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AFPSCharacter::AddControllerPitchInput);
	//绑定跳跃
	PlayerInputComponent->BindAction("Jump",IE_Pressed,this,&AFPSCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AFPSCharacter::StopJump);
	//绑定子弹发射
	PlayerInputComponent->BindAction("Fire",IE_Pressed,this,&AFPSCharacter::Fire);
}

//前后
void AFPSCharacter::MoveForward(float value)
{
	// 找出"前进"方向，并记录玩家想向该方向移动。
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, value);

}
//左右
void AFPSCharacter::MoveRight(float value)
{
	// 找出"右侧"方向，并记录玩家想向该方向移动。
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, value);
}
//跳跃
void AFPSCharacter::StartJump()
{
	bPressedJump = true;	//Acharacter基类中的变量，支持跳跃
}
void AFPSCharacter::StopJump()
{
	bPressedJump = false;	
}
//子弹发射
//子弹发射的关键在于：（1）子弹从何处开始发射；（2）子弹的类别是啥，如何让角色了解子弹类
void AFPSCharacter::Fire()
{
	
	//尝试发射子弹
	if (ProjectileClass)	//子弹类型已绑定
	{
		//获取摄像机变换（用于确定枪口位置）
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation,CameraRotation);	//返回视角的变换
		//设置MuzzleOffset，即枪口相对于摄像机位置的偏移量
		MuzzleOffset.Set(100.0f,0.0f,0.0f);
		//将MuzzleOffset从摄像机的坐标空间转换至世界坐标系下
		//摄像机本身可能是有倾角的，此时其坐标轴方向与世界坐标系有差异，因此需进行转换
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
		//使目标方向略向上倾斜（可理解为后坐力）
		FRotator MuzzleRotation =CameraRotation;
		MuzzleRotation.Pitch+=10.0f;

		UWorld* World=GetWorld();	//返回世界指针
		if (World)
		{
			//传递给生成物体的可选参数（是一个结构体，包含一些Actor生成的参数设置）
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner=this;		//设置生成的类的拥有者为本类，即FPSCharacter类
			SpawnParams.Instigator=GetInstigator();		//导致所生成的Actor施加伤害的对象

			//在枪口位置生成发射物——子弹
			/*SpawnActor函数：
			该函数模板在指定“位置”处使用指定的“旋转度”生成指定类“ProjectileClass”的实例，
			并返回和那个模板类类型一样的实例的指针，也就是 T*`。这要求指定的ProjectileClass类必
			须是模板类T的子类。除了类、位置及旋转度外，还可以指定拥有者Actor 、挑衅的的Pawn ， 
			及指定如果生成的 Actor 会侵占或碰撞另一个世界中已经存在的 Actor 生成操作
			*/
			AFPSProjectile* Projectile=World->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				//设置发射物的初始轨迹
				//根据子弹的旋转变换求取其“向前的方向”
				FVector LaunchDirection=MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
			}
		}
	}
}