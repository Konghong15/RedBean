#pragma once

class D3DHelper
{
public:
	static HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR EntryPoint, LPCSTR ShaderModel, ID3DBlob** ppBlobOut)
	{
		HRESULT hr = S_OK;

		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
		// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
		// Setting this flag improves the shader debugging experience, but still allows 
		// the shaders to be optimized and to run exactly the way they will run in 
		// the release configuration of this program.
		dwShaderFlags |= D3DCOMPILE_DEBUG;

		// Disable optimizations to further improve shader debugging
		dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

		ID3DBlob* pErrorBlob = nullptr;
		hr = D3DCompileFromFile(szFileName, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, EntryPoint, ShaderModel,
			dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
		if (FAILED(hr))
		{
			if (pErrorBlob)
			{
				MessageBoxA(NULL, (char*)pErrorBlob->GetBufferPointer(), "CompileShaderFromFile", MB_OK);
				pErrorBlob->Release();
			}
			return hr;
		}
		if (pErrorBlob) pErrorBlob->Release();

		return S_OK;
	}
};