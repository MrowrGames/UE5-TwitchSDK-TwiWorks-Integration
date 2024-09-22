using System;
using System.IO;
using UnrealBuildTool;

public class TwitchSDK : ModuleRules
{
	public TwitchSDK(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		// find core library
		string coreLib = ModuleDirectory;
		while (true)
		{
			if (File.Exists(Path.Combine(coreLib, "core", "include", "r66.hpp")))
			{
				coreLib = Path.Combine(coreLib, "core");
				break;
			}

			var parent = Directory.GetParent(coreLib);
			if (parent == null)
				throw new Exception("TwitchSDK core library not found!");
			coreLib = parent.FullName;
		}

		Console.WriteLine("Using core library at " + coreLib);

		var coreInclude = Path.Combine(coreLib, "include");
		var coreSrc = Path.Combine(coreLib, "src");

		CppStandard = CppStandardVersion.Cpp17;
		bEnableExceptions = true;
		bUseRTTI = true;

		PublicIncludePaths.AddRange(
			new string[] {
				coreInclude
			}
			);

		PrivateIncludePaths.AddRange(
			new string[] {
				coreSrc
			}
			);


		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
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
				"HTTP",
				"WebSockets",
				"Json",
				"DeveloperSettings",
			}
			);


		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
