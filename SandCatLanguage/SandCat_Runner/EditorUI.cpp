// This UI library from now on is called Otter
// v 0.0.1

// const real64 Editor_TitleHeight = 20.0f;
const real64 Editor_TitleHeight = 50.0f;
// const real64 Editor_ElementGutter = 15.0f;
const real64 Editor_ElementGutter = 25.0f;
const real64 Editor_SideGutter = 20.0f;
const real64 Editor_ScrollbarWidth = 10.0f;
const color Editor_UncheckedColor = EditorColor_SecondaryDark;
const color Editor_CheckedColor = EditorColor_Pink;

uint64 ElementIDer;

uint64
GetElementID()
{
	ElementIDer++;
	return (ElementIDer);
}

void
SetHorLayoutVerAdvance(editor_window* Window, real64 Advance)
{
	if (Window->HorLayoutVerAdvance < Advance)
	{
		Window->HorLayoutVerAdvance = Advance;
	}
}

void
otter_CreateWindow(editor_window* Window, string Title)
{
	Window->Title = Title;

	Window->NextVertPos = Editor_TitleHeight + 13 - Window->ScrollOffset;
	Window->LayoutState = Layout_VerticalFill;
	Window->TotalContentHeight = Editor_TitleHeight + 5.0f;

	Window->HorizontalLayoutsCount = 0;
	Window->HorizontalElementIndex = 0;

	Window->IsNested = false;

	if (Window->Collapsed)
	{
		for (int index = 0; index < 100; index++)
		{
			Window->HorizontalMembersCount[index] = 0;
		}
	}
}

#if 0
// Need a way to agnostically be able to drag a window around. This is currently dependent on input specific stuff.

void
DrawWindow(editor_window* Window,
           loaded_image* DropDownTriImage, loaded_image* RightTriImage,
           game_input* Input, render_layer* RenderObjects, render_layer* StencilObjects, list_head* AlphabetBitmaps)
{
	rect WindowAreaRect = Window->Rect;
	WindowAreaRect.TopLeft = Window->Rect.TopLeft + vector2{0, Editor_TitleHeight};

	if (!Window->Collapsed && !Window->IsNested)
	{
		// NOTE RendeLayer_Editor is only used for the window background / title
		PushRenderSquare(&RenderObjects[RenderLayer_Editor], MakeGLRectangle(Window->Rect, EditorColor_MainDark));

		PushRenderSquare(&StencilObjects[RenderLayer_Editor], MakeGLRectangle(Window->Rect, COLOR_BLACK));
		PushRenderSquare(&StencilObjects[RenderLayer_EditorTwo], MakeGLRectangle(Window->Rect, COLOR_BLACK));
		PushRenderSquare(&StencilObjects[RenderLayer_EditorThree], MakeGLRectangle(Window->Rect, COLOR_BLACK));
	}

	color TitleColor = color{1.0f, 1.0f, 1.0f, 0.8f};

	// title bar
	{
		rect TitleBarRectTop = {};
		TitleBarRectTop.TopLeft = Window->Rect.TopLeft;
		TitleBarRectTop.BottomRight = vector2{Window->Rect.BottomRight.X, Window->Rect.TopLeft.Y + Editor_TitleHeight};
		PushRenderSquare(&RenderObjects[RenderLayer_EditorTwo], MakeGLRectangle(TitleBarRectTop, EditorColor_MainDark));

		FontRenderString(Window->Title, TitleBarRectTop.TopLeft + vector2{80, (Editor_TitleHeight / 2.0f) + 15.0f}, 0.35f, TitleColor,
		                 AlphabetBitmaps, &RenderObjects[RenderLayer_EditorThree]);

		if (Input->MouseLeft.OnDown && !Input->MouseLeft.Claimed)
		{
			rect TitleBarRect = {};
			TitleBarRect.TopLeft = Window->Rect.TopLeft + vector2{25, 0};
			TitleBarRect.BottomRight = vector2{Window->Rect.BottomRight.X, Window->Rect.TopLeft.Y + Editor_TitleHeight};
			// PushRenderSquare(&RenderObjects[RenderLayer_EditorThree], MakeGLRectangle(TitleBarRect, EditorColor_Pink), Memory);
			if (RectContains(TitleBarRect, Input->MousePos))
			{
				Input->MouseLeft.Claimed = true;
				Window->IsDragging = true;
				Window->PrevMouseDragPos = Input->MousePos;
			}

			rect DropdownRect = {};
			DropdownRect.TopLeft = Window->Rect.TopLeft;
			DropdownRect.BottomRight = Window->Rect.TopLeft + vector2{80, Editor_TitleHeight};
			// PushRenderSquare(&RenderObjects[RenderLayer_Top], MakeGLRectangle(DropdownRect, EditorColor_Pink), Memory);

			if (RectContains(DropdownRect, Input->MousePos))
			{
				Input->MouseLeft.Claimed = true;
				Window->Collapsed = !Window->Collapsed;
				Window->WasOpen = false;
			}
		}
	}

	gl_texture DropDownTri = {};
	DropDownTri.Image = DropDownTriImage;
	DropDownTri.Center = Window->Rect.TopLeft + vector2{27, 25};
	DropDownTri.Scale = vector2{30, 30};
	if (Window->Collapsed)
	{
		DropDownTri.RadiansAngle = PI / 2.0f;
	}
	DropDownTri.Color = TitleColor;
	PushRenderTexture(&RenderObjects[RenderLayer_EditorThree], &DropDownTri);
}

void DragWindow(editor_window* Window, game_input* Input, render_layer* RenderObjects)
{
	if (Window->IsDragging)
	{
		Input->MouseLeft.Claimed = true;
		if (Input->MouseLeft.OnUp)
		{
			Window->IsDragging = false;
		}

		vector2 MouseDelta = Input->MousePos - Window->PrevMouseDragPos;
		Window->Rect.TopLeft = Window->Rect.TopLeft + MouseDelta;
		Window->Rect.BottomRight = Window->Rect.BottomRight + MouseDelta;

		Window->PrevMouseDragPos = Input->MousePos;
	}
}
#endif

void
ForceLayoutPass(editor_window* ParentWindow)
{
	ParentWindow->LayoutPass = true;

	for (int index = 0; index < 100; index++)
	{
		ParentWindow->HorizontalMembersCount[index] = 0;
	}
}

#if 0
void ResizeWindow(editor_window* Window, loaded_image* RightTriImage, game_input* Input, render_layer* RenderObjects)
{
	if (!Window->Collapsed)
	{
		gl_texture ResizeTri = {};
		ResizeTri.Image = RightTriImage;
		ResizeTri.Scale = vector2{40, 40};
		ResizeTri.Center = Window->Rect.BottomRight -
		                   vector2{ResizeTri.Scale.X / 2.0f, ResizeTri.Scale.Y / 2.0f} -
		                   vector2{8.0f, 8.0f};
		ResizeTri.Color = EditorColor_Pink;
		PushRenderTexture(&RenderObjects[RenderLayer_EditorThree], &ResizeTri);

		real64 ResizeHitboxAdjustment = 10.0f;
		rect ResizeRect = {};
		ResizeRect.TopLeft = Window->Rect.BottomRight - vector2{ResizeTri.Scale.X + ResizeHitboxAdjustment, ResizeTri.Scale.Y + ResizeHitboxAdjustment};
		ResizeRect.BottomRight = Window->Rect.BottomRight;
		// PushRenderSquare(&RenderObjects[RenderLayer_EditorThree], MakeGLRectangle(ResizeRect, EditorColor_Pink), Memory);
		if (Input->MouseLeft.OnDown && !Input->MouseLeft.Claimed && RectContains(ResizeRect, Input->MousePos))
		{
			Input->MouseLeft.Claimed = true;
			Window->IsResizing = true;
			Window->PrevMouseDragPos = Input->MousePos;
		}

		if (Window->IsResizing)
		{
			if (Input->MouseLeft.OnUp)
			{
				Window->IsResizing = false;
			}

			vector2 MouseDelta = Input->MousePos - Window->PrevMouseDragPos;
			Window->Rect.BottomRight = Window->Rect.BottomRight + MouseDelta;

			Window->PrevMouseDragPos = Input->MousePos;

			real64 MinWindowWidth = 100.0f;
			real64 MinWindowHeight = 50.0f;

			real64 CurrWidth = Window->Rect.BottomRight.X - Window->Rect.TopLeft.X;
			real64 CurrHeight = Window->Rect.BottomRight.Y - Window->Rect.TopLeft.Y;

			if (CurrWidth < MinWindowWidth)
			{
				Window->Rect.BottomRight = vector2{Window->Rect.TopLeft.X + MinWindowWidth, Window->Rect.BottomRight.Y};
			}

			if (CurrHeight < MinWindowHeight)
			{
				Window->Rect.BottomRight = vector2{Window->Rect.BottomRight.X, Window->Rect.TopLeft.Y + MinWindowHeight};
			}
		}
	}
}
#endif

