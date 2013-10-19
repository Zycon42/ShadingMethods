#version 420

in VertexData {
	vec3 normal;
	vec3 worldPos;
} VertexOut;

out vec4 color;

layout(binding = 0, std140) uniform CameraBlock {
	mat4 view;
	mat4 projection;
	mat4 viewProjection;
	vec3 pos;
} camera;

layout(binding = 2, std140) uniform MaterialBlock {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
} material;

layout(binding = 3, std140) uniform LightBlock {
	vec4 pos;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
} light;

void main() {
	// Normal of the computed fragment, in camera space
	vec3 n = normalize(VertexOut.normal);
	// Direction of the light (from the fragment to the light) we use directional light
	vec3 l = normalize(-light.pos.xyz);
	// Cosine of the angle between the normal and the light direction, 
	// clamped above 0
	//  - light is at the vertical of the triangle -> 1
	//  - light is perpendicular to the triangle -> 0
	//  - light is behind the triangle -> 0
	float cosTheta = clamp(dot(n,l), 0, 1);
	
	// Eye vector (towards the camera)
	vec3 E = normalize(camera.pos - VertexOut.worldPos);
	// Direction in which the triangle reflects the light
	vec3 R = reflect(-l,n);
	// Cosine of the angle between the Eye vector and the Reflect vector,
	// clamped to 0
	//  - Looking into the reflection -> 1
	//  - Looking elsewhere -> < 1
	float cosAlpha = clamp(dot(E,R), 0, 1);
	
	color = 
		// Ambient : simulates indirect lighting
		material.ambient * light.ambient +
		// Diffuse : "color" of the object
		material.diffuse * light.diffuse * cosTheta;
		// Specular : reflective highlight, like a mirror
		material.specular * light.specular * pow(cosAlpha, material.shininess);
}