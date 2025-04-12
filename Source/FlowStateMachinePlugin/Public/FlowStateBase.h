// JellyfishZero All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FlowStateBase.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LOG_FlowState, Log, All);

/**
 * State操作請求
 */
DECLARE_DELEGATE_OneParam(FOnStateRequest, FName);
DECLARE_DELEGATE(FOnStateRequestNoParam);

/**
 * 流程State基本類
 */
UCLASS(Blueprintable, BlueprintType, Abstract)
class FLOWSTATEMACHINEPLUGIN_API UFlowStateBase : public UObject
{
	GENERATED_BODY()

public:

#pragma region 操作請求

	/* 更改 State時 */
	FOnStateRequest OnChangeState;
	/* Push State時 */
	FOnStateRequest OnPushState;
	/* Pop State時 */
	FOnStateRequestNoParam OnPopState;

#pragma endregion

#pragma region 初始化

	/**
	 * 初始化狀態機
	 * @param InWorldContext - 持有State的Context
	 */
	UFUNCTION(BlueprintCallable, Category = "Flow State")
	void Initialize(UObject* InWorldContext);

	/**
	 * 初始化設置State操作時的Request
	 * @param Change - Change State時的Callback
	 * @param Push - Push State時的Callback
	 * @param Pop - Pop State時的Callback
	 */
	void SetupStateMachineCallbacks(FOnStateRequest Change, FOnStateRequest Push, FOnStateRequestNoParam Pop);

#pragma endregion

#pragma region State操作

	/**
	 * 更改State。
	 * 將逐一pop堆疊中的State並執行其Resume後再執行Finish。
	 * 最後執行更改State的Begin。
	 * @param StateName - State名稱
	 */
	UFUNCTION(BlueprintCallable, Category = "Flow State")
	void ChangeState(FName StateName);

	/**
	 * 推上State，
	 * 執行原State的Pause後，再執行推上State的Begin。
	 * @param StateName - State名稱
	 */
	UFUNCTION(BlueprintCallable, Category = "Flow State")
	void PushState(FName StateName);

	/**
	 * 離開當前State。
	 * 執行原State的Finish後，再執行堆疊中State的Resume。
	 */
	UFUNCTION(BlueprintCallable, Category = "Flow State")
	void PopState();

#pragma endregion

#pragma region State的行為

	/**
	 * State 開始
	 * 並執行藍圖實作OnBegin
	 * @param Data - 上一個State傳入的參數
	 */
	virtual void Begin(UObject* Data = nullptr);

	/**
	 * State 結束
	 * 並執行藍圖實作OnFinish
	 */
	virtual void Finish();

	/**
	 * State 恢復
	 * 並執行藍圖實作OnResume
	 * @param Data - 上一個State傳入的參數
	 */
	virtual void Resume(UObject* Data = nullptr);

	/**
	 * State 暫停
	 * 並執行藍圖實作OnPause
	 */
	virtual void Pause();

#pragma endregion

#pragma region 取得Data

	/**
	 * 取得欲傳出的State Data
	 * 這段理論上只在C++的State Machine 當中執行。
	 */
	UFUNCTION(BlueprintPure, Category = "Flow State")
	FORCEINLINE UObject* GetStateData() const;

	/**
	 * 取得欲傳出的State Data後便清除StateData
	 */
	UFUNCTION()
	FORCEINLINE UObject* TakeStateData();

#pragma endregion


	UWorld* GetWorld() const override;

protected:

#pragma region 各種狀態的藍圖實作

	/**
	 * State 開始(藍圖實作)
	 * @param Data - 上一個State傳入的參數
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Flow State")
	void OnBegin(UObject* Data = nullptr);

	/**
	 * State 結束(藍圖實作)
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Flow State")
	void OnFinish();

	/**
	 * State 恢復(藍圖實作)
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Flow State")
	void OnResume(UObject* Data = nullptr);

	/**
	 * State 暫停(藍圖實作)
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Flow State")
	void OnPause();

#pragma endregion

#pragma region 欲傳出的資料

	/**
	 * 設定想傳出給下一個State的資料
	 */
	UFUNCTION(BlueprintCallable, Category = "Flow State")
	FORCEINLINE void SetStateData(UObject* InStateData);

#pragma endregion

private:

	/** 持有StateMachine的Context */
	UPROPERTY()
	TObjectPtr<UObject> WorldContext = nullptr;

#pragma region 欲傳出的資料

	/**
	 * 欲傳出的State資料，
	 * 可接受各種UObject的繼承類。
	 */
	UPROPERTY()
	TObjectPtr<UObject> StateData = nullptr;

#pragma endregion

};

FORCEINLINE UObject* UFlowStateBase::GetStateData() const
{
	return IsValid(StateData) ? StateData.Get() : nullptr;
}

FORCEINLINE UObject* UFlowStateBase::TakeStateData()
{
	UObject* Temp = GetStateData();
	StateData = nullptr;
	return Temp;
}

FORCEINLINE void UFlowStateBase::SetStateData(UObject* InStateData)
{
	if (IsValid(InStateData))
	{
		StateData = InStateData;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SetStateData() 傳入了一個無效的UObject"));
		StateData = nullptr;
	}
}