////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_Input = nullptr;
	m_D3D = nullptr;
	m_Timer = nullptr;
	m_ShaderManager = nullptr;
	m_Light = nullptr;
	m_Position = nullptr;
	m_Camera = nullptr;
	m_FloorModel = nullptr;
	m_SatelliteModel = nullptr;
	m_RocketModel = nullptr;
	m_TreeModel = nullptr;
	m_SaturnModel = nullptr;
	m_SaturnRingModel = nullptr;
	m_EarthModel = nullptr;
	m_SunModel = nullptr;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	bool result;

	// Create the input object.  The input object will be used to handle reading the keyboard and mouse input from the user.
	m_Input = new InputClass;
	if (!m_Input)
	{
		return false;
	}

	// Initialize the input object.
	result = m_Input->Initialize(hinstance, hwnd, screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}

	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the shader manager object.
	m_ShaderManager = new ShaderManagerClass;
	if(!m_ShaderManager)
	{
		return false;
	}

	// Initialize the shader manager object.
	result = m_ShaderManager->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the shader manager object.", L"Error", MB_OK);
		return false;
	}

	// Create the timer object.
	m_Timer = new TimerClass;
	if (!m_Timer)
	{
		return false;
	}

	// Initialize the timer object.
	result = m_Timer->Initialize();
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the timer object.", L"Error", MB_OK);
		return false;
	}

	// Create the position object.
	m_Position = new PositionClass;
	if (!m_Position)
	{
		return false;
	}

	// Set the initial position and rotation of the viewer.
	m_Position->SetPosition(0.0f, -198.0f, -200.0f);
	m_Position->SetRotation(0.0f, 0.0f, 0.0f);

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	//m_Camera->SetPosition(0.0f, 0.0f, -10.0f);

	// Create the light object.
	m_Light = new LightClass;
	if(!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, 0.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(64.0f);

	// Create the model object.
	m_FloorModel = new ModelClass;
	if(!m_FloorModel)
	{
		return false;
	}

	// Initialize the model object.
	result = m_FloorModel->Initialize(m_D3D->GetDevice(), "../Engine/data/Floor.txt", L"../Engine/data/grass.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the first model object.", L"Error", MB_OK);
		return false;
	}

	// Create the second model object.
	m_SatelliteModel = new ModelClass;
	if(!m_SatelliteModel)
	{
		return false;
	}

	// Initialize the second model object.
	result = m_SatelliteModel->Initialize(m_D3D->GetDevice(), "../Engine/data/Satellite.txt", L"../Engine/data/Satellite.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the second model object.", L"Error", MB_OK);
		return false;
	}

	// Create rocket model object.
	m_RocketModel = new ModelClass;
	if(!m_RocketModel)
	{
		return false;
	}

	result = m_RocketModel->Initialize(m_D3D->GetDevice(), "../Engine/data/Rocket.txt", L"../Engine/data/Rocket.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the rocket model object.", L"Error", MB_OK);
		return false;
	}

	// Create the tree model
	m_TreeModel = new ModelClass;
	if(!m_TreeModel)
	{
		return false;
	}

	result = m_TreeModel->Initialize(m_D3D->GetDevice(), "../Engine/data/Tree.txt", L"../Engine/data/Tree.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the tree model object.", L"Error", MB_OK);
		return false;
	}

	// Create the saturn model
	m_SaturnModel = new ModelClass;
	if(!m_SaturnModel)
	{
		return false;
	}

	result = m_SaturnModel->Initialize(m_D3D->GetDevice(), "../Engine/data/Sphere.txt", L"../Engine/data/2k_saturn.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the saturn object.", L"Error", MB_OK);
		return false;
	}

	// Create the saturn model
	m_SaturnRingModel = new ModelClass;
	if (!m_SaturnModel)
	{
		return false;
	}

	result = m_SaturnRingModel->Initialize(m_D3D->GetDevice(), "../Engine/data/SaturnRing.txt", L"../Engine/data/SaturnRing.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the saturn ring object.", L"Error", MB_OK);
		return false;
	}

	// Create the third bump model object for models with normal maps and related vectors.
	m_EarthModel = new BumpModelClass;
	if(!m_EarthModel)
	{
		return false;
	}

	// Initialize the bump model object.
	result = m_EarthModel->Initialize(m_D3D->GetDevice(), "../Engine/data/Sphere.txt", L"../Engine/data/2k_earth_with_clouds.dds", 
								  L"../Engine/data/2k_earth_normal_map.dds");

	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the third model object.", L"Error", MB_OK);
		return false;
	}

	// Create the fourth fire model object.
	m_SunModel = new FireModelClass;
	if (!m_SunModel)
	{
		return false;
	}

	result = m_SunModel->Initialize(m_D3D->GetDevice(), "../Engine/data/Sphere.txt", L"../Engine/data/fire01.dds", //square or cube
		L"../Engine/data/noise01.dds", L"../Engine/data/alpha01.dds");

	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the fourth model object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the model objects.
	if(m_FloorModel)
	{
		m_FloorModel->Shutdown();
		delete m_FloorModel;
		m_FloorModel = 0;
	}

	if(m_SatelliteModel)
	{
		m_SatelliteModel->Shutdown();
		delete m_SatelliteModel;
		m_SatelliteModel = 0;
	}

	if(m_EarthModel)
	{
		m_EarthModel->Shutdown();
		delete m_EarthModel;
		m_EarthModel = 0;
	}

	if (m_SunModel)
	{
		m_SunModel->Shutdown();
		delete m_SunModel;
		m_SunModel = 0;
	}

	// Release the light object.
	if(m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the position object.
	if (m_Position)
	{
		delete m_Position;
		m_Position = 0;
	}

	// Release the shader manager object.
	if(m_ShaderManager)
	{
		m_ShaderManager->Shutdown();
		delete m_ShaderManager;
		m_ShaderManager = 0;
	}

	// Release the timer object.
	if (m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
	}

	// Release the D3D object.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	// Release the input object.
	if (m_Input)
	{
		m_Input->Shutdown();
		delete m_Input;
		m_Input = 0;
	}

	return;
}


