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

#ifndef CHECKHEADER_SLIB_RENDER_PROGRAM
#define CHECKHEADER_SLIB_RENDER_PROGRAM

#include "definition.h"

#include "base.h"
#include "texture.h"

#include "../math/matrix4.h"
#include "../graphics/color.h"

namespace slib
{

	class Primitive;
	class RenderEngine;
	class GLRenderEngine;

	class SLIB_EXPORT RenderProgramState : public Referable
	{
	public:
		GLRenderEngine* gl_engine;
		sl_uint32 gl_program;

	public:
		RenderProgramState();
		
		~RenderProgramState();
		
		SLIB_DELETE_CLASS_DEFAULT_MEMBERS(RenderProgramState)
		
	public:
		void setUniformFloatValue(sl_int32 uniformLocation, float value);
		
		void setUniformFloatArray(sl_int32 uniformLocation, const float* arr, sl_uint32 n);
		
		void setUniformIntValue(sl_int32 uniformLocation, sl_int32 value);
		
		void setUniformIntArray(sl_int32 uniformLocation, const sl_int32* value, sl_uint32 n);
		
		void setUniformFloat2Value(sl_int32 uniformLocation, const Vector2& value);
		
		void setUniformFloat2Array(sl_int32 uniformLocation, const Vector2* arr, sl_uint32 n);
		
		void setUniformFloat3Value(sl_int32 uniformLocation, const Vector3& value);
		
		void setUniformFloat3Array(sl_int32 uniformLocation, const Vector3* arr, sl_uint32 n);
		
		void setUniformFloat4Value(sl_int32 uniformLocation, const Vector4& value);
		
		void setUniformFloat4Array(sl_int32 uniformLocation, const Vector4* arr, sl_uint32 n);
		
		void setUniformMatrix3Value(sl_int32 uniformLocation, const Matrix3& value);
		
		void setUniformMatrix3Array(sl_int32 uniformLocation, const Matrix3* arr, sl_uint32 n);
		
		void setUniformMatrix4Value(sl_int32 uniformLocation, const Matrix4& value);
		
		void setUniformMatrix4Array(sl_int32 uniformLocation, const Matrix4* arr, sl_uint32 n);
		
		void setUniformTexture(sl_int32 uniformLocation, const Ref<Texture>& texture, sl_reg sampler = 0);
		
		void setUniformTextureArray(sl_int32 uniformLocation, const Ref<Texture>* textures, const sl_reg* samplers, sl_uint32 n);
		
	};

	class SLIB_EXPORT RenderProgramInstance : public RenderBaseObjectInstance
	{
		SLIB_DECLARE_OBJECT
		
	protected:
		RenderProgramInstance();
		
		~RenderProgramInstance();
		
	};

	class SLIB_EXPORT RenderProgram : public RenderBaseObject
	{
		SLIB_DECLARE_OBJECT
		
	protected:
		RenderProgram();
		
		~RenderProgram();

	public:
		virtual Ref<RenderProgramState> onCreate(RenderEngine* engine) = 0;
		
		virtual sl_bool onInit(RenderEngine* engine, RenderProgramState* state);
		
		virtual sl_bool onPreRender(RenderEngine* engine, RenderProgramState* state);
		
		virtual void onPostRender(RenderEngine* engine, RenderProgramState* state);

		virtual String getGLSLVertexShader(RenderEngine* engine);
		
		virtual String getGLSLFragmentShader(RenderEngine* engine);

		
		Ref<RenderProgramInstance> getInstance(RenderEngine* engine);
		
	};

	namespace priv
	{
		namespace render_program
		{

