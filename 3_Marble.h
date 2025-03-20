
#pragma once



GLuint shaderProgramObject_Marble = 0;

// for sphere
GLuint Vao_sphere_Marble= 0;
GLuint Vbo_sphere_position_Marble = 0;
GLuint Vbo_sphere_element_Marble = 0;
GLuint Vbo_sphere_normal_Marble = 0;

GLuint mvpMatrixUniform_Marble= 0;


// Noise Related Variables 
GLuint textureNoise_Marble;
GLuint textureSamplerUniform_Marble;
GLuint scaleUniform_Marble;


void initialize_Marble()
{
	// function declarations 
	void unintialize_Marble();

	 //------------ Vertex Shader --------------- 
	 const GLchar* vertexShaderSourceCode =
		 "#version 460 core" \
		 "\n " \
		 "in vec4 aPosition; " \
		 "in vec3 aNormal; " \
		 "uniform mat4 uMVPMatrix;" \
		 "uniform float u_Scale;" \
		 "out vec3 MC_Position;" \

		 "void main(void)" \
		 "{" \
			"MC_Position = vec3(aPosition) * u_Scale;" \

			"gl_Position = uMVPMatrix * aPosition ; " \
		 "}";
	 
	 // 2: Create vertex shadder object
	 GLuint vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
	
	 // 3: give vertex shader source code to opengl
	 glShaderSource(vertexShaderObject,1,(const GLchar**)&vertexShaderSourceCode,NULL);

	 // 4: let opengl compile vertex shader
	 glCompileShader(vertexShaderObject);

	 // 5.c
	 GLint status        = 0;
	 GLint infoLogLength = 0;
	 GLchar * szInfoLog   = NULL;

	 // 5.a
	 glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &status);
	 if (status == GL_FALSE)
	 {	// 5.b
		 glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength); 

		 if (infoLogLength > 0)
		 {   // 5.c
			 szInfoLog = (GLchar *)malloc(infoLogLength+1);

			 if (szInfoLog != NULL)
			 {   // 5.d
				 glGetShaderInfoLog(vertexShaderObject, infoLogLength+1, NULL, szInfoLog);  
				 // 5.e
				 fprintf(gpFILE, "vertex shadder Compilation Error Log : %s \n", szInfoLog);
				 // 5.f
				 free(szInfoLog);
				 szInfoLog = NULL;
				 // 5.g
			 }
		 }
		 unintialize_Marble();
	 }

	 //-------------- Fragment Shader ---------------
	 const GLchar* fragmentShaderSourceCode =
		 "#version 460 core" \
		 "\n" \
		 "in vec3 MC_Position;" \
		 "uniform sampler3D u_NoiseSampler;" \
		 "vec3 MarbleColor = vec3(0.9, 0.9, 0.9);" \
		 "vec3 VeinColor = vec3(0.22, 0.44, 0.22);" \
		 "out vec4 FragColor;" \
		 "float lightIntencity = 1.0;" \

		 "void main(void)" \
		 "{" \
			 "vec3 color = vec3(0.0, 0.0, 0.0);" \
			 "vec4 noiseVector = texture(u_NoiseSampler,MC_Position);" \

			 "float intensity = (abs(noiseVector[0] - 0.25) + abs(noiseVector[1] - 0.125) + abs(noiseVector[2] - 0.0625) + abs(noiseVector[3] - 0.03125));" \
			
		     "float sineValue = sin(MC_Position.y*6.0 + intensity*12.0) *0.5 + 0.5;" \
			
			 "color = mix(VeinColor,MarbleColor, sineValue)*lightIntencity;" \
			 "FragColor = vec4(color, 1.0); " \
		 "}";

	 GLuint fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	 glShaderSource(fragmentShaderObject, 1, (const GLchar**)&fragmentShaderSourceCode, NULL);
	 glCompileShader(fragmentShaderObject);

	 status = 0;
	 infoLogLength = 0;
	 szInfoLog = NULL;

	 glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &status);
	 if (status == GL_FALSE)
	 {
		 glGetShaderiv(fragmentShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		 if (infoLogLength > 0)
		 {
			 szInfoLog = (GLchar*)malloc(infoLogLength);
			 if (szInfoLog != NULL)
			 {
				 glGetShaderInfoLog(fragmentShaderObject, infoLogLength, NULL,szInfoLog);
				 fprintf(gpFILE,"Fragment shader Compilation Error Log : %s \n", szInfoLog); 

				 free(szInfoLog);
				 szInfoLog = NULL;
			 }
		 }
		 unintialize_Marble();
	 }

	 //------------ Shader Progarm --------------
	 // 7: create shader program
	 shaderProgramObject_Marble = glCreateProgram();

	 // 8 : Attach both shadders to this program
	 glAttachShader(shaderProgramObject_Marble, vertexShaderObject);
	 glAttachShader(shaderProgramObject_Marble, fragmentShaderObject);

	 // 9 : bind attribute locations with the shader program objects
	 glBindAttribLocation(shaderProgramObject_Marble, AMC_ATTRIBUTE_POSITION,"aPosition");
	 glBindAttribLocation(shaderProgramObject_Marble, AMC_ATTRIBUTE_NORMAL, "aNormal");

	 // 10: link the shader program
	 glLinkProgram(shaderProgramObject_Marble);

	 // 11:

	 status = 0;
	 infoLogLength = 0;
	 szInfoLog = NULL;

	 glGetProgramiv(shaderProgramObject_Marble,GL_LINK_STATUS,&status); 
	 if (status == GL_FALSE)
	 {
		 glGetProgramiv(shaderProgramObject_Marble,GL_INFO_LOG_LENGTH, &infoLogLength);
		 if (infoLogLength > 0)
		 {
			 szInfoLog = (GLchar*)malloc(infoLogLength);
			 if (szInfoLog != NULL)
			 {
				 glGetProgramInfoLog(shaderProgramObject_Marble, infoLogLength, NULL, szInfoLog);
				 fprintf(gpFILE, "Shadder program linking error Log : %s \n", szInfoLog);

				 free(szInfoLog);
				 szInfoLog = NULL; 
			 }
		 }
		 unintialize_Marble();
	 }

	 // Post Linkinng 
	 // get shader uniform locations 
	 //                      kunakadun gheu , kontya uniform location ch gheu  
	 mvpMatrixUniform_Marble      = glGetUniformLocation(shaderProgramObject_Marble, "uMVPMatrix");
	 textureSamplerUniform_Marble = glGetUniformLocation(shaderProgramObject_Marble, "u_NoiseSampler");
	 scaleUniform_Marble          = glGetUniformLocation(shaderProgramObject_Marble, "u_Scale");
	
	 // Declarations of Vertices 
	 getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);
	 NumVertices = getNumberOfSphereVertices();
	 NumElements = getNumberOfSphereElements();

	 //----------
	  // vao
	 glGenVertexArrays(1, &Vao_sphere_Marble);
	 glBindVertexArray(Vao_sphere_Marble);

	 // position vbo
	 glGenBuffers(1, &Vbo_sphere_position_Marble);
	 glBindBuffer(GL_ARRAY_BUFFER, Vbo_sphere_position_Marble);
	 glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_vertices), sphere_vertices, GL_STATIC_DRAW);

	 glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	 glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

	 glBindBuffer(GL_ARRAY_BUFFER, 0);

	 // normal vbo
	 glGenBuffers(1, &Vbo_sphere_normal_Marble);
	 glBindBuffer(GL_ARRAY_BUFFER, Vbo_sphere_normal_Marble);
	 glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_normals), sphere_normals, GL_STATIC_DRAW);

	 glVertexAttribPointer(AMC_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	 glEnableVertexAttribArray(AMC_ATTRIBUTE_NORMAL);

	 glBindBuffer(GL_ARRAY_BUFFER, 0);

	 // element vbo
	 glGenBuffers(1, &Vbo_sphere_element_Marble);
	 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Vbo_sphere_element_Marble);
	 glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphere_elements), sphere_elements, GL_STATIC_DRAW);
	 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	 // unbind vao
	 glBindVertexArray(0);
	
	//--------------------------------------------------------------------------------
	 // For noise 
	 createNoise3D(&textureNoise_Marble);
	 if (textureNoise_Marble == 0)
	 {
		 fprintf(gpFILE, "Failed To create Noise Texture \n");
	 }

	 glEnable(GL_TEXTURE_3D);
	//--------------------------------------------------------------------------------

}


