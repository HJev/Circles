#version 330

layout (location = 0) in vec3 Pos;

void main(){
	gl_Position = vec4(Pos);
}