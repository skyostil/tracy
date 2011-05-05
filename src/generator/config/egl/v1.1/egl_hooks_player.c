/*
 *  Hooks for the EGL player
 */

void eglCopyBuffers.@postcall()
{
    trPlayerDisplayPixmap(trGetPlayer(), target);
}

