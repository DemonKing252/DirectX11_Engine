#include "D3DCommon.h"
D3DUtil D3DUtil::instance = D3DUtil();


DirectXException::DirectXException(HRESULT hr)
{
	throw std::runtime_error(HrToString(hr));
}

std::string DirectXException::HrToString(HRESULT hr) const
{
	char s_str[64] = {};
	sprintf_s(s_str, "HRESULT of 0x%08X", static_cast<UINT>(hr));
	return std::string(s_str);
}