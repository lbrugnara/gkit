#ifndef GKIT_MODEL_FONT_H
#define GKIT_MODEL_FONT_H

typedef struct GKitCharacter* GKitCharacter;
typedef struct GKitFont* GKitFont;

/*
 * Function: gkit_font_load
 *  This function loads a font from the provided font file and
 *  returns an object containing the information of the file to be used
 *  to render text into a window
 *
 * Parameters:
 *  <const char> *file - File to a font file
 *
 * Returns:
 *  <GKitFont> - Object containing information of the loaded file
 *  
 * Notes:
 *  The object returned by this function must be freed using the
 *  <gkit_font_free> function
 */
GKitFont gkit_font_load(const char *file);

/*
 * Function: gkit_font_free
 *  Releases the memory used by a <GKitFont> object created with the
 *  <gkit_font_load> function
 *
 * Parameters:
 *  <GKitFont> font - Font object to be freed
 *
 * Returns:
 *  void - This function does not return a value
 *
 */
void gkit_font_free(GKitFont font);

#endif /* GKIT_MODEL_FONT_H */
