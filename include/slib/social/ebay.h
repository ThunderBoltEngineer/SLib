/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#ifndef CHECKHEADER_SLIB_SOCIAL_EBAY
#define CHECKHEADER_SLIB_SOCIAL_EBAY

#include "definition.h"

#include "oauth.h"

namespace slib
{
	
	class SLIB_EXPORT EbayUser
	{
	public:
		String userId;
		String email;
		
	public:
		EbayUser();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(EbayUser)

	public:
		static String getPublicProfileURL(const String& userId);
		
		String getPublicProfileURL() const;

	};
	
	typedef OAuthLoginResult EbayLoginResult;
	
	class SLIB_EXPORT EbayResult
	{
	public:
		sl_bool flagSuccess;
		UrlRequest* request;
		String response;
		
	public:
		EbayResult(UrlRequest*);
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(EbayResult)
		
	};
	
	typedef OAuthLoginParam EbayLoginParam;
	
	class SLIB_EXPORT EbayParam : public OAuthParam
	{
	public:
		sl_bool flagProduction;
		
	public:
		EbayParam(sl_bool flagProduction);

		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(EbayParam)
		
	public:
		void setRedirectUrl(const String& ruName, const String& loginRedirectUri);
		
	};
	
	class Ebay : public OAuth2
	{
		SLIB_DECLARE_OBJECT
		
	protected:
		Ebay(const EbayParam& param);
		
		~Ebay();
		
	public:
		static Ref<Ebay> create(const EbayParam& param);
		
		static void initialize(const EbayParam& param);
		
		static void initialize(sl_bool flagProduction, const String& ruName, const String& redirectUri, const String& appId, const String& appSecret = String::null());

		static Ref<Ebay> getInstance();
		
	public:
		String getRequestUrl(const String& path);
		
		void callTraditionalApi(const String& callName, const String& request, const Function<void(UrlRequest*)>& onComplete);
		
		void getUser(const Function<void(EbayResult&, EbayUser&)>& onComplete);
		
	protected:
		sl_bool m_flagProduction;
		
	};
	
}

#endif
