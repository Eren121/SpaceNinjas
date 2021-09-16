#version 330 core

uniform sampler2D u_Texture;
uniform vec4 u_Color;

in vec4 color; // Global color
in vec2 uv;

out vec4 out_Color;

void main()
{
    vec4 textureColor = texture(u_Texture, uv);

    out_Color = u_Color * color * textureColor;
}