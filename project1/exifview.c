//Gerard McGlone
//CS449 - Project 1 Part 2
//Exif Viewer

#include <stdio.h>

struct exifHeader
{
	unsigned short start_of_file; //size is 2 bytes
	unsigned short app1_marker; //size is 2 bytes
	unsigned short app1_length; //size is 2 bytes
	unsigned char exif_string[4]; //4 bytes
	unsigned short zero_byte;
	unsigned short endianess;
	unsigned short version_num;
	unsigned int offset; //size is 4 bytes
};

struct tiffTag
{
	unsigned short tag_identifier; //2 bytes
	unsigned short data_type; //2 bytes
	unsigned int num_data_items; //4 bytes
	unsigned int value_or_offset_data; //4 bytes
};

struct data
{
	unsigned char manu_string[40];
	unsigned char camera_model_string[40];
	unsigned int width;
	unsigned int height;
	unsigned short iso_speed;
	unsigned int exposure_speed_numerator;
	unsigned int exposure_speed_denom;
	unsigned int fstop_numerator;
	unsigned int fstop_denom;
	unsigned int lens_focal_numerator;
	unsigned int lens_focal_denom;
	unsigned char date[40];

};

int main(int argc, char **argv)
{
	unsigned short count;
	//unsigned short tiff_tag;
	FILE *f = fopen(argv[1], "rb");
	struct exifHeader header;
	struct tiffTag tiff_tag;
	struct data img_data;
	fread(&header, sizeof(header), 1, f); //read exif header into struct elements
//	printf("%x", header.offset);

	//verifying that there isn't a app0 section prior to app1
	if(header.app1_marker == 0xe0ff)
	{
		printf("APP0 found before APP1.\n");
	}
	
	fread(&count, 2, 1, f); //read in count at offset 20
//	printf("%d\n", ftell(f));
//	printf("%x\n", count);
	int i = 0;
	for (i; i < count; i++) //loop through all tags in the first exif block
	{
		fread(&tiff_tag, sizeof(tiff_tag), 1, f);
		//unsigned short tag_identifier = tiff_tag.tag_identifier;
		if(tiff_tag.tag_identifier == 0x010f)
		{
		//	printf("%d\n", tiff_tag.value_or_offset_data);
			int currentPos = ftell(f);
			fseek(f, tiff_tag.value_or_offset_data+12, SEEK_SET);
			int j = 0;
			for(j; j < tiff_tag.num_data_items; j++)
			{
				img_data.manu_string[j] = getc(f);
			}
			printf("%s\n", img_data.manu_string);

			fseek(f, currentPos, SEEK_SET);
		}
		else if (tiff_tag.tag_identifier == 0x0110)
		{
			//printf("kek\n");
			int currentPos = ftell(f);
			fseek(f, tiff_tag.value_or_offset_data+12, SEEK_SET);
			int j = 0;
			for(j; j < tiff_tag.num_data_items; j++)
			{
				img_data.camera_model_string[j] = getc(f);
			}
			printf("%s\n", img_data.camera_model_string);
			fseek(f, currentPos, SEEK_SET);
		}
		else if (tiff_tag.tag_identifier == 0x8769)
		{
			//use fseek to change point in file to next exif block
			//use tag.value_or_offset + 12 as the numeric parameter in fseek()
		}
	}
	









	return 0;
}
