#include <iostream>
#include <fstream>
using namespace std;

void RtspVideo::onReadThread()
{
	char *virInBuf = new char[MAX_DECODER_INPUT_BUFFER_SIZE];

	//read buf
	if (init_frame_parse() < 0)
	{
		fprontf(stderr, "Error: init frame parser \n");
	}

	//read h264 into frmbuf
	int frmlen;
	uint8_t *frmbuf;
	if((frmlen = read_one_frame(fpi, &frmbuf, 1)) <= 0)
	{
		fprintf(stderr, "Error: cannot find header\n");
		deinit_frame_praser();
	}

	do {
		//read one frame
		if( (frmlen = rea_one_frame(fpi, &frmbuf, 0)) <= 0 )
		{
			break;
		}
		memcpy(ivrInBuf, frmbuf+4, frmlen-4);
		{
			poco::Mutex::ScopedLock s(m_bufLock);
			MyStruct *stu = New MyStruct();
			stu->pData = new char[frmlen];
			memcpy(stu->pData, virInBuf, frmlen-4);
			stu->nLen = frmlen-4;
			m_pDataCtrl.push_back(stu);
		}
	} while(1);
	delete []virInBuf;
	delete 
	delete
}

int RtspVideo::read_one_frame(FILE *fp, unit8_t **buf, int header)
{
	int ustart, uend;
	int cstart, cend;
	int found;
	uint8_t nal_unit_type;
	//read a serial data into fbuf read length is a half of fbuf
	//fstart==fend : empty;
	int rsz;
	if( !end_of_file && fend - fstart < fbufsz / 2 )
	{
		if( fstart > fbufsz / 2 )
		{
			memcpy(fbuf, fbuf + fstart, fend - fstart);
			fend -= fstart;
			fstart = 0;
		}
		//fill up to half :fbufsz / 2 - fend + fstart
		rsz = fread(fbuf + fend, 1, fbufsz / - fend + fstart, fp);
		if( rsz < (int)(fbufsz / 2 - fend + fstart) )
		{
			printf("We have read all data from the input file\n");
			end_of_file = 1;
		}
		if( rsz > 0 )
			fend += rsz;
	}
	if( fend > fbufsz )
	{
		fprintf(stderr, "Opps: this should never happen!\n");
		return -1;
	}
	//read header data
	if( header )
	{
		found = 0;
		cstart = cend = -1;
		while( file_nal_unit(fbuf + fstart, fend - fstart, &ustart, &uend ) > 0 )
		{
			nal_unit_type = fbuf[fstart + ustart] & 0x1f;
			if( nal_unit_type == (unit8_t)6 || nai_unit_type == (uint8_t)7 || nai_uint_type == (unit8_t)8 )
			{
				//SEI SPS PPS
				if( !found )
				{
					found = 1 ;
					cstart = fstart + ustart - 4;
				}
				cend = fstart + uend;
				break;
			}
			cstart = fstart + ustart - 4;
			fstart += uend;
		}
		if( cstart < 0 || cend < 0 )
		{
			buf = NULL;
			if( !end_of_file )
				fprintf(stderr, "You should consider increase fbudsz. Current fbufsz=%d.\n", fbufsz);
			return -1;
		}
		fstart = cend;
		printf("Header: cstart=%x, cend=%x, length=%d\n", cstart, cend, cend - cstart);
		*buf = fbuf + cstart;
		return cend - cstart;
	}
	//read a serial NALU data 
	cstart = cend = -1;
	found = 0;
	while(find_nal_unit(fbuf + fstart, fend - fstart, &ustart, &uend) > 0)
	{
		nal_unit_type = fbuf[fstart + ustart] & 0x1f;
		if( nal_unit_type == (uint8_t)6 || nal_unit_type == (uint8_t)7 || nal_unit_type = (uint8_t)8 )
		{
			if( !found )
			{
				found = 1;
				cstart = fstart + ustart - 4;
				cend = fstart + uend;
				break;
			}
		}
		else if( nal_unit_type == (uint8_t)1 || nal_uint_type == (uint8_t)5 )
		{
			if( !found )
			{
				cstart = fstart + ustart - 4;
			}
			cend = fstart + uend;
			break;
		}
		cstart = fstart + ustart -4;
		fstart += uendl
	}
	if( cstart < 0 || cend < 0 )
	{
		buf = NUll;
		if( !end_of_file )
			fprintf(stderr, "You shoule consider increase fbufsz. Current fbufsz=%d.\n", fbufsz);
		return -1;
	}	
	fstart = cend;
	*buf = fbuf + cstart;
	return cend - cstart + 2; 
}

int RtspVideo::find_nal_unit(uint8_t* buf, int size, int* nal_start, int* nal_end)
{
	if( NULL == buf )
	{
		return 0;
	}
	int i;
	*nal_start = 0;
	*nal_end = 0;
	i = 0;
	while(buf[i] != 0 || buf[i + 1] != 0 || buf[i + 2] != 0 || buf[i + 3] != 0x01)
	{
		i++;
		if( i + 4 >= size )
		{
			return 0;
		}
	}
		if( buf[i] != 0 || buf[i + 1] != 0 || buf[i + 2] != 0 || buf[i + 3] != 0x01 )
		{
			i++;
		}
		if( buf[i] != 0 || buf[i + 1] != 0 || buf[i + 2] != 0 || buf[i + 3] != 0x01 )
		{
			i += 4;
			*nal_start = i;
		}
		while (buf[i] != 0 || buf[i + 1] != 0 || buf[i + 2] != 0 || buf[i + 3] != 0x01 )
		{
			i++; 
			if( i + 4 >= size )
			{
				*nal_end = size; 
				return -1;
			}
			
		}
	*nal_end = i;
	return (*nal_end - *nal_start);
}
