// Copyright Epic Games, Inc. All Rights Reserved.

#include "FlowStateMachinePlugin.h"
#include "FlowStateMachineSettings.h"
#include "Modules/ModuleManager.h" // FModuleManager 是核心功能，保留在外面

// 只在編輯器模式下才 Include Settings 相關的標頭檔
#if WITH_EDITOR
#include "ISettingsModule.h"      // Settings 模組的主要標頭檔
#include "UObject/Class.h"        // GetMutableDefault 需要用到
#include "Internationalization/Text.h" // NSLOCTEXT 需要用到
#endif // WITH_EDITOR

#define LOCTEXT_NAMESPACE "FFlowStateMachinePluginModule"

void FFlowStateMachinePluginModule::StartupModule()
{
	// 只在編輯器模式下註冊設定
#if WITH_EDITOR
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings(
			"Project",                          // 設定分頁
			"Game",                             // 設定類別
			"FlowStateMachine",                 // 設定項目名稱
			NSLOCTEXT("FlowStateMachine", "FlowStateMachineSettingsName", "Flow State Machine"), // 顯示名稱
			NSLOCTEXT("FlowStateMachine", "FlowStateMachineSettingsDesc", "Configure the Flow State Machine for the game."), // 描述
			GetMutableDefault<UFlowStateMachineSettings>() // 設定物件
		);
	}
#endif // WITH_EDITOR
}

void FFlowStateMachinePluginModule::ShutdownModule()
{
	// 只在編輯器模式下取消註冊設定
#if WITH_EDITOR
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Game", "FlowStateMachine");
	}
#endif // WITH_EDITOR
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FFlowStateMachinePluginModule, FlowStateMachinePlugin)