bool GraphicsClass::Frame()
{
	bool result;
	bool rocketTakeOff = false;

	// Update the system stats.
	m_Timer->Frame();

	// Read the user input.
	result = m_Input->Frame();
	if (!result)
	{
		return false;
	}

	// Check if the user pressed escape and wants to exit the application.
	if (m_Input->IsEscapePressed() == true)
	{
		return false;
	}

	// Do the frame input processing.
	result = HandleMovementInput(m_Timer->GetTime(), &rocketTakeOff);
	if (!result)
	{
		return false;
	}

	// Render the graphics.
	result = Render(rocketTakeOff);
	if (!result)
	{
		return false;
	}

	return true;
}

bool GraphicsClass::HandleMovementInput(float frameTime, bool* rocketTakeOff)
{
	bool keyDown;
	static bool hasLaunched;
	float posX, posY, posZ, rotX, rotY, rotZ;

	// Set the frame time for calculating the updated position.
	m_Position->SetFrameTime(frameTime);

	// Handle the input.
	keyDown = m_Input->IsAPressed();
	m_Position->TurnLeft(keyDown);

	keyDown = m_Input->IsDPressed();
	m_Position->TurnRight(keyDown);

	keyDown = m_Input->IsWPressed();
	m_Position->MoveForward(keyDown);

	keyDown = m_Input->IsSPressed();
	m_Position->MoveBackward(keyDown);

	keyDown = m_Input->IsLShiftPressed();
	m_Position->MoveUpward(keyDown);

	keyDown = m_Input->IsLCtrlPressed();
	m_Position->MoveDownward(keyDown);

	keyDown = m_Input->IsPgUpPressed();
	m_Position->LookUpward(keyDown);

	keyDown = m_Input->IsPgDownPressed();
	m_Position->LookDownward(keyDown);

	// Rocket takes off when F1 is pressed
	keyDown = m_Input->IsF1Pressed();
	if (!hasLaunched && keyDown)
	{
		*rocketTakeOff = true;
		hasLaunched = true;
	} else if (hasLaunched)
	{
		*rocketTakeOff = true;
	}

	m_Position->MouseRotate(m_Input->GetMouseXDelta(), m_Input->GetMouseYDelta());

	// Get the view point position/rotation.
	m_Position->GetPosition(posX, posY, posZ);
	m_Position->GetRotation(rotX, rotY, rotZ);
	m_Input->ResetCursorPos();

	// Set the position of the camera.
	m_Camera->SetPosition(posX, posY, posZ);
	m_Camera->SetRotation(rotX, rotY, rotZ);

	return true;
}

