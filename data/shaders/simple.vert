#version 420

layout(location = 0) in vec3 pos;

layout(binding = 0, std140) uniform CameraBlock
{
	mat4 view;
	mat4 projection;
	mat4 viewProjection;
	vec3 pos;
} camera;

void main() {
	gl_Position = camera.viewProjection * vec4(pos, 1);
}