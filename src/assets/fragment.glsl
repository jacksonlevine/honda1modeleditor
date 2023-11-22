#version 450 core
in vec3 vertexColor;
in vec2 TexCoord;
in vec3 pos;
out vec4 FragColor;
uniform sampler2D ourTexture;
uniform vec3 camPos;
uniform float brightness;
void main()
{

    vec4 texColor = texture(ourTexture, TexCoord);



    vec3 fogColor = vec3(0.2, 0.2, 0.7);
    float diss = pow(distance(pos, camPos)/45.0f, 2);

    if(gl_FragCoord.z < 0.9978f) { 
        diss = 0;
    } else { 
        diss = (diss-0.9978f)*70;
    }


    vec3 finalColor = mix(vertexColor, fogColor, max(diss/4.0, 0));
    FragColor = mix(vec4(finalColor, texColor.a) * texColor, vec4(fogColor, texColor.a), max(diss/4.0, 0));

    FragColor.a -= max(diss, 0.0);

    if(FragColor.a < 1.0) {
        discard;
    } 
    
    FragColor = FragColor * brightness;
}