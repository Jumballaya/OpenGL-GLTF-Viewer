#version 460 core

precision mediump float;

out vec4 outColor;

in vec2 texCoord;

uniform sampler2D u_texture;

void main() {
    outColor = texture(u_texture, texCoord);
}