void display_Marble()
{
	// function declarations 
	void update_Marble();
	void unintialize_Marble();

	// Code
	
	// ViewPort 

	
    if (cnt < 1)
	{
		glViewport((GLsizei)Width *1/2 ,0, (GLsizei)Width *1/2, (GLsizei)Height *1/2);
	}
	else if (cnt < 2)
	{
		glViewport(0, 0, (GLsizei)Width *1/2, (GLsizei)Height *1/2);
	}
	else if(cnt < 3)
	{
		glViewport(0, (GLsizei)Height * 1 / 2, (GLsizei)Width * 1 / 2, (GLsizei)Height * 1 / 2);
	}
	else 
	{
		glViewport((GLsizei)Width * 1 / 2, (GLsizei)Height * 1 / 2, (GLsizei)Width * 1 / 2, (GLsizei)Height * 1 / 2);
	}

	// 1: use shader program 
	glUseProgram(shaderProgramObject_Marble);

	// Transformation 
	mat4 modelViewMatrix = vmath::translate(0.0f, 0.0f, -1.5f);
	mat4 rotation = vmath::rotate(MovingAngle_1, 0.0f, 0.0f, 0.0f);

	mat4 modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix * rotation; // order of multiplication is VIMP

	// Push above MVP into vertex shaders mvpuniform 
	glUniformMatrix4fv(mvpMatrixUniform_Marble, 1, GL_FALSE, modelViewProjectionMatrix);

	// Bind Texture 
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, textureNoise_Marble);
	glUniform1i(textureSamplerUniform_Marble, 0);
	glUniform1f(scaleUniform_Marble, gScaleFactor);

	// bind vao
	glBindVertexArray(Vao_sphere_Marble);

	// draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Vbo_sphere_element_Marble);
	glDrawElements(GL_TRIANGLES, NumElements, GL_UNSIGNED_SHORT, 0);

	// unbind vao 
	glBindVertexArray(0);

	// unbind texture 
	glBindTexture(GL_TEXTURE_3D, 0);

	// 5 : unuse the shader program
	glUseProgram(0);

}


