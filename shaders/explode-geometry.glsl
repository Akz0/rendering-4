#version 330 core

layout (triangles) in;
layout (triangle_strip,max_vertices = 3) out;

out vec3 CurrentPosition;
out vec3 Normal;
out vec3 Color;
out vec2 TexCord;
out vec3 lightPosition;
out vec3 CameraPosition;
out mat3 TBN;
in DATA
{
	vec3 CurrentPosition;
	vec3 Normal;
	vec3 Color;
	vec2 TexCord;
	mat4 CameraMatrix;
	mat4 model;
	vec3 lightPosition;
	vec3 CameraPosition;
} data_in[];


void main(){
	
	vec3 edge0 = gl_in[1].gl_Position.xyz -  gl_in[0].gl_Position.xyz;
	vec3 edge1 = gl_in[2].gl_Position.xyz -  gl_in[0].gl_Position.xyz;

	vec2 dUV0 = data_in[1].TexCord - data_in[0].TexCord;
	vec2 dUV1 = data_in[2].TexCord - data_in[0].TexCord;

	float iD = 1.0f / (dUV0.x * dUV1.y - dUV1.x * dUV0.y);

	vec3 tangent = vec3(iD * (dUV1.y * edge0 - dUV0.y * edge1 ));
	vec3 biTangent = vec3(iD * (-dUV1.x * edge0 - dUV0.x * edge1 ));

	vec3 T = normalize(vec3(data_in[0].model * vec4(tangent,0.0f)));
	vec3 B = normalize(vec3(data_in[0].model * vec4(biTangent,0.0f)));
	vec3 N = normalize(vec3(data_in[0].model * vec4(cross(edge1,edge0),0.0f)));
	
	mat3 TBN = mat3(T,B,N);
	TBN = transpose(TBN);

	
	gl_Position = data_in[0].CameraMatrix * (gl_in[0].gl_Position);
	CurrentPosition = data_in[0].CurrentPosition;
	Normal = data_in[0].Normal;
	Color = data_in[0].Color;
	TexCord = data_in[0].TexCord;
	lightPosition = data_in[0].lightPosition;
	CameraPosition = data_in[0].CameraPosition;
	TBN = transpose(TBN);
	EmitVertex();

	gl_Position = data_in[1].CameraMatrix * (gl_in[1].gl_Position);
	CurrentPosition =data_in[1].CurrentPosition;
	Normal = data_in[1].Normal;
	Color = data_in[1].Color;
	TexCord = data_in[1].TexCord;
	lightPosition =  data_in[1].lightPosition;
	CameraPosition =  data_in[1].CameraPosition;
	TBN = transpose(TBN);
	EmitVertex();

	
	gl_Position = data_in[2].CameraMatrix * (gl_in[2].gl_Position);
	CurrentPosition =  data_in[2].CurrentPosition;
	Normal = data_in[2].Normal;
	Color = data_in[2].Color;
	TexCord = data_in[2].TexCord;
	lightPosition = data_in[2].lightPosition;
	CameraPosition = data_in[2].CameraPosition;
	TBN = transpose(TBN);
	EmitVertex();

	EndPrimitive();
}


