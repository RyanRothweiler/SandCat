// NOTE high means higher coordinates. low means 0
enum ui_anchor_type
{
	UIAnchor_High,
	UIAnchor_Middle,
	UIAnchor_Low,
};

struct ui_anchor
{
	ui_anchor_type VerticalAnchor;

	ui_anchor_type HorizontalAnchor;

	vector2 Offset;

	// window_info WindowInfo;

	bool32 VerticalStretch;
	bool32 HorizontalStretch;

	vector2 StretchOffset;

	// NOTE the streatching / anchoring overrides the scaling
	vector2 Scale;
};

enum LayoutType
{
	Layout_VerticalFill,

	Layout_HorizontalFit,
};

struct live_graph
{
	string Name;
	bool32 Claimed;

	// These are the graph points
	int32 Points[100];

	// This is the raw data.
	real64 RawData[100];
	real64 DataMin, DataMax;

	bool32 Showing;

	// If the graph is collecting new data
	bool32 Paused;

	real64 HistoricalLargest;
};

struct editor_window
{
	rect Rect;
	real64 NextVertPos;
	real64 NextHorPos;

	vector2 PrevMouseDragPos;
	string Title;

	LayoutType LayoutState;
	real64 HorLayoutVerAdvance;

	// When the layout state is changed, for the first frame, a layout pass is done, where the number of members is counted.
	// that count is then used to draw the members on the next frame.
	bool32 LayoutPass;
	int32 HorizontalMembersCount[100];
	int32 HorizontalLayoutsCount;
	// in the drawing pass, this is incremented each element which is drawn
	int32 HorizontalElementIndex;

	bool32 Collapsed;
	// NOTE this is used to hold the state of the nested window before it's parent window collapsed the nested window
	bool32 WasOpen;

	bool32 IsDragging;
	bool32 IsResizing;

	bool32 IsNested;

	int32 HotItemID;
	int32 SliderDragging;

	bool32 ShowingScrollbar;
	real64 ScrollPos;
	real64 ScrollOffset;
	real64 TotalContentHeight;
	real64 MouseScrollWeight;
};

rect
GetAnchoredRect(ui_anchor Anchor, rect ParentRect)
{
	rect FinalRect = {};

	real64 ParentWidth = ParentRect.BottomRight.X - ParentRect.TopLeft.X;
	real64 ParentHeight = ParentRect.BottomRight.Y - ParentRect.TopLeft.Y;

	vector2 HalfSize = Anchor.Scale / 2.0f;
	vector2 HalfWindow = vector2{ParentWidth / 2.0f, ParentHeight / 2.0f};

	if (Anchor.VerticalAnchor == UIAnchor_High)
	{
		FinalRect.TopLeft.Y = ParentHeight - Anchor.Scale.Y;
		FinalRect.BottomRight.Y = ParentHeight;
	}
	else if (Anchor.VerticalAnchor == UIAnchor_Middle)
	{
		FinalRect.TopLeft.Y = HalfWindow.Y - HalfSize.Y;
		FinalRect.BottomRight.Y = HalfWindow.Y + HalfSize.Y;
	}
	else if (Anchor.VerticalAnchor == UIAnchor_Low)
	{
		FinalRect.TopLeft.Y = 0;
		FinalRect.BottomRight.Y = Anchor.Scale.Y;
	}

	if (Anchor.HorizontalAnchor == UIAnchor_High)
	{
		FinalRect.TopLeft.X = ParentWidth - Anchor.Scale.X;
		FinalRect.BottomRight.X = ParentWidth;
	}
	else if (Anchor.HorizontalAnchor == UIAnchor_Middle)
	{
		FinalRect.TopLeft.X = HalfWindow.X - HalfSize.X;
		FinalRect.BottomRight.X = HalfWindow.X + HalfSize.X;
	}
	else if (Anchor.HorizontalAnchor == UIAnchor_Low)
	{
		FinalRect.TopLeft.X = 0;
		FinalRect.BottomRight.X = Anchor.Scale.X;
	}

	FinalRect.TopLeft = FinalRect.TopLeft + Anchor.Offset;
	FinalRect.BottomRight = FinalRect.BottomRight + Anchor.Offset;

	if (Anchor.VerticalStretch)
	{
		FinalRect.TopLeft.Y = 0;
		FinalRect.BottomRight.Y = ParentHeight;

		if (Anchor.VerticalAnchor == UIAnchor_High)
		{
			FinalRect.TopLeft.Y += Anchor.StretchOffset.Y;
		}

		if (Anchor.VerticalAnchor == UIAnchor_Low)
		{
			FinalRect.BottomRight.Y -= Anchor.StretchOffset.Y;
		}
	}

	if (Anchor.HorizontalStretch)
	{
		FinalRect.TopLeft.X = 0;
		FinalRect.BottomRight.X = ParentWidth;

		if (Anchor.HorizontalAnchor == UIAnchor_High)
		{
			FinalRect.TopLeft.X += Anchor.StretchOffset.X;
		}

		if (Anchor.HorizontalAnchor == UIAnchor_Low)
		{
			FinalRect.BottomRight.X -= Anchor.StretchOffset.X;
		}
	}

	FinalRect.TopLeft = FinalRect.TopLeft + ParentRect.TopLeft;
	FinalRect.BottomRight = FinalRect.BottomRight + ParentRect.TopLeft;

	return (FinalRect);
}