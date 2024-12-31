#include <win_zengine.h>
#include <glad.h>
#include <glfw3.h>

internal f32 g_prim_quadVerts[] =
{
	-0.5, -0.5,  0,
	 0.5, -0.5,  0,
	 0.5,  0.5,  0,

	-0.5, -0.5,  0,
	 0.5,  0.5,  0,
	-0.5,  0.5,  0
};

internal f32 g_prim_quadUVs[] =
{
	0, 0,
	1, 0,
	1, 1,

	0, 0,
	1, 1,
	0, 1
};

internal f32 g_prim_quadNormals[] =
{
	0,  0,  -1,
	0,  0,  -1,
	0,  0,  -1,

	0,  0,  -1,
	0,  0,  -1,
	0,  0,  -1
};

//////////////////////////////////////////////////
// draw_single_mesh_vert.glsl
//////////////////////////////////////////////////
static const char* draw_single_mesh_vert_text =
"#version 330\n"
"\n"
"uniform mat4 u_projection;\n"
"uniform mat4 u_modelView;\n"
"// Vertex Attrib 0\n"
"layout (location = 0) in vec3 i_position;\n"
"// // Vertex Attrib 1\n"
"layout (location = 1) in vec2 i_uv;\n"
"// // Vertex Attrib 2\n"
"layout (location = 2) in vec3 i_normal;\n"
"\n"
"out vec2 m_texCoord;\n"
"out vec3 m_normal;\n"
"out vec3 m_fragPos;\n"
"\n"
"void main()\n"
"{\n"
"   vec4 positionV4 = vec4(i_position, 1.0);\n"
"   gl_Position = u_projection * u_modelView * positionV4;\n"
"   m_texCoord = i_uv;\n"
"	m_normal = normalize(mat3(u_modelView) * i_normal);\n"
"	m_fragPos = vec3(u_modelView * positionV4);\n"
"}\n"
"\n"
;

//////////////////////////////////////////////////
// draw_single_mesh_frag.glsl
//////////////////////////////////////////////////
static const char* draw_single_mesh_frag_text =
"#version 330\n"
"\n"
"uniform vec4 u_colour;\n"
"uniform sampler2D u_diffuseTex;\n"
"\n"
"in vec2 m_texCoord;\n"
"in vec3 m_normal;\n"
"in vec3 m_fragPos;\n"
"\n"
"out vec4 outputColor;\n"
"\n"
"void main()\n"
"{\n"
"#if 1 // output texture\n"
"   vec4 diffuse = texture2D(u_diffuseTex, m_texCoord);\n"
"   diffuse *= u_colour;\n"
"   outputColor = diffuse;\n"
"#endif\n"
"\n"
"#if 0\n"
"   outputColor = vec4(1, 1, 1, 1);\n"
"#endif\n"
"#if 0\n"
"   outputColor = vec4(m_texCoord.x, m_texCoord.y, 1, 1);\n"
"#endif\n"
"#if 0\n"
"   outputColor = u_colour;\n"
"#endif\n"
"#if 0 // Output depth\n"
"   float depthValue = gl_FragCoord.z;\n"
"   outputColor = vec4(u_colour.x * depthValue, u_colour.y * depthValue, u_colour.z * depthValue, 1);\n"
"#endif\n"
"#if 0 // output texture with boolean transparency\n"
"   vec4 diffuse = texture2D(u_diffuseTex, m_texCoord);\n"
"   if (diffuse.w < 0.5) { discard; }\n"
"   outputColor = diffuse;\n"
"#endif\n"
"}\n"
;

///////////////////////////////////////////////////
// default handles
///////////////////////////////////////////////////
internal u32 _quadVAOHandle = 0;
internal u32 _quadVBOHandle = 0;

//internal GLuint g_quadMeshId;
internal GLuint g_magentaTexId;

// program
internal GLuint g_samplerDefault2d;
internal GLuint g_vertexDefaultShaderId; 
internal GLuint g_fragmentDefaultShaderId; 
internal GLuint g_defaultProgramId;

