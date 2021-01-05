#pragma once

//--------------------------------------------------------------------------------------------------------------------------------------------

struct ID3D11RenderTargetView;
struct ID3D11ShaderResourceView;
struct ID3D11DepthStencilView;
struct ID3D11Resource;
struct ID3D11UnorderedAccessView;

class  Texture;

//--------------------------------------------------------------------------------------------------------------------------------------------

class TextureView
{
public:
	TextureView();
	~TextureView();
	ID3D11RenderTargetView*		 GetRTVHandle() const							{ return m_rtv;  }
	ID3D11ShaderResourceView*	 GetSRVHandle() const							{ return m_srv; }
	ID3D11DepthStencilView*		 GetDSVHandle() const							{ return m_dsv; }
	ID3D11UnorderedAccessView*	 GetUAVHandle() const							{ return m_uav; }

public:
	Texture* m_owner; 

      union {
         ID3D11Resource*			m_handle;			// A01
         ID3D11ShaderResourceView*	m_srv;				// A03 - what is bound to a shader stage for reading
         ID3D11RenderTargetView*	m_rtv;				// A01 - alias as an rtv
		 ID3D11DepthStencilView*	m_dsv;
      	 ID3D11UnorderedAccessView* m_uav;
      }; 

private:

};