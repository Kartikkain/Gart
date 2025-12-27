#type vertex
#version 450 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_Texture;
layout(location = 3) in int a_EntityID;



layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjectionMatrix;
};



layout (location = 0) out vec2 v_Texture;
layout (location = 1) out vec4 v_Color;
layout (location = 2) flat out int v_EntityID;
			
void main()
{
	v_Texture = a_Texture;
	v_Color = a_Color;
	v_EntityID = a_EntityID;
	gl_Position = u_ViewProjectionMatrix * vec4(a_Position,1.0);
}

#type fragment
#version 450 core
			
layout(location = 0) out vec4 fragColor;
layout(location = 1) out int fragColor2;
			

layout (location = 0) in vec2 v_Texture;
layout (location = 1) in vec4 v_Color;
layout (location = 2) flat in int v_EntityID;


layout(binding = 0) uniform sampler2D u_Texture;
	

float ScreenPxRange()
{
	const float pxRange = 2.0;
	vec2 unitRange = vec2(pxRange)/vec2(textureSize(u_Texture,0));
	vec2 screenTexSize = vec2(1.0)/fwidth(v_Texture);
	return max(0.5*dot(unitRange,screenTexSize),1.0);
}

float median(float r, float g, float b)
{
	return max(min(r,g), min(max(r,g),b));
}

void main()
{
	vec4 texColor = v_Color * texture(u_Texture,v_Texture);
	
	vec3 msd = texture(u_Texture,v_Texture).rgb;
	float sd = median(msd.r, msd.g, msd.b);
	float screenPxDistance = ScreenPxRange() * (sd - 0.5);
	float Opacity = clamp(screenPxDistance + 0.5,0.0,1.0);

	if(Opacity == 0)
		discard;

	vec4 bgColor = vec4(vec3(0.0),1.0);

	fragColor = mix(bgColor, v_Color, Opacity);
	
	fragColor2 = v_EntityID;
}