#type vertex
#version 450 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 3) in int a_EntityID;



layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjectionMatrix;
};



layout (location = 0) out vec4 v_Color;
layout (location = 1) flat out int v_EntityID;
			
void main()
{
	v_Color = a_Color;
	v_EntityID = a_EntityID;
	gl_Position = u_ViewProjectionMatrix * vec4(a_Position,1.0);
}

#type fragment
#version 450 core
			
layout(location = 0) out vec4 fragColor;
layout(location = 1) out int fragColor2;
			

layout (location = 0) in vec4 v_Color;
layout (location = 1) flat in int v_EntityID;

	
void main()
{
	
	fragColor = v_Color;
	fragColor2 = v_EntityID;
}