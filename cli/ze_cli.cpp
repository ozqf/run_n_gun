#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN

#include <windows.h>
#include <shellapi.h> // for parsing command line tokens
#include <stdio.h>
#include <stdlib.h>

#include <zengine.h>

internal i32 g_bConsoleInit = FALSE;
internal HWND consoleHandle;

ze_internal void WriteI32(char** cursor, const i32 i)
{
	*(i32*)cursor = i;
	*cursor += sizeof(i32);
}

ze_internal i32 WriteStr(char* cursor, const char* data, i32 len)
{
	if (len <= 0)
	{
		len = (i32)strlen(data);
	}
	printf("Writing %d chars\n", len);
	i32 charsToWrite = len;
	memcpy(cursor, data, charsToWrite);
	// null terminator
	char* tar = cursor + charsToWrite;
	*tar = 0;
	return charsToWrite + 1;
}

ze_internal void PrintCharBuf(char* data, i32 len)
{
	for (i32 i = 0; i < len; ++i)
	{
		if (data[i] == 0)
		{
			printf("0\n");
			continue;
		}
		printf("%c", data[i]);
	}
}

ze_internal void PrintCharBufBytes(u8* data, i32 len)
{
	for (i32 i = 0; i < len; ++i)
	{
		u8 v = data[i];
		if (v <= 31)
		{
			printf("%d, ", v);
		}
		else
		{
			printf("%c, ", v);
		}
	}
	printf("\n");
}

ze_internal void WriteStrBlock(char* buffer, i32 bufSize, i32* cursor, i32 tag, char* data)
{
	i32 len = (i32)strlen(data);
	if (*cursor + len >= bufSize) { return; }
	printf("Tag %d has %d chars\n", tag, len);
	i32* ptr = (i32*)(buffer + *cursor);
	*ptr = tag;
	*cursor += sizeof(i32);
	ptr = (i32*)(buffer + *cursor);
	*ptr = len + 1;
	*cursor += sizeof(i32);
	
	*cursor += WriteStr(buffer + *cursor, data, len);
}

ze_internal char* FindBlock(i32 queryTag, char* buf, i32 len)
{
	char* cursor = buf;
	while ((cursor - buf) < len)
	{
		i32 tag = *(i32*)cursor;
		cursor += sizeof(i32);
		
		i32 blockSize = *(i32*)cursor;
		cursor += sizeof(i32);
		
		if (tag == queryTag)
		{
			return cursor;
		}
		
		cursor += blockSize;
	}
	return NULL;
}

ze_internal void PrintStringBlocks(char* buf, i32 len)
{
	char* cursor = buf;
	while ((cursor - buf) < len)
	{
		i32 tag = *(i32*)cursor;
		cursor += sizeof(i32);
		i32 blockSize = *(i32*)cursor;
		cursor += sizeof(i32);
		cursor += blockSize;
		printf("Tag %d, len %d\n", tag, blockSize);
	}
}

ze_internal void TestStringBlock()
{
	i32 bufSize = 1042;
	char* strings = (char*)Platform_Alloc(1024);
	i32 cursor = 0;
	
	WriteStrBlock(strings, bufSize, &cursor, 1, "Parsley");
	WriteStrBlock(strings, bufSize, &cursor, 2, "Sage");
	WriteStrBlock(strings, bufSize, &cursor, 3, "Rosemary");
	WriteStrBlock(strings, bufSize, &cursor, 4, "Thyme");
	PrintCharBufBytes((u8*)strings, cursor);
	PrintStringBlocks(strings, cursor);

	char* str2 = FindBlock(2, strings, bufSize);
	if (str2 != NULL)
	{
		printf("Block 2: %s\n", str2);	
	}
	else
	{
		printf("Failed to find block 2\n");
	}
}

ze_internal void TestStringCopy()
{
	char* strings = (char*)Platform_Alloc(1024);
	//char* cursor = strings;
	i32 written = 0;
	char* tar;
	
	tar = (strings + written);
	written += WriteStr(tar, "Parsley", -1);
	
	tar = (strings + written);
	written += WriteStr(tar, "Sage", -1);
	
	tar = (strings + written);
	written += WriteStr(tar, "Rosemary", -1 );
	
	tar = (strings + written);
	written += WriteStr((strings + written), "Thyme", -1);
	
	printf("Wrote %d chars\n", written);
	PrintCharBuf(strings, written);
}

ze_internal void PrintGrid()
{
	printf("########\n");
	
	printf("#      #\n");
	printf("#      #\n");
	printf("#      #\n");
	printf("#      #\n");
	printf("#      #\n");
	printf("#      #\n");
	printf("#      #\n");
	
	printf("########\n");
}

ze_internal void EngineLibTests()
{
	TestStringCopy();
	TestStringBlock();
	PrintGrid();
}

extern "C" int main(
    int argc, char** argv)
{
	printf("ZEALOUS ENGINE CLI TOOLS\n");
	printf("Built %s %s\n", __TIME__, __DATE__);
	EngineLibTests();
	return 0;
}
