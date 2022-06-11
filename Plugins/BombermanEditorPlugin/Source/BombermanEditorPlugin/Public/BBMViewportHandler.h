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





class FBBMViewportClient;
class FAdvancedPreviewScene;

class SBBMViewportWidget : public SViewport
{
	SLATE_BEGIN_ARGS(SBBMViewportWidget)
	{
	}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

	void SetSceneViewport(TSharedPtr<FSceneViewport> InSceneViewport);

	void SetViewportClient(TSharedPtr<FBBMViewportClient> InViewportClient);


public:
	TWeakPtr<FSceneViewport> SceneViewport;
	TWeakPtr<FBBMViewportClient> ViewportClient;
};



class  SBBMEditorViewport : public SEditorViewport
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

	virtual TSharedPtr<SViewport> GetViewportWidget();

	virtual void SetWorld(UWorld* InWorld);

	virtual void SetSceneViewport(TSharedPtr<FSceneViewport> InSceneViewport);
	
	virtual void SetViewportClient(TSharedPtr<FBBMViewportClient> InViewportClient);
	
	virtual void SetClient(TSharedPtr<FBBMViewportClient> InViewportClient);


	TSharedPtr<FBBMViewportClient>ViewportClient;

	TSharedPtr<FPreviewScene> PreviewScene;
	UWorld* World;

};


class  FBBMViewportClient : public FEditorViewportClient, public TSharedFromThis<FBBMViewportClient>
{
public:

	FBBMViewportClient(const TSharedPtr<SBBMEditorViewport>& InBBMEditorViewport, TSharedRef<FAdvancedPreviewScene> InPreviewScene, UWorld* InWorld);
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






