#version 330 core
out vec4 FragColor;

in vec2 Texcoord;
uniform sampler2D ourTex;

void main()
{
  FragColor = texture(ourTex, Texcoord);
}
