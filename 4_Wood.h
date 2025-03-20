
#pragma once

GLuint shaderProgramObject_Wood = 0;

// for sphere
GLuint Vao_sphere_Wood= 0;
GLuint Vbo_sphere_position_Wood = 0;
GLuint Vbo_sphere_element_Wood = 0;
GLuint Vbo_sphere_normal_Wood = 0;

GLuint mvpMatrixUniform_Wood= 0;


// Noise Related Variables 
GLuint textureNoise_Wood;
GLuint textureSamplerUniform_Wood;
GLuint scaleUniform_Wood;


void initialize_Wood()
{
	// function declarations 
	void unintialize_Wood();

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
		 unintialize_Wood();
	 }

	 //-------------- Fragment Shader ---------------
	 const GLchar* fragmentShaderSourceCode =
		 "#version 460 core \n" \
		 "\n" \
		 "in vec3 MC_Position; \n" \
		 "uniform sampler3D u_NoiseSampler; \n" \
		 "out vec4 FragColor; \n" \

		 "void main(void) \n" \
		 "{ \n" \

			 "float RingFreq    = 4.0; \n" \
			 "float LightGrains = 1.0; \n" \
			 "float DarkGrains  = 0.0; \n" \
			 "float GrainThreshold = 0.5; \n" \
			 "float Noisiness  = 3.0;  \n" \
			 "float GrainScale = 27.0; \n" \

			 "vec3 LightWood = vec3(0.6, 0.3, 0.1); \n" \
			 "vec3 DarkWood = vec3(0.4, 0.2, 0.07); \n" \

			 "float lightIntencity = 1.0; \n" \

			 "vec3 noiseVector = vec3(texture(u_NoiseSampler, MC_Position) * Noisiness); \n" \
			 "vec3 location = MC_Position + noiseVector; \n" \
			 "float dist = sqrt(location.x * location.x + location.z * location.z); \n" \
			 "dist *= RingFreq; \n" \
			 "float r = fract(dist + noiseVector[0] + noiseVector[1] + noiseVector[2]) * 2.0; \n" \
			 
			 "if (r > 1.0)     \n" \
				 "r = 2.0 - r; \n" \
			 
			 "vec3 color = mix(LightWood, DarkWood, r); \n" \
			 "r = fract((MC_Position.x + MC_Position.z) * GrainScale + 0.5); \n" \
			 "noiseVector[2] *= r; \n" \
			
			 "if (r < GrainThreshold) \n" \
				 "color += LightWood * LightGrains * noiseVector[2]; \n" \
			 "else \n" \
				 "color -= LightWood * DarkGrains * noiseVector[2];  \n" \

			 "color *= lightIntencity;      \n" \
			 "FragColor = vec4(color, 1.0); \n" \
		 "} \n";

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
		 unintialize_Wood();
	 }

	 //------------ Shader Progarm --------------
	 // 7: create shader program
	 shaderProgramObject_Wood = glCreateProgram();

	 // 8 : Attach both shadders to this program
	 glAttachShader(shaderProgramObject_Wood, vertexShaderObject);
	 glAttachShader(shaderProgramObject_Wood, fragmentShaderObject);

	 // 9 : bind attribute locations with the shader program objects
	 glBindAttribLocation(shaderProgramObject_Wood, AMC_ATTRIBUTE_POSITION,"aPosition");
	 glBindAttribLocation(shaderProgramObject_Wood, AMC_ATTRIBUTE_NORMAL, "aNormal");

	 // 10: link the shader program
	 glLinkProgram(shaderProgramObject_Wood);

	 // 11:

	 status = 0;
	 infoLogLength = 0;
	 szInfoLog = NULL;

	 glGetProgramiv(shaderProgramObject_Wood,GL_LINK_STATUS,&status); 
	 if (status == GL_FALSE)
	 {
		 glGetProgramiv(shaderProgramObject_Wood,GL_INFO_LOG_LENGTH, &infoLogLength);
		 if (infoLogLength > 0)
		 {
			 szInfoLog = (GLchar*)malloc(infoLogLength);
			 if (szInfoLog != NULL)
			 {
				 glGetProgramInfoLog(shaderProgramObject_Wood, infoLogLength, NULL, szInfoLog);
				 fprintf(gpFILE, "Shadder program linking error Log : %s \n", szInfoLog);

				 free(szInfoLog);
				 szInfoLog = NULL; 
			 }
		 }
		 unintialize_Wood();
	 }

	 // Post Linkinng 
	 // get shader uniform locations 
	 //                      kunakadun gheu , kontya uniform location ch gheu  
	 mvpMatrixUniform_Wood           = glGetUniformLocation(shaderProgramObject_Wood, "uMVPMatrix");
	 textureSamplerUniform_Wood = glGetUniformLocation(shaderProgramObject_Wood, "u_NoiseSampler");
	 scaleUniform_Wood           = glGetUniformLocation(shaderProgramObject_Wood, "u_Scale");
	
	 // Declarations of Vertices 
	 getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);
	 NumVertices = getNumberOfSphereVertices();
	 NumElements = getNumberOfSphereElements();

	 //----------
	  // vao
	 glGenVertexArrays(1, &Vao_sphere_Wood);
	 glBindVertexArray(Vao_sphere_Wood);

	 // position vbo
	 glGenBuffers(1, &Vbo_sphere_position_Wood);
	 glBindBuffer(GL_ARRAY_BUFFER, Vbo_sphere_position_Wood);
	 glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_vertices), sphere_vertices, GL_STATIC_DRAW);

	 glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	 glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

	 glBindBuffer(GL_ARRAY_BUFFER, 0);

	 // normal vbo
	 glGenBuffers(1, &Vbo_sphere_normal_Wood);
	 glBindBuffer(GL_ARRAY_BUFFER, Vbo_sphere_normal_Wood);
	 glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_normals), sphere_normals, GL_STATIC_DRAW);

	 glVertexAttribPointer(AMC_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	 glEnableVertexAttribArray(AMC_ATTRIBUTE_NORMAL);

	 glBindBuffer(GL_ARRAY_BUFFER, 0);

	 // element vbo
	 glGenBuffers(1, &Vbo_sphere_element_Wood);
	 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Vbo_sphere_element_Wood);
	 glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphere_elements), sphere_elements, GL_STATIC_DRAW);
	 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	 // unbind vao
	 glBindVertexArray(0);
	
	//--------------------------------------------------------------------------------
	 // For noise 
	 createNoise3D(&textureNoise_Wood );
	 if (textureNoise_Wood  == 0)
	 {
		 fprintf(gpFILE, "Failed To create Noise Texture \n");
	 }

	 glEnable(GL_TEXTURE_3D);
	//--------------------------------------------------------------------------------

}


