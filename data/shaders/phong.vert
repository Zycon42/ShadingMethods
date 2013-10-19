#version 420

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;

out VertexData {
	vec3 normal;
	vec3 worldPos;
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

void main() {
	// Normal of the the vertex, in world space
	VertexOut.normal = normalize(node.normalMatrix * vec4(normal, 0)).xyz;
	
	// Vertex pos in world space
	VertexOut.worldPos = (node.model * vec4(pos,1)).xyz;
	
	gl_Position = camera.viewProjection * vec4(VertexOut.worldPos, 1);
}