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

}

int main(int argc, char **argv)
{
	unsigned short count;
	unsigned short tiff_tag;
	FILE *f = fopen(argv[1], "rb");
	struct exifHeader header;
	struct tiffTag tiff_tag;
	fread(&header, 20, 1, f); //read exif header into struct elements
//	printf("%x", header.offset);

	//verifying that there isn't a app0 section prior to app1
	if(header.app1_marker == 0xe0ff)
	{
		printf("APP0 found before APP1.\n");
	}

	fread(&count, 2, 1, f); //read in count at offset 20
//	printf("%x\n", count);
	int i = 0;
	for (i; i < count; i++) //loop through all tags in the first exif block
	{
		fread(&tiff_tag, 12, 1, image);
		unsigned short tag_identifier = tiff_tag.tag_identifier;
		if(tag_indetifier == 0x010f)
		{

		}
		else if (tag_identifier == 0x0110)
		{

		}
		else if (tag_identifier == 0x8769)
		{
		
		}
	}
	









	return 0;
}
