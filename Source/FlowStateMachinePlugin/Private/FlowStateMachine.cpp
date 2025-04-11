// JellyfishZero All Rights Reserved


#include "FlowStateMachine.h"

#include "Engine/DataTable.h"
#include "FlowStateMachineSettings.h"
#include "FlowStateRow.h"

DEFINE_LOG_CATEGORY(LOG_FlowStateMachine);

void UFlowStateMachine::Initialize(UObject* InWorldContext)
{
	if(bInitialized)
	{
		UE_LOG(LOG_FlowStateMachine, Warning, TEXT("FlowStateMachine 已初始化過，忽略這次初始化"));
		return;
	}

	WorldContext = InWorldContext;
	bInitialized = true;

	UE_LOG(LOG_FlowStateMachine, Warning, TEXT("FlowStateMachine 初始化完成！"));

	AutoRegisterStatesFromSettings();
}

FName UFlowStateMachine::GetCurrentStateName() const
{
	if(StateStack.Num() > 0)
	{
		UFlowStateBase* Current = StateStack.Last();
		for(const auto& Pair : RegisteredStates)
		{
			if(Pair.Value == Current)
			{
				return Pair.Key;
			}
		}
	}
	return NAME_None;
}

void UFlowStateMachine::ChangeState(FName StateName)
{
	if(!RegisteredStates.Contains(StateName))
	{
		UE_LOG(LOG_FlowState, Error, TEXT("ChangeState: 未註冊的 State [%s]"), *StateName.ToString());
		return;
	}

	if(StateStack.Num() == 1 && StateStack.Last()->GetClass() == RegisteredStates[StateName]->GetClass())
	{
		UE_LOG(LOG_FlowState, Warning, TEXT("ChangeState: State [%s] 已經是當前狀態，略過。"), *StateName.ToString());
		return;
	}

	UObject* OutData = nullptr;

	// Pop 所有現有 State
	while(StateStack.Num() > 0)
	{
		UFlowStateBase* Current = StateStack.Pop();
		Current->Pause();

		if(!OutData)
		{
			OutData = Current->TakeStateData();
		}

		Current->Finish();
	}

	// Push 新的 State
	UFlowStateBase* NewState = RegisteredStates[StateName];
	StateStack.Add(NewState);
	NewState->Begin(OutData);
}

void UFlowStateMachine::PushState(FName StateName)
{
	if(!RegisteredStates.Contains(StateName))
	{
		UE_LOG(LOG_FlowState, Error, TEXT("PushState: 未註冊的 State [%s]"), *StateName.ToString());
		return;
	}

	if(StateStack.Num() > 0 && StateStack.Last()->GetClass() == RegisteredStates[StateName]->GetClass())
	{
		UE_LOG(LOG_FlowState, Warning, TEXT("PushState: State [%s] 已經是當前狀態，略過。"), *StateName.ToString());
		return;
	}

	UObject* OutData = nullptr;

	if(StateStack.Num() > 0)
	{
		UFlowStateBase* Current = StateStack.Last();
		OutData = Current->TakeStateData();
		Current->Pause();
	}

	UFlowStateBase* NewState = RegisteredStates[StateName];
	StateStack.Add(NewState);
	NewState->Begin(OutData);
}

void UFlowStateMachine::PopState()
{
	if(StateStack.Num() < 2)
	{
		UE_LOG(LOG_FlowState, Warning, TEXT("PopState: StateStack < 2"));
		return;
	}

	UObject* OutData = nullptr;

	// Finish 當前 State
	UFlowStateBase* TopState = StateStack.Pop();
	OutData = TopState->TakeStateData();
	TopState->Finish();

	// Resume 下層 State
	if(StateStack.Num() > 0)
	{
		UFlowStateBase* Previous = StateStack.Last();
		Previous->Resume(OutData);
	}
}

UWorld* UFlowStateMachine::GetWorld() const
{
	return IsValid(WorldContext) ? WorldContext->GetWorld() : nullptr;
}

void UFlowStateMachine::AutoRegisterStatesFromSettings()
{
	const UFlowStateMachineSettings* Settings = GetDefault<UFlowStateMachineSettings>();
	if(!Settings)
	{
		UE_LOG(LOG_FlowState,Error,TEXT("AutoRegisterStatesFromSettings: 找不到 FlowStateMachineSettings"));
		return;
	}

	UDataTable* Table = Settings->StateTable.LoadSynchronous();
	if(!Table)
	{
		UE_LOG(LOG_FlowState,Error,TEXT("AutoRegisterStatesFromSettings: 無法讀取 StateTable"));
		return;
	}

	static const FString Context(TEXT("FlowStateMachine_LoadTable"));
	TArray<FFlowStateRow*> Rows;
	Table->GetAllRows<FFlowStateRow>(Context, Rows);

	for(FFlowStateRow* Row : Rows)
	{
		if(!Row || !Row->StateClass)
			continue;

		UFlowStateBase* StateInstance = NewObject<UFlowStateBase>(this, Row->StateClass);
		if(!StateInstance)
			continue;
		StateInstance->Initialize(WorldContext);

		// 綁定 delegate
		StateInstance->SetupStateMachineCallbacks(
			FOnStateRequest::CreateUObject(this, &UFlowStateMachine::ChangeState),
			FOnStateRequest::CreateUObject(this, &UFlowStateMachine::PushState),
			FOnStateRequestNoParam::CreateUObject(this, &UFlowStateMachine::PopState)
		);

		RegisterState(Row->StateName, StateInstance);
	}
}

void UFlowStateMachine::RegisterState(FName StateName, UFlowStateBase* State)
{
	if(!State || StateName.IsNone())
		return;

	if(RegisteredStates.Contains(StateName))
	{
		UE_LOG(LOG_FlowStateMachine, Warning, TEXT("State [%s] 已經註冊過，將覆蓋原有的"), *StateName.ToString());
	}

	RegisteredStates.Add(StateName, State);
}
