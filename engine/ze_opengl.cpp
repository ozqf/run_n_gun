#include <win_zengine.h>
#include <glad.h>
#include <glfw3.h>

ze_external void ZRGL_UploadTexture(u8 *pixels, i32 width, i32 height, u32 *handle)
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

    GLuint texID = *handle;
    glBindTexture(GL_TEXTURE_2D, texID);
    // Assuming images are always RGBA here
    // Make sure conversion of pixel encoding is correct.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // Clear binding
    glBindTexture(GL_TEXTURE_2D, 0);
    // if (bVerbose == YES)
    // { printf("Uploaded %s to tex handle %d\n", path, handle); }

    //return handle;
}