rect
GetNextVerticalRect(editor_window* Window, real64 RectHeight)
{
	rect FinalRect = {};
	if (Window->LayoutState == Layout_VerticalFill)
	{
		real64 WindowWidth = RectWidth(Window->Rect);

		ui_anchor BarAnchor = {};
		BarAnchor.VerticalAnchor = UIAnchor_Low;
		BarAnchor.HorizontalAnchor = UIAnchor_Middle;
		BarAnchor.Scale = vector2{WindowWidth, RectHeight};
		BarAnchor.StretchOffset = vector2{10, 0};

		rect BarRect = GetAnchoredRect(BarAnchor, Window->Rect);

		BarRect.TopLeft = BarRect.TopLeft + vector2{0, Window->NextVertPos};
		BarRect.BottomRight = BarRect.BottomRight + vector2{0, Window->NextVertPos};

		if (Window->ShowingScrollbar)
		{
			BarRect.BottomRight = BarRect.BottomRight - vector2{Editor_ScrollbarWidth, 0};
		}

		BarRect.BottomRight = BarRect.BottomRight - vector2{Editor_SideGutter, 0};
		BarRect.TopLeft = BarRect.TopLeft + vector2{Editor_SideGutter, 0};

		FinalRect = BarRect;
	}
	else if (Window->LayoutState == Layout_HorizontalFit)
	{
		real64 WindowWidth = RectWidth(Window->Rect);
		real64 ElementWidth = WindowWidth / Window->HorizontalMembersCount[Window->HorizontalLayoutsCount];

		ui_anchor ElementAnchor = {};
		ElementAnchor.VerticalAnchor = UIAnchor_Low;
		ElementAnchor.HorizontalAnchor = UIAnchor_Low;
		ElementAnchor.Scale = vector2{ElementWidth, RectHeight};
		ElementAnchor.StretchOffset = vector2{10, 0};

		rect ElementRect = GetAnchoredRect(ElementAnchor, Window->Rect);

		real64 ElementHorGutter = 10.0f;
		ElementRect.TopLeft = ElementRect.TopLeft +
		                      vector2{(ElementWidth * Window->HorizontalElementIndex) + ElementHorGutter, Window->NextVertPos};
		ElementRect.BottomRight = ElementRect.BottomRight +
		                          vector2{ElementWidth * Window->HorizontalElementIndex - ElementHorGutter, Window->NextVertPos};

		// real64 EdgeAdjustment = 4.0f;
		if (Window->HorizontalElementIndex == 0)
		{
			ElementRect.TopLeft = ElementRect.TopLeft - vector2{ElementHorGutter - Editor_SideGutter, 0};
		}
		if (Window->HorizontalElementIndex == Window->HorizontalMembersCount[Window->HorizontalLayoutsCount] - 1)
		{
			ElementRect.BottomRight = ElementRect.BottomRight + vector2{ElementHorGutter - Editor_SideGutter, 0};
		}

		if (Window->ShowingScrollbar)
		{
			ElementRect.BottomRight = ElementRect.BottomRight - vector2{Editor_ScrollbarWidth, 0};
		}

		Window->HorizontalElementIndex++;
		FinalRect = ElementRect;
	}

	return (FinalRect);
}

void
AdvanceLayout(editor_window* Window, real64 AmountToAdvance)
{
	switch (Window->LayoutState)
	{
	case Layout_VerticalFill :
		Window->NextVertPos += AmountToAdvance;
		Window->TotalContentHeight += AmountToAdvance;
		break;

	case Layout_HorizontalFit :
		Window->NextHorPos += AmountToAdvance;
		break;
	}
}

void
AdvanceVerticalLayout(editor_window* Window, real64 AmountToAdvance)
{
	Window->NextVertPos += AmountToAdvance;
	Window->TotalContentHeight += AmountToAdvance;
}

#if 0
// NOTE returns button OnPressed.
bool32
AddButton(editor_window* ParentWindow, string ButtonName, real64 ButtonHeight,
          game_input* Input, list_head* AlphabetBitmaps, render_layer* RenderObjects)
{
	bool32 ReturnState = false;
	int32 ButtonID = (int32)GetElementID();

	if (!ParentWindow->Collapsed)
	{
		if (ParentWindow->LayoutPass)
		{
			ParentWindow->HorizontalMembersCount[ParentWindow->HorizontalLayoutsCount]++;
		}
		else
		{
			rect ButtonRect = GetNextVerticalRect(ParentWindow, ButtonHeight);
			// PushRenderSquare(&RenderObjects[RenderLayer_Top], MakeGLRectangle(ButtonRect, color{1.0f, 0.0f, 0.0f, 0.3f}), Memory);

			AdvanceLayout(ParentWindow, Editor_ElementGutter + ButtonHeight);
			SetHorLayoutVerAdvance(ParentWindow, 60.0f);

			real32 StateAlpha = 1.0f;
			if (RectContains(ButtonRect, Input->MousePos))
			{
				StateAlpha = 0.8f;
				if (Input->MouseLeft.OnDown)
				{
					StateAlpha = 0.6f;
					ParentWindow->HotItemID = ButtonID;
				}
				if (Input->MouseLeft.OnUp && ParentWindow->HotItemID == ButtonID)
				{
					ReturnState = true;
				}
			}

			if (Input->TouchStatus[0].OnDown && RectContains(ButtonRect, Input->TouchPosition[0]))
			{
				ReturnState = true;
			}

			color FinalButtonColor = EditorColor_Blue;
			FinalButtonColor.A = StateAlpha;
			PushRenderSquare(&RenderObjects[RenderLayer_EditorTwo], MakeGLRectangle(ButtonRect, FinalButtonColor));

			color FinalWordColor = COLOR_WHITE;
			FinalWordColor.A = StateAlpha;
			real64 ButtonWidth = RectWidth(ButtonRect);
			FontRenderString(ButtonName, ButtonRect.TopLeft + vector2{ButtonWidth / 2.0f, (ButtonHeight / 2.0f) + 12.0f}, 0.30f, FinalWordColor, StringAlign_Middle,
			                 AlphabetBitmaps, &RenderObjects[RenderLayer_EditorTwo]);

			if (!RectContains(ParentWindow->Rect, ButtonRect.TopLeft) && !RectContains(ParentWindow->Rect, ButtonRect.TopLeft))
			{
				ReturnState = false;
			}

		}
	}

	return (ReturnState);
}
#endif

// TODO remove the drop down triangle image and just construct it with vertices.
void
NestWindow(editor_window* ParentWindow, editor_window* ChildWindow,
           loaded_image* DropDownTriImage, loaded_image* RightTriImage,
           game_input* Input, list_head* AlphabetBitmaps, render_layer* RenderObjects, render_layer* StencilObjects)
{
	ChildWindow->IsNested = true;

	if (!ParentWindow->Collapsed)
	{
		if (ChildWindow->Collapsed && ChildWindow->WasOpen)
		{
			ChildWindow->Collapsed = false;
		}

		// window drawing
		{
			real64 WindowHeight = RectHeight(ChildWindow->Rect);
			rect BarRect = GetNextVerticalRect(ParentWindow, WindowHeight);

			ChildWindow->Rect = BarRect;

			real64 PrevBottomRightX = ChildWindow->Rect.BottomRight.X;
			// ResizeWindow(ChildWindow, RightTriImage, Input, RenderObjects, Memory);
			// SetWindowHeightToContent(ChildWindow);

			ChildWindow->Rect.BottomRight.X = PrevBottomRightX;

			rect OutlineRect = GetOutlineRect(ChildWindow->Rect, 2.0f);
			if (ChildWindow->Collapsed)
			{
				OutlineRect.BottomRight.Y = OutlineRect.TopLeft.Y + Editor_TitleHeight + 4.0f;
			}

			PushRenderSquare(&RenderObjects[RenderLayer_EditorTwo], MakeGLRectangle(OutlineRect, EditorColor_SecondaryDark));
			PushRenderSquare(&RenderObjects[RenderLayer_EditorTwo], MakeGLRectangle(GetOutlineRect(OutlineRect, -5.0f), EditorColor_MainDark));
			DrawWindow(ChildWindow, DropDownTriImage, RightTriImage, Input, RenderObjects, StencilObjects, AlphabetBitmaps);

			AdvanceVerticalLayout(ParentWindow, Editor_ElementGutter + RectHeight(OutlineRect) - 4.0);
		}
	}

	if (ParentWindow->Collapsed && !ChildWindow->Collapsed)
	{
		ChildWindow->Collapsed = true;
		ChildWindow->WasOpen = true;
	}
}

