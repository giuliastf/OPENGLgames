#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;

out vec3 frag_color;  // Pass the normal to the fragment shader

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Output value to fragment shader
out vec3 worldPosition;
out vec3 worldNormal;

void main()
{
    worldNormal = normalize(mat3(Model) * in_normal);

    worldPosition = (Model * vec4(in_position, 1.0)).xyz;

    frag_color = mat3(transpose(inverse(Model))) * in_normal;  // Transform normal to world space
    gl_Position = Projection * View * Model * vec4(in_position, 1.0);
} // Vertex Shader