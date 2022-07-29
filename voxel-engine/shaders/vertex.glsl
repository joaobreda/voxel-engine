#version 330 core
layout (location = 0) in vec4 aPos;

out vec3 ourColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos.xyz, 1.0f);
    // Snow
	if (aPos.w == 1)
	    ourColor = vec3(255.0f/255.0f, 255.0f/255.0f, 255.0f/255.0f);
	// Rock
	else if (aPos.w == 2)
	    ourColor = vec3(142.0f/255.0f, 142.0f/255.0f, 142.0f/255.0f);
    // Dirt
	else if (aPos.w == 3)
	    ourColor = vec3(113.0f/255.0f, 79.0f/255.0f, 54.0f/255.0f);
    // Grass
	else if (aPos.w == 4)
	    ourColor = vec3(11.0f/255.0f, 128.0f/255.0f, 0.0f/255.0f);
    // Sand
	else if (aPos.w == 5)
	    ourColor = vec3(240.0f/255.0f, 240.0f/255.0f, 64.0f/255.0f);
    // Shallow
	else
	    ourColor = vec3(0.0f/255.0f, 0.0f/255.0f, 255.0f/255.0f);
}