// returns which radio button is down
int32
AddRadioButtons(string RadioLabels[], int32 LabelsCount, int32 CurrSelection, string Title, editor_window* ParentWindow,
                loaded_image* CircleImage,
                game_input* Input, list_head* AlphabetBitmaps, render_layer* RenderObjects)
{
	int32 NextIndex = CurrSelection;
	if (!ParentWindow->Collapsed)
	{
		if (ParentWindow->LayoutPass)
		{
			ParentWindow->HorizontalMembersCount[ParentWindow->HorizontalLayoutsCount]++;
		}
		else
		{
			rect AreaRect = GetNextVerticalRect(ParentWindow, 20);
			SetHorLayoutVerAdvance(ParentWindow, 150.0f);

			vector2 TitlePos = AreaRect.TopLeft + vector2{0.0f, 25.0f};
			FontRenderString(Title, TitlePos, 0.4f, COLOR_WHITE,
			                 AlphabetBitmaps, &RenderObjects[RenderLayer_EditorTwo]);

			rect UnderlineRect = {};
			UnderlineRect.TopLeft = TitlePos + vector2{0, 4.0f};
			UnderlineRect.BottomRight = vector2{AreaRect.BottomRight.X, UnderlineRect.TopLeft.Y - 2.0f};
			// PushRenderSquare(&RenderObjects[RenderLayer_Editor], MakeGLRectangle(UnderlineRect, EditorColor_SecondaryDark), Memory);

			real64 LineSize = 50.0f;
			for (int32 RadioIndex = 0; RadioIndex < LabelsCount; RadioIndex++)
			{
				vector2 BackgroundCirclePos = vector2{AreaRect.TopLeft.X + 25.0f, 50.0f + UnderlineRect.BottomRight.Y + (RadioIndex * LineSize)};

				if (RectContains(ParentWindow->Rect, BackgroundCirclePos))
				{
					gl_texture BackgroundCircle = {};
					BackgroundCircle.Image = CircleImage;
					BackgroundCircle.Center = BackgroundCirclePos;
					BackgroundCircle.Scale = vector2{45, 45};
					BackgroundCircle.Color = Editor_UncheckedColor;
					PushRenderTexture(&RenderObjects[RenderLayer_EditorTwo], &BackgroundCircle);

					if (CurrSelection == RadioIndex)
					{
						gl_texture ForegroundCircle = {};
						ForegroundCircle.Image = CircleImage;
						ForegroundCircle.Center = BackgroundCircle.Center;
						ForegroundCircle.Scale = vector2{BackgroundCircle.Scale.X / 2.0f, BackgroundCircle.Scale.X / 2.0f};
						ForegroundCircle.Color = Editor_CheckedColor;
						PushRenderTexture(&RenderObjects[RenderLayer_EditorTwo], &ForegroundCircle);
					}

					FontRenderString(RadioLabels[RadioIndex], BackgroundCircle.Center + vector2{50.0f, 13.0f}, 0.3f, COLOR_WHITE,
					                 AlphabetBitmaps, &RenderObjects[RenderLayer_EditorTwo]);

					real64 WordWidth = GetWordPixelWidth(RadioLabels[RadioIndex], AlphabetBitmaps) * 0.065f;

					rect ButtonRect = {};
					ButtonRect.TopLeft = BackgroundCircle.Center - (BackgroundCircle.Scale);
					ButtonRect.BottomRight = BackgroundCircle.Center + (BackgroundCircle.Scale) + vector2{WordWidth + 20.0f, 0};
					// PushRenderSquare(&RenderObjects[RenderLayer_EditorTwo], MakeGLRectangle(ButtonRect, EditorColor_Pink));
					if (RectContains(ButtonRect, Input->MousePos) && Input->MouseLeft.OnDown)
					{
						Input->MouseLeft.Claimed = true;
						NextIndex = RadioIndex;
					}
				}
			}

			AdvanceLayout(ParentWindow, Editor_ElementGutter + (LineSize * LabelsCount) + 40.0f);
		}
	}

	return (NextIndex);
}

// NOTE all of this logic is duplicated from radio buttons logic. Basically only the rendering is different.
// int32
// AddSelectFromGroup(editor_window* ParentWindow, string RadioLabels[], int32 LabelsCount, int32 CurrSelection, string Title, real64 Height,
//                    game_input* Input, list_head* AlphabetBitmaps, render_layer* RenderObjects)
// {
// 	int32 NextIndex = CurrSelection;
// 	if (!ParentWindow->Collapsed)
// 	{
// 		if (ParentWindow->LayoutPass)
// 		{
// 			ParentWindow->HorizontalMembersCount[ParentWindow->HorizontalLayoutsCount]++;
// 		}
// 		else
// 		{
// 			rect AreaRect = GetNextVerticalRect(ParentWindow, Height);
// 			SetHorLayoutVerAdvance(ParentWindow, Height + Editor_ElementGutter);

// 			PushRenderSquare(&RenderObjects[RenderLayer_EditorTwo], MakeGLRectangle(AreaRect, EditorColor_SecondaryDark));
// 			PushRenderSquare(&RenderObjects[RenderLayer_EditorTwo], MakeGLRectangle(GetOutlineRect(AreaRect, -5.0f), EditorColor_MainDark));

// 			// vector2 TitlePos = AreaRect.TopLeft + vector2{0.0f, 25.0f};
// 			// FontRenderString(Title, TitlePos, 0.4f, COLOR_WHITE,
// 			//                  AlphabetBitmaps, &RenderObjects[RenderLayer_EditorTwo]);

// 			// rect UnderlineRect = {};
// 			// UnderlineRect.TopLeft = TitlePos + vector2{0, 4.0f};
// 			// UnderlineRect.BottomRight = vector2{AreaRect.BottomRight.X, UnderlineRect.TopLeft.Y - 2.0f};
// 			// // PushRenderSquare(&RenderObjects[RenderLayer_Editor], MakeGLRectangle(UnderlineRect, EditorColor_SecondaryDark), Memory);

// 			real64 LineSize = 50.0f;
// 			for (int32 RadioIndex = 0; RadioIndex < LabelsCount; RadioIndex++)
// 			{
// 				vector2 NextPosCenter = vector2{AreaRect.TopLeft.X + 25.0f, AreaRect.TopLeft.Y + 50.0f + (RadioIndex * LineSize)};
// 				// 	gl_texture BackgroundCircle = {};
// 				// 	BackgroundCircle.Image = CircleImage;
// 				// BackgroundCircle.Center =
// 				// 	BackgroundCircle.Scale = vector2{45, 45};
// 				// 	BackgroundCircle.Color = Editor_UncheckedColor;
// 				// 	PushRenderTexture(&RenderObjects[RenderLayer_EditorTwo], &BackgroundCircle);

// 				// 	if (CurrSelection == RadioIndex)
// 				// 	{
// 				// 		gl_texture ForegroundCircle = {};
// 				// 		ForegroundCircle.Image = CircleImage;
// 				// 		ForegroundCircle.Center = BackgroundCircle.Center;
// 				// 		ForegroundCircle.Scale = vector2{BackgroundCircle.Scale.X / 2.0f, BackgroundCircle.Scale.X / 2.0f};
// 				// 		ForegroundCircle.Color = Editor_CheckedColor;
// 				// 		PushRenderTexture(&RenderObjects[RenderLayer_EditorTwo], &ForegroundCircle);
// 				// 	}

// 				FontRenderString(RadioLabels[RadioIndex], NextPosCenter + vector2{50.0f, 13.0f}, 0.3f, COLOR_WHITE,
// 				                 AlphabetBitmaps, &RenderObjects[RenderLayer_EditorTwo]);

// 				// 	real64 WordWidth = GetWordPixelWidth(RadioLabels[RadioIndex], AlphabetBitmaps) * 0.065f;