///////////////////////////////////////////////////
// ASSET UPLOAD
///////////////////////////////////////////////////

#define MAX_BATCH_SIZE 100

#define ZRGL_DATA_ATTRIB_VERTICES 0
#define ZRGL_DATA_ATTRIB_UVS 1
#define ZRGL_DATA_ATTRIB_NORMALS 2

ze_external void ZE_UploadTexture(u8 *pixels, i32 width, i32 height, u32 *handle, i32 bDataTexture)
{
    if (pixels == NULL)
    {
        printf("ERROR UploadTex - pixels are null\n");
        return;
    }
    if (width <= 0)
    {
        printf("ERROR UploadTex - width <= 0\n");
        return;
    }
    if (height <= 0)
    {
        printf("ERROR UploadTex - height <= 0\n");
        return;
    }
    // Upload to GPU
    if (*handle == 0)
    { 
        glGenTextures(1, handle);
    }

    u16 pixelDataType = GL_UNSIGNED_BYTE;
    if (bDataTexture)
    {
        pixelDataType = GL_FLOAT;
    }

    GLuint texID = *handle;
    glBindTexture(GL_TEXTURE_2D, texID);
    // Assuming images are always RGBA here
    // Make sure conversion of pixel encoding is correct.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, pixelDataType, pixels);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // Clear binding
    glBindTexture(GL_TEXTURE_2D, 0);
    // if (bVerbose == YES)
    // { printf("Uploaded %s to tex handle %d\n", path, handle); }
    printf("Uploaded %d pixels for handle %d\n", width * height, texID);
    //return handle;
}

ze_external void ZE_UploadMesh(i32 numVerts, f32* verts, f32* uvs, f32* normals, u32* vaoHandle, u32* vboHandle)
{
    // if handles already exist we're just updating
    if (*vaoHandle == 0)
    {
        // generate handles
        glGenVertexArrays(1, vaoHandle);
        glGenBuffers(1, vboHandle);
    }

    glBindVertexArray(*vaoHandle);
    glBindBuffer(GL_ARRAY_BUFFER, *vboHandle);

    
    i32 vec3Size = sizeof(f32) * 3;
    i32 vec2Size = sizeof(f32) * 2;
    i32 m4x4Size = sizeof(f32) * 16;
    
    //////////////////////////////////////////
    // Calculate size
    i32 numVertBytes = numVerts * vec3Size;
    i32 numUVBytes = vec2Size * numVerts;
    i32 numNormalBytes = numVertBytes;
    i32 staticBytes = numVertBytes + numUVBytes + numNormalBytes;

    // Add space for up to 100 model view matrices:
    i32 maxInstances = MAX_BATCH_SIZE;
    i32 instanceDataBytes = maxInstances * m4x4Size;

    i32 totalBytes = staticBytes + instanceDataBytes;
    
    /////////////////////////////////////////
    // upload sub-buffers and configure pointers

    i32 vertDataAttrib = ZRGL_DATA_ATTRIB_VERTICES;
    i32 uvDataAttrib = ZRGL_DATA_ATTRIB_UVS;
    i32 normalDataAttrib = ZRGL_DATA_ATTRIB_NORMALS;

    GLenum vboUsage = GL_DYNAMIC_DRAW;
    
    // Allocate buffer for all three arrays verts-normals-uvs
    // send null ptr for data, we're not uploading yet
    glBufferData(GL_ARRAY_BUFFER, totalBytes, NULL, vboUsage);
    // Upload sub-buffers
    zeSize vertOffset = 0;
    zeSize uvOffset = numVertBytes;
    zeSize normalOffset = numVertBytes + numUVBytes;

    // BUFFER: - All Verts | All Normals | All Uvs -
    glBufferSubData(GL_ARRAY_BUFFER, vertOffset, numVertBytes, verts);
    glBufferSubData(GL_ARRAY_BUFFER, normalOffset, numNormalBytes, normals);
    glBufferSubData(GL_ARRAY_BUFFER, uvOffset, numUVBytes, uvs);

    // enable use of static data
    glEnableVertexAttribArray(vertDataAttrib);
    glEnableVertexAttribArray(uvDataAttrib);
    glEnableVertexAttribArray(normalDataAttrib);
    GLenum glDataType = GL_FLOAT;
    // setup how to read the static data sections
    glVertexAttribPointer(vertDataAttrib, 3, glDataType, GL_FALSE, 0, 0);
    glVertexAttribPointer(uvDataAttrib, 2, glDataType, GL_FALSE, 0, (void *)uvOffset);
    glVertexAttribPointer(normalDataAttrib, 3, glDataType, GL_FALSE, 0, (void *)normalOffset);
    
    // clear binding
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    printf("Uploaded %d verts for vao %d, vbo %d\n", numVerts, *vaoHandle, *vboHandle);
}