void display_Wood()
{
	// function declarations 
	void update_Wood();
	void unintialize_Wood();

	// Code
	
	// ViewPort 
	   
	if (cnt < 1)
	{
		glViewport(0, 0, (GLsizei)Width *1/2, (GLsizei)Height *1/2);
	}
	else if(cnt < 2)
	{
		glViewport(0, (GLsizei)Height * 1 / 2, (GLsizei)Width * 1 / 2, (GLsizei)Height * 1 / 2);
	}
	else if(cnt < 3)
	{
		glViewport((GLsizei)Width * 1 / 2, (GLsizei)Height * 1 / 2, (GLsizei)Width * 1 / 2, (GLsizei)Height * 1 / 2);
	}
	else 
	{
		glViewport((GLsizei)Width * 1 / 2, 0, (GLsizei)Width * 1 / 2, (GLsizei)Height * 1 / 2);
	}

	// 1: use shader program 
	glUseProgram(shaderProgramObject_Wood);

	// Transformation 
	mat4 modelViewMatrix = vmath::translate(0.0f, 0.0f, -1.5f);
	mat4 rotation = vmath::rotate(MovingAngle_1, 0.0f, 0.0f, 0.0f);

	mat4 modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix * rotation; // order of multiplication is VIMP

	// Push above MVP into vertex shaders mvpuniform 
	glUniformMatrix4fv(mvpMatrixUniform_Wood, 1, GL_FALSE, modelViewProjectionMatrix);

	// Bind Texture 
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, textureNoise_Wood );
	glUniform1i(textureSamplerUniform_Wood , 0);
	glUniform1f(scaleUniform_Wood , gScaleFactor);

	// bind vao
	glBindVertexArray(Vao_sphere_Wood);

	// draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Vbo_sphere_element_Wood);
	glDrawElements(GL_TRIANGLES, NumElements, GL_UNSIGNED_SHORT, 0);

	// unbind vao 
	glBindVertexArray(0);

	// unbind texture 
	glBindTexture(GL_TEXTURE_3D, 0);

	// 5 : unuse the shader program
	glUseProgram(0);
}


void update_Wood()
{
	
	if (isScaled == TRUE)
	{
		gScaleFactor = gScaleFactor - 0.001f;
		if (gScaleFactor < 1.0f)
		{
			isScaled = FALSE;
		}
	}
	else
	{
		gScaleFactor = gScaleFactor + 0.001f;
		if (gScaleFactor > 2.5f)
		{
			isScaled = TRUE;
		}
	}
}

void unintialize_Wood()
{
	// Shader program unintialize_Wood
	if (shaderProgramObject_Wood)
	{
		glUseProgram(shaderProgramObject_Wood);
		
		GLint numShaders = 0;

		glGetProgramiv(shaderProgramObject_Wood, GL_ATTACHED_SHADERS, &numShaders);
		
		if (numShaders > 0)
		{
			GLuint* pShaders = (GLuint*)malloc(numShaders * sizeof(GLuint));

			if (pShaders != NULL)
			{
				glGetAttachedShaders(shaderProgramObject_Wood, numShaders, NULL, pShaders);
				
				for (GLint i = 0; i < numShaders; i++)
				{
					glDetachShader(shaderProgramObject_Wood, pShaders[i]);
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
		glDeleteProgram(shaderProgramObject_Wood);
		shaderProgramObject_Wood = 0;
	 }
	
	//-------------------
	// delete Vbo_sphere_normal_Wood 
	if (Vbo_sphere_normal_Wood)
	{
		glDeleteBuffers(1, &Vbo_sphere_normal_Wood);
		Vbo_sphere_normal_Wood = 0;
	}

	// delete Vbo_sphere_element_Wood
	if (Vbo_sphere_element_Wood)
	{
		glDeleteBuffers(1, &Vbo_sphere_element_Wood);
		Vbo_sphere_element_Wood = 0;
	}

	// delte Vbo_sphere_position_Wood
	if (Vbo_sphere_position_Wood)
	{
		glDeleteBuffers(1, &Vbo_sphere_position_Wood);
		Vbo_sphere_position_Wood= 0;
	}

	// delete vao
	if (Vao_sphere_Wood)
	{
		glDeleteVertexArrays(1, &Vao_sphere_Wood);
		Vao_sphere_Wood = 0;
	}

	//--------
	// delte noise texture 
	if (textureNoise_Wood)
	{
		glDeleteTextures(1, &textureNoise_Wood);
		textureNoise_Wood = 0;
	}
	//--------


}