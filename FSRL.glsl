
in vec3 frag_position;
in vec3 frag_normal;

uniform samplerCube cubemap;  
vec3 camera_position = vec3(0,0,3);

void main()
{             
	vec3 r = reflect(normalize(frag_position-camera_position),normalize(frag_normal));
    gl_FragColor = texture(cubemap, r);

    //gl_FragColor = vec4(frag_uv0,1);
}