// 				// 	rect ButtonRect = {};
// 				// 	ButtonRect.TopLeft = BackgroundCircle.Center - (BackgroundCircle.Scale);
// 				// 	ButtonRect.BottomRight = BackgroundCircle.Center + (BackgroundCircle.Scale) + vector2{WordWidth + 20.0f, 0};
// 				// 	// PushRenderSquare(&RenderObjects[RenderLayer_EditorTwo], MakeGLRectangle(ButtonRect, EditorColor_Pink));
// 				// 	if (RectContains(ButtonRect, Input->MousePos) && Input->MouseLeft.OnDown)
// 				// 	{
// 				// 		Input->MouseLeft.Claimed = true;
// 				// 		NextIndex = RadioIndex;
// 				// 	}
// 			}

// 			// scrolling
// 			{
// 				real64 TotalHeight = LineSize * LabelsCount;
// 				ParentWindow->ScrollPos = DoScroller(TotalHeight, Height, Window->ScrollPos,
// 				                                     Window, AreaRet, Input, RenderObjects);
// 				// Window->ShowingScrollbar = true;
// 				real64 HeightNeeded = Abs(WindowHeight - Window->TotalContentHeight);
// 				Window->ScrollOffset = HeightNeeded * Window->ScrollPos;
// 			}

// 			AdvanceLayout(ParentWindow, Editor_ElementGutter + Height);
// 		}
// 	}

// 	return (NextIndex);
// }

bool32
AddCheckbox(editor_window* ParentWindow, string Label, bool32 prevValue,
            game_input* Input, list_head* AlphabetBitmaps, render_layer* RenderObjects)
{
	if (!ParentWindow->Collapsed)
	{
		if (ParentWindow->LayoutPass)
		{
			ParentWindow->HorizontalMembersCount[ParentWindow->HorizontalLayoutsCount]++;
		}
		else
		{
			rect AreaRect = GetNextVerticalRect(ParentWindow, 20);

			real64 SquareSize = 40;
			AdvanceLayout(ParentWindow, Editor_ElementGutter + SquareSize);
			SetHorLayoutVerAdvance(ParentWindow, 10.0f);

			vector2 SquareOffset = vector2{0.0f, 5.0f};
			rect SquareRect = {};
			SquareRect.TopLeft = AreaRect.TopLeft + SquareOffset;
			SquareRect.BottomRight = AreaRect.TopLeft + vector2{SquareSize, SquareSize} + SquareOffset;
			PushRenderSquare(&RenderObjects[RenderLayer_EditorTwo], MakeGLRectangle(SquareRect, Editor_UncheckedColor));

			if (prevValue)
			{
				rect CheckedRect = GetOutlineRect(SquareRect, -10.0f);
				PushRenderSquare(&RenderObjects[RenderLayer_EditorTwo], MakeGLRectangle(CheckedRect, Editor_CheckedColor));
			}

			FontRenderString(Label, SquareRect.TopLeft + vector2{75.0f, 35.0f}, 0.30f, COLOR_WHITE,
			                 AlphabetBitmaps, &RenderObjects[RenderLayer_EditorTwo]);

			real64 WordWidth = GetWordPixelWidth(Label, AlphabetBitmaps) * 0.065f;
			rect ButtonRect = SquareRect;
			ButtonRect.BottomRight = SquareRect.BottomRight + vector2{WordWidth + 20.0f, 0};
			// PushRenderSquare(&RenderObjects[RenderLayer_EditorTwo], MakeGLRectangle(ButtonRect, EditorColor_Pink), Memory);
			if (RectContains(ButtonRect, Input->MousePos) && Input->MouseLeft.OnDown)
			{
				return (!prevValue);
			}
		}
	}

	return (prevValue);
}

rect
DrawHorGuide(real64 VerticalOffset, rect AreaRect, render_layer* RenderLayer)
{
	real64 EdgeAdjustment = 10.0f;
	real64 GuideLineSize = 5.0f;
	rect GuideRect = {};
	GuideRect.TopLeft = vector2{AreaRect.TopLeft.X + EdgeAdjustment, AreaRect.TopLeft.Y + VerticalOffset};
	GuideRect.BottomRight = vector2{AreaRect.BottomRight.X - EdgeAdjustment, AreaRect.TopLeft.Y + VerticalOffset + GuideLineSize};
	PushRenderSquare(RenderLayer, MakeGLRectangle(GuideRect, EditorColor_SecondaryDark));

	return (GuideRect);
}


real64
SliderHandle(editor_window* ParentWindow, real64 CurrentValue, range TotalRange, range HorPosRange, rect GuideRect,
             game_input* Input, render_layer* RenderObjects)
{
	int32 ButtonID = (int32)GetElementID();

	real64 GuideLength = RectWidth(GuideRect);

	vector2 HandleCenter = vector2{0, (GuideRect.TopLeft.Y + GuideRect.BottomRight.Y) / 2.0f};
	real64 Percentage = GetPercentageInRange(CurrentValue, TotalRange);
	HandleCenter.X = GuideRect.TopLeft.X + (GuideLength * Percentage);

	real64 HandleWidth = 14.0f;
	real64 HandleHeight = 40.0f;
	rect HandleRect = {};
	HandleRect.TopLeft = HandleCenter - vector2{HandleWidth / 2.0f, HandleHeight / 2.0f};
	HandleRect.BottomRight = HandleCenter + vector2{HandleWidth / 2.0f, HandleHeight / 2.0f};
	rect HandleHitBox = GetOutlineRect(HandleRect, 15.0f);
	// PushRenderSquare(&RenderObjects[RenderLayer_EditorTwo], MakeGLRectangle(HandleHitBox, color{1.0f, 0.0f, 0.0f, 0.5f}));

	if (RectContains(HandleHitBox, Input->MousePos) && Input->MouseLeft.OnDown)
	{
		Input->MouseLeft.Claimed = true;
		ParentWindow->SliderDragging = ButtonID;
	}

	if (Input->MouseLeft.IsDown && ParentWindow->SliderDragging == ButtonID)
	{
		HandleCenter.X = Input->MousePos.X;
		HandleCenter.X = ClampValue(HorPosRange.Min, HorPosRange.Max, HandleCenter.X);
	}

	if (Input->MouseLeft.OnUp)
	{
		ParentWindow->SliderDragging = -1;
	}

	real64 HeldPercentage = Abs((GuideRect.BottomRight.X - HandleCenter.X) / GuideLength);
	CurrentValue = GetValueAtPercentage(HeldPercentage, TotalRange);

	return (CurrentValue);
}

real64
AddSlider(editor_window* ParentWindow, range Range, string Label, real64 CurrValue,
          loaded_image* CircleImage, game_input* Input, list_head* AlphabetBitmaps, render_layer* RenderObjects)
{
	if (!ParentWindow->Collapsed)
	{
		if (ParentWindow->LayoutPass)
		{
			ParentWindow->HorizontalMembersCount[ParentWindow->HorizontalLayoutsCount]++;
		}
		else
		{
			SetHorLayoutVerAdvance(ParentWindow, 80.0f);

			rect AreaRect = GetNextVerticalRect(ParentWindow, 20);
			// PushRenderSquare(&RenderObjects[RenderLayer_EditorThree], MakeGLRectangle(AreaRect, COLOR_RED), Memory);

			FontRenderString(Label + (string)CurrValue, AreaRect.TopLeft + vector2{10, 25.0f}, 0.3f, COLOR_WHITE,
			                 AlphabetBitmaps, &RenderObjects[RenderLayer_EditorTwo]);

			real64 GuideVerticalOffset = 35.0f;
			rect GuideRect = DrawHorGuide(GuideVerticalOffset, AreaRect, &RenderObjects[RenderLayer_EditorTwo]);
			real64 GuideLength = RectWidth(GuideRect);

			CurrValue = SliderHandle(ParentWindow, CurrValue, Range, range{GuideRect.TopLeft.X, GuideRect.BottomRight.X},
			                         GuideRect, Input, RenderObjects);

			vector2 HandleCenter = vector2{0, (GuideRect.TopLeft.Y + GuideRect.BottomRight.Y) / 2.0f};
			real64 MaxPercentage = GetPercentageInRange(CurrValue, Range);
			HandleCenter.X = GuideRect.TopLeft.X + (GuideLength * MaxPercentage);

			real64 HandleSize = 35.0f;

			gl_texture HandleCircle = {};
			HandleCircle.Image = CircleImage;
			HandleCircle.Center = HandleCenter;
			HandleCircle.Scale = vector2{HandleSize, HandleSize};
			HandleCircle.Color = EditorColor_Pink;
			PushRenderTexture(&RenderObjects[RenderLayer_EditorTwo], &HandleCircle);

			AdvanceLayout(ParentWindow, Editor_ElementGutter + GuideVerticalOffset + 70.0f);
		}
	}

	return (CurrValue);
}

