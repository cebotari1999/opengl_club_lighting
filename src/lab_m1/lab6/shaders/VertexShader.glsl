#version 330

// Input
// TODO(student): Get vertex attributes from each location
layout (location = 0) in vec3 v_position;
layout (location = 3) in vec3 v_normal;
layout (location = 2) in vec2 v_coordinate;
layout (location = 1) in vec3 v_color;
// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float Time;

// Output
// TODO(student): Output values to fragment shader
out vec3 frag_color;
out vec3 frag_position;
out vec3 frag_normal;
out vec2 frag_coordinate;

void main()
{
    // TODO(student): Send output to fragment shader
    frag_position = v_position;
    frag_normal = v_normal + vec3(sin(Time), cos(Time), sin(Time) - cos(Time));
    frag_color = v_color;
    frag_coordinate = v_coordinate;
    // TODO(student): Compute gl_Position
    vec4 vec_final_position = Model * vec4(v_position, 1.f) + vec4(1.f, cos(Time), 0.f, 0.f) * 2.f;
    gl_Position = Projection * View * Model * vec_final_position;
}
