#version 420
uniform sampler2D earth_day;
uniform sampler2D specularity_map;

in vec4 f_color;
in vec4 f_normal;
in vec3 f_coord3d;
in vec2 f_texcoord;

out vec4 output_color;

layout (std140) uniform Global{	
    mat4 camera_rotation; 
    mat4 view; 
    mat4 projection; 
    mat4 scaling;
    mat4 sun_angle;
};

void main(void) {
    float occ = f_color.a;
    float ambient = 0.09;
    vec3 light_source = (camera_rotation * sun_angle * vec4(0.0, 0.0, 20.0, 1.0)).xyz;
    vec3 N = normalize(f_normal).xyz;
    vec3 L = light_source - f_coord3d; // light source
    float sqr_distance = dot(L, L);
    L = normalize(L);
    vec3 V = -normalize(f_coord3d - inverse(view)[3].xyz);
    vec3 R = -reflect(L, N);

    float lambertian = max(dot(N, L), 0.0);
    float specular = 0.0;
    float spec_angle = max(dot(V, R), 0.0);    

    vec3 earth_color = vec3(texture2D(earth_day, f_texcoord + vec2(0.5, 0.0)));
    float specularity = vec3(texture2D(specularity_map, f_texcoord + vec2(0.5, 0.0))).x;
    specular = pow(spec_angle, 20) * specularity;
    vec3 diffuse_color = earth_color;
    vec3 light_color = vec3(0.89, 0.855, 0.686) * 500.0;

    vec3 color = vec3(0.0);
    color += diffuse_color * ambient; // ambient
    color += diffuse_color * lambertian * light_color / sqr_distance; // diffuse
    color += 0.2 * vec3(1.0) * specular * light_color / sqr_distance; // specular

    output_color = vec4(color, occ);    
}
