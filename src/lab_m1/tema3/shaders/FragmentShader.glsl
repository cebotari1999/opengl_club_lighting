#version 330

in vec3 world_position;
in vec3 world_normal;
in vec2 texcoord;

uniform sampler2D texture_1;
uniform vec3 lights_positions[64];
uniform vec3 spot_lights[4];
uniform vec3 lights_colors[64];

uniform vec3 light_direction, eye_position;

uniform float material_kd, material_ks, material_shininess;


uniform float x, y;
uniform int mode, is_floor, add_texture;

uniform float sphere_rotation_angle, cone_roation_angle;

uniform float cut_off_input;

uniform vec3 object_color;
vec3 texture_color;

layout(location = 0) out vec4 out_color;


vec3 amount_floor_lights() {
    vec3 W, L, V, H;
    vec3 amount = vec3(0, 0, 0);
    float specular_light, diffuse_light;

    W = normalize(world_normal);

    for (int i = 0; i < 64; i++) {
        V = normalize(eye_position - world_position);
        L = normalize(lights_positions[i] - world_position);
        H = normalize(L + V);

        diffuse_light = material_kd * max(dot(normalize(W),L), 0);
        specular_light = material_ks * pow(max(dot(normalize(W), H), 0), 3);

        if (diffuse_light > 0)
        {
            specular_light = material_ks * pow(max(dot(W, H), 0), material_shininess);
        }
       
        float att = 0;

        if (distance(lights_positions[i], world_position) <= 2.0) {
            float dist = distance(lights_positions[i], world_position);
            att = pow(2.0 - dist, 2);
        }


        amount += lights_colors[i] * att * (diffuse_light + specular_light);
    }
        return amount;
}


vec3 globe_light() {
    vec3 W, L, V, H;
    vec3 amount = vec3(0, 0, 0);
    float specular_light, diffuse_light;

    W = normalize(world_normal);
    L = normalize(vec3(4.5, 5.5, 4.5) - world_position);
    H = normalize(L + V );

    diffuse_light =  0.5 * max(dot(normalize(W),L), 0);
    specular_light = 0.1 * pow(max(dot(normalize(W), H), 0), 3);


    if (diffuse_light > 0)
    {
        specular_light = 0.1 * pow(max(dot(W, H), 0), material_shininess);
    }
        
    vec3 light_dir = world_position - vec3(4.5, 5.5, 4.5);
    vec2 texcoord1;

    texcoord1.x = (1.0 / (2 * 3.14159)) * atan (light_dir.x, light_dir.z);
    texcoord1.y = (1.0 / 3.14159) * acos (light_dir.y / length (light_dir));
    texcoord1.x += sphere_rotation_angle;
    texture_color = texture(texture_1, texcoord1).xyz;

    amount  += texture_color * (1.2 * (diffuse_light + specular_light));

    return amount;
   }

vec3 spot_light() {
    vec3 W, L, V, H;
    float specular_light, diffuse_light;
    vec3 amount = vec3(0, 0, 0);

    W = normalize(world_normal);

    for (int i = 0; i < 4; i++) {
        float cut_off = radians(cut_off_input);
        vec3 l = light_direction;
        if (x == 1) {
            l.z -= cone_roation_angle * 0.3;
        }

        if (y == 1) {
            l.x += cone_roation_angle * 0.3;
        }
      
        vec3 L = normalize(spot_lights[i] - world_position);
        float spot_light = dot(-L, l);
        float spot_light_limit = cos(cut_off);
        float linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
        float light_att_factor = pow(linear_att, 2);
        
        if (spot_light > cos(cut_off)) {
            amount += (lights_colors[i] * 2 * light_att_factor);
        } 
    }

    return amount;
}

void main()
{
    vec3 W, L, V, H;
    vec3 flor, globe, spot;
    float specular_light, diffuse_light;

    flor = amount_floor_lights();
    globe = globe_light();
    spot = spot_light();

    if (mode == 0) {
        if (add_texture == 1) {
            out_color = vec4(texture_color, 1);
        } else {
            if(is_floor == 1) {
                out_color = vec4(0.5 * object_color + globe  + flor + spot, 0.5f);
            } else {
                out_color = vec4(globe  + flor + spot, 0.5f);
            } 
        }
    }

    if (mode == 1) {
        if (is_floor == 1) {
            out_color = vec4(object_color + flor, 1);
        } else {
            out_color = vec4(flor, 1);
        }
    }

    if (mode == 2) {
        out_color = vec4(spot, 0.5f);
    }
     
    if (mode == 3) {
        if (add_texture == 1) {
            out_color = vec4(texture_color, 1);
        } else {
            out_color = vec4(globe, 1);
       }
    }
}
