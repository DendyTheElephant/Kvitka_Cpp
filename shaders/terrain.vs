#version 460 core

// input attributes
layout(location = 0) in vec3 in_vertex_position;
layout(location = 1) in vec3 in_vertex_normal;
layout(location = 2) in vec2 in_vertex_textureCoordinate;

uniform mat4 u_ModelViewProjectionMatrix; // modelviewprojection matrix (constant for all the vertices)

out float v2f_Height;

void main()
{
	// gl_Position is a special variable used to store the final position.
	// Multiply the vertex by the matrix to get the final point in normalized screen coordinates.		
   	gl_Position = u_ModelViewProjectionMatrix * vec4(in_vertex_position.x, in_vertex_position.y, in_vertex_position.z, 1.0);

	v2f_Height = in_vertex_position.y / 4.0;
}