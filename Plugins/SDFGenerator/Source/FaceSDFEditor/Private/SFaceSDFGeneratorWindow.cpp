#include "SFaceSDFGeneratorWindow.h"

#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "Widgets/Input/SEditableTextBox.h"

// SObjectPropertyEntryBox 所在的头文件
#include "PropertyCustomizationHelpers.h"

#include "AssetRegistry/AssetData.h"
#include "Engine/StaticMesh.h"

#define LOCTEXT_NAMESPACE "SFaceSDFGeneratorWindow"

void SFaceSDFGeneratorWindow::Construct(const FArguments& InArgs)
{
    ChildSlot
        [
            SNew(SVerticalBox)

                // 标题
                + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(10.0f)
                [
                    SNew(STextBlock)
                        .Text(LOCTEXT("WindowTitle", "Face SDF Generator V1"))
                ]

                //Mesh部分
                + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(8.0f)
                [
                    SNew(SHorizontalBox)

                        + SHorizontalBox::Slot()
                        .AutoWidth()
                        .VAlign(VAlign_Center)
                        .Padding(0, 0, 10, 0)
                        [
                            SNew(STextBlock)
                                .Text(LOCTEXT("SelectMeshLabel", "Select Mesh:"))
                        ]

                        + SHorizontalBox::Slot()
                        .FillWidth(1.0f)
                        [
                            SNew(SObjectPropertyEntryBox)
                                .AllowedClass(UStaticMesh::StaticClass())
                                // static mesh only
                                .ObjectPath(this, &SFaceSDFGeneratorWindow::GetSelectedMeshAsset)
                                // 获取当前选中的资源
                                .OnObjectChanged(this, &SFaceSDFGeneratorWindow::OnMeshChanged)
                                // 改变时的回调
                        ]
                ]

            //LOD部分
            + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(8.0f)
                [
                    SNew(SHorizontalBox)

                        + SHorizontalBox::Slot()
                        .AutoWidth()
                        .VAlign(VAlign_Center)
                        .Padding(0, 0, 10, 0)
                        [
                            // ✅修复：LOCTEXT
                            SNew(STextBlock).Text(LOCTEXT("LODLabel", "LOD:"))
                        ]

                        + SHorizontalBox::Slot()
                        .FillWidth(1.0f)
                        [
                            SNew(SNumericEntryBox<int32>)
                                .Value(this, &SFaceSDFGeneratorWindow::GetLODValue)
                                .OnValueChanged(this, &SFaceSDFGeneratorWindow::OnLODChanged)
                                .MinValue(0)
                        ]
                ]

            //Section部分
            + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(8.0f)
                [
                    SNew(SHorizontalBox)

                        + SHorizontalBox::Slot()
                        .AutoWidth()
                        .VAlign(VAlign_Center)
                        .Padding(0, 0, 10, 0)
                        [
                            // ✅修复：LOCTEXT
                            SNew(STextBlock).Text(LOCTEXT("SectionLabel", "Section:"))
                        ]

                        + SHorizontalBox::Slot()
                        .FillWidth(1.0f)
                        [
                            SNew(SNumericEntryBox<int32>)
                                .Value(this, &SFaceSDFGeneratorWindow::GetSectionValue)
                                .OnValueChanged(this, &SFaceSDFGeneratorWindow::OnSectionChanged)
                                .MinValue(0)
                        ]
                ]

            //UVChannel部分
            + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(8.0f)
                [
                    SNew(SHorizontalBox)

                        + SHorizontalBox::Slot()
                        .AutoWidth()
                        .VAlign(VAlign_Center)
                        .Padding(0, 0, 10, 0)
                        [
                            // ✅修复：LOCTEXT
                            SNew(STextBlock).Text(LOCTEXT("UVChannelLabel", "UV Channel:"))
                        ]

                        + SHorizontalBox::Slot()
                        .FillWidth(1.0f)
                        [
                            SNew(SNumericEntryBox<int32>)
                                .Value(this, &SFaceSDFGeneratorWindow::GetUVChannelValue)
                                .OnValueChanged(this, &SFaceSDFGeneratorWindow::OnUVChannelChanged)
                                .MinValue(0)
                        ]
                ]

            //Resolution部分
            + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(8.0f)
                [
                    SNew(SHorizontalBox)

                        + SHorizontalBox::Slot()
                        .AutoWidth()
                        .VAlign(VAlign_Center)
                        .Padding(0, 0, 10, 0)
                        [
                            SNew(STextBlock).Text(LOCTEXT("ResolutionLabel", "Resolution:"))
                        ]

                        + SHorizontalBox::Slot()
                        .FillWidth(1.0f)
                        [
                            SNew(SNumericEntryBox<int32>)
                                .Value(this, &SFaceSDFGeneratorWindow::GetResolutionValue)
                                .OnValueChanged(this, &SFaceSDFGeneratorWindow::OnResolutionChanged)
                                .MinValue(64)
                        ]
                ]

            //OutputName部分
            + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(8.0f)
                [
                    SNew(SHorizontalBox)

                        + SHorizontalBox::Slot()
                        .AutoWidth()
                        .VAlign(VAlign_Center)
                        .Padding(0, 0, 10, 0)
                        [
                            SNew(STextBlock).Text(LOCTEXT("OutputNameLabel", "Output Name:"))
                        ]

                        + SHorizontalBox::Slot()
                        .FillWidth(1.0f)
                        [
                            SNew(SEditableTextBox)
                                .Text(this, &SFaceSDFGeneratorWindow::GetOutputNameText)
                                .OnTextChanged(this, &SFaceSDFGeneratorWindow::OnOutputNameChanged)
                        ]
                ]

            + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(20.0f)
                .HAlign(HAlign_Center)
                [
                    SNew(SButton)
                        .Text(LOCTEXT("GenerateBtn", "Generate SDF"))
                        .OnClicked(FOnClicked::CreateSP(this, &SFaceSDFGeneratorWindow::OnGenerateClicked))
                ]
        ];
}

