# 安裝

1. 下載該插件後，於Unreal專案的Plugins下解壓縮。(可直接Clone該專案或者使用[Release](https://github.com/JellyfishZero/FlowStateMachinePlugin/releases)版本)
1. 對專案的.uproject右鍵，選擇Generate Visual Studio Project Files。
1. 打開專案的.sln檔案，重新編譯。
1. 開啟專案，從上方列表Tools->Plugins開啟Plugins頁面，搜尋FlowStateMachinePlugin，確認插件有被開啟。

![image](https://github.com/user-attachments/assets/cd68ce59-0455-4c7c-a422-30c9675e5d36)

# 建議使用方式
## 搭建StateMachine
1. 建立自己的GameInstance，Override Init()。
2. 宣告StateMachine變數於.h當中。
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

6. 接著可透過藍圖或C++，繼承FlowStateBase，來製作需要的State。若State要生效，須將其新增至第4步製作的Data Table當中。

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
>  PushState時，當前的State若有設定StateData，會將StateData傳遞給欲Push的State，並可由其`Begin()`獲取。

3. PopState：
> 該指令會將當前的State從Stack當中pop掉，依序觸發`Finish()`，和前一個State的`Resume()`。
>
> PopState時，當前的State若有設定StateData，會將StateData傳遞給前一個的State，並可由其`Resume()`獲取。

4. 可呼叫State切換方法的地方：
	* StateMachine本身
	* State：State可自行呼叫Change、Pop和Push無須透過StateMachine本身。
## Debug

1. FlowStateMachine中，有`DumpStates()`可供調用，會顯示出目前State堆疊的狀況，並指出當前State。
```cpp
	UFUNCTION(BlueprintCallable, Category = "Flow State")
	void DumpStates(float Seconds = 10.f);
```

## 題外話

Plugin的圖示，是我親手繪製，再由Chat GPT美化。
我未來製作的Plugin(若有的話)，都會選擇黑暗精靈當成我的插件圖標，美好性癖值得推廣。

原稿：
![PluginLogo](https://github.com/user-attachments/assets/591fb5f7-74a6-402a-a6f9-6b7fef579964)
ChatGPT:
![PluginLogo3](https://github.com/user-attachments/assets/7ec04412-b3d2-40a9-94ed-4ad6085f4ad6)