///////////////////////////////////////////////////
// SHADER PARAMETERS
///////////////////////////////////////////////////

ze_external void ZR_SetProg1i(GLuint prog, char *uniform, GLint value)
{
    i32 loc = glGetUniformLocation(prog, uniform);
    
    if (loc == -1)
    {
        printf("Failed to find loc for i uniform %s on prog %d\n", uniform, prog);
        return;
    }
    glUniform1i(loc, value);
    
}

ze_external void ZR_SetProg1f(GLuint prog, char *uniform, GLfloat value)
{
    i32 loc = glGetUniformLocation(prog, uniform);
    
    if (loc == -1)
    {
        printf("Failed to find loc for f uniform %s on prog %d\n", uniform, prog);
        return;
    }
    glUniform1f(loc, value);
    
}
#if 1
ze_external void ZR_SetProgVec3f(GLuint prog, char *uniform, f32 x, f32 y, f32 z)
{
    i32 loc = glGetUniformLocation(prog, uniform);
    
    if (loc == -1)
    {
        printf("Failed to find loc for v3f uniform %s on prog %d\n", uniform, prog);
        return;
    }
    glUniform3f(loc, x, y, z);
    
}

ze_external void ZR_SetProgVec4f(GLuint prog, char *uniform, f32 x, f32 y, f32 z, f32 w)
{
    i32 loc = glGetUniformLocation(prog, uniform);
    
    if (loc == -1)
    {
        printf("Failed to find loc for v4f uniform %s on prog %d\n", uniform, prog);
        return;
    }
    glUniform4f(loc, x, y, z, w);
    
}
#endif
ze_external void ZR_SetProgM4x4(GLuint prog, char *uniform, f32 *matrix)
{
    i32 loc = glGetUniformLocation(prog, uniform);
    
    if (loc == -1)
    {
        printf("Failed to find loc for m4x4 uniform %s on prog %d\n", uniform, prog);
        return;
    }
    glUniformMatrix4fv(loc, 1, GL_FALSE, matrix);
    
}

ze_external void ZR_PrepareTextureUnit1D(
    GLint programId,
    GLint glTextureUnit,
    i32 textureUnit,
    char *uniformName,
    GLint texture,
    GLuint sampler)
{
    GLint loc = glGetUniformLocation(programId, uniformName);
    if (loc == -1)
    {
        printf("Failed to find loc for tex unit uniform %s on prog %d\n",
               uniformName, programId);
        return;
    }
    glUniform1i(loc, textureUnit);
    
    glActiveTexture(glTextureUnit);
    
    glBindTexture(GL_TEXTURE_1D, texture);
    
    glBindSampler(textureUnit, sampler);
    
}

ze_external void ZR_PrepareTextureUnit2D(
    GLint programId,
    GLint glTextureUnit,
    i32 textureUnit,
    char *uniformName,
    GLint texture,
    GLuint samplerHandle)
{
    GLint loc = glGetUniformLocation(programId, uniformName);
    glUniform1i(loc, textureUnit);
    
    glActiveTexture(glTextureUnit);
    
    glBindTexture(GL_TEXTURE_2D, texture);
    
    GLuint sampler = samplerHandle != 0 ? samplerHandle :  g_samplerDefault2d;

    glBindSampler(textureUnit, sampler);
    
}

