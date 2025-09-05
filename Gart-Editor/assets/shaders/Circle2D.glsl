#type vertex
#version 450 core
			
layout(location = 0) in vec3 a_WorldPosition;
layout(location = 1) in vec3 a_LocalPosition;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in float a_Thickness;
layout(location = 4) in float a_Fade;
layout(location = 5) in int a_EntityID;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjectionMatrix;
};

layout (location = 0) out vec3 v_LocalPosition;
layout (location = 1) out vec4 v_Color;
layout (location = 2) out float v_Thickness;
layout (location = 3) out float v_Fade;
layout (location = 4) flat out int v_EntityID;
			
void main()
{
	v_LocalPosition = a_LocalPosition;
	v_Color = a_Color;
	v_Thickness = a_Thickness;
	v_Fade = a_Fade;
	v_EntityID = a_EntityID;
	gl_Position = u_ViewProjectionMatrix * vec4(a_WorldPosition,1.0);
}

#type fragment
#version 450 core
			
layout(location = 0) out vec4 fragColor;
layout(location = 1) out int fragColor2;
			

layout (location = 0) in vec3 v_LocalPosition;
layout (location = 1) in vec4 v_Color;
layout (location = 2) in float v_Thickness;
layout (location = 3) in float v_Fade;
layout (location = 4) flat in int v_EntityID;

void main()
{
	vec2 uv = v_LocalPosition.xy;
	// Calculate distance and fill circle with white
	float distance = 1.0 - length(uv);
	float color = smoothstep(0.0f, v_Fade, distance);
	color *= smoothstep(v_Thickness + v_Fade, v_Thickness, distance);
	
	if (color == 0.0)
		discard;

	fragColor = v_Color;
	fragColor.a *= color;

	fragColor2 = v_EntityID;
}