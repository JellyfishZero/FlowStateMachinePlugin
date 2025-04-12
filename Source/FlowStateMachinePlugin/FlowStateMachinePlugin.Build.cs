// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FlowStateMachinePlugin : ModuleRules
{
    public FlowStateMachinePlugin(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[] {
				// ... add public include paths required here ...
			}
            );

        PrivateIncludePaths.AddRange(
            new string[] {
				// ... add other private include paths required here ...
			}
            );

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
				// ... add other public dependencies that you statically link with here ...
			}
            );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
				// ... add private dependencies that you statically link with here ...	
			}
            );

        // 只在編輯器建置時才加入 Settings 模組依賴
        if (Target.bBuildEditor == true)
        {
            // 因為 Settings 模組主要用於 StartupModule 的內部實現，放入 Private 比較合適
            PrivateDependencyModuleNames.Add("Settings");
        }

        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
				// ... add any modules that your module loads dynamically here ...
			}
            );
    }
}
