#version 330

// Input
// TODO(student): Get color value from vertex shader
in vec3 frag_color;
in vec3 worldPosition;
in vec3 worldNormal;

uniform vec3 eyePosition;

uniform vec3 materialKa;
uniform vec3 materialKd;
uniform vec3 materialKs;
uniform int materialShininess;

uniform vec3 lightPosition;

// uniforma trimisa din functia RenderSimpleMesh (objec_color ia valoarea parametrului color din functie)
uniform vec3 object_color;

// Output
layout(location = 0) out vec4 out_color;

vec3 ComputePhongIllumination(vec3 lightPosition)
{
    vec3 N = normalize(worldNormal);
    vec3 L = normalize(lightPosition - worldPosition);
    vec3 diffuseComponent = materialKd * max(dot(N, L), 0.0);

    vec3 V = normalize(eyePosition - worldPosition);
    vec3 R = reflect(-L, N);
    // vec3 specularComponent = materialKs * pow(max(dot(V, R), 0.0), materialShininess);
    //
    // if (length(diffuseComponent) < 0.0)
    // {
    //     specularComponent = vec3(0.0);
    // }

    vec3 illumination = diffuseComponent;

    return illumination;
}

vec3 ComputeAmbientComponent()
{
    vec3 globalAmbientColor = vec3(0.25);

    vec3 ambientComponent = materialKa * globalAmbientColor;

    return ambientComponent;
}

void main()
{
    // TODO(student): Write pixel out color
    // out_color = vec4(object_color, 1);
    vec3 illumination =  ComputePhongIllumination(lightPosition) + ComputeAmbientComponent();
    out_color = vec4(illumination, 1.0);
} // Fragment Shader