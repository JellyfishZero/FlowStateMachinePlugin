# 安裝

1. 下載該插件後，於Unreal專案的Plugins下解壓縮。
1. 對專案的.uproject右鍵，選擇Generate Visual Studio Project Files。
1. 打開專案的.sln檔案，重新編譯。
1. 開啟專案，從上方列表Tools->Plugins開啟Plugins頁面，搜尋FlowStateMachinePlugin，確認插件有被開啟。

# 建議使用方式
## 生成StateMachine
1. 建立自己的GameInstance，Override Init()。
2. 宣告變數於.h當中。
```cpp
  UPROPERTY(BlueprintReadOnly)
  UFlowStateMachine* StateMachine;
```
3. 並在Init()中，Create UFlowStateMachine賦值，並初始化。
```cpp
void UMyGameInstance::Init()
{
	Super::Init();

	StateMachine = NewObject<UFlowStateMachine>();
	StateMachine->Initialize(this);
}
```
4. 回到引擎中，於Content Browser中，Create Data Table。資料結構選擇：`FlowStateRow`。

![image](https://github.com/user-attachments/assets/3129393b-35e7-4111-908b-0b078b3105be)

5. 再到Project Setting中，找到`Flow State Machine`頁籤，將上一步製作完成的Data Table賦予給他。

![image](https://github.com/user-attachments/assets/ad890ca1-049f-431d-b106-274f54a9338e)

6. 接著可透過藍圖或C++，繼承FlowStateBase，來製作需要的State。若State要生效，要記得將其新增至第4步時，製作的Data Table當中。

7. 載入第一個State的方式為，在任一BeginPlay處(如：GameMode)，取用GameInstance當中的FlowStateMachine，Call `ChangeState(State Id)`。(State Id 意為DataTable中的Row ID)。

## State流程說明

1. ChangeState：
>  該指令會將StateStack的所有State都pop掉，pop時會依序觸發State的`Pause()`和`Finish()`。
>  最後再執行下一個(欲Change)的State的`Begin()`。
>
>  注意：關於`Pause()`是否需要執行，我還在考慮。還是只執行`Finish()`即可。
>
>  ChangeState時，當前的State若有設定StateData，會將StateData傳遞給欲Change的State，並可由其`Begin()`獲取。

2. PushState：
> 該指令會將新的State push進Stack當中，依序觸發當前State的`Pause()`，並觸發下一個State的`Begin()`。
>
>  PushState時，當前的State若有設定StateData，會將StateData傳遞給欲Change的State，並可由其`Begin()`獲取。
