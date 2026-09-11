#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "ToolMenus.h"
#include "Framework/Application/SlateApplication.h"
#include "Widgets/SWindow.h"
#include "SFaceSDFGeneratorWindow.h"

#define LOCTEXT_NAMESPACE "FaceSDFEditor"

class FFaceSDFEditorModule : public IModuleInterface
{
public:
    virtual void StartupModule() override
    {
        UToolMenus::RegisterStartupCallback(
            FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FFaceSDFEditorModule::RegisterMenus)//加载用事件
        );
    }

    virtual void ShutdownModule() override
    {
        UToolMenus::UnRegisterStartupCallback(this);
        if (UToolMenus* ToolMenus = UToolMenus::Get())
        {
            ToolMenus->UnregisterOwner(this);
        }
    }

private:
    void RegisterMenus()
    {
        UToolMenus* ToolMenus = UToolMenus::Get();
        if (!ToolMenus) return;

        // 获取主菜单栏
        UToolMenu* MenuBar = ToolMenus->ExtendMenu(TEXT("LevelEditor.MainMenu"));
        if (!MenuBar) return;

        // 主菜单区块创建
        FToolMenuSection& Section = MenuBar->AddSection(TEXT("FaceSDFSection"), LOCTEXT("FaceSDFSection", "Face SDF"));

      //子菜单
        Section.AddSubMenu(
            TEXT("FaceSDF"),//Tag
            LOCTEXT("FaceSDF", "Face SDF"),
            LOCTEXT("FaceSDFTooltip", "Face SDF Tools"),
            //调用函数
            FNewToolMenuDelegate::CreateRaw(this, &FFaceSDFEditorModule::FillMenu)
        );
    }

    void FillMenu(UToolMenu* InMenu)
    {
        FToolMenuSection& Section = InMenu->AddSection(TEXT("FaceSDFOptions"), LOCTEXT("FaceSDFOptions", "Options"));

        Section.AddMenuEntry(
            TEXT("OpenGenerator"),
            LOCTEXT("OpenGenerator", "Open Generator"),
            LOCTEXT("OpenGeneratorTooltip", "Open Face SDF Generator"),
            FSlateIcon(),
            //打开后内容
            FUIAction(FExecuteAction::CreateRaw(this, &FFaceSDFEditorModule::OpenGeneratorWindow))
        );
    }

    void OpenGeneratorWindow()
    {
        TSharedRef<SWindow> Window = SNew(SWindow)
            .Title(LOCTEXT("FaceSDFGeneratorTitle", "Face SDF Generator"))
            .ClientSize(FVector2D(720.0f, 480.0f))
            [
                SNew(SFaceSDFGeneratorWindow)
            ];

        FSlateApplication::Get().AddWindow(Window);
    }
};

IMPLEMENT_MODULE(FFaceSDFEditorModule, FaceSDFEditor)

#undef LOCTEXT_NAMESPACE