void update_Marble()
{
	
	if (isScaled == TRUE)
	{
		gScaleFactor = gScaleFactor - 0.001f;
		if (gScaleFactor < 0.0f)
		{
			isScaled = FALSE;
		}
	}
	else
	{
		gScaleFactor = gScaleFactor + 0.001f;
		if (gScaleFactor > 2.0f)
		{
			isScaled = TRUE;
		}
	}
}

void unintialize_Marble()
{
	// Shader program uninitialize
	if (shaderProgramObject_Marble)
	{
		glUseProgram(shaderProgramObject_Marble);
		
		GLint numShaders = 0;

		glGetProgramiv(shaderProgramObject_Marble, GL_ATTACHED_SHADERS, &numShaders);
		
		if (numShaders > 0)
		{
			GLuint* pShaders = (GLuint*)malloc(numShaders * sizeof(GLuint));

			if (pShaders != NULL)
			{
				glGetAttachedShaders(shaderProgramObject_Marble, numShaders, NULL, pShaders);
				
				for (GLint i = 0; i < numShaders; i++)
				{
					glDetachShader(shaderProgramObject_Marble, pShaders[i]);
					glDeleteShader(pShaders[i]);
					pShaders[i] = 0;
				}
				// 6
				free(pShaders);
				pShaders = NULL;
			}
		}
		// 7
		glUseProgram(0);
		//8
		glDeleteProgram(shaderProgramObject_Marble);
		shaderProgramObject_Marble = 0;
	 }
	
	//-------------------
	// delete Vbo_sphere_normal_Marble_Marble 
	if (Vbo_sphere_normal_Marble)
	{
		glDeleteBuffers(1, &Vbo_sphere_normal_Marble);
		Vbo_sphere_normal_Marble = 0;
	}

	// delete Vbo_sphere_element_Marble_Marble
	if (Vbo_sphere_element_Marble)
	{
		glDeleteBuffers(1, &Vbo_sphere_element_Marble);
		Vbo_sphere_element_Marble = 0;
	}

	// delte Vbo_sphere_position_Marble
	if (Vbo_sphere_position_Marble)
	{
		glDeleteBuffers(1, &Vbo_sphere_position_Marble);
		Vbo_sphere_position_Marble= 0;
	}

	// delete vao
	if (Vao_sphere_Marble)
	{
		glDeleteVertexArrays(1, &Vao_sphere_Marble);
		Vao_sphere_Marble = 0;
	}

	//--------
	// delte noise texture 
	if (textureNoise_Marble)
	{
		glDeleteTextures(1, &textureNoise_Marble);
		textureNoise_Marble = 0;
	}
	//--------


}