real64
DoScroller(real64 HeightWanting, real64 HeightAvailable, real64 CurrentScrollPos,
           editor_window* Window, rect WindowRect,
           game_input* Input, render_layer* RenderObjects)
{
	int32 ID = (int32)GetElementID();

	real64 HeightNeeded = Abs(HeightAvailable - HeightWanting);

	real64 ScrollHandleLength = 20.0f;

	real64 WindowWidth = RectWidth(WindowRect);
	rect ScrollBG = {};
	ScrollBG.TopLeft = WindowRect.TopLeft + vector2{WindowWidth - Editor_ScrollbarWidth, Editor_TitleHeight};
	ScrollBG.BottomRight = WindowRect.BottomRight + vector2{0, - 25.0f - ScrollHandleLength};

	rect ScrollLine = ScrollBG;
	ScrollLine.BottomRight = vector2{ScrollLine.TopLeft.X + 2.0f, ScrollLine.BottomRight.Y + ScrollHandleLength};
	PushRenderSquare(&RenderObjects[RenderLayer_EditorTwo], MakeGLRectangle(ScrollLine, EditorColor_SecondaryDark));

	real64 ScrollYOffset = RectHeight(ScrollBG) * CurrentScrollPos;
	rect ScrollHandle = {};
	ScrollHandle.TopLeft = ScrollBG.TopLeft + vector2{0, ScrollYOffset};
	ScrollHandle.BottomRight = ScrollHandle.TopLeft + vector2{Editor_ScrollbarWidth, ScrollHandleLength};

	rect ScrollHitbox = GetOutlineRect(ScrollHandle, 2.0f);
	// PushRenderSquare(&RenderObjects[RenderLayer_EditorTwo], MakeGLRectangle(ScrollHitbox, EditorColor_Pink), Memory);
	if (RectContains(ScrollHitbox, Input->MousePos))
	{
		Window->HotItemID = ID;
	}

	if (Window->HotItemID == ID && Input->MouseLeft.IsDown)
	{
		Input->MouseLeft.Claimed = true;

		ScrollHandle.TopLeft.Y = Input->MousePos.Y - (ScrollHandleLength / 2.0f);
		ScrollHandle.BottomRight.Y = Input->MousePos.Y + (ScrollHandleLength / 2.0f);

		CurrentScrollPos = (ScrollHandle.TopLeft.Y - ScrollBG.TopLeft.Y) /  RectHeight(ScrollBG);
	}

	if (RectContains(Window->Rect, Input->MousePos) && !Input->ScrollClaimed)
	{
		real64 ScrollSpeed = 500;
		Window->ScrollPos -= (Input->MouseScrollDelta / ScrollSpeed) / (HeightNeeded) * 500.0f;
		Input->ScrollClaimed = true;
	}

	if (Input->MouseLeft.OnUp)
	{
		Window->HotItemID = -1;
	}
	CurrentScrollPos = ClampValue(0.0, 1.0f, Window->ScrollPos);

	ScrollYOffset = RectHeight(ScrollBG) * Window->ScrollPos;
	ScrollHandle.TopLeft = ScrollBG.TopLeft + vector2{0, ScrollYOffset};
	ScrollHandle.BottomRight = ScrollHandle.TopLeft + vector2{Editor_ScrollbarWidth, ScrollHandleLength};
	PushRenderSquare(&RenderObjects[RenderLayer_EditorTwo], MakeGLRectangle(ScrollHandle, EditorColor_Pink));

	return (CurrentScrollPos);
}

range
AddRangeSlider(editor_window* ParentWindow, range CurrentRange, range TotalRange, string Label,
               game_input* Input, list_head* AlphabetBitmaps, render_layer* RenderObjects, memory_arena* Memory)
{
	real64 SignAdj = 0.0f;

	if (!ParentWindow->Collapsed)
	{
		if (ParentWindow->LayoutPass)
		{
			ParentWindow->HorizontalMembersCount[ParentWindow->HorizontalLayoutsCount]++;
		}
		else
		{
			SetHorLayoutVerAdvance(ParentWindow, 80.0f);

			rect AreaRect = GetNextVerticalRect(ParentWindow, 20);
			// PushRenderSquare(&RenderObjects[RenderLayer_EditorThree], MakeGLRectangle(AreaRect, COLOR_RED), Memory);

			FontRenderString(Label, AreaRect.TopLeft + vector2{10, 25.0f}, 0.3f, COLOR_WHITE,
			                 AlphabetBitmaps, &RenderObjects[RenderLayer_EditorTwo]);

			real64 GuideVerticalOffset = 35.0f;
			rect GuideRect = DrawHorGuide(GuideVerticalOffset, AreaRect, &RenderObjects[RenderLayer_EditorTwo]);

			real64 VerticalOffset = 40.0f;
			FontRenderString(TotalRange.Min, AreaRect.TopLeft + vector2{10, GuideVerticalOffset + VerticalOffset}, 0.28f, COLOR_WHITE,
			                 AlphabetBitmaps, &RenderObjects[RenderLayer_EditorTwo]);

			real64 GuideLength = RectWidth(GuideRect);
			FontRenderString(TotalRange.Max, AreaRect.TopLeft + vector2{GuideLength + 10.0f, GuideVerticalOffset + VerticalOffset}, 0.28f, COLOR_WHITE, StringAlign_Right,
			                 AlphabetBitmaps, &RenderObjects[RenderLayer_EditorTwo]);

			vector2 MinHandleCenter = vector2{0, (GuideRect.TopLeft.Y + GuideRect.BottomRight.Y) / 2.0f};
			real64 MinPercentage = GetPercentageInRange(CurrentRange.Min, TotalRange);
			MinHandleCenter.X = GuideRect.TopLeft.X + (GuideLength * MinPercentage);

			vector2 MaxHandleCenter = vector2{0, (GuideRect.TopLeft.Y + GuideRect.BottomRight.Y) / 2.0f};
			real64 MaxPercentage = GetPercentageInRange(CurrentRange.Max, TotalRange);
			MaxHandleCenter.X = GuideRect.TopLeft.X + (GuideLength * MaxPercentage);

			rect InsideRect = {};
			real64 InsideSize = 2.0f;
			InsideRect.TopLeft = vector2{MinHandleCenter.X, GuideRect.TopLeft.Y - InsideSize};
			InsideRect.BottomRight = vector2{MaxHandleCenter.X, GuideRect.BottomRight.Y + InsideSize};
			PushRenderSquare(&RenderObjects[RenderLayer_EditorTwo], MakeGLRectangle(InsideRect, EditorColor_Pink));

			real64 TriangleHeight = 40.0f;
			real64 TriangleLength = 30.0f;
			real64 HorizontalSlide = 20.0f;
			gl_color_points* MaxPoints = &GetNextEntity(&RenderObjects[RenderLayer_EditorTwo], RenderEntity_ColorPoints)->ColorPoints;
			MaxPoints->PointsCount = 3;
			MaxPoints->Color = EditorColor_Pink;
			MaxPoints->Points = (vector2*)ArenaAllocate(Memory, sizeof(vector2) * MaxPoints->PointsCount);
			MaxPoints->Points[0] = MinHandleCenter - vector2{HorizontalSlide, TriangleHeight / 2.0f};
			MaxPoints->Points[1] = MinHandleCenter + vector2{ -HorizontalSlide, TriangleHeight / 2.0f};
			MaxPoints->Points[2] = MinHandleCenter + vector2{TriangleLength - HorizontalSlide, 0.0f};

			MaxPoints = &GetNextEntity(&RenderObjects[RenderLayer_EditorTwo], RenderEntity_ColorPoints)->ColorPoints;
			MaxPoints->PointsCount = 3;
			MaxPoints->Color = EditorColor_Pink;
			MaxPoints->Points = (vector2*)ArenaAllocate(Memory, sizeof(vector2) * MaxPoints->PointsCount);
			MaxPoints->Points[0] = MaxHandleCenter + vector2{HorizontalSlide, TriangleHeight / 2.0f};
			MaxPoints->Points[1] = MaxHandleCenter - vector2{ -HorizontalSlide, TriangleHeight / 2.0f};
			MaxPoints->Points[2] = MaxHandleCenter - vector2{TriangleLength - HorizontalSlide, 0.0f};

			// Min handle
			CurrentRange.Min =
			    SliderHandle(ParentWindow, CurrentRange.Min, TotalRange, range{GuideRect.TopLeft.X, MaxHandleCenter.X},
			                 GuideRect, Input, RenderObjects);

			// Max handle
			CurrentRange.Max =
			    SliderHandle(ParentWindow, CurrentRange.Max, TotalRange, range{MinHandleCenter.X, GuideRect.BottomRight.X},
			                 GuideRect, Input, RenderObjects);

			// Middle handle
			{
				rect MiddleHandleBox = rect{};
				real64 InnerMovement = 20.0f;
				real64 HandleHeight = 70.0f;
				MiddleHandleBox.TopLeft = MinHandleCenter + vector2{InnerMovement, HandleHeight / 2.0f};
				MiddleHandleBox.BottomRight = MaxHandleCenter - vector2{InnerMovement, HandleHeight / 2.0f};
				// PushRenderSquare(&RenderObjects[RenderLayer_EditorTwo], MakeGLRectangle(MiddleHandleBox, color{1.0f, 0.0f, 0.0f, 0.5f}));

				// if (RectContains(MiddleHandleBox, Input->MousePos) && Input->MouseLeft.OnDown)
				// {
				// 	Input->MouseLeft.Claimed = true;
				// 	ParentWindow->SliderDragging = MiddleHandleID;
				// }

				// if (Input->MouseLeft.IsDown && ParentWindow->SliderDragging == MiddleHandleID)
				// {
				// 	real64 HalfDist = (MaxHandleCenter.X - MinHandleCenter.X) / 2.0f;

				// 	MinHandleCenter.X = Input->MousePos.X - HalfDist;
				// 	MaxHandleCenter.X = Input->MousePos.X + HalfDist;
				// }

				// if (Input->MouseLeft.OnUp)
				// {
				// 	ParentWindow->SliderDragging = -1;
				// }

				// real64 MinHeldPercentage = Abs((GuideRect.BottomRight.X - MinHandleCenter.X) / GuideLength);
				// CurrentRange.Min = GetValueAtPercentage(MinHeldPercentage, TotalRange);

				// real64 MaxHeldPercentage = Abs((GuideRect.BottomRight.X - MinHandleCenter.X) / GuideLength);
				// CurrentRange.Max = GetValueAtPercentage(MaxHeldPercentage, TotalRange);
			}

			AdvanceLayout(ParentWindow, Editor_ElementGutter + GuideVerticalOffset + 500.0f);
		}
	}

	return (CurrentRange);

}

