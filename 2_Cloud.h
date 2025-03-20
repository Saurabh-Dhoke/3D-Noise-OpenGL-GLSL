
#pragma once



GLuint shaderProgramObject_Cloud = 0;

// for sphere
GLuint Vao_sphere_Cloud = 0;
GLuint Vbo_sphere_position_CLoud = 0;
GLuint Vbo_sphere_element_Cloud = 0;
GLuint Vbo_sphere_normal_Cloud = 0;

GLuint mvpMatrixUniform_Cloud = 0;


// Noise Related Variables 
GLuint textureNoiseCloud;
GLuint textureSamplerUniformCloud;
GLuint scaleUniformCloud;


void initialize_Cloud()
{
	// function declarations 
	void unintialize_Cloud();

	
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
		 unintialize_Cloud();
	 }

	 //-------------- Fragment Shader ---------------
	 const GLchar* fragmentShaderSourceCode =
		 "#version 460 core" \
		 "\n" \
		 "in vec3 MC_Position;" \
		 "uniform sampler3D u_NoiseSampler;" \
		 "vec3 skyColor = vec3(0.1, 0.1, 0.4);" \
		 "vec3 cloudColor = vec3(1.0, 1.0, 1.0);" \
		 "out vec4 FragColor;" \
		 "float lightIntensity = 1.5;" \

		 "void main(void)" \
		 "{" \
			 "vec3 color = vec3(0.0, 0.0, 0.0);" \
			 "vec4 noiseVector = texture(u_NoiseSampler,MC_Position);" \
			 "float intensity = (noiseVector[0] + noiseVector[1] + noiseVector[2] + noiseVector[3] * 0.33);" \
			 "color = mix(skyColor, cloudColor, intensity)*lightIntensity;" \
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
		 unintialize_Cloud();
	 }

	 //------------ Shader Progarm --------------
	 // 7: create shader program
	 shaderProgramObject_Cloud = glCreateProgram();

	 // 8 : Attach both shadders to this program
	 glAttachShader(shaderProgramObject_Cloud, vertexShaderObject);
	 glAttachShader(shaderProgramObject_Cloud, fragmentShaderObject);

	 // 9 : bind attribute locations with the shader program objects
	 glBindAttribLocation(shaderProgramObject_Cloud, AMC_ATTRIBUTE_POSITION,"aPosition");
	 glBindAttribLocation(shaderProgramObject_Cloud, AMC_ATTRIBUTE_NORMAL, "aNormal");

	 // 10: link the shader program
	 glLinkProgram(shaderProgramObject_Cloud);

	 // 11:

	 status = 0;
	 infoLogLength = 0;
	 szInfoLog = NULL;

	 glGetProgramiv(shaderProgramObject_Cloud,GL_LINK_STATUS,&status); 
	 if (status == GL_FALSE)
	 {
		 glGetProgramiv(shaderProgramObject_Cloud,GL_INFO_LOG_LENGTH, &infoLogLength);
		 if (infoLogLength > 0)
		 {
			 szInfoLog = (GLchar*)malloc(infoLogLength);
			 if (szInfoLog != NULL)
			 {
				 glGetProgramInfoLog(shaderProgramObject_Cloud, infoLogLength, NULL, szInfoLog);
				 fprintf(gpFILE, "Shadder program linking error Log : %s \n", szInfoLog);

				 free(szInfoLog);
				 szInfoLog = NULL; 
			 }
		 }
		 unintialize_Cloud();
	 }

	 // Post Linkinng 
	 // get shader uniform locations 
	 //                      kunakadun gheu , kontya uniform location ch gheu  
	 mvpMatrixUniform_Cloud           = glGetUniformLocation(shaderProgramObject_Cloud, "uMVPMatrix");
	 textureSamplerUniformCloud = glGetUniformLocation(shaderProgramObject_Cloud, "u_NoiseSampler");
	 scaleUniformCloud          = glGetUniformLocation(shaderProgramObject_Cloud, "u_Scale");
	
	 // Declarations of Vertices 
	 getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);
	 NumVertices = getNumberOfSphereVertices();
	 NumElements = getNumberOfSphereElements();

	 //----------
	  // vao
	 glGenVertexArrays(1, &Vao_sphere_Cloud);
	 glBindVertexArray(Vao_sphere_Cloud);

	 // position vbo
	 glGenBuffers(1, &Vbo_sphere_position_CLoud);
	 glBindBuffer(GL_ARRAY_BUFFER, Vbo_sphere_position_CLoud);
	 glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_vertices), sphere_vertices, GL_STATIC_DRAW);

	 glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	 glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

	 glBindBuffer(GL_ARRAY_BUFFER, 0);

	 // normal vbo
	 glGenBuffers(1, &Vbo_sphere_normal_Cloud);
	 glBindBuffer(GL_ARRAY_BUFFER, Vbo_sphere_normal_Cloud);
	 glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_normals), sphere_normals, GL_STATIC_DRAW);

	 glVertexAttribPointer(AMC_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	 glEnableVertexAttribArray(AMC_ATTRIBUTE_NORMAL);

	 glBindBuffer(GL_ARRAY_BUFFER, 0);

	 // element vbo
	 glGenBuffers(1, &Vbo_sphere_element_Cloud);
	 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Vbo_sphere_element_Cloud);
	 glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphere_elements), sphere_elements, GL_STATIC_DRAW);
	 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	 // unbind vao
	 glBindVertexArray(0);
	
	//--------------------------------------------------------------------------------
	 // For noise 
	 createNoise3D(&textureNoiseCloud);
	 if (textureNoiseCloud == 0)
	 {
		 fprintf(gpFILE, "Failed To create Noise Texture \n");
	 }

	 glEnable(GL_TEXTURE_3D);
	//--------------------------------------------------------------------------------

}


