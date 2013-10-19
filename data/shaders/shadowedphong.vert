#version 420

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;

out VertexData {
	vec3 normal;
	vec3 worldPos;
	vec4 shadowCoord;
} VertexOut;

layout(binding = 0, std140) uniform CameraBlock {
	mat4 view;
	mat4 projection;
	mat4 viewProjection;
	vec3 pos;
} camera;

layout(binding = 1, std140) uniform NodeBlock {
	mat4 model;
	mat4 normalMatrix;
} node;

layout(binding = 3, std140) uniform LightBlock {
	vec4 pos;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	mat4 viewProjection;
} light;

// bias matrix to convert shadowCoord to texture space
const mat4 biasMatrix = mat4(
	0.5, 0.0, 0.0, 0.0,
	0.0, 0.5, 0.0, 0.0,
	0.0, 0.0, 0.5, 0.0,
	0.5, 0.5, 0.5, 1.0
);

void main() {
	// Normal of the the vertex, in world space
	VertexOut.normal = normalize(node.normalMatrix * vec4(normal, 0)).xyz;
	
	// Vertex pos in world space
	VertexOut.worldPos = (node.model * vec4(pos,1)).xyz;
	
	gl_Position = camera.viewProjection * vec4(VertexOut.worldPos, 1);
	
	VertexOut.shadowCoord = biasMatrix * light.viewProjection * vec4(VertexOut.worldPos, 1);
}