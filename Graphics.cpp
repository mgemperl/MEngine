// Mostly implemented by Charles Kelly

#include "Graphics.h"
#include "TextureManager.h"

using namespace Space;

namespace MEngineNS
{

//=============================================================================
// Constructor
//=============================================================================
Graphics::Graphics()
{
    m_Direct3D = NULL;
    m_Device3D = NULL;
    m_bFullscreen = false;
    m_Width = GAME_WIDTH;    // m_Width & m_Height are replaced in initialize()
    m_Height = GAME_HEIGHT;
    m_BackColor = SETCOLOR_ARGB(255,0,0,128); // dark blue
}

//=============================================================================
// Destructor
//=============================================================================
Graphics::~Graphics()
{
    ReleaseAll();
}

//=============================================================================
// Release all
//=============================================================================
void Graphics::ReleaseAll()
{
    SAFE_RELEASE(m_Device3D);
    SAFE_RELEASE(m_Direct3D);
}

//=============================================================================
// Initialize DirectX graphics
// throws GameException on error
//=============================================================================
void Graphics::Initialize(HWND hw, int w, int h, bool full)
{
    m_Hwnd = hw;
    m_Width = w;
    m_Height = h;
    m_bFullscreen = full;

    //initialize Direct3D
    m_Direct3D = Direct3DCreate9(D3D_SDK_VERSION);
    if (m_Direct3D == NULL)
        throw GameException(GameExceptionNS::FATAL_ERROR, "Error initializing Direct3D");

    InitD3Dpp();        // init D3D presentation parameters
    if(m_bFullscreen)      // if full-screen mode
    {
        if(IsAdapterCompatible())   // is the adapter compatible
            // set the refresh rate with a compatible one
            m_d3dpp.FullScreen_RefreshRateInHz = pMode.RefreshRate;
        else
			throw GameException(GameExceptionNS::FATAL_ERROR,
            "The graphics device does not support the specified resolution and/or format.");
    }

    // determine if graphics card supports harware texturing and lighting and vertex shaders
    D3DCAPS9 caps;
    DWORD behavior;
    m_Result = m_Direct3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
    // If device doesn't support HW T&L or doesn't support 1.1 vertex 
    // shaders in hardware, then switch to software vertex processing.
    if( (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 ||
            caps.VertexShaderVersion < D3DVS_VERSION(1,1) )
        behavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;  // use software only processing
    else
        behavior = D3DCREATE_HARDWARE_VERTEXPROCESSING;  // use hardware only processing

    //create Direct3D device
    m_Result = m_Direct3D->CreateDevice(
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        m_Hwnd,
        behavior,
        &m_d3dpp, 
        &m_Device3D);

	if (FAILED(m_Result))
	{
		throw GameException(GameExceptionNS::FATAL_ERROR,
			"Error creating Direct3D device");
	}
 
	m_Result = D3DXCreateSprite(m_Device3D, &m_pSprite);
	if (FAILED(m_Result))
	{
		throw GameException(GameExceptionNS::FATAL_ERROR, 
			"Error creating Direct3D sprite");
	}

	m_Result = D3DXCreateLine(m_Device3D, &m_pPolygon);

	if (FAILED(m_Result))
	{
		throw GameException(GameExceptionNS::FATAL_ERROR,
			"Error creating Direct3D line");
	}
}

void Graphics::SaveTexture(const char* textureName, TextureManager* pTexture)
{
	m_textures.emplace(textureName, pTexture);
}

TextureManager* Graphics::RetrieveTexture(const char* texture)
{
	if (m_textures.count(texture) == 0)
	{
		throw GameException(GameExceptionNS::FATAL_ERROR,
			"Texture does not exist");
	}

	return m_textures[texture];
}

void Graphics::ReleaseSavedTextures()
{
	for (std::pair<std::string, TextureManager*> texture : m_textures)
	{
		delete texture.second;
	}

	m_textures.clear();
}

void Graphics::OnLostDevice()
{
	for (std::pair<std::string, TextureManager*> texture : m_textures)
	{
		texture.second->OnLostDevice();
	}
}

void Graphics::OnResetDevice()
{
	for (std::pair<std::string, TextureManager*> texture : m_textures)
	{
		texture.second->OnResetDevice();
	}
}

HRESULT Graphics::LoadTexture(const char* filename, COLOR_ARGB transcolor, UINT& width,
	UINT& height, LP_TEXTURE& pTexture)
{
	D3DXIMAGE_INFO info;
	bool result = E_FAIL;

	try
	{
		if (filename == NULL)
		{
			pTexture = NULL;
			return D3DERR_INVALIDCALL;
		}

		result = D3DXGetImageInfoFromFile(filename, &info);
	
		if (result != D3D_OK)
		{
			return result;
		}

		width = info.Width;
		height = info.Height;

		result = D3DXCreateTextureFromFileEx(
			m_Device3D,
			filename,
			info.Width,
			info.Height,
			1,
			0,
			D3DFMT_UNKNOWN,
			D3DPOOL_DEFAULT,
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			transcolor,
			&info,
			NULL,
			&pTexture );

	}
	catch (...)
	{

	}


	return result;
}

void Graphics::DrawSprite(const SpriteData& data, COLOR_ARGB color)
{
	// If there is a texture, draw the sprite
	if (data.texture)
	{
		// Get center of the sprite
		D3DXVECTOR2 center = D3DXVECTOR2(
			(float)(data.width / 2.0f * data.scale),
			(float)(data.height / 2.0f * data.scale));

		// Get position of the sprite on screen
		D3DXVECTOR2 translate = D3DXVECTOR2((float)data.x, (float)data.y);
	
		// Get the scaling of the sprite
		D3DXVECTOR2 scaling(data.scale, data.scale);
	
		if (data.flipHorizontal)
		{
			scaling.x *= -1;
		
			center.x -= (float)(data.width * data.scale);
			translate.x += (float)(data.width * data.scale);
		}

		if (data.flipVertical)
		{
			scaling.y *= -1;

			center.y -= (float)(data.height * data.scale);
			translate.y += (float)(data.height * data.scale);
		}

		D3DXMATRIX matrix;
		D3DXMatrixTransformation2D(
			&matrix,				// the matrix
			NULL,					// keep scaling origin at top left
			0x0f,					// no scaling rotation
			&scaling,				// scaling scalar
			&center,				// rotation center
			(float)(data.angle),	// rotation amount
			&translate);			// location

		m_pSprite->SetTransform(&matrix);
		m_pSprite->Draw(data.texture, &data.rect, NULL, NULL, color);
	}
}

void Graphics::DrawPolygon(const std::vector<Point2D<float>>& vertices,
	int width, COLOR_ARGB color)
{
	// WARNING: Crazy C-style stuff ahead
	D3DXVECTOR2* pVertices = (D3DXVECTOR2*)malloc(
		sizeof(D3DXVECTOR2) * vertices.size());

	D3DXVECTOR2* pVertex = pVertices;
	for (Point2D<float> point : vertices)
	{
		*pVertex++ = D3DXVECTOR2(point.GetX(), point.GetY());
	}
	pVertex = NULL;

	m_pPolygon->SetWidth(width);
	m_pPolygon->Draw(pVertices, vertices.size(), color);
	
	free(pVertices);
}

void Graphics::FillPolygon(const std::vector<Point2D<float>>& vertices,
	COLOR_ARGB color)
{
	VertexC* vertexArray = (VertexC*)malloc(sizeof(VertexC) * vertices.size());
	for (int index = 0; index < vertices.size(); index++)
	{
		vertexArray[index] = 
			VertexC(vertices[index].GetX(), vertices[index].GetY(), 0, color);
	}
	
	LPDIRECT3DVERTEXBUFFER9 pVertexBuffer = NULL;
	CreateVertexBuffer(vertexArray, vertices.size(), pVertexBuffer);

	if (pVertexBuffer != NULL)
	{
		return;
	}

	m_Device3D->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_Device3D->SetStreamSource(0, pVertexBuffer, 0, sizeof(VertexC));
	m_Device3D->SetFVF(D3DFVF_VERTEX);
	m_Result = m_Device3D->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, vertices.size() - 2);
	m_Device3D->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	free(vertexArray);
}

HRESULT Graphics::CreateVertexBuffer(VertexC vertices[], UINT size,
	LPDIRECT3DVERTEXBUFFER9 vertexBuffer)
{
	HRESULT hr = E_FAIL;

	hr = m_Device3D->CreateVertexBuffer(size, D3DUSAGE_WRITEONLY,
		D3DFVF_VERTEX, D3DPOOL_DEFAULT, &vertexBuffer, NULL);

	if (FAILED(hr))
	{
		return hr;
	}

	void* pBuf;
	hr = vertexBuffer->Lock(0, size, (void**)&pBuf, 0);

	if (FAILED(hr))
	{
		return hr;
	}

	memcpy(pBuf, vertices, size);
	vertexBuffer->Unlock();
}

//=============================================================================
// Initialize D3D presentation parameters
//=============================================================================
void Graphics::InitD3Dpp()
{
    try{
        ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));              // fill the structure with 0
        // fill in the parameters we need
        m_d3dpp.BackBufferWidth   = m_Width;
        m_d3dpp.BackBufferHeight  = m_Height;
        if(m_bFullscreen)                                  // if m_Fullscreen
            m_d3dpp.BackBufferFormat  = D3DFMT_X8R8G8B8;  // 24 bit color
        else
            m_d3dpp.BackBufferFormat  = D3DFMT_UNKNOWN;   // use desktop setting
        m_d3dpp.BackBufferCount   = 1;
        m_d3dpp.SwapEffect        = D3DSWAPEFFECT_DISCARD;
        m_d3dpp.hDeviceWindow     = m_Hwnd;
        m_d3dpp.Windowed          = (!m_bFullscreen);
        m_d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;
    } catch(...)
    {
		throw GameException(GameExceptionNS::FATAL_ERROR,
			"Error initializing D3D presentation parameters");

    }
}

