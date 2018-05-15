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

enum ShaderType {
	ShaderTypeVertex = VK_SHADER_STAGE_VERTEX_BIT,
	ShaderTypePixel = VK_SHADER_STAGE_FRAGMENT_BIT,
	ShaderTypeGeometry = VK_SHADER_STAGE_GEOMETRY_BIT,
	ShaderTypeCompute = VK_SHADER_STAGE_COMPUTE_BIT
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

enum class DescriptorType {
	Uniform = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
//	Sampler = VK_DESCRIPTOR_TYPE_SAMPLER,
//	Texture = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
	InputAttachment = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,
	TextureSampler = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
};

enum class IndexBufferFormat {
	Uint16 = VK_INDEX_TYPE_UINT16,
	Uint32 = VK_INDEX_TYPE_UINT32
};

enum class SamplerFilter {
	Linear = VK_FILTER_LINEAR,
	Nearest = VK_FILTER_NEAREST
};

enum class SamplerAddressMode {
	Repeat = VK_SAMPLER_ADDRESS_MODE_REPEAT,
	MirroredRepeat = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT,
	ClampToEdge = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
	ClampToBorder = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER,
	MirrorClampToEdge = VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE
};

enum class SamplerBorderColor {
	White = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE,
	Black = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK,
	TransparentBlack = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK,
};

enum class Format {
	R8 = VK_FORMAT_R8_UNORM,
	R8G8 = VK_FORMAT_R8G8_UNORM,
	R8G8B8 = VK_FORMAT_R8G8B8_UNORM,
	R8G8B8A8 = VK_FORMAT_R8G8B8A8_UNORM,

	R32 = VK_FORMAT_R32_SFLOAT,
	R32G32 = VK_FORMAT_R32G32_SFLOAT,
	R32G32B32 = VK_FORMAT_R32G32B32_SFLOAT,
	R32G32B32A32 = VK_FORMAT_R32G32B32A32_SFLOAT,

	D16 = VK_FORMAT_D16_UNORM,
	D32 = VK_FORMAT_D32_SFLOAT,
	D24S8 = VK_FORMAT_D24_UNORM_S8_UINT,
	D32S8 = VK_FORMAT_D32_SFLOAT_S8_UINT
};

enum class CommandBufferType {
	Unknown = ~0,
	Primary = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
	Secondary = VK_COMMAND_BUFFER_LEVEL_SECONDARY
};

enum class CommandBufferUsage {
	Simultaneous = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,
	OneTime = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
};

}