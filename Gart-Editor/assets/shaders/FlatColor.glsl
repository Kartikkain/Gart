#type vertex
#version 330 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;

out vec4 v_Color;

uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_Transform;
			
void main()
{
	v_Color = a_Color;
	gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Position,1.0);
}

#type fragment
#version 330 core
			
layout(location = 0) out vec4 fragColor;
			
in vec4 v_Color;
in vec2 v_Texture;

uniform vec4 u_Color;	
uniform sampler2D u_Texture;		
	
void main()
{
	fragColor = u_Color;
}