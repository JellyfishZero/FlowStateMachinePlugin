// Copyright Epic Games, Inc. All Rights Reserved.

#include "FlowStateMachinePlugin.h"
#include "FlowStateMachineSettings.h"
#include "ISettingsModule.h"

#define LOCTEXT_NAMESPACE "FFlowStateMachinePluginModule"

void FFlowStateMachinePluginModule::StartupModule()
{
	if(ISettingsModule* SettingSModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingSModule->RegisterSettings(
			"Project",
			"Game",
			"FlowStateMachine",
			NSLOCTEXT("FlowStateMachine", "FlowStateMachineSettings", "Flow State Machine"),
			NSLOCTEXT("FlowStateMachine", "FlowStateMachineSettingsDesc", "設定遊戲的 State Machine"),
			GetMutableDefault<UFlowStateMachineSettings>()
		);
	}
}

void FFlowStateMachinePluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FFlowStateMachinePluginModule,FlowStateMachinePlugin)