ze_external void ZR_PrepareTextureUnitCubeMap(
    GLint programId,
    GLint glTextureUnit,
    i32 textureUnit,
    char *uniformName,
    GLint texture,
    GLint sampler)
{
    GLint loc = glGetUniformLocation(programId, uniformName);
    glUniform1i(loc, textureUnit);
    
    glActiveTexture(glTextureUnit);
    
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    
    glBindSampler(textureUnit, sampler);
    
}
#if 0
ze_external void ZR_PrepareShader(ZRShader* shader)
{
    glUseProgram(shader->handle);
    
    if ((shader->flags & ZR_SHADER_FLAG_NO_DEPTH) != 0)
    {
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glDepthMask(GL_FALSE);
    }
    else
    {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glDepthMask(GL_TRUE);
    }
    if ((shader->flags & ZR_SHADER_FLAG_BLEND) != 0)
    {
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_ONE, GL_ONE);
    }
    else
    {
        glDisable(GL_BLEND);
    }
}
#endif
///////////////////////////////////////////////////////////
// Build shaders
///////////////////////////////////////////////////////////

ze_internal zErrorCode ZRGL_LinkProgram(GLuint programId)
{
    glLinkProgram(programId);
    
    i32 linkStatus;
    glGetProgramiv(programId, GL_LINK_STATUS, &linkStatus);
    
    if (linkStatus == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &maxLength);
        
        printf("!LINK FAILED for prog %d. logged %d chars\n", programId, maxLength);
        GLchar *buf = (GLchar *)Platform_Alloc(sizeof(GLchar) * maxLength + 1);
        glGetProgramInfoLog(programId, maxLength, NULL, buf);
        
        printf("LOG: (%d chars):\n%s\n", maxLength, buf);
        Platform_Free((void *)buf);

        //ZE_ASSERT(0, "Shader link failed");
        return ZERROR_CODE_UNSPECIFIED;
    }
    return ZERROR_CODE_NONE;
}

ze_external void ZRGL_PrintShaderCompileLog(GLuint shaderId)
{
    GLint maxLength = 0;
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);
    GLchar *buf = (GLchar *)Platform_Alloc(sizeof(GLchar) * maxLength + 1);
    glGetShaderInfoLog(shaderId, maxLength, NULL, buf);
    
    printf("LOG: (%d chars):\n%s\n", maxLength, buf);
    Platform_Free((void *)buf);
}

ze_external zErrorCode ZRGL_CreateProgram(
    const char *vertexShader,
    const char *fragmentShader,
    u32* vertexProgId,
    u32* fragProgId,
    u32* progId,
    char* label)
{
    //printf("---------------------------------\n");
    //printf("Building shader program \"%s\"\n", shaderName);
    *vertexProgId = 0;
    *fragProgId = 0;
    *progId = 0;

    // Vertex shader
    GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderId, 1, &vertexShader, NULL);
    glCompileShader(vertexShaderId);

    int vertCompilationStatus;
    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &vertCompilationStatus);
    //printf("Vert shader compile status: %d\n", vertCompilationStatus);
    if (vertCompilationStatus == GL_FALSE)
    {
        printf("ERROR: Vert shader for \"%s\" compile error\n", label);
        ZRGL_PrintShaderCompileLog(vertexShaderId);
        return ZERROR_CODE_UNSPECIFIED;
    }

    // Fragment shader
    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderId, 1, &fragmentShader, NULL);
    glCompileShader(fragmentShaderId);

    int fragCompilationStatus;
    glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &fragCompilationStatus);
    //printf("frag shader compile status: %d\n", fragCompilationStatus);
    if (fragCompilationStatus == GL_FALSE)
    {
        printf("ERROR: Frag shader for \"%s\" compile error\n", label);
        ZRGL_PrintShaderCompileLog(fragmentShaderId);
        return ZERROR_CODE_UNSPECIFIED;
    }

    // Create program
    GLuint programId = glCreateProgram();
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);

    // Link
    zErrorCode err = ZRGL_LinkProgram(programId);
    if (err != ZERROR_CODE_NONE)
    {
        printf("FAILED TO LINK SHADER \"%s\", Id %d\n", label, programId);
        printf("---------------------------------\n");
        return err;
    }

    *vertexProgId = vertexShaderId;
    *fragProgId = fragmentShaderId;
    *progId = programId;
    
    printf("Built shader \"%s\", Id %d\n", label, programId);
    return ZERROR_CODE_NONE;
}

