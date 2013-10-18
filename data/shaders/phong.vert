#version 420

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;

out VertexData {
	vec3 normal;
	vec3 eyeDir;
	vec3 lightDir;
} VertexOut;

layout(binding = 0) uniform CameraBlock {
	mat4 view;
	mat4 projection;
	mat4 viewProjection;
} camera;

layout(binding = 1) uniform NodeBlock {
	mat4 model;
} node;

layout(binding = 3) uniform LightBlock {
	vec4 pos;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
} light;

void main() {
	// TODO: move this to NodeBlock
	mat4 modelView = camera.view * node.model;
	mat4 normalMatrix = transpose(inverse(modelView));

	// Normal of the the vertex, in camera space
	VertexOut.normal = normalize(modelView * vec4(normal, 0)).xyz;
	
	// Vector that goes from the vertex to the camera, in camera space.
	// In camera space, the camera is at the origin (0,0,0).
	vec3 vecPos = (modelView * vec4(pos,1)).xyz;
	VertexOut.eyeDir = vec3(0,0,0) - vecPos;
	
	// Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
	vec3 lightPos = (camera.view * light.pos).xyz;
	VertexOut.lightDir = lightPos + VertexOut.eyeDir;
	
	gl_Position = camera.viewProjection * node.model * vec4(pos, 1);
}