// NOTE this must come after all of the layout
void
ScrollWindow(editor_window* Window, int32 ID, game_input* Input, render_layer* RenderObjects)
{
	Window->ShowingScrollbar = false;
	Window->ScrollOffset = 0.0f;
	if (!Window->Collapsed)
	{
		real64 WindowHeight = RectHeight(Window->Rect);
		if (Window->TotalContentHeight > WindowHeight)
		{
			Window->ScrollPos = DoScroller(Window->TotalContentHeight, WindowHeight, Window->ScrollPos,
			                               Window, Window->Rect, Input, RenderObjects);
			Window->ShowingScrollbar = true;
			real64 HeightNeeded = Abs(WindowHeight - Window->TotalContentHeight);
			Window->ScrollOffset = HeightNeeded * Window->ScrollPos;
		}
		else
		{
			Window->ScrollPos = 0.0f;
		}
	}
}

void
AddLabel(string Label, editor_window* ParentWindow, render_layer* RenderObjects, list_head* AlphabetBitmaps, real64 FontSize = 0.3f)
{
	if (!ParentWindow->Collapsed)
	{
		if (ParentWindow->LayoutPass)
		{
			ParentWindow->HorizontalMembersCount[ParentWindow->HorizontalLayoutsCount]++;
		}
		else
		{
			real64 LayoutOffset = 0.0f;
			if (ParentWindow->LayoutState == Layout_HorizontalFit)
			{
				LayoutOffset = 25.0f;
			}

			rect AreaRect = GetNextVerticalRect(ParentWindow, 20);
			FontRenderString(Label, AreaRect.TopLeft + vector2{0, 20.0f + LayoutOffset}, FontSize, COLOR_WHITE,
			                 AlphabetBitmaps, &RenderObjects[RenderLayer_EditorTwo]);

			AdvanceLayout(ParentWindow, Editor_ElementGutter + 4.0f);
			SetHorLayoutVerAdvance(ParentWindow, 30.0f);

		}
	}
}

void
AddVerticalSpace(real64 SpaceAmount, editor_window* ParentWindow)
{
	AdvanceVerticalLayout(ParentWindow, SpaceAmount);
}

void
FitWindowToContent(editor_window* Window)
{
	Window->Rect.BottomRight.Y = Window->Rect.TopLeft.Y + Window->TotalContentHeight;
	// Window->CanResize = false;
}

void
BeginLayout(editor_window* Window, LayoutType LayoutType)
{
	Window->LayoutState = LayoutType;
	Window->HorLayoutVerAdvance = 0.0f;

	if (Window->LayoutState == Layout_HorizontalFit)
	{
		Window->NextHorPos = Editor_SideGutter;

		if (Window->HorizontalMembersCount[Window->HorizontalLayoutsCount] == 0)
		{
			Window->LayoutPass = true;
		}
	}
}

void
EndLayout(editor_window* Window)
{
	if (Window->LayoutState == Layout_HorizontalFit)
	{
		Window->HorizontalElementIndex = 0;
		Window->HorizontalLayoutsCount++;
		AdvanceVerticalLayout(Window, Window->HorLayoutVerAdvance + Editor_ElementGutter);
	}

	Window->LayoutPass = false;
	Window->LayoutState = Layout_VerticalFill;
}

rect
GetWindowRect(editor_window* Window, real64 Height)
{
	rect WindowRect = GetNextVerticalRect(Window, Height);
	AdvanceVerticalLayout(Window, Height + Editor_ElementGutter);
	return (WindowRect);
}

