
#include <zrgl.h>

void GLAPIENTRY MessageCallback( GLenum source,
				 GLenum type,
				 GLuint id,
				 GLenum severity,
				 GLsizei length,
				 const GLchar* message,
				 const void* userParam )
{
  fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		   ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
			type, severity, message );
}

ze_external void ZR_GetAsciiUVs(char ascii, f32* minX, f32* minY, f32* maxX, f32* maxY)
{
	const i32 asciiSheetWidth = 16;
	const f32 quadWidth = 1.0f / (f32)asciiSheetWidth;
	u8 c = (u8)ascii;
	i32 x = c % asciiSheetWidth;
	i32 y = c / asciiSheetWidth;
	*minX = x * quadWidth;
	*minY = 1.f - (y * quadWidth);
	*maxX = *minX + quadWidth;
	*maxY = (*minY + quadWidth);
}
