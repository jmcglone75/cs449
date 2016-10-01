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
	unsigned char endianess[2];
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
	FILE *f = fopen(argv[1], "rb");
	struct exifHeader header;
	struct tiffTag tiff_tag;
	struct data img_data;
	fread(&header, sizeof(header), 1, f); //read exif header into struct elements

	//verifying that there isn't a app0 section prior to app1
	if(header.app1_marker == 0xe0ff)
	{
		printf("APP0 found before APP1.\n");
	}

	if(strcmp(header.exif_string, "Exif") != 0)
	{
		printf("Error: Tag was not found.\n");
	}

	if(strcmp(header.endianess, "II*") != 0)
	{
		printf("Cannot support this type of endianess\n");
	}
	
	fread(&count, 2, 1, f); //read in count at offset 20
	
	int i = 0;
	for (i; i < count; i++) //loop through all tags in the first exif block
	{
		fread(&tiff_tag, sizeof(tiff_tag), 1, f);
		if(tiff_tag.tag_identifier == 0x010f)
		{
			int currentPos = ftell(f);
			fseek(f, tiff_tag.value_or_offset_data+12, SEEK_SET);

			int j = 0;
			for(j; j < tiff_tag.num_data_items; j++)
			{
				img_data.manu_string[j] = getc(f);
			}

			fseek(f, currentPos, SEEK_SET);
		}
		else if (tiff_tag.tag_identifier == 0x0110)
		{
			int currentPos = ftell(f);
			fseek(f, tiff_tag.value_or_offset_data+12, SEEK_SET);

			int j = 0;
			for(j; j < tiff_tag.num_data_items; j++)
			{
				img_data.camera_model_string[j] = getc(f);
			}
			
			fseek(f, currentPos, SEEK_SET);
		}
		else if (tiff_tag.tag_identifier == 0x8769)
		{
			fseek(f, tiff_tag.value_or_offset_data+12, SEEK_SET);
			i = count; //break out of loop
		}
	}
	
	fread(&count, 2, 1, f); //read in TIFF count of next exif block
	
	i = 0;
	for(i; i < count; i++)
	{
		fread(&tiff_tag, sizeof(tiff_tag), 1, f); //read in next TIFF tag

		if(tiff_tag.tag_identifier == 0xa002)
		{
			img_data.width = tiff_tag.value_or_offset_data;
		}
		else if(tiff_tag.tag_identifier == 0xa003)
		{
			img_data.height = tiff_tag.value_or_offset_data;
		}
		else if(tiff_tag.tag_identifier == 0x8827)
		{
			img_data.iso_speed = tiff_tag.value_or_offset_data;
		}
		else if(tiff_tag.tag_identifier == 0x829a)
		{
			int currentPos = ftell(f);
			fseek(f, tiff_tag.value_or_offset_data+12, SEEK_SET);
			
			fread(&img_data.exposure_speed_numerator, 4, 1, f);
			fread(&img_data.exposure_speed_denom, 4, 1, f);

			fseek(f, currentPos, SEEK_SET);
		}
		else if(tiff_tag.tag_identifier == 0x829d)
		{
			int currentPos = ftell(f);
			fseek(f, tiff_tag.value_or_offset_data+12, SEEK_SET);
			
			fread(&img_data.fstop_numerator, 4, 1, f);
			fread(&img_data.fstop_denom, 4, 1, f);

			fseek(f, currentPos, SEEK_SET);
			
		}
		else if(tiff_tag.tag_identifier == 0x920a)
		{
			int currentPos = ftell(f);
			fseek(f, tiff_tag.value_or_offset_data+12, SEEK_SET);
			
			fread(&img_data.lens_focal_numerator, 4, 1, f);
			fread(&img_data.lens_focal_denom, 4, 1, f);

			fseek(f, currentPos, SEEK_SET);
		}
		else if(tiff_tag.tag_identifier == 0x9003)
		{
			int currentPos = ftell(f);
			fseek(f, tiff_tag.value_or_offset_data+12, SEEK_SET);
			int j = 0;
			for(j; j < tiff_tag.num_data_items; j++)
			{
				img_data.date[j] = getc(f);
			}

			fseek(f, currentPos, SEEK_SET);
		}
	}

	float fstop = ((float) img_data.fstop_numerator)/img_data.fstop_denom; //get fstop's float value

	printf("\nManufacturer:\t%s\n", img_data.manu_string);
	printf("Model:\t\t%s\n", img_data.camera_model_string);
	printf("Exposure Time:\t%d/%d second\n", img_data.exposure_speed_numerator, img_data.exposure_speed_denom);
	printf("F-stop:\t\tf/%.3g\n", fstop);
	printf("ISO:\t\tISO %d\n", img_data.iso_speed);
	printf("Date Taken:\t%s\n", img_data.date);
	printf("Focal Length:\t%d mm\n", img_data.lens_focal_numerator/img_data.lens_focal_denom);
	printf("Width:\t\t%d pixels\n", img_data.width);
	printf("Height:\t\t%d pixels\n", img_data.height);

	return 0;
}
