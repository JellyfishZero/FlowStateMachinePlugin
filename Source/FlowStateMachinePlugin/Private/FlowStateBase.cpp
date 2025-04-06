// JellyfishZero All Rights Reserved


#include "FlowStateBase.h"

DEFINE_LOG_CATEGORY(LOG_FlowState);

void UFlowStateBase::Initialize(UObject* InWorldContext)
{
	WorldContext = InWorldContext;
}

void UFlowStateBase::SetupStateMachineCallbacks(FOnStateRequest Change, FOnStateRequest Push, FOnStateRequestNoParam Pop)
{
	OnChangeState = Change;
	OnPushState = Push;
	OnPopState = Pop;
}

void UFlowStateBase::ChangeState(FName StateName)
{
	if (OnChangeState.IsBound())
	{
		OnChangeState.Execute(StateName);
	}
}

void UFlowStateBase::PushState(FName StateName)
{
	if (OnPushState.IsBound())
	{
		OnPushState.Execute(StateName);
	}
}

void UFlowStateBase::PopState()
{
	if (OnPopState.IsBound())
	{
		OnPopState.Execute();
	}
}

void UFlowStateBase::Begin(UObject* Data)
{
	OnBegin(Data);
}

void UFlowStateBase::Finish()
{
#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
	if (StateData.IsValid() == false && StateData != nullptr)
	{
		UE_LOG(LOG_FlowState, Warning, TEXT("State [%s] 進入Finish, 但其StateData已經無效！"), *GetName());
	}
#endif
	OnFinish();
}

void UFlowStateBase::Resume(UObject* Data)
{
	OnResume(Data);
}

void UFlowStateBase::Pause()
{
#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
	if (StateData.IsValid() == false && StateData != nullptr)
	{
		UE_LOG(LOG_FlowState, Warning, TEXT("State [%s] 進入Pause, 但其StateData已經無效！"), *GetName());
	}
#endif
	OnPause();
}

UWorld* UFlowStateBase::GetWorld() const
{
	return IsValid(WorldContext) ? WorldContext->GetWorld() : nullptr;
}
