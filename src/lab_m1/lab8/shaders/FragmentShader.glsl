#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

// TODO(student): Declare any other uniforms
uniform int spot_light;
uniform float cut_off_input;

uniform vec3 object_color;
varying vec2 val;
// Output
layout(location = 0) out vec4 out_color;



void main()
{
   float R = 1.0;
    float R2 = 0.5;
    float dist = sqrt(dot(val,val));
    if (dist >= R || dist <= R2) {
        discard;
    }
    float sm = smoothstep(R,R-0.01,dist);
    float sm2 = smoothstep(R2,R2+0.01,dist);
    float alpha = sm*sm2;
    out_color = vec4(0.0, 0.0, 1.0, alpha);
    
   
}
