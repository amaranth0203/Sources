#ifndef _DATA_STRUCT_H_
#define _DATA_STRUCT_H_

#include <stdint.h>
#include <Winsock2.h>
#include <Windows.h>
#define QDEBUG_MARKER "Qualcomm Camera Debug" // 22 byte include '\0'

typedef struct QCAEC_HEADER {
  unsigned char marker_1[6] ; // QCAEC\0
  unsigned char size[4] ;
  unsigned char data[0x2e-4-6] ; // from chromatix , lux index offset is 0x2e
  unsigned char lux_index[2] ;
} QCAEC_HEADER ;

typedef struct QCAWB_HEADER {
  unsigned char marker_1[6] ; // QCAWB\0
  unsigned char size[4] ;
  union u {
    struct qcom_awb_data {
      unsigned char data_qcom[0x83-4-6] ; // from chromatix , color temperature offset is 0x83
      unsigned char color_temperature_qcom[2] ;
    } ;
    struct alc_awb_data {
      unsigned char muiQcReserved[4] ;
      unsigned char alcs[4] ; // 'ALCS'
      unsigned char data_alc[0xde-4-4] ; // offset from ALC Zhao, 2017.03.22
      unsigned char color_temperature_alc[2] ;
    } ;
  } ;
} QCAWB_HEADER ;

typedef struct QDEBUG_HEADER {
  unsigned char marker_1[2] ; // 0xff 0xff
  unsigned char marker_2[22] ; // Qualcomm Camera Debug\0
  unsigned char fmt_version_number[4] ;
  unsigned char _3A_SW_version_number[8] ;
  unsigned char marker_3[6] ; // QCTUN\0
  unsigned char chromatix_version[4] ; 
  unsigned char AAA_header_verison[4] ; // 00 00 03 09
} QDEBUG_HEADER ;

typedef struct JPEG_HEADER {
  unsigned char SOI[2] ;
  unsigned char app1[2] ;
  unsigned char size[2] ;
} JPEG_HEADER ;

#endif
