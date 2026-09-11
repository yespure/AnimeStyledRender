using UnrealBuildTool;

public class FaceSDFEditor : ModuleRules
{
    public FaceSDFEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "UnrealEd",
                "Slate",
                "SlateCore",
                "ToolMenus",
                "PropertyEditor",
                "InputCore"
            }
        );
    }
}