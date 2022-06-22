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

// Output
layout(location = 0) out vec4 out_color;


void main()
{
   vec3 normalized_world_normal = normalize(world_normal);
   vec3 L = normalize( light_position - world_position);
   vec3 V = normalize( eye_position - world_position);
   vec3 H = normalize( L + V );
    // TODO(student): Define ambient, diffuse and specular light components
    float ambient_light = 0.25;
    float diffuse_light = material_kd * max (dot(normalized_world_normal,L), 0);
    float specular_light = material_ks * pow(max(dot(normalized_world_normal, H), 0), 3);
    // It's important to distinguish between "reflection model" and
    // "shading method". In this shader, we are experimenting with the Phong
    // (1975) and Blinn-Phong (1977) reflection models, and we are using the
    // Phong (1975) shading method. Don't mix them up!
    if (diffuse_light > 0)
    {
        specular_light = material_ks * pow(max(dot(normalized_world_normal, H), 0), material_shininess);
    }


     // TODO(student): If (and only if) the light is a spotlight, we need to do
    // some additional things.
    float spot;
    if (spot_light == 1) {
        float cut_off = radians(cut_off_input);
        float spot_light1 = dot(-L, light_direction);
        float spot_light_limit = cos(cut_off);
        float linear_att = (spot_light1 - spot_light_limit) / (1.0f - spot_light_limit);
        float light_att_factor = pow(linear_att, 2);
        float light = max(dot(world_normal, L),0.0);

        if (spot_light1 > cos(cut_off)) {
            spot = light_att_factor;
       } 
       
    }


    // TODO(student): Compute the total light. You can just add the components
    // together, but if you're feeling extra fancy, you can add individual
    // colors to the light components. To do that, pick some vec3 colors that
    // you like, and multiply them with the respective light components.
    float distance = distance(light_position, world_position);
    float attenuation = 1 / pow(distance, 2);

    // TODO(student): Write pixel out color
    out_color = vec4(object_color * (ambient_light + (spot_light == 1 ? spot : attenuation) * (diffuse_light + specular_light)), 1);
    
    
   
}
