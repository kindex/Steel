/*id*********************************************************
	Unit: Audio Engine
	Part of: Steel Engine
	(C) DiVision, 2004-2006
	Authors:
		* Kuzmi4 [Andrey Movlyaiko, andrey.movlyaiko@gmail.com]
	License:
		Steel Engine License
	File history:
		[2006.07.29 20:28:15] Kuzmi4: File Creation.
    Description:
		Звуковой движок.
 ************************************************************/

#include "wav.h"
#include "../rstream.h"
#include <fstream>

//using namespace std;

bool WAV::init(const std::string name, const std::string dir)
{

	// check file existance
	rstream f;
	
	if(!f.open(dir + "/" + name, "wav"))
		return false;


	filebuf *pbuf;
	long sizel;
	char *buffer_;

	pbuf = f.rdbuf();
	// get file size using buffer's members
	sizel = pbuf->pubseekoff (0,ios::end,ios::in);
	pbuf->pubseekpos (0,ios::in);
	// allocate memory to contain file data
	buffer_ = new char[sizel];
	// get file data  
	pbuf->sgetn (buffer_, sizel);
	f.close();


	WavChunkHdr chunkHdr;
	WavFmtExHdr fmtExHdr;
	WavFileHdr fileHdr;
	WavSmplHdr smplHdr;
	WavFmtHdr fmtHdr;
	char *stream;
	
	format = FORMAT_MONO16;
	data = NULL;
	size = 0;
	frequency = 22050;
	loop = false;
	if (buffer_)
	{
		stream = buffer_;
		if (stream)
		{
			memcpy(&fileHdr, stream, sizeof(WavFileHdr));
			stream += sizeof(WavFileHdr);
			fileHdr.size = ((fileHdr.size + 1) & ~1) - 4;
			while ((fileHdr.size != 0) && (memcpy(&chunkHdr, stream, sizeof(WavChunkHdr))))
			{
				stream += sizeof(WavChunkHdr);
				if (!memcmp(chunkHdr.id, "fmt ", 4))
				{
					memcpy(&fmtHdr, stream, sizeof(WavFmtHdr));
					if (fmtHdr.format==0x0001)
					{
						format = (fmtHdr.channels == 1 ?
								  (fmtHdr.bitsPerSample == 8 ? FORMAT_MONO8 : FORMAT_MONO16):
								  (fmtHdr.bitsPerSample == 8 ? FORMAT_STEREO8 : FORMAT_STEREO16));
						frequency = fmtHdr.samplesPerSec;
						stream += chunkHdr.size;
					} 
					else
					{
						memcpy(&fmtExHdr, stream, sizeof(WavFmtExHdr));
						stream += chunkHdr.size;
					}
				}
				else if (!memcmp(chunkHdr.id, "data", 4))
				{
					if (fmtHdr.format == 0x0001)
					{
						size = chunkHdr.size;
						data = malloc(chunkHdr.size + 31);
						if (data) memcpy(data, stream, chunkHdr.size);
						memset(((char *) data) + chunkHdr.size, 0, 31);
						stream += chunkHdr.size;
					}
					else if (fmtHdr.format == 0x0011)
					{
						//IMA ADPCM
					}
					else if (fmtHdr.format == 0x0055)
					{
						//MP3 WAVE
					}
				}
				else if (!memcmp(chunkHdr.id, "smpl", 4))
				{
					memcpy(&smplHdr, stream, sizeof(WavSmplHdr));
					loop = (smplHdr.loops ? true : false);
					stream += chunkHdr.size;
				}
				else stream += chunkHdr.size;
				stream += chunkHdr.size & 1;
				fileHdr.size -= (((chunkHdr.size + 1) & ~1) + 8);
			}
		}
	}
	else 
		return false;
	return true;
}
