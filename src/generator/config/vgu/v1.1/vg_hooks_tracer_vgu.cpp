/*
 *  VGU binding api functions
 */

/* State management helpers */
#define GET(PATH)        trGetStateValue(event, PATH)
#define GET_INT(PATH)    ((int)GET(PATH))
 
