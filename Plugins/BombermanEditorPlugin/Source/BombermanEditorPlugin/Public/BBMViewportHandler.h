#pragma once

#include "EditorComponents.h"
#include "EditorViewportClient.h"
#include "IBBMWorldPresetEditor.h"
#include "SEditorViewport.h"
#include "SLevelViewport.h"
#include "Tickable.h"
#include "Widgets/SViewport.h"
#include "Slate/SceneViewport.h"
#include "Templates/SharedPointer.h"
#include "Editor/UnrealEd/Public/SCommonEditorViewportToolbarBase.h"




class FBBMViewportClient;
class FAdvancedPreviewScene;



class  SBBMEditorViewport : public SEditorViewport, public ICommonEditorViewportToolbarInfoProvider
{
	SLATE_BEGIN_ARGS(SBBMEditorViewport)
	{
	}
	SLATE_END_ARGS()
public:

	SBBMEditorViewport();
	~SBBMEditorViewport();


	virtual void Construct(const FArguments& InArgs);

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

	virtual TSharedRef<FEditorViewportClient> MakeEditorViewportClient() override;


	virtual void SetWorld(UWorld* InWorld);

	virtual void SetSceneViewport(TSharedPtr<FSceneViewport> InSceneViewport);
	
	virtual void SetViewportClient(TSharedPtr<FBBMViewportClient> InViewportClient);
	
	virtual void SetClient(TSharedPtr<FBBMViewportClient> InViewportClient);


	virtual TSharedRef<class SEditorViewport> GetViewportWidget() override;
	virtual TSharedPtr<FExtender> GetExtenders() const override;
	virtual void OnFloatingButtonClicked() override;

	void BindCommands() override;
	void OnFocusViewportToSelection() override;

	TSharedPtr<class FBBMViewportClient> ViewportClient;
	TSharedPtr<FAdvancedPreviewScene> PreviewScene;
	UWorld* World;

};


class  FBBMViewportClient : public FEditorViewportClient, public TSharedFromThis<FBBMViewportClient>
{
public:

	FBBMViewportClient(const TSharedRef<SBBMEditorViewport>& InBBMEditorViewport, const TSharedRef<FAdvancedPreviewScene>& InPreviewScene, UWorld* InWorld);
	~FBBMViewportClient();

	virtual void Tick(float DeltaSeconds) override;
	virtual void Draw(const FSceneView* View, FPrimitiveDrawInterface* PDI) override;
	virtual void DrawCanvas(FViewport& InViewport, FSceneView& View, FCanvas& Canvas) override;

	virtual bool InputKey(const FInputKeyEventArgs& EventArgs) override;

	virtual void SetWorld(UWorld* InWorld);


	float DeltaTime;
	UWorld* World;
	float GridSize;
	int32 CellSize;

	
	TSharedPtr<FAdvancedPreviewScene> AdvancedPreviewScene;
	TSharedPtr<SBBMEditorViewport> BBMEditorViewportPtr;
	FEditorCommonDrawHelper DrawHelper;


};






