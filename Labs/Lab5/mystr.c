void my_strcpy(char *dest, char *src)
{
	while(*dest++ = *src++);
}

void my_strcat(char *dest, char *src)
{
	while (*dest != '\0')
		dest++;
	
	while((*dest++ = *src++) != '\0')
		;
}