			struct RenderProgramStateItem
			{
				const char* gl_name;
				sl_int32 gl_location;
				sl_uint32 type; // 1: Uniform, 2: Attribute
				sl_uint32 attrType; // 0: Float, 1: Int8, 2: Uint8, 3: Int16, 4: Uint16
				sl_uint32 attrOffset;
				sl_uint32 attrCount;
			};
		
#define PRIV_SLIB_RENDERPROGRAM_STATE_ITEM(NAME, GL_NAME, TYPE, ATTR_TYPE, ATTR_OFFSET, ATTR_COUNT) \
			struct T_##NAME { \
				const char* gl_name; \
				sl_int32 gl_location; \
				sl_int32 type; \
				sl_uint32 attrType; \
				sl_uint32 attrOffset; \
				sl_uint32 attrCount; \
				SLIB_INLINE T_##NAME() : gl_name(GL_NAME), gl_location(-1), type(TYPE), attrType(ATTR_TYPE), attrOffset(ATTR_OFFSET), attrCount(ATTR_COUNT) {} \
			} NAME;

			class RenderProgramStateTemplate : public RenderProgramState
			{
			public:
				sl_int32 _indexFirstAttribute;
				sl_int32 _indexLastAttribute;
				sl_uint32 _sizeVertexData;
				RenderProgramStateItem items[1];
			};

		}
	}

#define SLIB_RENDER_PROGRAM_STATE_BEGIN(TYPE, VERTEX_TYPE) \
	class SLIB_EXPORT TYPE : public slib::RenderProgramState \
	{ \
	public: \
		sl_int32 _indexFirstAttribute; \
		sl_int32 _indexLastAttribute; \
		sl_uint32 _sizeVertexData; \
		typedef VERTEX_TYPE VertexType; \
		SLIB_INLINE TYPE() : _indexFirstAttribute(-1), _indexLastAttribute(-1), _sizeVertexData(sizeof(VertexType)) {}

#define SLIB_RENDER_PROGRAM_STATE_END \
	PRIV_SLIB_RENDERPROGRAM_STATE_ITEM(LAST_ITEM, sl_null, 0, 0, 0, 0) \
};