void
LiveGraph(editor_window* Window, real64 GraphHeight, live_graph* LiveGraph, int32 PastArrayCount, real64 NewData, bool32 Paused,
          render_layer* RenderObjects, list_head* AlphabetBitmaps, game_input* Input, memory_arena* Memory)
{
	if (!Window->Collapsed)
	{
		rect GraphRect = GetWindowRect(Window, GraphHeight);
		PushRenderSquare(&RenderObjects[RenderLayer_EditorThree], MakeGLRectangle(GraphRect, EditorColor_SecondaryDark));
		PushRenderSquare(&RenderObjects[RenderLayer_EditorThree], MakeGLRectangle(GetOutlineRect(GraphRect, -5), EditorColor_MainDark));

		int32 ArraySize = PastArrayCount;

		gl_color_points* Points = &GetNextEntity(&RenderObjects[RenderLayer_EditorThree], RenderEntity_ColorPoints)->ColorPoints;
		Points->PointsCount = (ArraySize * 2) - 1;
		Points->Points = (vector2*)ArenaAllocate(Memory, sizeof(vector2) * Points->PointsCount);
		Points->Color = EditorColor_LightBlue;

		int32 PointCounter = 0;
		real64 GraphBottom = GraphRect.BottomRight.Y;
		Points->Points[PointCounter] = vector2{GraphRect.TopLeft.X, GraphBottom};
		PointCounter++;

		// move data
		if (!Paused)
		{
			for (int32 Index = 1; Index < ArraySize; Index++)
			{
				LiveGraph->Points[Index - 1] = LiveGraph->Points[Index];
				LiveGraph->RawData[Index - 1] = LiveGraph->RawData[Index];
			}

			// int32 FPSPercent = (int32)(((real64)GameState->PrevFrameFPS / 60.0f) * 100.0f);
			real64 NewPercentage = (NewData / LiveGraph->DataMax) * 100.0f;
			NewPercentage = (int32)ClampValue((int64)0, (int64)100, (int64)NewPercentage);

			LiveGraph->Points[ArraySize - 1] = (int32)NewPercentage;
			LiveGraph->RawData[ArraySize - 1] = NewData;
		}

		bool32 MouseOver = RectContains(GraphRect, Input->MousePos);
		real64 ClosestData = 0;
		real64 ClosestDist = 100000;

		// render dots
		for (int32 Index = 1; Index < ArraySize; Index++)
		{
			// x
			real64 PointGap = RectWidth(GraphRect) / ArraySize;
			real64 CurrX = (Index * PointGap) + GraphRect.TopLeft.X;

			// y
			real64 CurrValue = 1.0 - (LiveGraph->Points[Index] / 100.0f);
			real64 PrevValue = LiveGraph->Points[Index - 1] / 100.0f;
			real64 TotalHeight = RectHeight(GraphRect);
			real64 CurrY = (CurrValue * TotalHeight) + GraphRect.TopLeft.Y;

			// together
			vector2 CurrCenter = vector2{CurrX, CurrY};

			real64 PointHeight = 3.0f;
			real64 PointWidth = 3.0f;
			rect PointRect = {};
			PointRect.TopLeft = CurrCenter - vector2{PointWidth, PointHeight};
			PointRect.BottomRight = CurrCenter + vector2{PointWidth, PointHeight};
			// PushRenderSquare(&RenderObjects[RenderLayer_EditorThree], MakeGLRectangle(PointRect, EditorColor_Blue), Memory);

			Points->Points[PointCounter] = CurrCenter;
			PointCounter++;
			Points->Points[PointCounter] = vector2{CurrCenter.X, GraphBottom};
			PointCounter++;

			Assert(PointCounter <= Points->PointsCount);

			if (MouseOver)
			{
				real64 ThisDist = Abs(Input->MousePos.X - CurrCenter.X);
				if (ThisDist < ClosestDist)
				{
					ClosestDist = ThisDist;
					ClosestData = LiveGraph->RawData[Index];
				}
			}
		}

		// mouse over effects
		if (MouseOver)
		{
			real64 XPos = Input->MousePos.X;
			real64 LineWidth = 3.0f;
			real64 HalfLine = LineWidth / 2.0f;

			rect LineRect = {};
			LineRect.TopLeft = vector2{XPos - HalfLine, GraphRect.TopLeft.Y};
			LineRect.BottomRight = vector2{XPos + HalfLine, GraphRect.BottomRight.Y};
			PushRenderSquare(&RenderObjects[RenderLayer_EditorThree], MakeGLRectangle(LineRect, EditorColor_Blue));

			FontRenderString((string)((int32)ClosestData), LineRect.TopLeft + vector2{0, 30.0f}, 0.3f, color{1.0f, 1.0f, 1.0f, 0.9f},
			                 AlphabetBitmaps, &RenderObjects[RenderLayer_EditorThree]);
		}


		if (NewData > LiveGraph->HistoricalLargest)
		{
			LiveGraph->HistoricalLargest = NewData;
		}
		// FontRenderString("Largest : " + (string)LiveGraph->HistoricalLargest, GraphRect.TopLeft + vector2{0, 0}, 0.28f, color{1.0f, 1.0f, 1.0f, 0.85f},
		// AlphabetBitmaps, &RenderObjects[RenderLayer_EditorThree]);
	}
}

void
TogglebleGraph(editor_window* Window, live_graph* Graph, real64 GraphHeight, string GraphTitle, real64 NextPoint,
               render_layer* RenderObjects, list_head* AlphabetBitmaps, memory_arena* Memory, game_input* GameInput)
{
	BeginLayout(Window, Layout_HorizontalFit);
	{
		AddLabel(GraphTitle,  Window, RenderObjects, AlphabetBitmaps);
		Graph->Showing = AddCheckbox(Window, "", Graph->Showing, GameInput, AlphabetBitmaps, RenderObjects);
	}
	EndLayout(Window);

	AdvanceLayout(Window, 10.0f);

	if (Graph->Showing)
	{
		LiveGraph(Window, GraphHeight, Graph, 100, NextPoint, Graph->Paused, RenderObjects, AlphabetBitmaps, GameInput, Memory);
	}
}

void
HorizontalSpacer(editor_window* ParentWindow)
{
	real64 SignAdj = 0.0f;

	if (!ParentWindow->Collapsed)
	{
		if (ParentWindow->LayoutPass)
		{
			ParentWindow->HorizontalMembersCount[ParentWindow->HorizontalLayoutsCount]++;
		}
		else
		{
			rect AreaRect = GetNextVerticalRect(ParentWindow, 20);
		}
	}
}

color
ColorSliders(editor_window* ParentWindow, string Title, color CurrColor,
             loaded_image* CircleImage, game_input* GameInput, list_head* AlphabetBitmaps, render_layer* RenderObjects)
{

	AddLabel(Title, ParentWindow, RenderObjects, AlphabetBitmaps);
	BeginLayout(ParentWindow, Layout_HorizontalFit);
	{
		// Red
		CurrColor.R = (real32)AddSlider(ParentWindow, range{0.0f, 1.0f}, "R ", CurrColor.R,
		                                CircleImage, GameInput, AlphabetBitmaps, RenderObjects);

		// Green
		CurrColor.G = (real32)AddSlider(ParentWindow, range{0.0f, 1.0f}, "G ", CurrColor.G,
		                                CircleImage, GameInput, AlphabetBitmaps, RenderObjects);

		// Blue
		CurrColor.B = (real32)AddSlider(ParentWindow, range{0.0f, 1.0f}, "B ", CurrColor.B,
		                                CircleImage, GameInput, AlphabetBitmaps, RenderObjects);

		// Alpha
		CurrColor.A = (real32)AddSlider(ParentWindow, range{0.0f, 1.0f}, "A ", CurrColor.A,
		                                CircleImage, GameInput, AlphabetBitmaps, RenderObjects);
	}
	EndLayout(ParentWindow);

	return (CurrColor);
}

void
ParticleSystemComponentEditor(editor_window* Window,
                              particle_system_component* Component, range RandomRange,
                              game_state* GameState, game_input* GameInput, game_memory* Memory)
{
	BeginLayout(Window, Layout_HorizontalFit);
	{
		AddLabel(Component->Name, Window, GameState->RenderObjects, GameState->Assets->AlphabetBitmaps, 0.45f);

		if (AddButton(Window, "Add Position", 60,
		              GameInput, GameState->Assets->AlphabetBitmaps, GameState->RenderObjects))
		{
			for (int32 Index = 0; Index < ARRAY_SIZE(Component->TimeValues, time_value); Index++)
			{
				if (!Component->TimeValues[Index].Using)
				{
					Component->TimeValues[Index].Using = true;
					ForceLayoutPass(Window);
					break;
				}
			}
		}
	}
	EndLayout(Window);

	for (int32 Index = 0; Index < ARRAY_SIZE(Component->TimeValues, time_value); Index++)
	{
		time_value* Value = &Component->TimeValues[Index];
		if (Value->Using)
		{
			BeginLayout(Window, Layout_HorizontalFit);
			{
				Value->IsRandomized =
				    AddCheckbox(Window, (string)Index + " - Ranged", Value->IsRandomized,
				                GameInput, GameState->Assets->AlphabetBitmaps, GameState->RenderObjects);

				if (Value->IsRandomized)
				{
					Value->RandomRange =
					    AddRangeSlider(Window, Value->RandomRange, RandomRange, "",
					                   GameInput, GameState->Assets->AlphabetBitmaps, GameState->RenderObjects, &Memory->TransientMemory);
				}
				else
				{
					Value->Value =
					    AddSlider(Window, RandomRange, "",
					              Value->Value, &GameState->Assets->CircleImage,
					              GameInput, GameState->Assets->AlphabetBitmaps, GameState->RenderObjects);
				}
			}
			EndLayout(Window);
		}
	}

	BeginLayout(Window, Layout_HorizontalFit);
	{
		for (int32 Index = 0; Index < ARRAY_SIZE(Component->TimeValues, time_value); Index++)
		{
			time_value* Value = &Component->TimeValues[Index];
			if (Value->Using)
			{
				if (AddButton(Window, "Delete - " + (string)Index, 60,
				              GameInput, GameState->Assets->AlphabetBitmaps, GameState->RenderObjects))
				{
					Value->Using = false;
					ForceLayoutPass(Window);
				}
			}
		}
	}
	EndLayout(Window);
}

// int32
// AddDropDown(editor_window* Window, string[] Options, int32 CurrentSelection
//             game_input* Input, list_head* AlphabetBitmaps, render_layer* RenderObjects)
// {
// 	int32 ButtonID = (int32)GetElementID();