bool GraphicsClass::Render(bool rocketTakeOff)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, translateMatrix;
	bool result;
	
	XMFLOAT3 scrollSpeeds, scales;
	XMFLOAT2 distortion1, distortion2, distortion3;
	float distortionScale, distortionBias;
	static float frameTime = 0.0f;

	int treeCount;
	
	frameTime += 0.01f;
	if (frameTime > 1000.0f)
	{
		frameTime = 0.0f;
	}
	
	scrollSpeeds = XMFLOAT3(0.5f, 1.6f, 2.f);
	
	scales = XMFLOAT3(1.0f, 2.0f, 3.0f);
	
	distortion1 = XMFLOAT2(0.1f, 0.2f);
	distortion2 = XMFLOAT2(0.1f, 0.3f);
	distortion3 = XMFLOAT2(0.1f, 0.1f);
	
	distortionScale = 0.8f;
	distortionBias = 0.5f;

	static float rotation = 0.0f;

	// Update the rotation variable each frame.
	rotation += (float)XM_PI * 0.0005f * m_Timer->GetTime();

	static float rocketHeight = 0.0f;

	// Move Rocket upwards if take-off is triggered
	if (rocketTakeOff)
		rocketHeight += 1 * m_Timer->GetTime();

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_D3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	
	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixScaling(3.0f, 1.0f, 3.0f));
	translateMatrix = XMMatrixTranslation(0.0f, -200.0f, 0.0f);
	worldMatrix = XMMatrixMultiply(worldMatrix, translateMatrix);
	

	// Render the first model using the texture shader.
	m_FloorModel->Render(m_D3D->GetDeviceContext());
	result = m_ShaderManager->RenderTextureShader(m_D3D->GetDeviceContext(), m_FloorModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_FloorModel->GetTexture());
	if(!result)
	{
		return false;
	}

	// Setup the rotation and translation of the Rocket
	m_D3D->GetWorldMatrix(worldMatrix);

	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixScaling(0.05f, 0.05f, 0.05f));
	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixTranslation(0.0f, -200.f + rocketHeight * 0.2f, 0.0f));

	// render the rocket model
	m_RocketModel->Render(m_D3D->GetDeviceContext());
	result = m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_RocketModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_RocketModel->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

	// Setup positions and render the trees
	constexpr float treePosX[200] = {-40, -152, -31, 190, -165, 164, 277, -197, -118, -150, 4, 35, -108, 298, -48, -30, -102, -273, 92, -237, -213, -78, -175, -157, 223, -150, -212, 98, -45, -76, 74, 24, 44, -171, 69, -56, -296, 249, 235, 280, 264, 32, 240, 101, 177, -269, 143, -102, 106, -106, 81, 60, -157, 227, 122, -298, 288, -287, -14, -124, 56, -264, -296, -3, 196, 142, 203, -248, -218, -5, 152, -297, -210, -288, 219, 201, 213, -12, -39, -94, 282, 108, 181, -254, 18, 288, 243, 17, 20, -62, 52, -26, -248, -177, 223, 165, 288, -40, 131, 270, -182, -126, 276, -211, 195, 10, -27, 236, -178, -262, -224, -83, 123, -70, -295, 106, -286, 220, 33, 13, 200, -212, -287, 83, 111, -202, -34, -212, 207, 236, 44, 156, -120, -78, 106, -193, -264, -245, 148, 38, -99, 194, -148, 222, -121, -130, 22, 96, -179, 74, -198, 114, -158, 131, -46, 146, -47, 185, -68, 297, -102, -47, -223, 280, 195, 179, -37, 253, 77, -259, 107, 255, -268, -261, 107, 104, -126, -267, 195, 13, -255, -151, 139, -168, 123, -49, 180, -171, 257, 14, -100, 269, 192, -177, -154, 221, -52, 176, 68, -153};
	constexpr float treePosZ[200] = {-63, 28, 67, 81, -220, 10, 109, 262, -187, -172, -240, -179, 279, 70, -51, -191, 224, -62, -210, -83, 213, 144, 77, 299, -285, -80, 153, -120, 145, -29, 122, 12, 134, -102, 233, 59, 149, -220, -245, 162, 116, -168, 120, -198, 137, 101, -238, 189, -283, -67, 214, -142, -237, 1, 201, 78, -134, 215, 60, -89, -222, 0, -150, 135, -297, -186, -47, -24, -143, -77, -286, 227, 7, -197, 187, 266, -182, -143, -111, -99, 63, -74, -3, -90, -8, 274, 57, -110, 283, 0, -173, 299, -83, 7, 259, -66, 202, 228, 103, 241, 125, -179, -66, -247, -294, -197, -154, 284, -188, 131, 50, -221, 133, 74, 79, 220, 206, 95, 192, 250, 78, 284, 217, -8, -225, -98, -98, 264, -208, 24, 192, -141, -113, 172, 238, 44, -283, -120, -27, 195, -164, 201, 126, 110, -278, -275, 271, -260, 52, -236, 211, 236, -254, -189, 87, 166, -48, -131, 56, -251, 32, 253, 0, 113, 278, 274, -119, -225, 202, 100, 140, 202, 39, -112, -146, 261, -43, -23, 30, -126, -250, -111, 104, -16, 214, -21, 248, -74, -171, -271, 111, -208, 42, -79, 78, -121, -92, -156, 161, 269};

	for (int i = 0; i < 200; i++)
	{
		m_D3D->GetWorldMatrix(worldMatrix);

		worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixScaling(0.05f, 0.05f, 0.05f));
		worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixTranslation(-150.f, 0.f, 270.f));
		worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixTranslation(treePosX[i], -202.f, treePosZ[i]));

		m_TreeModel->Render(m_D3D->GetDeviceContext());
		result = m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_TreeModel->GetIndexCount(), worldMatrix,
			viewMatrix, projectionMatrix, m_TreeModel->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), 
			m_Light->GetDiffuseColor(), m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	}

	// Setup the rotation and translation of the Satellite
	m_D3D->GetWorldMatrix(worldMatrix);

	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixScaling(0.003f, 0.003f, 0.003f));

	// self rotation
	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixRotationY(-rotation * 0.2f));

	// orbit about the earth
	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixTranslation(50.0f, 0.0f, 0.0f));
	XMVECTOR SatAxis1 = XMVectorSet(0.2f, 1, 0, 0);
	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixRotationAxis(SatAxis1, rotation * 0.3f));

	// orbit about the sun
	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixTranslation(100.0f, 0.0f, 50.0f));
	XMVECTOR SatAxis2 = XMVectorSet(0, 1, 0, 0);
	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixRotationAxis(SatAxis2, rotation * 0.2f));
	

	// Render the second model using the light shader.
	m_SatelliteModel->Render(m_D3D->GetDeviceContext());
	result = m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_SatelliteModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
									   m_SatelliteModel->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), 
									   m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	if(!result)
	{
		return false;
	}

	// Setup the rotation and translation of the earth model.
	m_D3D->GetWorldMatrix(worldMatrix);
	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixScaling(0.00001f, 0.00001f, 0.00001f));

	worldMatrix = XMMatrixRotationY(rotation / 3.0f);

	translateMatrix = XMMatrixTranslation(100.f, 0.0f, 50.0f);
	worldMatrix = XMMatrixMultiply(worldMatrix, translateMatrix);

	XMVECTOR earthAxis = XMVectorSet(0.f, 1, 0.f, 0.f);

	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixRotationAxis(earthAxis, rotation * 0.2f));
	

	// Render the earth model using the bump map shader.
	m_EarthModel->Render(m_D3D->GetDeviceContext());
	result = m_ShaderManager->RenderBumpMapShader(m_D3D->GetDeviceContext(), m_EarthModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
												  m_EarthModel->GetColorTexture(), m_EarthModel->GetNormalMapTexture(), m_Light->GetDirection(), 
												  m_Light->GetDiffuseColor());
	if(!result)
	{
		return false;
	}

	m_D3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);


	// Setup the rotation and translation of saturn
	m_D3D->GetWorldMatrix(worldMatrix);

	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixScaling(0.0001f, 0.0001f, 0.0001f));

	worldMatrix = XMMatrixRotationY(rotation / 3.0f);

	translateMatrix = XMMatrixTranslation(300.f, 0.0f, 50.0f);
	worldMatrix = XMMatrixMultiply(worldMatrix, translateMatrix);

	XMVECTOR saturnAxis = XMVectorSet(0.f, 1, 0.f, 0.f);

	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixRotationAxis(saturnAxis, rotation * 0.1f));

	// Render saturn model
	m_SaturnModel->Render(m_D3D->GetDeviceContext());
	result = m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_SaturnModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_SaturnModel->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());


	// Setup the rotation and translation of saturn rings
	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixScaling(0.001f, 0.001f, 0.001f));

	worldMatrix = XMMatrixRotationY(-rotation);

	translateMatrix = XMMatrixTranslation(300.f, 0.0f, 50.0f);
	worldMatrix = XMMatrixMultiply(worldMatrix, translateMatrix);

	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixRotationAxis(saturnAxis, rotation * 0.1f));

	// Render rings model
	m_SaturnRingModel->Render(m_D3D->GetDeviceContext());
	result = m_ShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), m_SaturnRingModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_SaturnRingModel->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());


	// Setup the rotation and translation of the sun.
	m_D3D->GetWorldMatrix(worldMatrix);

	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixScaling(1.5f, 1.5f, 1.5f));// (1.0f, 1.0f, 1.0f));
	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixRotationY(-rotation));
	worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixTranslation(0.0f, 0.0f, 50.0f));


	// Turn on alpha blending.
	m_D3D->TurnOnAlphaBlending();
	
	m_SunModel->Render(m_D3D->GetDeviceContext());

	// Render the sun using the fire shader.
	result = m_ShaderManager->RenderFireShader(m_D3D->GetDeviceContext(), m_SunModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_SunModel->GetTexture1(), m_SunModel->GetTexture2(), m_SunModel->GetTexture3(), frameTime, scrollSpeeds,
		scales, distortion1, distortion2, distortion3, distortionScale, distortionBias);
	if (!result)
	{
		return false;
	}

	// Turn off alpha blending.
	m_D3D->TurnOffAlphaBlending();


	// Present the rendered scene to the screen.
	m_D3D->EndScene();


	return true;
}