FReply SFaceSDFGeneratorWindow::OnGenerateClicked()
{
    // 这里以后写真正的生成逻辑
    UE_LOG(LogTemp, Warning, TEXT("Face SDF: Generate Clicked. Mesh: %s, LOD: %d, Section: %d, UV: %d, Res: %d"),
        SelectedMesh.IsValid() ? *SelectedMesh->GetName() : TEXT("None"),
        LODIndex, SectionIndex, UVChannel, Resolution);

    return FReply::Handled();
}

void SFaceSDFGeneratorWindow::OnMeshChanged(const FAssetData& AssetData)
{
    // 当用户选择了一个 Mesh 时触发
    SelectedMesh = Cast<UStaticMesh>(AssetData.GetAsset());
    UE_LOG(LogTemp, Warning, TEXT("Mesh changed to: %s"), SelectedMesh.IsValid() ? *SelectedMesh->GetName() : TEXT("None"));
}

FString SFaceSDFGeneratorWindow::GetSelectedMeshAsset() const
{
    if (SelectedMesh.IsValid())
    {
        // 返回资源的路径字符串，UI 就会显示它
        return SelectedMesh->GetPathName();
    }

    return FString(); // 空字符串，UI 就会显示 None
}

TOptional<int32> SFaceSDFGeneratorWindow::GetLODValue() const
{
    return LODIndex;
}

void SFaceSDFGeneratorWindow::OnLODChanged(int32 NewValue)
{
    LODIndex = NewValue;
}

TOptional<int32> SFaceSDFGeneratorWindow::GetSectionValue() const
{
    return SectionIndex;
}

void SFaceSDFGeneratorWindow::OnSectionChanged(int32 NewValue)
{
    SectionIndex = NewValue;
}

TOptional<int32> SFaceSDFGeneratorWindow::GetUVChannelValue() const
{
    return UVChannel;
}

void SFaceSDFGeneratorWindow::OnUVChannelChanged(int32 NewValue)
{
    UVChannel = NewValue;
}

TOptional<int32> SFaceSDFGeneratorWindow::GetResolutionValue() const
{
    return Resolution;
}

void SFaceSDFGeneratorWindow::OnResolutionChanged(int32 NewValue)
{
    Resolution = NewValue;
}

// OutputName 文本双向绑定
FText SFaceSDFGeneratorWindow::GetOutputNameText() const
{
    return FText::FromString(OutputName);
}

void SFaceSDFGeneratorWindow::OnOutputNameChanged(const FText& NewText)
{
    OutputName = NewText.ToString();
}

#undef LOCTEXT_NAMESPACE