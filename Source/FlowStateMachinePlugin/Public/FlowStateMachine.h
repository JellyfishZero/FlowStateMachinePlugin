// JellyfishZero All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FlowStateBase.h"
#include "FlowStateMachine.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LOG_FlowStateMachine, Log, All);

/**
 * StateMachine
 */
UCLASS()
class FLOWSTATEMACHINEPLUGIN_API UFlowStateMachine : public UObject
{
	GENERATED_BODY()

public:

	/**
	 * 初始化狀態機
	 * 建議初始化呼叫位置：GameInstance Init()
	 * @param InWorldContext - StateMachine 執行的 Context
	 */
	UFUNCTION(BlueprintCallable, Category = "Flow State")
	void Initialize(UObject* InWorldContext);

	/**
	 * 取得目前的 State 名稱
	 */
	UFUNCTION(BlueprintPure, Category = "Flow State")
	FName GetCurrentStateName() const;

	/**
	 * 更改為指定 State，初次進入State也請呼叫這個。
	 * 初次呼叫位置，建議在GameMode之類的BeginPlay()後。
	 * @param StateName - 名稱
	 */
	UFUNCTION(BlueprintCallable, Category = "Flow State")
	void ChangeState(FName StateName);

	/**
	 * 推上新的 State
	 * @param StateName - 名稱
	 */
	UFUNCTION(BlueprintCallable, Category = "Flow State")
	void PushState(FName StateName);

	/**
	 * 彈出當前 State
	 */
	UFUNCTION(BlueprintCallable, Category = "Flow State")
	void PopState();

	UWorld* GetWorld() const override;

	/**
	 * [Debug] Dump目前的State堆疊
	 * 在螢幕上和Log上顯示目前的State堆疊。
	 * 展開堆疊State，並標記當前的State。
	 * @param Seconds - 顯示時間(秒)
	 */
	UFUNCTION(BlueprintCallable, Category = "Flow State")
	void DumpStates(float Seconds = 10.f);

private:

	/** State 執行堆疊 */
	UPROPERTY()
	TArray<UFlowStateBase*> StateStack;

	/** 已註冊的 States */
	UPROPERTY()
	TMap<FName, UFlowStateBase*> RegisteredStates;

	/** 持有StateMachine的Context */
	UPROPERTY()
	TObjectPtr<UObject> WorldContext = nullptr;

	/** 初始化標記 */
	bool bInitialized = false;

	/** 初始化時自動註冊設定檔裡的 State */
	void AutoRegisterStatesFromSettings();

	/**
	 * 註冊 State
	 * @param StateName - 名稱
	 * @param State - State 實例
	 */
	UFUNCTION(BlueprintCallable, Category = "Flow State")
	void RegisterState(FName StateName, UFlowStateBase* State);
};