void display_Cloud()
{
	// function declarations 
	void update_Cloud();
	void unintialize_Cloud();

	// Code
	
	// ViewPort 

	
	if(cnt < 1)
	{
		glViewport((GLsizei)Width *1/2 , (GLsizei)Height *1/2, (GLsizei)Width *1/2, (GLsizei)Height *1/2);
	}
	else if (cnt < 2)
	{
		glViewport((GLsizei)Width *1/2 ,0, (GLsizei)Width *1/2, (GLsizei)Height *1/2);
	}
	else if (cnt < 3)
	{
		glViewport(0, 0, (GLsizei)Width *1/2, (GLsizei)Height *1/2);
	}
	else
	{
		glViewport(0, (GLsizei)Height * 1 / 2, (GLsizei)Width * 1 / 2, (GLsizei)Height * 1 / 2);
	}

	// 1: use shader program 
	glUseProgram(shaderProgramObject_Cloud);

	// Transformation 
	mat4 modelViewMatrix = vmath::translate(0.0f, 0.0f, -1.5f);
	mat4 rotation = vmath::rotate(MovingAngle_1, 0.0f, 0.0f, 0.0f);

	mat4 modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix * rotation; // order of multiplication is VIMP

	// Push above MVP into vertex shaders mvpuniform 
	glUniformMatrix4fv(mvpMatrixUniform_Cloud, 1, GL_FALSE, modelViewProjectionMatrix);

	// Bind Texture 
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, textureNoiseCloud);
	glUniform1i(textureSamplerUniformCloud, 0);
	glUniform1f(scaleUniformCloud, gScaleFactor);

	// bind vao
	glBindVertexArray(Vao_sphere_Cloud);

	// draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Vbo_sphere_element_Cloud);
	glDrawElements(GL_TRIANGLES, NumElements, GL_UNSIGNED_SHORT, 0);

	// unbind vao 
	glBindVertexArray(0);

	// unbind texture 
	glBindTexture(GL_TEXTURE_3D, 0);

	// 5 : unuse the shader program
	glUseProgram(0);

}


void update_Cloud()
{
	
	if (isScaled == TRUE)
	{
		gScaleFactor = gScaleFactor - 0.002f;
		if (gScaleFactor < 1.0f)
		{
			isScaled = FALSE;
		}
	}
	else
	{
		gScaleFactor = gScaleFactor + 0.002f;
		if (gScaleFactor > 2.5f)
		{
			isScaled = TRUE;

		}
	}
}

void unintialize_Cloud()
{
	// Shader program uninitialize
	if (shaderProgramObject_Cloud)
	{
		glUseProgram(shaderProgramObject_Cloud);
		
		GLint numShaders = 0;

		glGetProgramiv(shaderProgramObject_Cloud, GL_ATTACHED_SHADERS, &numShaders);
		
		if (numShaders > 0)
		{
			GLuint* pShaders = (GLuint*)malloc(numShaders * sizeof(GLuint));

			if (pShaders != NULL)
			{
				glGetAttachedShaders(shaderProgramObject_Cloud, numShaders, NULL, pShaders);
				
				for (GLint i = 0; i < numShaders; i++)
				{
					glDetachShader(shaderProgramObject_Cloud, pShaders[i]);
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
		glDeleteProgram(shaderProgramObject_Cloud);
		shaderProgramObject_Cloud = 0;
	 }
	
	//-------------------
	// delete Vbo_sphere_normal_Cloud 
	if (Vbo_sphere_normal_Cloud)
	{
		glDeleteBuffers(1, &Vbo_sphere_normal_Cloud);
		Vbo_sphere_normal_Cloud = 0;
	}

	// delete Vbo_sphere_element_Cloud
	if (Vbo_sphere_element_Cloud)
	{
		glDeleteBuffers(1, &Vbo_sphere_element_Cloud);
		Vbo_sphere_element_Cloud = 0;
	}

	// delte Vbo_sphere_position_CLoud
	if (Vbo_sphere_position_CLoud)
	{
		glDeleteBuffers(1, &Vbo_sphere_position_CLoud);
		Vbo_sphere_position_CLoud = 0;
	}

	// delete vao
	if (Vao_sphere_Cloud)
	{
		glDeleteVertexArrays(1, &Vao_sphere_Cloud);
		Vao_sphere_Cloud = 0;
	}
	//--------
	// delte noise texture 
	if (textureNoiseCloud)
	{
		glDeleteTextures(1, &textureNoiseCloud);
		textureNoiseCloud = 0;
	}
	//--------


}