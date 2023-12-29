 #version 330

#define MAX_LIGHTS 16

#define DIRECTIONAL 0
#define POINT 1
#define SPOT 2

struct Light {
    int type;
    vec3 color;
    vec3 position;
    vec3 direction;
    vec3 diffuse;
    vec3 specular;
    vec3 attenuation; // x*d^2 + y*d + z
    vec2 cone_angles; // x: inner_angle, y: outer_angle
};

uniform Light lights[MAX_LIGHTS];
uniform int light_count;


struct Material {
    sampler2D albedo;
    sampler2D specular;
    sampler2D ambient_occlusion;
    sampler2D roughness;
    sampler2D emissive;
};

uniform Material material;

in Varyings {
    vec4 color;
    vec2 tex_coord;
    vec3 normal;
    vec3 view;
    vec3 world;
} fs_in;

out vec4 frag_color;

void main(){
    //normalize vectors
    vec3 view = normalize(fs_in.view);
    vec3 normal = normalize(fs_in.normal);

    vec3 material_diffuse = texture(material.albedo, fs_in.tex_coord).rgb;
    vec3 material_specular = texture(material.specular, fs_in.tex_coord).rgb;
    vec3 material_ambient = material_diffuse * texture(material.ambient_occlusion, fs_in.tex_coord).r;
    
    float material_roughness = texture(material.roughness, fs_in.tex_coord).r;
    float material_shininess = 2.0 / pow(clamp(material_roughness, 0.001, 0.999), 4.0) - 2.0;

    // Set to 0 in entities without emissive properties.
    vec3 material_emissive = texture(material.emissive, fs_in.tex_coord).rgb;

    frag_color = vec4(material_emissive  , 1.0);

    int clamped_light_count = min(MAX_LIGHTS, light_count);
    for(int i = 0; i < clamped_light_count; i++){
        Light light = lights[i];

        vec3 direction_to_light = -light.direction;
        if(light.type != DIRECTIONAL){
            direction_to_light = normalize(light.position - fs_in.world);
        }
        
        // Diffuse component
        vec3 diffuse = light.diffuse * material_diffuse * max(0, dot(normal, direction_to_light));
        
         // Specular component
        vec3 reflected = reflect(-direction_to_light, normal);
        vec3 specular = light.specular * material_specular * pow(max(0, dot(view, reflected)), material_shininess);

        // Ambient component
        vec3 total_ambiance = material_ambient *  light.color;

        // Attenuation calculation
        float attenuation = 1;
        if(light.type != DIRECTIONAL){
            float d = distance(light.position, fs_in.world);
            attenuation /= dot(light.attenuation, vec3(d*d, d, 1));

            // Spotlight specific calculations
            if(light.type == SPOT){
                float angle = acos(dot(-direction_to_light, light.direction));
                attenuation *= smoothstep(light.cone_angles.y, light.cone_angles.x, angle);
            }
        }

        // Accumulate light contributions
        frag_color.rgb += ((diffuse + specular) * attenuation )+ total_ambiance ;
    }
}