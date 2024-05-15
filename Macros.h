#pragma once


#define SET_PRINT_TO_DIFFERENT_COLOR(s){printf("%s",s);}	// 1 for bold, 92 for light green color
#define SET_PRINT_UNDERLINE_TEXT(){printf("\033[4m");}//for underline
#define SET_PRINT_TO_DEFAULT(){printf("\033[0m");} // Reset text color to default (usually white)


#define FREE_RETURN_ZERO(ptr){free(ptr); return 0;}
#define FREE_RETURN_MINUS_ONE(ptr){free(ptr); return -1;}
#define FREE_RETURN_ONE(ptr){free(ptr); return 1;}
#define FREE_RETURN(ptr){free(ptr); return;}
#define FREE_RETURN_NULL(ptr) { free(ptr); return NULL; }

#define CHECK_NULL_FREE_RETURN_ZERO(ptr){if(!ptr){printf("--ERROR ALLOCATING SPACE--\n"); FREE_RETURN_ZERO(ptr);}}


#define PRINT_MSG_RETURN_ZERO(msg){printf("%s\n",msg); return 0;}
#define PRINT_MSG_RETURN(msg){printf("%s\n", msg);}
#define PRINT_MSG_RETURN_NULL(msg){printf("%s\n",msg); return NULL;}
#define PRINT_MSG_RETURN_MINUS_ONE(msg) { printf("%s\n", msg); return -1; }


#define CLOSE_FILE_RETURN_(fp) {fclose(fp);	return;}
#define CLOSE_FILE_RETURN_ZERO(fp) {fclose(fp);	return 0;}

#define PRINT_MSG_CLOSE_FILE_RETURN(msg,fp){printf("%s\n", msg);fclose(fp);	return;}
#define PRINT_MSG_CLOSE_FILE_RETURN_ZERO(msg,fp){printf("%s\n", msg);fclose(fp);return 0;}

#define PRINT_BOOLEAN(num)if(num==1){ printf("YES\n");} else{ printf("NO\n");}