#version 460 core

in vec2 TexCoord;
flat in int TextureIndex;

uniform sampler2D TexSlot[32];

out vec4 FragColor;

void main()
{
    if(TextureIndex == 0) 
    { 
        FragColor = texture(TexSlot[0], TexCoord);
    } 
    else if(TextureIndex  == 1) 
    { 
        FragColor = texture(TexSlot[1], TexCoord);
    } 
    else if(TextureIndex  == 2) 
    { 
        FragColor = texture(TexSlot[2], TexCoord);
    } 
    else if(TextureIndex  == 3) 
    { 
        FragColor = texture(TexSlot[3], TexCoord);
    } 
    else if(TextureIndex  == 4) 
    { 
        FragColor = texture(TexSlot[4], TexCoord);
    }
    else if(TextureIndex  == 5) 
    { 
        FragColor = texture(TexSlot[5], TexCoord);
    }
    else if(TextureIndex  == 6) 
    { 
        FragColor = texture(TexSlot[6], TexCoord);
    }
    else if(TextureIndex  == 7) 
    { 
        FragColor = texture(TexSlot[7], TexCoord);
    }
    else if(TextureIndex  == 8) 
    { 
        FragColor = texture(TexSlot[8], TexCoord);
    }
    else if(TextureIndex  == 9) 
    { 
        FragColor = texture(TexSlot[9], TexCoord);
    }
    else if(TextureIndex  == 10) 
    { 
        FragColor = texture(TexSlot[10], TexCoord);
    }
    else if(TextureIndex  == 11) 
    { 
        FragColor = texture(TexSlot[11], TexCoord);
    }
     else if(TextureIndex  == 12) 
    { 
        FragColor = texture(TexSlot[12], TexCoord);
    }
     else if(TextureIndex  == 13) 
    { 
        FragColor = texture(TexSlot[13], TexCoord);
    }
}
