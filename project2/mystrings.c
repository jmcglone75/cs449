#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
        FILE *f = fopen(argv[1], "rb");
        int offset = 0;
        int count = 0;

        fseek(f, 0, SEEK_END);
        int length = ftell(f);
        fseek(f, 0, SEEK_SET);

        while (offset < length)
	 {
                char curr;
                fread(&curr, 1, 1, f);
                offset = ftell(f);

                int begin;
                int curr_value = (int) curr;

                while ((curr_value >= 32 && curr_value <= 126) || curr_value == 9)
		{
                        begin = offset - 1;
                        count++;
                        fread(&curr, 1, 1, f);
                        curr_value = (int) curr;
		}
                int end = ftell(f);
                if (count >= 4)
		{
                        fseek(f, begin, SEEK_SET);
                        char string[end-begin]; //creates char array one size bigger than the string for the 0 terminator
                        fread(&string, end-begin-1, 1, f);
                        string[end-begin-1] = '\0'; //add 0 terminator
                        printf("%s\n", string);
		}
                count = 0;
	}
        fclose(f);
        return 0;
}
