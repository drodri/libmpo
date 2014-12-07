#ifndef MPO_H_INCLUDED
#define MPO_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>
/**
 * \file
 * \brief
 * \author Lectem
 */


typedef enum
{
    MP_LONG = 4,
    MP_UNDEFINED = 7
}
MPVal_types;

/* See the DC-007_E Specification. */
/* 5.2.2.1  Table 3, page 13 */
typedef enum{ LITTLE_ENDIAN = 0x49492A00,
    BIG_ENDIAN = 0x4D4D002A }MPExt_ByteOrder;

/* See the DC-007_E Specification. */
/* 5.2.2.3  Table 3, page 13 */
typedef enum
{
    /**MP Index IFD**/
    /*Mandatory*/
    MPTag_MPFVersion        = 0xB000,
    MPTag_NumberOfImages    = 0xB001,
    MPTag_MPEntry           = 0xB002,
    /*Optional*/
    MPTag_ImageUIDList      = 0xB003,
    MPTag_TotalFrames       = 0xB004,

    /**Individual image tags**/
    MPTag_IndividualNum     = 0xb101,
    MPTag_PanOrientation    = 0xb201,
    MPTag_PanOverlapH       = 0xb202,
    MPTag_PanOverlapV       = 0xb203,
    MPTag_BaseViewpointNum  = 0xb204,
    MPTag_ConvergenceAngle  = 0xb205,
    MPTag_BaselineLength    = 0xb206,
    MPTag_VerticalDivergence= 0xb207,
    MPTag_AxisDistanceX     = 0xb208,
    MPTag_AxisDistanceY     = 0xb209,
    MPTag_AxisDistanceZ     = 0xb20a,
    MPTag_YawAngle          = 0xb20b,
    MPTag_PitchAngle        = 0xb20c,
    MPTag_RollAngle         = 0xb20d

}MPExt_MPTags;

typedef union
{
    INT32 value;
    struct {
        unsigned int MPTypeCode:24;
        unsigned int imgType:3;
        unsigned int reserved:2;
        unsigned int representativeImage:1;
        unsigned int dependentChild:1;
        unsigned int dependentParent:1;
    } data;
}
MPExt_IndividualImageAttr;

typedef enum
{
    MPType_LargeThumbnail_Mask  =0x10000,
    MPType_LargeThumbnail_Class1=0x10001, /*VGA Equivalent*/
    MPType_LargeThumbnail_Class2=0x10002, /*Full HD Equivalent*/
    MPType_MultiFrame_Mask      =0x20000,
    MPType_MultiFrame_Panorama  =0x20001,
    MPType_MultiFrame_Disparity =0x20002,
    MPType_MultiFrame_MultiAngle=0x20003,
    MPType_Baseline             =0x30000
}
MPExt_MPType;

typedef struct
{
    INT16 type;
    INT32 EntriesTabLength;
    INT32 FirstEntryOffset;
}
MPExt_MPEntryIndexFields;

typedef struct
{
    MPExt_IndividualImageAttr individualImgAttr;
    INT32 size;
    INT32 offset;
    INT16 dependentImageEntry1;
    INT16 dependentImageEntry2;
}
MPExt_MPEntry;

typedef struct
{
    char MPF_identifier[4];
    MPExt_ByteOrder byte_order;
    INT32 first_IFD_offset;
    /*MP Index IFD*/
    INT16 count;
    char version[4];
    long numberOfImages;
    long currentEntry;
    MPExt_MPEntryIndexFields EntryIndex;
    INT32 nextIFDOffset;

    INT16 count_attr_IFD;

    MPExt_MPEntry* MPentry;
}
MPExt_Data;

typedef struct
{
    MPExt_Data APP02;
    struct jpeg_compress_struct *cinfo;
    JOCTET ** images_data;
}
mpo_compress_struct;


typedef struct
{
    MPExt_Data APP02;
    struct jpeg_decompress_struct *cinfo;
    JOCTET ** images_data;
}
mpo_decompress_struct;


void decompress_mpo(char* filename);
void decompress_mpo_from_mem(unsigned char* src,long size,int isFirstImage);
boolean MPExtReadAPP02AsFirstImage(j_decompress_ptr cinfo);
boolean MPExtReadAPP02 (j_decompress_ptr cinfo);



inline char isLittleEndian();

#endif // MPO_H_INCLUDED
