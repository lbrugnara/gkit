#ifndef GKIT_FONT_H
#define GKIT_FONT_H

#include <fllib.h>

typedef struct GKitFont* GKitFont;
typedef struct GKitCharacter* GKitCharacter;

GKitFont gkit_font_load(const char *file);
void gkit_font_free(GKitFont font);

#endif /* GKIT_FONT_H */
