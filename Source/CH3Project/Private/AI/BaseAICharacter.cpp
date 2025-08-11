#include "AI/BaseAICharacter.h"
#include "AI/BaseAIController.h"
#include "AI/AIHealthComponent.h"
#include "Character/CH3Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "BrainComponent.h"



ABaseAICharacter::ABaseAICharacter()
{
	AIControllerClass = ABaseAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	UCharacterMovementComponent* Movement = GetCharacterMovement();
	Movement->MaxWalkSpeed = WalkSpeed;	
	Movement->RotationRate = FRotator(0.0f, 360.0f, 0.0f);

	// 캐릭터의 회전을 컨트롤러의 회전과 일치
	bUseControllerRotationYaw = true;	
	// 캐릭터의 이동 방향으로 자동 회전하는 기능 off AI컨트롤러의 SetFocus에 제어됨
	GetCharacterMovement()->bOrientRotationToMovement = false;

	HealthComponent = CreateDefaultSubobject<UAIHealthComponent>(TEXT("HealthComponent"));

	WeaponMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMeshComp"));
	WeaponMeshComp->SetupAttachment(GetMesh(), TEXT("WeaponSocket"));	
}

void ABaseAICharacter::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("AI Character %s has begun play."), *GetName());
}

void ABaseAICharacter::setMoveSpeed(float NewSpeed)
{
	if(UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		Movement->MaxWalkSpeed = NewSpeed;
		UE_LOG(LogTemp, Warning, TEXT("AI Character %s speed set to: %f"), *GetName(), NewSpeed);
	}
}

void ABaseAICharacter::PerformMeleeAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("%s Performs MELEE ATTACK!"), *GetName());
}

void ABaseAICharacter::PerformRangedAttack()
{
	if (GetWorld() == nullptr || WeaponMeshComp == nullptr) return;

	//레이저의 시작 위치와 방향을 계산
	const FVector StartLocation = WeaponMeshComp->GetSocketLocation(TEXT("AIMuzzleSocket"));
	const FVector ForwardVector = GetActorForwardVector();
	const float FireRange = 5000.0f; // 50m Range
	const FVector EndLocation = StartLocation + (ForwardVector * FireRange);

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	//월드에 보이지 않는 라인 트레이스를 수행하여 적을 맞추는지 확인
	TArray<FHitResult> HitResults;
	bool bHit = GetWorld()->LineTraceMultiByChannel(
		HitResults,    // 명중 결과(출력)
		StartLocation,  //시작 점
		EndLocation,	//끝 점
		ECollisionChannel::ECC_Pawn, // 폰 타입과 충돌하도록 설정
		CollisionParams	//충돌 파라미터
	);
	//명중 결과 처리
	if (bHit)
	{
		for (const FHitResult& Hit : HitResults)
		{
			if (Hit.GetActor())
			{
				UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *Hit.GetActor()->GetName());
			}
		}
		AActor* HitActor = HitResult.GetActor();
		//Enemy 태그가 있는 경우에만 처리
		//if (HitActor && HitActor->ActorHasTag("Enemy"))
		// 위 코드를 AI캐릭터와 플레이어 캐릭터 둘 다에게 적용하기 위해 조건을 변경함
		if (HitActor && (HitActor->IsA(ACH3Character::StaticClass()) || HitActor->IsA(ABaseAICharacter::StaticClass())))
		{
			UGameplayStatics::ApplyDamage(
				HitActor,  //피해를 입힐 액터
				10.0f,     //피해량
				GetController(), //피해를 입힌 컨트롤러(AI)
				this,      //실제 피해를 입힌 액터(AI)
				UDamageType::StaticClass() //피해 유형
			);
		}
	}
	//디버그용으로 레이저를 시각적으로 표시
	DrawDebugLine(
		GetWorld(),
		StartLocation,
		EndLocation,
		FColor::Red,
		false, // 지속되지 않음
		1.0f,  // 지속 시간
		0,     // Depth Priority
		1.0f   // 두께
	);
	UE_LOG(LogTemp, Warning, TEXT("%s Performs RANGED ATTACK!"), *GetName());
}


void ABaseAICharacter::HandleDeath()
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{	// 비헤이비어트리 를 중지하고, AI의 뇌 컴포넌트를 정지
		if (AIController && AIController->GetBrainComponent())
		{
			AIController->GetBrainComponent()->StopLogic(TEXT("Death"));
		}
		// 시야 고정 해제
		AIController->ClearFocus(EAIFocusPriority::Gameplay);
	}
	   // 총기 컴포넌트 제거
	if (WeaponMeshComp)
	{
		WeaponMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	// 애니메이션 클래스 제거
	GetMesh()->SetAnimClass(nullptr); 
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);

	// 콜리전 제거 및 캡슐컴포넌느 모든 종류 충돌 무시 메시가 충돌 담당
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	// 레그돌 변경 및 시뮬레이션 활성화
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);

	SetLifeSpan(10.0f); 
}
