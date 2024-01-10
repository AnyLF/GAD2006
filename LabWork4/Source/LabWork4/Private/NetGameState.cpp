#include "NetGameState.h"
#include "NetPlayerState.h"
#include "NetGameMode.h"
#include "NetBaseCharacter.h"
#include "Net/UnrealNetwork.h"


ANetGameState::ANetGameState() :
	WinningPlayer(-1),
	Timer(30.0f),
	RemainingTime(Timer)
{

}

void ANetGameState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ANetGameState, WinningPlayer);
	DOREPLIFETIME_CONDITION(ANetGameState, RemainingTime, COND_OwnerOnly);
}

void ANetGameState::OnRep_Winner()
{
	if (WinningPlayer >= 0)
	{
		OnVictory();
	}
}

void ANetGameState::TriggerRestart_Implementation()
{
	OnRestart();
}

ANetPlayerState* ANetGameState::GetPlayerStateByIndex(int PlayerIndex)
{
	for (APlayerState* PS : PlayerArray)
	{
		ANetPlayerState* State = Cast<ANetPlayerState>(PS);
		if (State && State->PlayerIndex == PlayerIndex)
		{
			return State;
		}
	}

	return nullptr;
}

void ANetGameState::StartTimer()
{	
	RemainingTime = Timer;
	GetWorldTimerManager().SetTimer(TimerHandle_Timer, this, &ANetGameState::UpdateTimer, 1.0f, true);
}

void ANetGameState::StopTimer_Implementation()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Timer);
}

void ANetGameState::UpdateTimer()
{
	UpdateTimerDisplay();

	if (RemainingTime > 0)
	{
		RemainingTime -= 1.0f;
	}
	else
	{
		ANetGameMode* GMode = Cast<ANetGameMode>(GetWorld()->GetAuthGameMode());
		if (GMode)
		{
			GMode->TimeOver();
		}
	}
}

void ANetGameState::OnRep_RemainingTime()
{
	UpdateTimerDisplay();
}