//=============================================================================
// Display the backbuffer
//=============================================================================
HRESULT Graphics::ShowBackbuffer()
{
    m_Result = E_FAIL;    // default to fail, replace on success
    // Display backbuffer to screen
    m_Result = m_Device3D->Present(NULL, NULL, NULL, NULL);
    return m_Result;
}

//=============================================================================
// Checks the adapter to see if it is compatible with the BackBuffer m_Height,
// m_Width and refresh rate specified in m_d3dpp. Fills in the pMode structure with
// the format of the compatible mode, if found.
// Pre: m_d3dpp is initialized.
// Post: Returns true if compatible mode found and pMode structure is filled.
//       Returns false if no compatible mode found.
//=============================================================================
bool Graphics::IsAdapterCompatible()
{
    UINT modes = m_Direct3D->GetAdapterModeCount(D3DADAPTER_DEFAULT, 
                                            m_d3dpp.BackBufferFormat);
    for(UINT i=0; i<modes; i++)
    {
        m_Result = m_Direct3D->EnumAdapterModes( D3DADAPTER_DEFAULT, 
                                        m_d3dpp.BackBufferFormat,
                                        i, &pMode);
        if( pMode.Height == m_d3dpp.BackBufferHeight &&
            pMode.Width  == m_d3dpp.BackBufferWidth &&
            pMode.RefreshRate >= m_d3dpp.FullScreen_RefreshRateInHz)
            return true;
    }
    return false;
}

//=============================================================================
// Test for lost device
//=============================================================================
HRESULT Graphics::GetDeviceState()
{ 
    m_Result = E_FAIL;    // default to fail, replace on success
    if (m_Device3D == NULL)
        return  m_Result;
    m_Result = m_Device3D->TestCooperativeLevel(); 
    return m_Result;
}

//=============================================================================
// Reset the graphics device
//=============================================================================
HRESULT Graphics::Reset()
{
    m_Result = E_FAIL;    // default to fail, replace on success
    InitD3Dpp();                        // init D3D presentation parameters
    m_Result = m_Device3D->Reset(&m_d3dpp);   // attempt to reset graphics device
	m_Device3D->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_Device3D->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_Device3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    return m_Result;
}

}