#type vertex
#version 450 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_Texture;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TillingFactor;
layout(location = 5) in int a_EntityID;



layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjectionMatrix;
};



layout (location = 0) out vec2 v_Texture;
layout (location = 1) out vec4 v_Color;
layout (location = 2) out float v_TexIndex;
layout (location = 3) out float v_TillingFactor;
layout (location = 4) flat out int v_EntityID;
			
void main()
{
	v_Texture = a_Texture;
	v_Color = a_Color;
	v_TexIndex = a_TexIndex;
	v_TillingFactor = a_TillingFactor;
	v_EntityID = a_EntityID;
	gl_Position = u_ViewProjectionMatrix * vec4(a_Position,1.0);
}

#type fragment
#version 450 core
			
layout(location = 0) out vec4 fragColor;
layout(location = 1) out int fragColor2;
			

layout (location = 0) in vec2 v_Texture;
layout (location = 1) in vec4 v_Color;
layout (location = 2) in float v_TexIndex;
layout (location = 3) in float v_TillingFactor;  
layout (location = 4) flat in int v_EntityID;


layout(binding = 0) uniform sampler2D u_Texture[32];		
	
void main()
{
	vec4 texColor = v_Color;
	switch(int(v_TexIndex))
	{
		case 0: texColor *= texture(u_Texture[0],v_Texture * v_TillingFactor); break;
		case 1: texColor *= texture(u_Texture[1],v_Texture * v_TillingFactor); break;
		case 2: texColor *= texture(u_Texture[2],v_Texture * v_TillingFactor); break;
		case 3: texColor *= texture(u_Texture[3],v_Texture * v_TillingFactor); break;
		case 4: texColor *= texture(u_Texture[4],v_Texture * v_TillingFactor); break;
		case 5: texColor *= texture(u_Texture[5],v_Texture * v_TillingFactor); break;
		case 6: texColor *= texture(u_Texture[6],v_Texture * v_TillingFactor); break;
		case 7: texColor *= texture(u_Texture[7],v_Texture * v_TillingFactor); break;
		case 8: texColor *= texture(u_Texture[8],v_Texture * v_TillingFactor); break;
		case 9: texColor *= texture(u_Texture[9],v_Texture * v_TillingFactor); break;
		case 10: texColor *= texture(u_Texture[10],v_Texture * v_TillingFactor); break;
		case 11: texColor *= texture(u_Texture[11],v_Texture * v_TillingFactor); break;
		case 12: texColor *= texture(u_Texture[12],v_Texture * v_TillingFactor); break;
		case 13: texColor *= texture(u_Texture[13],v_Texture * v_TillingFactor); break;
		case 14: texColor *= texture(u_Texture[14],v_Texture * v_TillingFactor); break;
		case 15: texColor *= texture(u_Texture[15],v_Texture * v_TillingFactor); break;
		case 16: texColor *= texture(u_Texture[16],v_Texture * v_TillingFactor); break;
		case 17: texColor *= texture(u_Texture[17],v_Texture * v_TillingFactor); break;
		case 18: texColor *= texture(u_Texture[18],v_Texture * v_TillingFactor); break;
		case 19: texColor *= texture(u_Texture[19],v_Texture * v_TillingFactor); break;
		case 20: texColor *= texture(u_Texture[20],v_Texture * v_TillingFactor); break;
		case 21: texColor *= texture(u_Texture[21],v_Texture * v_TillingFactor); break;
		case 22: texColor *= texture(u_Texture[22],v_Texture * v_TillingFactor); break;
		case 23: texColor *= texture(u_Texture[23],v_Texture * v_TillingFactor); break;
		case 24: texColor *= texture(u_Texture[24],v_Texture * v_TillingFactor); break;
		case 25: texColor *= texture(u_Texture[25],v_Texture * v_TillingFactor); break;
		case 26: texColor *= texture(u_Texture[26],v_Texture * v_TillingFactor); break;
		case 27: texColor *= texture(u_Texture[27],v_Texture * v_TillingFactor); break;
		case 28: texColor *= texture(u_Texture[28],v_Texture * v_TillingFactor); break;
		case 29: texColor *= texture(u_Texture[29],v_Texture * v_TillingFactor); break;
		case 30: texColor *= texture(u_Texture[30],v_Texture * v_TillingFactor); break;
		case 31: texColor *= texture(u_Texture[31],v_Texture * v_TillingFactor); break;
		
	}
	fragColor = texColor;
	fragColor2 = v_EntityID;
}