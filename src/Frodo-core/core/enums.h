#pragma once

#include <platforms/platform.h>

namespace fd {

enum class MonitorOrientation {
	Unspecified,
	Identity,
	Rotate90,
	Rotate180,
	Rotate270
};

enum class LogLevel {
	Info,
	Debug,
	Warning,
	Fatal
};

enum class LogDeviceType {
	Console,
	File,
	Custom
};

#ifdef FD_DX
enum class PrimitiveTopology {
	PointList = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
	LineLize = D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
	LineStrip = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
	TriangleList= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
	TriangleStrip = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
};

enum class PolygonMode {
	Line = D3D11_FILL_WIREFRAME,
	Fill = D3D11_FILL_SOLID
};

enum class CullMode {
	None = D3D11_CULL_NONE,
	Back = D3D11_CULL_BACK,
	Front = D3D11_CULL_FRONT
};

enum class FrontFace {
	Clockwise,
	CounterClockwise
};

enum class ComparisonFunc {
	Never = D3D11_COMPARISON_NEVER,
	Less = D3D11_COMPARISON_LESS,
	Equal = D3D11_COMPARISON_EQUAL,
	LessEqual = D3D11_COMPARISON_LESS_EQUAL,
	Greater = D3D11_COMPARISON_GREATER,
	NotEqual = D3D11_COMPARISON_NOT_EQUAL,
	GreaterEqual = D3D11_COMPARISON_GREATER_EQUAL,
	Always = D3D11_COMPARISON_ALWAYS
};

enum class StencilOp {
	Kepp = D3D11_STENCIL_OP_KEEP,
	Zero = D3D11_STENCIL_OP_ZERO,
	Replace = D3D11_STENCIL_OP_REPLACE,
	IncrementClamp= D3D11_STENCIL_OP_INCR_SAT,
	DecrementClamp = D3D11_STENCIL_OP_DECR_SAT,
	Invert = D3D11_STENCIL_OP_INVERT,
	Increment= D3D11_STENCIL_OP_INCR,
	Decrement = D3D11_STENCIL_OP_DECR
};

enum class BlendFactor {
	Zero = D3D11_BLEND_ZERO,
	One = D3D11_BLEND_ONE,
	SrcColor = D3D11_BLEND_SRC_COLOR,
	InvSrcColor = D3D11_BLEND_INV_SRC_COLOR,
	SrcAlpha = D3D11_BLEND_SRC_ALPHA,
	InvSrcAlpha = D3D11_BLEND_INV_SRC_ALPHA,
	DestAlpha = D3D11_BLEND_DEST_ALPHA,
	InvDestAlpha = D3D11_BLEND_INV_DEST_ALPHA,
	DestColor = D3D11_BLEND_DEST_COLOR,
	InvDestColor = D3D11_BLEND_INV_DEST_COLOR,
	SrcAlphaClamp = D3D11_BLEND_SRC_ALPHA_SAT,
	BlendFactor = D3D11_BLEND_BLEND_FACTOR,
	InvBlendFactor = D3D11_BLEND_INV_BLEND_FACTOR,
	Src1Color = D3D11_BLEND_SRC1_COLOR,
	InvSrc1Color = D3D11_BLEND_INV_SRC1_COLOR,
	Src1Alpha = D3D11_BLEND_SRC1_ALPHA,
	InvSrc1Alpha = D3D11_BLEND_INV_SRC1_ALPHA
};

enum class BlendOp {
	Add = D3D11_BLEND_OP_ADD,
	Subtract = D3D11_BLEND_OP_SUBTRACT,
	RevSubtract = D3D11_BLEND_OP_REV_SUBTRACT,
	Min = D3D11_BLEND_OP_MIN,
	Max = D3D11_BLEND_OP_MAX
};

enum ColorComponentFlag {
	Red = D3D11_COLOR_WRITE_ENABLE_RED,
	Green = D3D11_COLOR_WRITE_ENABLE_GREEN,
	Blue = D3D11_COLOR_WRITE_ENABLE_BLUE,
	Alpha = D3D11_COLOR_WRITE_ENABLE_ALPHA,
	All = D3D11_COLOR_WRITE_ENABLE_ALL
};

enum ShaderType {
	Vertex, 
	Pixel,
	Geometry,
	Compute,
};

enum class BufferInputRate {
	PerVertex =D3D11_INPUT_PER_VERTEX_DATA,
	PerInstance = D3D11_INPUT_PER_INSTANCE_DATA
};

enum class BufferLayoutAttribType {
	Float = DXGI_FORMAT_R32_FLOAT,
	Vec2 = DXGI_FORMAT_R32G32_FLOAT,
	Vec3 = DXGI_FORMAT_R32G32B32_FLOAT,
	Vec4 = DXGI_FORMAT_R32G32B32A32_FLOAT,
	Mat4
};

enum class BufferType {
	Uniform,
	Sampler,
	Texture
};

enum class IndexBufferFormat {
	Uint16 = DXGI_FORMAT_R16_UINT,
	Uint32 = DXGI_FORMAT_R32_UINT
};

#else

enum class PrimitiveTopology {
	PointList = VK_PRIMITIVE_TOPOLOGY_POINT_LIST,
	LineList = VK_PRIMITIVE_TOPOLOGY_LINE_LIST,
	LineStrip = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP,
	TriangleList = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
	TriangleStrip = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP
};

enum class PolygonMode {
	Point = VK_POLYGON_MODE_POINT,
	Line = VK_POLYGON_MODE_LINE,
	Fill = VK_POLYGON_MODE_FILL
};

enum class CullMode {
	None = VK_CULL_MODE_NONE,
	Back = VK_CULL_MODE_BACK_BIT,
	Front = VK_CULL_MODE_FRONT_BIT
};

enum class FrontFace {
	Clockwise = VK_FRONT_FACE_CLOCKWISE,
	CounterClockwise= VK_FRONT_FACE_CLOCKWISE
};

enum class ComparisonFunc {
	Never = VK_COMPARE_OP_NEVER,
	Less = VK_COMPARE_OP_LESS,
	Equal  = VK_COMPARE_OP_EQUAL,
	LessEqual = VK_COMPARE_OP_LESS_OR_EQUAL,
	Greater = VK_COMPARE_OP_GREATER,
	NotEqual = VK_COMPARE_OP_NOT_EQUAL,
	GreaterEqual = VK_COMPARE_OP_GREATER_OR_EQUAL,
	Always = VK_COMPARE_OP_ALWAYS
};

enum class StencilOp {
	Keep = VK_STENCIL_OP_KEEP,
	Zero = VK_STENCIL_OP_ZERO,
	Replace = VK_STENCIL_OP_REPLACE,
	IncrementClamp = VK_STENCIL_OP_INCREMENT_AND_CLAMP,
	DecrementClamp = VK_STENCIL_OP_DECREMENT_AND_CLAMP,
	Invert = VK_STENCIL_OP_INVERT,
	Increment = VK_STENCIL_OP_INCREMENT_AND_WRAP,
	Decrement = VK_STENCIL_OP_DECREMENT_AND_WRAP
};

enum class BlendFactor {
	Zero = VK_BLEND_FACTOR_ZERO,
	One = VK_BLEND_FACTOR_ONE,
	SrcColor = VK_BLEND_FACTOR_SRC_COLOR,
	InvSrcColor = VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR,
	SrcAlpha = VK_BLEND_FACTOR_SRC_ALPHA,
	InvSrcAlpha = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
	DestAlpha = VK_BLEND_FACTOR_DST_ALPHA,
	InvDestAlpha = VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA,
	DestColor = VK_BLEND_FACTOR_DST_COLOR,
	InvDestColor = VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR,
	SrcAlphaClamp = VK_BLEND_FACTOR_SRC_ALPHA_SATURATE,
	BlendFactor = VK_BLEND_FACTOR_CONSTANT_COLOR,
	InvBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR,
	Src1Color = VK_BLEND_FACTOR_SRC1_COLOR,
	InvSrc1Color = VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR,
	Src1Alpha = VK_BLEND_FACTOR_SRC1_ALPHA,
	InvSrc1Alpha = VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA
};

enum class BlendOp {
	Add = VK_BLEND_OP_ADD,
	Subtract = VK_BLEND_OP_SUBTRACT,
	RevSubtract = VK_BLEND_OP_REVERSE_SUBTRACT,
	Min = VK_BLEND_OP_MIN,
	Max = VK_BLEND_OP_MAX
};

enum ColorComponentFlag  {
	Red = VK_COLOR_COMPONENT_R_BIT,
	Green = VK_COLOR_COMPONENT_G_BIT,
	Blue = VK_COLOR_COMPONENT_B_BIT,
	Alpha = VK_COLOR_COMPONENT_A_BIT,
	All = Red | Green | Blue | Alpha
};

enum class ShaderType {
	Vertex = VK_SHADER_STAGE_VERTEX_BIT,
	Pixel = VK_SHADER_STAGE_FRAGMENT_BIT,
	Geometry = VK_SHADER_STAGE_GEOMETRY_BIT,
	Compute = VK_SHADER_STAGE_COMPUTE_BIT
};

enum class BufferInputRate {
	PerVertex = VK_VERTEX_INPUT_RATE_VERTEX,
	PerInstance = VK_VERTEX_INPUT_RATE_INSTANCE
};

enum class BufferLayoutAttribType {
	Float = VK_FORMAT_R32_SFLOAT,
	Vec2 = VK_FORMAT_R32G32_SFLOAT,
	Vec3 = VK_FORMAT_R32G32B32_SFLOAT,
	Vec4 = VK_FORMAT_R32G32B32A32_SFLOAT,
	Mat4
};

enum class BufferType {
	Uniform = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
	Sampler = VK_DESCRIPTOR_TYPE_SAMPLER,
	Texture = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE
};

enum class IndexBufferFormat {
	Uint16 = VK_INDEX_TYPE_UINT16,
	Uint32 = VK_INDEX_TYPE_UINT32
};

#endif

}