#define SLIB_RENDER_PROGRAM_STATE_UNIFORM_INT(NAME, GL_NAME) \
	PRIV_SLIB_RENDERPROGRAM_STATE_ITEM(NAME, #GL_NAME, 1, 0, 0, 0) \
	SLIB_INLINE void set##NAME(sl_int32 value) { if (NAME.gl_location >= 0) setUniformIntValue(NAME.gl_location, value); }

#define SLIB_RENDER_PROGRAM_STATE_UNIFORM_INT_ARRAY(NAME, GL_NAME) \
	PRIV_SLIB_RENDERPROGRAM_STATE_ITEM(NAME, #GL_NAME, 1, 0, 0, 0) \
	SLIB_INLINE void set##NAME(const sl_int32* values, sl_uint32 count) { if (NAME.gl_location >= 0) setUniformIntArray(NAME.gl_location, values, count); }

#define SLIB_RENDER_PROGRAM_STATE_UNIFORM_FLOAT(NAME, GL_NAME) \
	PRIV_SLIB_RENDERPROGRAM_STATE_ITEM(NAME, #GL_NAME, 1, 0, 0, 0) \
	SLIB_INLINE void set##NAME(float value) { if (NAME.gl_location >= 0) setUniformFloatValue(NAME.gl_location, value); }

#define SLIB_RENDER_PROGRAM_STATE_UNIFORM_FLOAT_ARRAY(NAME, GL_NAME) \
	PRIV_SLIB_RENDERPROGRAM_STATE_ITEM(NAME, #GL_NAME, 1, 0, 0, 0) \
	SLIB_INLINE void set##NAME(const float* values, sl_uint32 count) { if (NAME.gl_location >= 0) setUniformFloatArray(NAME.gl_location, values, count); }

#define SLIB_RENDER_PROGRAM_STATE_UNIFORM_VECTOR2(NAME, GL_NAME) \
	PRIV_SLIB_RENDERPROGRAM_STATE_ITEM(NAME, #GL_NAME, 1, 0, 0, 0) \
	SLIB_INLINE void set##NAME(const slib::Vector2& value) { if (NAME.gl_location >= 0) setUniformFloat2Value(NAME.gl_location, value); }

#define SLIB_RENDER_PROGRAM_STATE_UNIFORM_VECTOR2_ARRAY(NAME, GL_NAME) \
	PRIV_SLIB_RENDERPROGRAM_STATE_ITEM(NAME, #GL_NAME, 1, 0, 0, 0) \
	SLIB_INLINE void set##NAME(const slib::Vector2* values, sl_uint32 count) { if (NAME.gl_location >= 0) setUniformFloat2Array(NAME.gl_location, values, count); }

#define SLIB_RENDER_PROGRAM_STATE_UNIFORM_VECTOR3(NAME, GL_NAME) \
	PRIV_SLIB_RENDERPROGRAM_STATE_ITEM(NAME, #GL_NAME, 1, 0, 0, 0) \
	SLIB_INLINE void set##NAME(const slib::Vector3& value) { if (NAME.gl_location >= 0) setUniformFloat3Value(NAME.gl_location, value); }

#define SLIB_RENDER_PROGRAM_STATE_UNIFORM_VECTOR3_ARRAY(NAME, GL_NAME) \
	PRIV_SLIB_RENDERPROGRAM_STATE_ITEM(NAME, #GL_NAME, 1, 0, 0, 0) \
	SLIB_INLINE void set##NAME(const slib::Vector3* values, sl_uint32 count) { if (NAME.gl_location >= 0) setUniformFloat3Array(NAME.gl_location, values, count); }

#define SLIB_RENDER_PROGRAM_STATE_UNIFORM_VECTOR4(NAME, GL_NAME) \
	PRIV_SLIB_RENDERPROGRAM_STATE_ITEM(NAME, #GL_NAME, 1, 0, 0, 0) \
	SLIB_INLINE void set##NAME(const slib::Vector4& value) { if (NAME.gl_location >= 0) setUniformFloat4Value(NAME.gl_location, value); }

#define SLIB_RENDER_PROGRAM_STATE_UNIFORM_VECTOR4_ARRAY(NAME, GL_NAME) \
	PRIV_SLIB_RENDERPROGRAM_STATE_ITEM(NAME, #GL_NAME, 1, 0, 0, 0) \
	SLIB_INLINE void set##NAME(const slib::Vector4* values, sl_uint32 count) { if (NAME.gl_location >= 0) setUniformFloat4Array(NAME.gl_location, values, count); }

#define SLIB_RENDER_PROGRAM_STATE_UNIFORM_MATRIX3(NAME, GL_NAME) \
	PRIV_SLIB_RENDERPROGRAM_STATE_ITEM(NAME, #GL_NAME, 1, 0, 0, 0) \
	SLIB_INLINE void set##NAME(const slib::Matrix3& value) { if (NAME.gl_location >= 0) setUniformMatrix3Value(NAME.gl_location, value); }

#define SLIB_RENDER_PROGRAM_STATE_UNIFORM_MATRIX3_ARRAY(NAME, GL_NAME) \
	PRIV_SLIB_RENDERPROGRAM_STATE_ITEM(NAME, #GL_NAME, 1, 0, 0, 0) \
	SLIB_INLINE void set##NAME(const slib::Matrix3* values, sl_uint32 count) { if (NAME.gl_location >= 0) setUniformMatrix3Array(NAME.gl_location, values, count); }

#define SLIB_RENDER_PROGRAM_STATE_UNIFORM_MATRIX4(NAME, GL_NAME) \
	PRIV_SLIB_RENDERPROGRAM_STATE_ITEM(NAME, #GL_NAME, 1, 0, 0, 0) \
	SLIB_INLINE void set##NAME(const slib::Matrix4& value) { if (NAME.gl_location >= 0) setUniformMatrix4Value(NAME.gl_location, value); }

#define SLIB_RENDER_PROGRAM_STATE_UNIFORM_MATRIX4_ARRAY(NAME, GL_NAME) \
	PRIV_SLIB_RENDERPROGRAM_STATE_ITEM(NAME, #GL_NAME, 1, 0, 0, 0) \
	SLIB_INLINE void set##NAME(const slib::Matrix4* values, sl_uint32 count) { if (NAME.gl_location >= 0) setUniformMatrix4Array(NAME.gl_location, values, count); }

#define SLIB_RENDER_PROGRAM_STATE_UNIFORM_TEXTURE(NAME, GL_NAME) \
	PRIV_SLIB_RENDERPROGRAM_STATE_ITEM(NAME, #GL_NAME, 1, 0, 0, 0) \
	SLIB_INLINE void set##NAME(const Ref<slib::Texture>& texture) { if (NAME.gl_location >= 0) setUniformTexture(NAME.gl_location, texture); }

#define SLIB_RENDER_PROGRAM_STATE_UNIFORM_TEXTURE_ARRAY(NAME, GL_NAME) \
	PRIV_SLIB_RENDERPROGRAM_STATE_ITEM(NAME, #GL_NAME, 1, 0, 0, 0) \
	SLIB_INLINE void set##NAME(const Ref<slib::Texture>* textures, sl_uint32 count) { if (NAME.gl_location >= 0) setUniformTextureArray(NAME.gl_location, textures, count); }

#define SLIB_RENDER_PROGRAM_STATE_ATTRIBUTE_FLOAT(MEMBER, GL_NAME) \
	PRIV_SLIB_RENDERPROGRAM_STATE_ITEM(GL_NAME, #GL_NAME, 2, 0, (sl_uint32)(sl_size)(&(((VertexType*)0)->MEMBER)), sizeof(((VertexType*)0)->MEMBER) / sizeof(float))

#define SLIB_RENDER_PROGRAM_STATE_ATTRIBUTE_INT8(NAME, GL_NAME) \
	PRIV_SLIB_RENDERPROGRAM_STATE_ITEM(GL_NAME, #GL_NAME, 2, 1, (sl_uint32)(sl_size)(&(((VertexType*)0)->MEMBER)), sizeof(((VertexType*)0)->MEMBER))

#define SLIB_RENDER_PROGRAM_STATE_ATTRIBUTE_UINT8(NAME, GL_NAME) \
	PRIV_SLIB_RENDERPROGRAM_STATE_ITEM(GL_NAME, #GL_NAME, 2, 2, (sl_uint32)(sl_size)(&(((VertexType*)0)->MEMBER)), sizeof(((VertexType*)0)->MEMBER))

#define SLIB_RENDER_PROGRAM_STATE_ATTRIBUTE_INT16(NAME, GL_NAME) \
	PRIV_SLIB_RENDERPROGRAM_STATE_ITEM(GL_NAME, #GL_NAME, 2, 3, (sl_uint32)(sl_size)(&(((VertexType*)0)->MEMBER)), sizeof(((VertexType*)0)->MEMBER)/2)

#define SLIB_RENDER_PROGRAM_STATE_ATTRIBUTE_UINT16(NAME, GL_NAME) \
	PRIV_SLIB_RENDERPROGRAM_STATE_ITEM(GL_NAME, #GL_NAME, 2, 4, (sl_uint32)(sl_size)(&(((VertexType*)0)->MEMBER)), sizeof(((VertexType*)0)->MEMBER)/2)


	namespace priv
	{
		namespace render_program
		{

			class SLIB_EXPORT RenderProgramTemplate : public RenderProgram
			{
			public:
				sl_bool onInit(RenderEngine* engine, RenderProgramState* state) override;
				
				sl_bool onPreRender(RenderEngine* engine, RenderProgramState* state) override;
				
				void onPostRender(RenderEngine* engine, RenderProgramState* state) override;

			};

		}
	}


	template <class StateType>
	class SLIB_EXPORT RenderProgramT : public priv::render_program::RenderProgramTemplate
	{
	public:
		Ref<RenderProgramState> onCreate(RenderEngine* engine) override
		{
			return new StateType;
		}
		
	};

#define SLIB_RENDER_GL_SET_VERTEX_FLOAT_ARRAY_ATTRIBUTE(engine, location, VertexData, member) \
	engine->setVertexFloatArrayAttribute(location, (sl_uint32)(sl_size)(&(((VertexData*)0)->member)), sizeof(((VertexData*)0)->member) / sizeof(float), sizeof(VertexData));


	struct RenderVertex2D_PositionTexture
	{
		Vector2 position;
		Vector2 texCoord;
	};

	SLIB_RENDER_PROGRAM_STATE_BEGIN(RenderProgramState2D_PositionTexture, RenderVertex2D_PositionTexture)
		SLIB_RENDER_PROGRAM_STATE_UNIFORM_MATRIX3(Transform, u_Transform)
		SLIB_RENDER_PROGRAM_STATE_UNIFORM_TEXTURE(Texture, u_Texture)
		SLIB_RENDER_PROGRAM_STATE_UNIFORM_MATRIX3(TextureTransform, u_TextureTransform)
		SLIB_RENDER_PROGRAM_STATE_UNIFORM_VECTOR4(Color, u_Color)

		SLIB_RENDER_PROGRAM_STATE_ATTRIBUTE_FLOAT(position, a_Position)
		SLIB_RENDER_PROGRAM_STATE_ATTRIBUTE_FLOAT(texCoord, a_TexCoord)
	SLIB_RENDER_PROGRAM_STATE_END

	class SLIB_EXPORT RenderProgram2D_PositionTexture : public RenderProgramT<RenderProgramState2D_PositionTexture>
	{
	public:
		String getGLSLVertexShader(RenderEngine* engine) override;
		
		String getGLSLFragmentShader(RenderEngine* engine) override;
		
	};

	class SLIB_EXPORT RenderProgram2D_PositionTextureYUV : public RenderProgram2D_PositionTexture
	{
	public:
		String getGLSLFragmentShader(RenderEngine* engine) override;
		
	};

	class SLIB_EXPORT RenderProgram2D_PositionTextureOES : public RenderProgram2D_PositionTexture
	{
	public:
		String getGLSLFragmentShader(RenderEngine* engine) override;
		
	};


	struct RenderVertex2D_PositionColor
	{
		Vector2 position;
		Color4f color;
	};

	SLIB_RENDER_PROGRAM_STATE_BEGIN(RenderProgramState2D_PositionColor, RenderVertex2D_PositionColor)
		SLIB_RENDER_PROGRAM_STATE_UNIFORM_MATRIX3(Transform, u_Transform)
		SLIB_RENDER_PROGRAM_STATE_UNIFORM_VECTOR4(Color, u_Color)

		SLIB_RENDER_PROGRAM_STATE_ATTRIBUTE_FLOAT(position, a_Position)
		SLIB_RENDER_PROGRAM_STATE_ATTRIBUTE_FLOAT(color, a_Color)
	SLIB_RENDER_PROGRAM_STATE_END

	class SLIB_EXPORT RenderProgram2D_PositionColor : public RenderProgramT<RenderProgramState2D_PositionColor>
	{
	public:
		String getGLSLVertexShader(RenderEngine* engine) override;
		
		String getGLSLFragmentShader(RenderEngine* engine) override;
		
	};


	struct RenderVertex2D_Position
	{
		Vector2 position;
	};

	SLIB_RENDER_PROGRAM_STATE_BEGIN(RenderProgramState2D_Position, RenderVertex2D_Position)
		SLIB_RENDER_PROGRAM_STATE_UNIFORM_MATRIX3(Transform, u_Transform)
		SLIB_RENDER_PROGRAM_STATE_UNIFORM_VECTOR4(Color, u_Color)

		SLIB_RENDER_PROGRAM_STATE_ATTRIBUTE_FLOAT(position, a_Position)
	SLIB_RENDER_PROGRAM_STATE_END

	class SLIB_EXPORT RenderProgram2D_Position : public RenderProgramT<RenderProgramState2D_Position>
	{
	public:
		String getGLSLVertexShader(RenderEngine* engine) override;
		
		String getGLSLFragmentShader(RenderEngine* engine) override;
		
	};


	struct RenderVertex3D_PositionNormalColor
	{
		Vector3 position;
		Vector3 normal;
		Color4f color;
	};

	SLIB_RENDER_PROGRAM_STATE_BEGIN(RenderProgramState3D_PositionNormalColor, RenderVertex3D_PositionNormalColor)
		SLIB_RENDER_PROGRAM_STATE_UNIFORM_MATRIX4(Transform, u_Transform)
		SLIB_RENDER_PROGRAM_STATE_UNIFORM_MATRIX4(MatrixModelViewIT, u_MatrixModelViewIT)
		SLIB_RENDER_PROGRAM_STATE_UNIFORM_VECTOR3(DirectionalLight, u_DirectionalLight)
		SLIB_RENDER_PROGRAM_STATE_UNIFORM_VECTOR3(DiffuseColor, u_DiffuseColor)
		SLIB_RENDER_PROGRAM_STATE_UNIFORM_VECTOR3(AmbientColor, u_AmbientColor)
		SLIB_RENDER_PROGRAM_STATE_UNIFORM_FLOAT(Alpha, u_Alpha)

		SLIB_RENDER_PROGRAM_STATE_ATTRIBUTE_FLOAT(position, a_Position)
		SLIB_RENDER_PROGRAM_STATE_ATTRIBUTE_FLOAT(normal, a_Normal)
		SLIB_RENDER_PROGRAM_STATE_ATTRIBUTE_FLOAT(color, a_Color)
	SLIB_RENDER_PROGRAM_STATE_END

	class SLIB_EXPORT RenderProgram3D_PositionNormalColor : public RenderProgramT<RenderProgramState3D_PositionNormalColor>
	{
	public:
		String getGLSLVertexShader(RenderEngine* engine) override;
		
		String getGLSLFragmentShader(RenderEngine* engine) override;
		
	};


	struct RenderVertex3D_PositionColor
	{
		Vector3 position;
		Color4f color;
	};

	SLIB_RENDER_PROGRAM_STATE_BEGIN(RenderProgramState3D_PositionColor, RenderVertex3D_PositionColor)
		SLIB_RENDER_PROGRAM_STATE_UNIFORM_MATRIX4(Transform, u_Transform)
		SLIB_RENDER_PROGRAM_STATE_UNIFORM_VECTOR4(Color, u_Color)

		SLIB_RENDER_PROGRAM_STATE_ATTRIBUTE_FLOAT(position, a_Position)
		SLIB_RENDER_PROGRAM_STATE_ATTRIBUTE_FLOAT(color, a_Color)
	SLIB_RENDER_PROGRAM_STATE_END

	class SLIB_EXPORT RenderProgram3D_PositionColor : public RenderProgramT<RenderProgramState3D_PositionColor>
	{
	public:
		String getGLSLVertexShader(RenderEngine* engine) override;
		
		String getGLSLFragmentShader(RenderEngine* engine) override;
		
	};


	struct RenderVertex3D_PositionNormalTexture
	{
		Vector3 position;
		Vector3 normal;
		Vector2 texCoord;
	};

	SLIB_RENDER_PROGRAM_STATE_BEGIN(RenderProgramState3D_PositionNormalTexture, RenderVertex3D_PositionNormalTexture)
		SLIB_RENDER_PROGRAM_STATE_UNIFORM_MATRIX4(Transform, u_Transform)
		SLIB_RENDER_PROGRAM_STATE_UNIFORM_MATRIX4(MatrixModelViewIT, u_MatrixModelViewIT)
		SLIB_RENDER_PROGRAM_STATE_UNIFORM_VECTOR3(DirectionalLight, u_DirectionalLight)
		SLIB_RENDER_PROGRAM_STATE_UNIFORM_VECTOR3(DiffuseColor, u_DiffuseColor)
		SLIB_RENDER_PROGRAM_STATE_UNIFORM_VECTOR3(AmbientColor, u_AmbientColor)
		SLIB_RENDER_PROGRAM_STATE_UNIFORM_FLOAT(Alpha, u_Alpha)
		SLIB_RENDER_PROGRAM_STATE_UNIFORM_TEXTURE(Texture, u_Texture)

		SLIB_RENDER_PROGRAM_STATE_ATTRIBUTE_FLOAT(position, a_Position)
		SLIB_RENDER_PROGRAM_STATE_ATTRIBUTE_FLOAT(normal, a_Normal)
		SLIB_RENDER_PROGRAM_STATE_ATTRIBUTE_FLOAT(texCoord, a_TexCoord)
	SLIB_RENDER_PROGRAM_STATE_END

	class SLIB_EXPORT RenderProgram3D_PositionNormalTexture : public RenderProgramT<RenderProgramState3D_PositionNormalTexture>
	{
	public:
		String getGLSLVertexShader(RenderEngine* engine) override;
		
		String getGLSLFragmentShader(RenderEngine* engine) override;
		
	};


	struct RenderVertex3D_PositionTexture
	{
		Vector3 position;
		Vector2 texCoord;
	};

	SLIB_RENDER_PROGRAM_STATE_BEGIN(RenderProgramState3D_PositionTexture, RenderVertex3D_PositionTexture)
		SLIB_RENDER_PROGRAM_STATE_UNIFORM_MATRIX4(Transform, u_Transform)
		SLIB_RENDER_PROGRAM_STATE_UNIFORM_VECTOR3(DiffuseColor, u_Color)
		SLIB_RENDER_PROGRAM_STATE_UNIFORM_TEXTURE(Texture, u_Texture)

		SLIB_RENDER_PROGRAM_STATE_ATTRIBUTE_FLOAT(position, a_Position)
		SLIB_RENDER_PROGRAM_STATE_ATTRIBUTE_FLOAT(texCoord, a_TexCoord)
	SLIB_RENDER_PROGRAM_STATE_END

	class SLIB_EXPORT RenderProgram3D_PositionTexture : public RenderProgramT<RenderProgramState3D_PositionTexture>
	{
	public:
		String getGLSLVertexShader(RenderEngine* engine) override;
		
		String getGLSLFragmentShader(RenderEngine* engine) override;
		
	};


	struct RenderVertex3D_PositionNormal
	{
		Vector3 position;
		Vector3 normal;
	};

	SLIB_RENDER_PROGRAM_STATE_BEGIN(RenderProgramState3D_PositionNormal, RenderVertex3D_PositionNormal)
		SLIB_RENDER_PROGRAM_STATE_UNIFORM_MATRIX4(Transform, u_Transform)
		SLIB_RENDER_PROGRAM_STATE_UNIFORM_MATRIX4(MatrixModelViewIT, u_MatrixModelViewIT)
		SLIB_RENDER_PROGRAM_STATE_UNIFORM_VECTOR3(DirectionalLight, u_DirectionalLight)
		SLIB_RENDER_PROGRAM_STATE_UNIFORM_VECTOR3(DiffuseColor, u_DiffuseColor)
		SLIB_RENDER_PROGRAM_STATE_UNIFORM_VECTOR3(AmbientColor, u_AmbientColor)
		SLIB_RENDER_PROGRAM_STATE_UNIFORM_FLOAT(Alpha, u_Alpha)

		SLIB_RENDER_PROGRAM_STATE_ATTRIBUTE_FLOAT(position, a_Position)
		SLIB_RENDER_PROGRAM_STATE_ATTRIBUTE_FLOAT(normal, a_Normal)
	SLIB_RENDER_PROGRAM_STATE_END

	class SLIB_EXPORT RenderProgram3D_PositionNormal : public RenderProgramT<RenderProgramState3D_PositionNormal>
	{
	public:
		String getGLSLVertexShader(RenderEngine* engine) override;
		
		String getGLSLFragmentShader(RenderEngine* engine) override;
		
	};


	struct RenderVertex3D_Position
	{
		Vector3 position;
	};

	SLIB_RENDER_PROGRAM_STATE_BEGIN(RenderProgramState3D_Position, RenderVertex3D_Position)
		SLIB_RENDER_PROGRAM_STATE_UNIFORM_MATRIX4(Transform, u_Transform)
		SLIB_RENDER_PROGRAM_STATE_UNIFORM_VECTOR4(Color, u_Color)

		SLIB_RENDER_PROGRAM_STATE_ATTRIBUTE_FLOAT(position, a_Position)
	SLIB_RENDER_PROGRAM_STATE_END

	class SLIB_EXPORT RenderProgram3D_Position : public RenderProgramT<RenderProgramState3D_Position>
	{
	public:
		String getGLSLVertexShader(RenderEngine* engine) override;
		
		String getGLSLFragmentShader(RenderEngine* engine) override;
		
	};

}

#endif

