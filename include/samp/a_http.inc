/*****************************************************
 *                                                   *
 * Copyright © 2018 SA-MP Team. All rights reserved. *
 * SAMP Version: 0.3.DL R1                           *
 *                                                   *
 *****************************************************/

// HTTP requests
#define HTTP_GET						1
#define HTTP_POST						2
#define HTTP_HEAD						3

// HTTP error response codes
// These codes compliment ordinary HTTP response codes returned in 'response_code'
// (10x) (20x OK) (30x Moved) (40x Unauthorised) (50x Server Error)  
#define HTTP_ERROR_BAD_HOST				1
#define HTTP_ERROR_NO_SOCKET			2
#define HTTP_ERROR_CANT_CONNECT			3
#define HTTP_ERROR_CANT_WRITE			4
#define HTTP_ERROR_CONTENT_TOO_BIG		5
#define HTTP_ERROR_MALFORMED_RESPONSE	6

native HTTP(index, type, url[], data[], callback[]);

// example HTTP callback: public MyHttpResponse(index, response_code, data[]) { ... }