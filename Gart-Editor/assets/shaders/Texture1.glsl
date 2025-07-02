#type vertex
#version 330 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_Texture;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TillingFactor;
layout(location = 5) in int a_EntityID;

out vec2 v_Texture;
out vec4 v_Color;
out float v_TexIndex;
out float v_TillingFactor;
flat out int v_EntityID;

uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_Transform;
			
void main()
{
	v_Texture = a_Texture;
	v_Color = a_Color;
	v_TexIndex = a_TexIndex;
	v_TillingFactor = a_TillingFactor;
	v_EntityID = a_EntityID;
	//gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Position,1.0);
	gl_Position = u_ViewProjectionMatrix * vec4(a_Position,1.0);
}

#type fragment
#version 330 core
			
layout(location = 0) out vec4 fragColor;
layout(location = 1) out int fragColor2;
			
in vec2 v_Texture;
in vec4 v_Color;
in float v_TexIndex;
in float v_TillingFactor;
flat in int v_EntityID;

uniform vec4 u_Color;
uniform float u_Tilling;
uniform sampler2D u_Texture[32];		
	
void main()
{
	fragColor = texture(u_Texture[int(v_TexIndex)],v_Texture * v_TillingFactor) * v_Color;
	fragColor2 = v_EntityID;
	//fragColor = v_Color;
}