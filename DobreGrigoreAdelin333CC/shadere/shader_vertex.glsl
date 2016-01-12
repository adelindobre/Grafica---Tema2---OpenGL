#version 330
	layout(location = 0) in vec3 in_position;
	uniform mat4 Model, View, Projection;
	void main(){
		gl_Position = Projection * View * Model * vec4(in_position, 1); 
	}