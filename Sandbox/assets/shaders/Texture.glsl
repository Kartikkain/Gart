#type vertex
#version 330 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_Texture;

out vec4 v_Color;
out vec2 v_Texture;

uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_Transform;
			
void main()
{
	v_Color = a_Color;
	v_Texture = a_Texture;
	gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Position,1.0);
}

#type fragment
#version 330 core
			
layout(location = 0) out vec4 fragColor;
			
in vec4 v_Color;
in vec2 v_Texture;

uniform vec3 u_Color;	
uniform sampler2D u_Texture;		
	
void main()
{
	fragColor = texture(u_Texture,v_Texture);
}