// 	if (!ParentWindow->Collapsed)
// 	{
// 		if (ParentWindow->LayoutPass)
// 		{
// 			ParentWindow->HorizontalMembersCount[ParentWindow->HorizontalLayoutsCount]++;
// 		}
// 		else
// 		{
// 			rect ButtonRect = GetNextVerticalRect(ParentWindow, ButtonHeight);
// 			PushRenderSquare(&RenderObjects[RenderLayer_Top], MakeGLRectangle(ButtonRect, color{1.0f, 0.0f, 0.0f, 0.3f}), Memory);

// 			AdvanceLayout(ParentWindow, Editor_ElementGutter + ButtonHeight);
// 			SetHorLayoutVerAdvance(ParentWindow, 60.0f);

// 			real32 StateAlpha = 1.0f;
// 			if (RectContains(ButtonRect, Input->MousePos))
// 			{
// 				StateAlpha = 0.8f;
// 				if (Input->MouseLeft.OnDown)
// 				{
// 					StateAlpha = 0.6f;
// 					ParentWindow->HotItemID = ButtonID;
// 				}
// 				if (Input->MouseLeft.OnUp && ParentWindow->HotItemID == ButtonID)
// 				{
// 					ReturnState = true;
// 				}
// 			}

// 			if (Input->TouchStatus[0].OnDown && RectContains(ButtonRect, Input->TouchPosition[0]))
// 			{
// 				ReturnState = true;
// 			}

// 			color FinalButtonColor = EditorColor_Blue;
// 			FinalButtonColor.A = StateAlpha;
// 			PushRenderSquare(&RenderObjects[RenderLayer_EditorTwo], MakeGLRectangle(ButtonRect, FinalButtonColor));

// 			color FinalWordColor = COLOR_WHITE;
// 			FinalWordColor.A = StateAlpha;
// 			real64 ButtonWidth = RectWidth(ButtonRect);
// 			FontRenderString(ButtonName, ButtonRect.TopLeft + vector2{ButtonWidth / 2.0f, (ButtonHeight / 2.0f) + 12.0f}, 0.30f, FinalWordColor, StringAlign_Middle,
// 			                 AlphabetBitmaps, &RenderObjects[RenderLayer_EditorTwo]);

// 		}
// 	}

// 	return (CurrentSelection);
// }

void
ParticleSystemEditor(particle_system* System, editor_window* Window,
                     game_state* GameState, game_input* GameInput, game_memory* Memory, string RootAssetPath)
{
	ResizeWindow(Window, &GameState->Assets->RightTriImage, GameInput, GameState->RenderObjects);

	BeginLayout(Window, Layout_HorizontalFit);
	{
		if (AddButton(Window, "Reset System", 60,
		              GameInput, GameState->Assets->AlphabetBitmaps, GameState->RenderObjects))
		{
			InitParticleSystem(System, GameState, Memory->PlatformGetClockMS);
		}

		if (AddButton(Window, "Save System", 60,
		              GameInput, GameState->Assets->AlphabetBitmaps, GameState->RenderObjects))
		{
			SaveParticleSystem(System, RootAssetPath, Memory->PlatformWriteFile);
		}

		if (AddButton(Window, "Close", 60,
		              GameInput, GameState->Assets->AlphabetBitmaps, GameState->RenderObjects))
		{
			GameState->ShowingParticleEditor = false;
		}
	}
	EndLayout(Window);

	// NOTE this is here, but I never really used it so??
	// BeginLayout(Window, Layout_HorizontalFit);
	// {
	// 	string PauseString = "Pause System";
	// 	if (System->Paused)
	// 	{
	// 		PauseString = "Play System";
	// 	}
	// 	if (AddButton(Window, PauseString, 60,
	// 	              GameInput, GameState->Assets->AlphabetBitmaps, GameState->RenderObjects))
	// 	{
	// 		System->Paused = !System->Paused;
	// 	}

	// 	if (System->Paused)
	// 	{
	// 		if (AddButton(Window, "Step System", 60,
	// 		              GameInput, GameState->Assets->AlphabetBitmaps, GameState->RenderObjects))
	// 		{
	// 			StepParticleSystem(System, GameState->StateSerializing.FrameCounter, GameState, Memory->PlatformGetClockMS);
	// 		}
	// 	}
	// 	else
	// 	{
	// 		HorizontalSpacer(Window);
	// 	}
	// }
	// EndLayout(Window);

	// Move center of particle system
	{
		if (AddButton(Window, "Move Center", 60,
		              GameInput, GameState->Assets->AlphabetBitmaps, GameState->RenderObjects))
		{
			GameState->EditorMoving = EditorMoving_ParticleCenter;
		}

		if (GameState->EditorMoving == EditorMoving_ParticleCenter)
		{
			System->Center = GameInput->MousePos;
			if (System->Paused)
			{
				InitParticleSystem(System, GameState, Memory->PlatformGetClockMS);
			}

			if (GameInput->MouseLeft.OnDown)
			{
				GameInput->MouseLeft.Claimed = true;
				GameState->EditorMoving = EditorMoving_Nothing;
			}
		}
	}

	AddVerticalSpace(30.0f, Window);
	real64 GroupSpacing = 50.0f;

	//Emitter type
	BeginLayout(Window, Layout_HorizontalFit);
	{
		System->EmitType =
		    (emit_type)
		    AddRadioButtons(EmitTypeNames, sizeof(EmitTypeNames) / sizeof(string), (uint32)System->EmitType, "Emit Type",
		                    Window, &GameState->Assets->CircleImage,
		                    GameInput, GameState->Assets->AlphabetBitmaps, GameState->RenderObjects);

		if (System->EmitType == Emit_Circle)
		{
			System->CircleEmitRadius =
			    AddSlider(Window, range{0.0f, 20.0f}, "Radius :  ",
			              System->CircleEmitRadius,
			              &GameState->Assets->CircleImage, GameInput, GameState->Assets->AlphabetBitmaps, GameState->RenderObjects);
		}
		else
		{
			HorizontalSpacer(Window);
		}
	}
	EndLayout(Window);

	System->Life =
	    (uint64)AddSlider(Window, range{0.0f, 200.0f}, "Life :  ",
	                      (real64)System->Life,
	                      &GameState->Assets->CircleImage, GameInput, GameState->Assets->AlphabetBitmaps, GameState->RenderObjects);

	real64 VelocityScale = 1.0f;
	ParticleSystemComponentEditor(Window, &System->Components[Comp_VelocityX], range{ -VelocityScale, VelocityScale}, GameState, GameInput, Memory);
	AddVerticalSpace(GroupSpacing, Window);

	ParticleSystemComponentEditor(Window, &System->Components[Comp_VelocityY], range{ -VelocityScale, VelocityScale}, GameState, GameInput, Memory);
	AddVerticalSpace(GroupSpacing, Window);

	ParticleSystemComponentEditor(Window, &System->Components[Comp_Scale], range{0.0f, 200.0f}, GameState, GameInput, Memory);
	AddVerticalSpace(GroupSpacing, Window);

	ParticleSystemComponentEditor(Window, &System->Components[Comp_ColorR], range{0.0f, 1.0f}, GameState, GameInput, Memory);
	AddVerticalSpace(GroupSpacing, Window);

	ParticleSystemComponentEditor(Window, &System->Components[Comp_ColorG], range{0.0f, 1.0f}, GameState, GameInput, Memory);
	AddVerticalSpace(GroupSpacing, Window);

	ParticleSystemComponentEditor(Window, &System->Components[Comp_ColorB], range{0.0f, 1.0f}, GameState, GameInput, Memory);
	AddVerticalSpace(GroupSpacing, Window);

	ParticleSystemComponentEditor(Window, &System->Components[Comp_ColorA], range{0.0f, 1.0f},  GameState, GameInput, Memory);
	AddVerticalSpace(GroupSpacing, Window);

	ParticleSystemComponentEditor(Window, &System->Components[Comp_Rotation], range{ -100.0f, 100.0f},  GameState, GameInput, Memory);
	AddVerticalSpace(GroupSpacing, Window);

	ScrollWindow(Window, ID_LINE, GameInput, GameState->RenderObjects);
	DrawWindow(Window, &GameState->Assets->DropDownTriImage, &GameState->Assets->RightTriImage,
	           GameInput, GameState->RenderObjects, GameState->StencilObjects, GameState->Assets->AlphabetBitmaps);

}