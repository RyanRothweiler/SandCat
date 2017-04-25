// This is all the old opengl stuff. Not supported currently as I'm moving entirely into unity for the moment.

#ifdef USING_OPENGL
int CALLBACK
WinMain(HINSTANCE Instance,
        HINSTANCE PrevInstance,
        LPSTR CommandLine,
        int ShowCode)
{
	// NOTE this is the virtual pixel size. The actual pixel size of the window is different.
	int32 WindowWidth = 1920 * 0.8f;
	int32 WindowHeight = 1080 * 0.8f;

	real32 WindowSizeAdjustment = 100.0f;

	WNDCLASSA WindowClass = {};

	WindowClass.style = CS_HREDRAW | CS_VREDRAW;
	WindowClass.lpfnWndProc = Win32MainWindowCallback;
	WindowClass.hInstance = Instance;
	WindowClass.hCursor = LoadCursor(0, IDC_ARROW);
	//    WindowClass.hIcon;
	WindowClass.lpszClassName = "TowerWindowClass";
	if (RegisterClassA(&WindowClass)) {
		HWND Window =
		    CreateWindowExA(
		        0, // WS_EX_TOPMOST|WS_EX_LAYERED,
		        WindowClass.lpszClassName,
		        "SandCat",
		        WS_VISIBLE,
		        // WS_MAXIMIZE | WS_VISIBLE,
		        0, 0,
		        WindowWidth, WindowHeight,
		        0,
		        0,
		        Instance,
		        0);
		if (Window) {
			// Init open gl
			HDC DeviceContext = GetDC(Window);

			PIXELFORMATDESCRIPTOR DesiredPixelFormat = {};
			DesiredPixelFormat.nSize = sizeof(DesiredPixelFormat);
			DesiredPixelFormat.nVersion = 1;
			DesiredPixelFormat.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
			DesiredPixelFormat.cColorBits = 24;
			DesiredPixelFormat.cAlphaBits = 8;
			DesiredPixelFormat.cStencilBits = 8;
			DesiredPixelFormat.iLayerType = PFD_MAIN_PLANE;

			int SuggestedPixelFormatIndex = ChoosePixelFormat(DeviceContext, &DesiredPixelFormat);
			PIXELFORMATDESCRIPTOR SuggestedPixelFormat = {};
			DescribePixelFormat(DeviceContext, SuggestedPixelFormatIndex, sizeof(SuggestedPixelFormat), &SuggestedPixelFormat);
			SetPixelFormat(DeviceContext, SuggestedPixelFormatIndex, &SuggestedPixelFormat);

			HGLRC OpenGLRC = wglCreateContext(DeviceContext);
			if (!wglMakeCurrent(DeviceContext, OpenGLRC)) {
				Assert(0);
				// TODO diagnostic
			}

			ReleaseDC(Window, DeviceContext);

			RECT ScreenSize;
			GetWindowRect(Window, &ScreenSize);
			window_info WindowInfo = {};
			WindowInfo.Width = ScreenSize.right - 15;
			WindowInfo.Height = ScreenSize.bottom - 40;

			// monitor is 105
			// device is 538
			// WindowInfo.Width = (uint32)(VirtualWidth / WindowSizeAdjustment);
			// WindowInfo.Height = (uint32)(VirtualHeight / WindowSizeAdjustment);
			// SetWindowPos(Window, HWND_TOP, 0, 0, WindowInfo.Width, WindowInfo.Height, NULL);

			render_layer RenderObjects[3];

			game_def GameDefinition = LoadGameDefinition();
			controls Controls = {};
			Controls.Controls.UpArrow.Name = "UpArrow";
			Controls.Controls.DownArrow.Name = "DownArrow";
			Controls.Controls.LeftArrow.Name = "LeftArrow";
			Controls.Controls.RightArrow.Name = "RightArrow";
			Controls.Controls.W.Name = "W";
			Controls.Controls.A.Name = "A";
			Controls.Controls.S.Name = "S";
			Controls.Controls.D.Name = "D";
			Controls.Controls.Q.Name = "Q";
			Controls.Controls.E.Name = "E";

			bool32 CanDoAction = true;

			GlobalRunning = true;
			while (GlobalRunning) {
				UpdateControls(&Controls, Window);

				// Game stuff

				// reset all the lists
				{
					for (uint32 LayerIndex = 0;  LayerIndex < RenderLayer_Count; LayerIndex++) {
						RenderObjects[LayerIndex].EntitiesCount = 0;
					}
				}

				//all rendering stuff needs to happen in here
				{
					// This is for ShieldPush2.sc
					if (false) {
						vector2 GridTopLeft = vector2{50.0f, 50.0f};
						real64 GridCellSize = 50;
						// int32 GridCellCount  = FluentSearch("F", GameDefinition.Fluents, GameDefinition.FluentsCount,
						// GameDefinition.Entities, GameDefinition.EntitiesCount)->Value;

						int32 GridCellCount = FluentSearch("GridMax'y", GameDefinition.Fluents, GameDefinition.FluentsCount,
						                                   GameDefinition.InstancedEntities, GameDefinition.InstancedEntitiesCount).Fluent->Value;


						// square grid outline
						{
							real32 Grey = 0.8f;
							real64 SquareSide = GridCellSize * GridCellCount;
							gl_square GridOutline = MakeGLSquare(GridTopLeft + vector2{SquareSide / 2.0f, SquareSide / 2.0f}, SquareSide + GridCellSize, color{Grey, Grey, Grey, 1.0f});
							PushRenderSquare(&RenderObjects[0], GridOutline);
						}

						// if (FindFluent("V_BlockX", GameDefinition.Fluents, GameDefinition.FluentsCount,
						// GameDefinition.Entities, GameDefinition.EntitiesCount) != NULL) {
						{
							vector2 BlockPos = {FluentSearch("BlockPos'x", GameDefinition.Fluents, GameDefinition.FluentsCount,
							                                 GameDefinition.InstancedEntities, GameDefinition.InstancedEntitiesCount).Fluent->Value,
							                    FluentSearch("BlockPos'y", GameDefinition.Fluents, GameDefinition.FluentsCount,
							                                 GameDefinition.InstancedEntities, GameDefinition.InstancedEntitiesCount).Fluent->Value
							                   };

							vector2 ScreenPos = (BlockPos * vector2{GridCellSize, GridCellSize}) + GridTopLeft;
							gl_square GridOutline = MakeGLSquare(ScreenPos, GridCellSize, COLOR_BLACK);
							PushRenderSquare(&RenderObjects[0], GridOutline);
						}
						// }

						// if (FindFluent("H_BlockX", GameDefinition.Fluents, GameDefinition.FluentsCount,
						//                GameDefinition.Entities, GameDefinition.EntitiesCount) != NULL) {
						// 	vector2 BlockPos = {FindFluent("H_BlockX", GameDefinition.Fluents, GameDefinition.FluentsCount,
						// 	                               GameDefinition.Entities, GameDefinition.EntitiesCount)->Value,
						// 	                    FindFluent("H_BlockY", GameDefinition.Fluents, GameDefinition.FluentsCount,
						// 	                               GameDefinition.Entities, GameDefinition.EntitiesCount)->Value
						// 	                   };

						// 	vector2 ScreenPos = (BlockPos * vector2{GridCellSize, GridCellSize}) + GridTopLeft;
						// 	gl_square GridOutline = MakeGLSquare(ScreenPos, GridCellSize, COLOR_BLACK);
						// 	PushRenderSquare(&RenderObjects[0], GridOutline);
						// }

						// if (FindFluent("PlayerHoldingShield", GameDefinition.Fluents, GameDefinition.FluentsCount,
						//                GameDefinition.Entities, GameDefinition.EntitiesCount) == NULL) {
						// 	vector2 ShieldPos = {FindFluent("ShieldPosX", GameDefinition.Fluents, GameDefinition.FluentsCount,
						// 	                                GameDefinition.Entities, GameDefinition.EntitiesCount)->Value,
						// 	                     FindFluent("ShieldPosY", GameDefinition.Fluents, GameDefinition.FluentsCount,
						// 	                                GameDefinition.Entities, GameDefinition.EntitiesCount)->Value
						// 	                    };

						// 	vector2 Center = (ShieldPos * vector2{GridCellSize, GridCellSize}) + GridTopLeft;

						// 	rect ShieldRect = {};
						// 	ShieldRect.TopLeft = Center - vector2{(GridCellSize / 2.0f) - 5.0f, (GridCellSize / 2.0f) + 5.0f};
						// 	ShieldRect.BottomRight = Center + vector2{(GridCellSize / 2.0f) - 5.0f, (GridCellSize / 2.0f) + 5.0f};

						// 	gl_square GridOutline = MakeGLRectangle(ShieldRect, COLOR_BLUE);
						// 	PushRenderSquare(&RenderObjects[0], GridOutline);
						// }

						// if (FindFluent("UsePlayerPos", GameDefinition.Fluents, GameDefinition.FluentsCount,
						//                GameDefinition.Entities, GameDefinition.EntitiesCount) != NULL) {


						vector2 PlayerPos = {FluentSearch("PlayerPos'x", GameDefinition.Fluents, GameDefinition.FluentsCount,
						                                  GameDefinition.InstancedEntities, GameDefinition.InstancedEntitiesCount).Fluent->Value,
						                     FluentSearch("PlayerPos'y", GameDefinition.Fluents, GameDefinition.FluentsCount,
						                                  GameDefinition.InstancedEntities, GameDefinition.InstancedEntitiesCount).Fluent->Value
						                    };

						vector2 PlayerScreenPos = (PlayerPos * vector2{GridCellSize, GridCellSize}) + GridTopLeft;
						gl_square GridOutline = MakeGLSquare(PlayerScreenPos, GridCellSize, COLOR_RED);
						PushRenderSquare(&RenderObjects[0], GridOutline);
					}
				}

				// control bindings
				{
					if (CanDoAction) {

						int32 Index = 0;
						for (; Index < GameDefinition.BoundCount; Index++) {
							bool32 BindingDone = false;
							string KeyWanting = GameDefinition.BoundEvents[Index].KeyName;

							for (int32 ContIndex = 0; ContIndex < ARRAY_SIZE(Controls.AllControls, button); ContIndex++) {
								if (Controls.AllControls[ContIndex].IsDown && KeyWanting == Controls.AllControls[ContIndex].Name) {

									for (int32 BEIndex = 0; BEIndex < GameDefinition.BoundEvents[Index].NextEventName; BEIndex++) {
										string EventWanting = GameDefinition.BoundEvents[Index].EventNames[BEIndex];

										// find the event to do
										for (int32 EventIndex = 0; EventIndex < GameDefinition.EventsCount; EventIndex++) {
											if (!BindingDone && GameDefinition.Events[EventIndex].Name == EventWanting) {

												event* EventDoing = &GameDefinition.Events[EventIndex];

												if (EventValid(EventDoing->ConditionalTokens, EventDoing->ConditionalsCount, &GameDefinition)) {
													TokensChangeState(EventDoing->ConsquenceTokens, EventDoing->NextConsqFluent, &GameDefinition);
													CanDoAction = false;
													BindingDone = true;
												}
											}
										}
									}
								}
							}
						}
					} else {
						bool32 AllButtonsUp = true;
						for (int32 Index = 0; Index < ARRAY_SIZE(Controls.AllControls, button); Index++) {
							if (Controls.AllControls[Index].IsDown) {
								AllButtonsUp = false;
							}
						}

						if (AllButtonsUp) {
							CanDoAction = true;
						}
					}

					// 		// can only do another action once the frst input has been released
					// 	}

				}

				// render stuff!
				{
					glMatrixMode(GL_MODELVIEW);
					glLoadIdentity();

					glOrtho(ScreenSize.left, WindowInfo.Width, WindowInfo.Height, ScreenSize.top, 1, -1);

					glEnable(GL_ALPHA);
					glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
					glColorMask(1, 1, 1, 1);

					for (int32 LayerIndex = 0;
					        LayerIndex < RenderLayer_Count;
					        LayerIndex++) {
						for (uint32 RenderIndex = 0;
						        RenderIndex < (uint32)RenderObjects[LayerIndex].EntitiesCount;
						        RenderIndex++) {
							render_entity* Entity = &RenderObjects[LayerIndex].Entities[RenderIndex];
							switch (Entity->Type) {
							case RenderEntity_Texture: {
								RenderTexture(&Entity->Texture);
								break;
							}

							case RenderEntity_Line: {
								RenderLine(&Entity->Line);
								break;
							}

							case RenderEntity_Square: {
								RenderSquare(&Entity->Square);
								break;
							}

							case RenderEntity_ColorPoints: {
								RenderColorPoints(&Entity->ColorPoints);
								break;
							}
							}
						}
					}

					glFlush();

					HDC DC = GetDC(Window);
					SwapBuffers(DC);
					ReleaseDC(Window, DC);
				}
			}
		}
	}
}

#else