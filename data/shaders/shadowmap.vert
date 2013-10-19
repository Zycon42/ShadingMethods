#version 420

layout(location = 0) in vec3 pos;

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

void main() {
	gl_Position =  light.viewProjection * node.model * vec4(pos, 1);
}