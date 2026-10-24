using UnrealBuildTool;

public class DarkWorldEditor : ModuleRules
{
	public DarkWorldEditor(ReadOnlyTargetRules target) : base(target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"DarkWorld",
			"Slate",
			"SlateCore",
			"PropertyEditor",
			"UnrealEd",
			"Kismet"
		});
	}
}
