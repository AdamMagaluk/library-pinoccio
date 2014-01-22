#include "HqHandler.h"

#define USE_TLS


const char HqHandler::host[] = "api.pinocc.io";
#ifndef USE_TLS
// 22757 for TLS, 22756 for plain
const uint16_t HqHandler::port = 22756;
const uint8_t HqHandler::cacert[] = {};
const size_t HqHandler::cacert_len = 0;
#else
// 22757 for TLS, 22756 for plain
const uint16_t HqHandler::port = 22757;

// CA certificate that signed the server certificate.
//  - Using the server certificate here doesn't work, only the CA that
//    signed it is checked (except self-signed certificates where the
//    CA and server certificates are the same, though this was not
//    tested).
//  - No checks of the server certificate (like hostname) are done,
//    other than to confirm that it was indeed signed by the right CA.
//    This means that if you use a server certificate signed by a
//    commercial CA, _any_ other certificate signed by the same CA
//    will also pass the check, which is probably not what you want...
//  - This should be a certificate in (binary) DER format. To convert
//    it to something that can be pasted below, you can use the
//    `xxd -i` command, which should be available on Linux and MacOS X.
//
const uint8_t HqHandler::cacert[] = {
  // This is the Pinoccio HQ CA certificate:
  //
  //     Data:
  //         Version: 3 (0x2)
  //         Serial Number: 9227808474710451669 (0x800fc2eaae74a5d5)
  //     Signature Algorithm: sha1WithRSAEncryption
  //         Issuer: CN=Pinoccio Certificate Authority, ST=NV, C=US/emailAddress=cert@pinocc.io, O=Pinoccio, OU=Carlo api
  //         Validity
  //             Not Before: Jan 20 16:08:22 2014 GMT
  //             Not After : Jan 19 16:08:22 2019 GMT
  //         Subject: CN=Pinoccio Certificate Authority, ST=NV, C=US/emailAddress=cert@pinocc.io, O=Pinoccio, OU=Carlo api
  0x30, 0x82, 0x03, 0xa7, 0x30, 0x82, 0x02, 0x8f, 0xa0, 0x03, 0x02, 0x01,
  0x02, 0x02, 0x09, 0x00, 0x80, 0x0f, 0xc2, 0xea, 0xae, 0x74, 0xa5, 0xd5,
  0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01,
  0x05, 0x05, 0x00, 0x30, 0x81, 0x89, 0x31, 0x27, 0x30, 0x25, 0x06, 0x03,
  0x55, 0x04, 0x03, 0x13, 0x1e, 0x50, 0x69, 0x6e, 0x6f, 0x63, 0x63, 0x69,
  0x6f, 0x20, 0x43, 0x65, 0x72, 0x74, 0x69, 0x66, 0x69, 0x63, 0x61, 0x74,
  0x65, 0x20, 0x41, 0x75, 0x74, 0x68, 0x6f, 0x72, 0x69, 0x74, 0x79, 0x31,
  0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x08, 0x13, 0x02, 0x4e, 0x56,
  0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x55,
  0x53, 0x31, 0x1d, 0x30, 0x1b, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7,
  0x0d, 0x01, 0x09, 0x01, 0x16, 0x0e, 0x63, 0x65, 0x72, 0x74, 0x40, 0x70,
  0x69, 0x6e, 0x6f, 0x63, 0x63, 0x2e, 0x69, 0x6f, 0x31, 0x11, 0x30, 0x0f,
  0x06, 0x03, 0x55, 0x04, 0x0a, 0x13, 0x08, 0x50, 0x69, 0x6e, 0x6f, 0x63,
  0x63, 0x69, 0x6f, 0x31, 0x12, 0x30, 0x10, 0x06, 0x03, 0x55, 0x04, 0x0b,
  0x13, 0x09, 0x43, 0x61, 0x72, 0x6c, 0x6f, 0x20, 0x61, 0x70, 0x69, 0x30,
  0x1e, 0x17, 0x0d, 0x31, 0x34, 0x30, 0x31, 0x32, 0x30, 0x31, 0x36, 0x30,
  0x38, 0x32, 0x32, 0x5a, 0x17, 0x0d, 0x31, 0x39, 0x30, 0x31, 0x31, 0x39,
  0x31, 0x36, 0x30, 0x38, 0x32, 0x32, 0x5a, 0x30, 0x81, 0x89, 0x31, 0x27,
  0x30, 0x25, 0x06, 0x03, 0x55, 0x04, 0x03, 0x13, 0x1e, 0x50, 0x69, 0x6e,
  0x6f, 0x63, 0x63, 0x69, 0x6f, 0x20, 0x43, 0x65, 0x72, 0x74, 0x69, 0x66,
  0x69, 0x63, 0x61, 0x74, 0x65, 0x20, 0x41, 0x75, 0x74, 0x68, 0x6f, 0x72,
  0x69, 0x74, 0x79, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x08,
  0x13, 0x02, 0x4e, 0x56, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04,
  0x06, 0x13, 0x02, 0x55, 0x53, 0x31, 0x1d, 0x30, 0x1b, 0x06, 0x09, 0x2a,
  0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x09, 0x01, 0x16, 0x0e, 0x63, 0x65,
  0x72, 0x74, 0x40, 0x70, 0x69, 0x6e, 0x6f, 0x63, 0x63, 0x2e, 0x69, 0x6f,
  0x31, 0x11, 0x30, 0x0f, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x13, 0x08, 0x50,
  0x69, 0x6e, 0x6f, 0x63, 0x63, 0x69, 0x6f, 0x31, 0x12, 0x30, 0x10, 0x06,
  0x03, 0x55, 0x04, 0x0b, 0x13, 0x09, 0x43, 0x61, 0x72, 0x6c, 0x6f, 0x20,
  0x61, 0x70, 0x69, 0x30, 0x82, 0x01, 0x22, 0x30, 0x0d, 0x06, 0x09, 0x2a,
  0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x01, 0x05, 0x00, 0x03, 0x82,
  0x01, 0x0f, 0x00, 0x30, 0x82, 0x01, 0x0a, 0x02, 0x82, 0x01, 0x01, 0x00,
  0x97, 0x36, 0x7a, 0x77, 0xf7, 0xd0, 0x54, 0x37, 0xa7, 0x84, 0x8a, 0xce,
  0x83, 0xdb, 0xc5, 0xda, 0xfc, 0x12, 0x4a, 0xd4, 0x1c, 0x93, 0x0f, 0x65,
  0x65, 0xa6, 0xaa, 0xce, 0x80, 0x41, 0xf9, 0x37, 0xdc, 0x39, 0x8d, 0xad,
  0x3c, 0x8e, 0x29, 0x60, 0x71, 0xa8, 0x36, 0xcc, 0xd8, 0x28, 0xa1, 0x89,
  0xf9, 0x74, 0xfd, 0x54, 0x4e, 0x46, 0xe2, 0xb4, 0x07, 0xad, 0x23, 0x7f,
  0x52, 0xc6, 0x3d, 0x65, 0xe5, 0xc1, 0xd4, 0x40, 0xcc, 0xf8, 0xf2, 0x69,
  0x2d, 0x03, 0x89, 0xab, 0x38, 0x39, 0xd0, 0x0e, 0xcb, 0x5b, 0x04, 0xb7,
  0x40, 0x36, 0x8e, 0x21, 0x44, 0x6f, 0x87, 0x69, 0xa9, 0x6a, 0x7b, 0x9d,
  0x29, 0xdc, 0x9b, 0x01, 0x58, 0x58, 0x3d, 0xf0, 0x21, 0xa4, 0x2b, 0x3c,
  0x9a, 0x08, 0x8d, 0xcc, 0x76, 0xb8, 0x06, 0x0f, 0x50, 0xe6, 0x1f, 0x9d,
  0x93, 0x19, 0xa8, 0xb3, 0x63, 0xe5, 0x15, 0x7f, 0x08, 0xe5, 0x8b, 0x76,
  0xe8, 0xed, 0x6d, 0xa2, 0x02, 0x2e, 0xce, 0xad, 0x18, 0x0b, 0x88, 0xf3,
  0x8a, 0x11, 0x42, 0xe2, 0xcd, 0xe0, 0xe3, 0x2c, 0xbc, 0xa9, 0x77, 0x1d,
  0xba, 0xdc, 0x34, 0x5a, 0x12, 0x6f, 0xfd, 0x49, 0x5a, 0xe6, 0x37, 0xd3,
  0xe7, 0x3b, 0x0b, 0xba, 0xfc, 0x65, 0x01, 0x80, 0xb1, 0xc0, 0x05, 0x84,
  0xa9, 0xd4, 0xbd, 0xb7, 0x00, 0x99, 0xc5, 0xc4, 0x0b, 0xe3, 0xfd, 0xe5,
  0x29, 0x57, 0xa5, 0xba, 0x0c, 0x93, 0x9d, 0x36, 0x8b, 0xca, 0x50, 0x42,
  0x5d, 0xfa, 0x7a, 0x26, 0xe8, 0xd3, 0xfb, 0xa1, 0x51, 0x98, 0xad, 0xb9,
  0x25, 0x78, 0x09, 0xd9, 0x32, 0x51, 0x5d, 0x09, 0xa2, 0x2d, 0x12, 0x27,
  0x3f, 0xb4, 0x17, 0x99, 0x8e, 0x99, 0x48, 0xe3, 0x44, 0x86, 0xe4, 0x01,
  0xc0, 0x03, 0x1c, 0x86, 0xa1, 0x53, 0xcc, 0xf4, 0xe2, 0x3a, 0x83, 0x09,
  0x78, 0x7d, 0xa6, 0xf3, 0x02, 0x03, 0x01, 0x00, 0x01, 0xa3, 0x10, 0x30,
  0x0e, 0x30, 0x0c, 0x06, 0x03, 0x55, 0x1d, 0x13, 0x04, 0x05, 0x30, 0x03,
  0x01, 0x01, 0xff, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7,
  0x0d, 0x01, 0x01, 0x05, 0x05, 0x00, 0x03, 0x82, 0x01, 0x01, 0x00, 0x8f,
  0xe9, 0xae, 0x28, 0x6e, 0xd0, 0xce, 0x2b, 0x3a, 0x00, 0x3f, 0x4e, 0x85,
  0x21, 0xc6, 0x6a, 0x5e, 0x13, 0xb5, 0xe1, 0xd8, 0x3d, 0xae, 0x4f, 0x35,
  0xf5, 0xe9, 0x7c, 0xa1, 0xae, 0xc5, 0x98, 0x07, 0x36, 0xc8, 0xeb, 0x3b,
  0xc6, 0x66, 0xdc, 0x80, 0x3e, 0xd5, 0xa1, 0x44, 0xe6, 0xa2, 0xcd, 0xf7,
  0xf2, 0x1b, 0xc5, 0x9e, 0xdf, 0x2f, 0xe9, 0x50, 0x54, 0xa3, 0x61, 0x31,
  0x27, 0xc0, 0x1c, 0xd8, 0xd2, 0x31, 0xdf, 0x73, 0xb7, 0xa2, 0xd5, 0x45,
  0x89, 0x61, 0x9f, 0xa4, 0x3f, 0x0e, 0x78, 0x77, 0xc6, 0x57, 0x78, 0xc2,
  0xb8, 0xb8, 0x99, 0xc3, 0xd3, 0x83, 0x64, 0x76, 0x6b, 0xb9, 0x87, 0x74,
  0x57, 0x31, 0x27, 0x9c, 0x35, 0x33, 0xff, 0x48, 0xde, 0x2a, 0x93, 0x51,
  0x77, 0xc5, 0xdd, 0xac, 0xc4, 0x7b, 0xe7, 0x9e, 0x21, 0xa5, 0xdb, 0xa1,
  0xec, 0x16, 0x0a, 0x0a, 0x8c, 0x9a, 0x4c, 0xd8, 0xaf, 0xb1, 0x14, 0x06,
  0x05, 0x74, 0x8c, 0x9a, 0x36, 0x1d, 0x3c, 0xec, 0xfd, 0x18, 0x19, 0xe8,
  0x67, 0xbb, 0x93, 0xcc, 0x8a, 0x63, 0x9e, 0xc5, 0xb1, 0x9f, 0xfd, 0x09,
  0xb5, 0x56, 0xdd, 0x92, 0x9d, 0x2c, 0x9e, 0x5f, 0xc6, 0x2e, 0x21, 0xa4,
  0x3a, 0x48, 0x4b, 0x59, 0x83, 0x00, 0x12, 0x6d, 0x54, 0xb1, 0x9e, 0xae,
  0x1c, 0x9c, 0x1c, 0x28, 0xe1, 0x5f, 0xbb, 0x27, 0x34, 0x73, 0x01, 0x7f,
  0xf8, 0x6c, 0x33, 0x57, 0xb2, 0xc2, 0x9f, 0xac, 0xe9, 0x8e, 0xaf, 0x8c,
  0xb8, 0x73, 0xb3, 0xf7, 0x1b, 0xc5, 0xfc, 0x40, 0xda, 0x1a, 0xd9, 0x5a,
  0xbe, 0xeb, 0x98, 0x2c, 0x28, 0x94, 0xb7, 0x14, 0xcb, 0xe7, 0x27, 0xb3,
  0x33, 0xc0, 0x03, 0xba, 0xca, 0xca, 0x98, 0xce, 0x49, 0xc0, 0x6b, 0x74,
  0x0a, 0xfe, 0x59, 0x31, 0x86, 0x3e, 0x41, 0x4c, 0x97, 0x6a, 0x41, 0xd8,
  0x9d, 0x22, 0x2c
};
const size_t HqHandler::cacert_len = sizeof(cacert);
#endif