ze_external void ZRGL_DrawTest()
{
    printf("ZRGL - draw test...\n");
    f32 prj[16];
    f32 view[16];
    f32 model[16];
    M4x4_ToIdentity(prj);
    M4x4_ToIdentity(view);
    M4x4_ToIdentity(model);
    view[M4x4_W2] = -1;

    f32 vm[16];
    M4x4_Multiply(model, view, vm);

	M4x4_SetupOrthoProjection(prj, 8, Window_GetAspectRatio());

    glUseProgram(g_defaultProgramId);
    ZR_SetProgM4x4(g_defaultProgramId, "u_projection", prj);
    ZR_SetProgM4x4(g_defaultProgramId, "u_modelView", vm);
    ZR_SetProgVec4f(g_defaultProgramId, "u_colour", 1, 1, 1, 1);
    
    ZR_PrepareTextureUnit2D(g_defaultProgramId, GL_TEXTURE0, 0, "u_diffuseTex", g_magentaTexId, 0);
    glBindVertexArray(_quadVAOHandle);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    Platform_SwapBuffers();
    printf("...ZRGL - draw test\n");
}

ze_internal u32 ZRGL_CreateTexture(i32 imgSize, u8 r, u8 g, u8 b, u8 a)
{
    if (imgSize < 2)
	{
		imgSize = 2;
	}
    i32 numPixels = imgSize * imgSize;
	i32 numBytes = numPixels * 4;
    u8* pixels = (u8*)Platform_Alloc(numBytes);
    for (int i = 0; i < numBytes; i += 4)
    {
        pixels[i + 0] = r;
        pixels[i + 1] = g;
        pixels[i + 2] = b;
        pixels[i + 3] = a;
    }
	GLuint handle;
    ZE_UploadTexture(pixels, imgSize, imgSize, &handle);
	return handle;
}

ze_external zErrorCode ZRGL_Init()
{
    ZE_UploadMesh(6, g_prim_quadVerts, g_prim_quadUVs, g_prim_quadNormals, &_quadVAOHandle, &_quadVBOHandle);

	g_magentaTexId = ZRGL_CreateTexture(32, 255, 0, 255, 255);
    /*i32 imgSize = 32;
    i32 numPixels = imgSize * imgSize;
	i32 numBytes = numPixels * 4;
    u8* pixels = (u8*)Platform_Alloc(numBytes);
    for (int i = 0; i < numBytes; i += 4)
    {
        pixels[i + 0] = 255;
        pixels[i + 1] = 0;
        pixels[i + 2] = 255;
        pixels[i + 3] = 255;
    }
    ZE_UploadTexture(pixels, imgSize, imgSize, &g_magentaTexId);
	*/
    glGenSamplers(1, &g_samplerDefault2d);
    ZRGL_CreateProgram(
        draw_single_mesh_vert_text,
        draw_single_mesh_frag_text,
        &g_vertexDefaultShaderId,
        &g_fragmentDefaultShaderId,
        &g_defaultProgramId,
        "default");
    zErrorCode err = ZRGL_LinkProgram(g_defaultProgramId);
    if (err != ZERROR_CODE_NONE)
    {
        printf("Error code %d linking default program\n", g_defaultProgramId);
        return ZERROR_CODE_UNSPECIFIED;
    }
    return ZERROR_CODE_NONE;
}

