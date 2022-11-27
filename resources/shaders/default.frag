#version 330 core

in vec3 color;
in vec2 uv;

out vec4 FragColor;

uniform sampler2D tex0;

void main()
{
    FragColor = texture(tex0, uv) * vec4(color.rgb, 1.0);
}
