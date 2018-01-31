/*************************************************************************************************************************************
 *                                                                                                                                   *
 *                                                      )(   Data Convert   )(                                                       *
 *                                                                                                                                   *
 * Copyright © 2018 Abyss Morgan. All rights reserved.                                                                               *
 * Contributors: Crayder, Shiny                                                                                                      *
 *                                                                                                                                   *
 * Website:  http://8.ct8.pl                                                                                                         *
 * Download: http://8.ct8.pl/r/download                                                                                              *
 *                                                                                                                                   *
 * Plugins: SScanf                                                                                                                   *
 * Modules: None                                                                                                                     *
 *                                                                                                                                   *
 * File Version: 2.1.1                                                                                                               *
 * SScanf Version: 2.8.2                                                                                                             *
 *                                                                                                                                   *
 * Definitions:                                                                                                                      *
 * FIRST_FILE_ADDRESS                                                                                                                *
 * INVALID_FILE_ID                                                                                                                   *
 * INVALID_FILE_HANDLE                                                                                                               *
 * INT_NUMBER_LEN                                                                                                                    *
 * HEX_NUMBER_LEN                                                                                                                    *
 * BIN_NUMBER_LEN                                                                                                                    *
 * OCT_NUMBER_LEN                                                                                                                    *
 *                                                                                                                                   *
 * Functions:                                                                                                                        *
 * strcopy(input[],output[]);  //string copy for warning 224: indeterminate array size                                               *
 * fstrcopy(input[],output[],maxdest = sizeof(output));   //fast string copy                                                         *
 * ABCToCBA(input[],output[]);                                                                                                       *
 * AnyToInt(string[],type,&bool:flag);                                                                                               *
 * IntToAny(value,output[],type);                                                                                                    *
 * rot13(string[]);                                                                                                                  *
 * code128(input[],output[]);                                                                                                        *
 * swap_int({Float,_}:int1,{Float,_}:int2);                                                                                          *
 * swap_string(string1[], string2[], dest1 = sizeof(string1), dest2 = sizeof(string2));                                              *
 * bool:ContainInSeries(value,...);                                                                                                  *
 * bool:ContainInSeriesFloat(Float:value,Float:...);                                                                                 *
 * StringEqual(string1[],string2[],ignorecase);                                                                                      *
 * StringNotEqual(string1[],string2[],ignorecase);                                                                                   *
 * base64_decode(input[],output[]); //d:base64                                                                                       *
 * base64_encode(input[],output[]); //e:base64                                                                                       *
 * StripNewLine(string[]);                                                                                                           *
 * strreplace(string[],const search[],const replacement[],bool:ignorecase = false,pos = 0,limit = -1,maxlength = sizeof(string));    *
 * strlower(string[]);                                                                                                               *
 * strupper(string[]);                                                                                                               *
 *                                                                                                                                   *
 * File functions:                                                                                                                   *
 * GetFileHandleID(File:handle);                                                                                                     *
 * GetFileID(File:handle);                                                                                                           *
 * fcopy(input[],output[]);                                                                                                          *
 * frename(oldname[],newname[]);                                                                                                     *
 *                                                                                                                                   *
 * Validators:                                                                                                                       *
 * bool:IsDecimalNumber(string[]);                                                                                                   *
 * bool:IsHexNumber(string[]);                                                                                                       *
 * bool:IsBinaryNumber(string[]);                                                                                                    *
 * bool:IsOctalNumber(string[]);                                                                                                     *
 * bool:IsFloatNumber(string[]);                                                                                                     *
 *                                                                                                                                   *
 * Convert to Bool:                                                                                                                  *
 * bool:StringToBool(string[]);                                                                                                      *
 * bool:IntToBool(value);                                                                                                            *
 *                                                                                                                                   *
 * Convert to Int:                                                                                                                   *
 * StringToInt(string[]);                                                                                                            *
 * BoolToInt(bool:boolean);                                                                                                          *
 * HexToInt(string[]);                                                                                                               *
 * BinToInt(string[]);                                                                                                               *
 * OctToInt(string[]);                                                                                                               *
 *                                                                                                                                   *
 * Convert to Hex:                                                                                                                   *
 * IntToHex(value);                                                                                                                  *
 * IntToHexEx(value);                                                                                                                *
 * BinToHex(string[]);                                                                                                               *
 * BinToHexEx(string[]);                                                                                                             *
 * OctToHex(string[]);                                                                                                               *
 *                                                                                                                                   *
 * Convert to Bin:                                                                                                                   *
 * IntToBin(value);                                                                                                                  *
 * HexToBin(string[]);                                                                                                               *
 * OctToBin(string[]);                                                                                                               *
 *                                                                                                                                   *
 * Convert to Oct:                                                                                                                   *
 * IntToOct(value);                                                                                                                  *
 * HexToOct(string[]);                                                                                                               *
 * BinToOct(string[]);                                                                                                               *
 *                                                                                                                                   *
 * Binary Data Convert:                                                                                                              *
 * FloatToInt(Float:value); //Convert Float binary data to Integer binary data !                                                     *
 * Float:IntToFloat(value); //Convert Integer binary data to Float binary data !                                                     *
 *                                                                                                                                   *
 *************************************************************************************************************************************/

/*
//Check Version DataConvert.inc
#if !defined _Data_Convert
	#error [ADM] You need DataConvert.inc v2.1.1
#elseif !defined Data_Convert_Version
	#error [ADM] Update you DataConvert.inc to v2.1.1
#elseif (Data_Convert_Version < 20101)
	#error [ADM] Update you DataConvert.inc to v2.1.1
#endif
*/

#if defined _Data_Convert
	#endinput
#endif
#define _Data_Convert

#if defined _base64_included
	#error [ADM] Please remove deprecated file base64.inc
#endif

//Update Checker
#if !defined HTTP
	#tryinclude <a_http>
#endif

#if !defined HTTP
	#error [ADM] Please include a_http.inc before DataConvert.inc
#endif

#if !defined sscanf
	#error [ADM] You need SScanf v2.8.2 (8.ct8.pl/d/SScanf%202.8.2)
#endif

#define Data_Convert_Version						(20101) //a.b.c 10000*a+100*b+c
#define DataConvert_SAMP_VERSION					"0.3.7"

#define DataConvert_Function::						stock
#define DataConvert_Public::%0(%1)					forward %0(%1); public %0(%1)
#define DataConvert::								DataConvert_

new const DataConvert::LettersTable[] = {
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
};
	
new const DataConvert::Decoder[] = {
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  62, 0,  0,  0,  63, 52, 53,
	54, 55, 56, 57, 58, 59, 60, 61, 0,  0,
	0,  0,  0,  0,  0,  0,  1,  2,  3,  4,
	5,  6,  7,  8,  9,  10, 11, 12, 13, 14,
	15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
	25, 0,  0,  0,  0,  0,  0,  26, 27, 28,
	29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
	39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
	49, 50, 51, 0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0
};

#if !defined isnull
	#define isnull(%1) 					((!(%1[0])) || (((%1[0]) == '\1') && (!(%1[1]))))
#endif

#define INT_NUMBER_LEN					12
#define HEX_NUMBER_LEN					12
#define BIN_NUMBER_LEN					36
#define OCT_NUMBER_LEN					14

#define FIRST_FILE_ADDRESS				(0x004E5A18)
#define INVALID_FILE_ID					(0xFFB1A5E7)
#define INVALID_FILE_HANDLE				(File:0)
#define GetFileHandleID(%0)				(_:%0)
#define StringToInt(%0)					strval(%0)

//swapvars by Crayder
#define swap_int(%0,%1)					(((%0) ^= (%1)), ((%1) ^= (%0)), ((%0) ^= (%1)))

#define BoolToInt(%0)					((%0)?1:0)
#define IntToBool(%0)					(((%0) == 1)?true:false)

#define FloatToInt(%0)					(_:%0)
#define IntToFloat(%0)					(Float:%0)

#define StringEqual(%0,%1,%2) 			(!strcmp((%0),(%1),(%2)))
#define StringNotEqual(%0,%1,%2)		(strcmp((%0),(%1),(%2)) != 0)

#define DataConvert_GetActiveCount() 	GetSVarInt("ADM:DataConvert:ACTIVE")

DataConvert::Function:: ABCToCBA(input[],output[]){
	for(new i = 0, j = strlen(input); i < j; i++){
		output[j-(i+1)] = input[i];
	}
	output[strlen(input)] = EOS;
}

DataConvert::Function:: AnyToInt(string[],type,&bool:flag){
	new DC_Symbol, len = strlen(string), i = 1, value;
	if(string[0] >= 0x30 && string[0] <= 0x39){
		value = string[0] - 48;
	} else {
		value = string[0] - 55;
	}
	if(value >= type){
		flag = false;
		return 0;
	}

	while(i < len){
		if(string[i] >= 0x30 && string[i] <= 0x39){
	    	DC_Symbol = string[i] - 48;
		} else {
			DC_Symbol = string[i] - 55;
		}
		if(DC_Symbol >= type){
			flag = false;
			return 0;
		}
		value = value * type + DC_Symbol;
		i++;
	}
	flag = true;
	return value;
}

DataConvert::Function:: IntToAny(value,output[],type){
	new tmp = value, val[256], i = 0;
	while(tmp > 0){
		val[i] = (tmp % type);
		if(val[i] < 10){
			val[i] += 0x30;
		} else {
			val[i] += 0x37;
		}
		tmp /= type;
		i++;
	}
	val[i] = EOS;
	ABCToCBA(val,output);
}

DataConvert::Function:: strcopy(input[],output[]){
	for(new i = 0, j = strlen(input); i < j; i++){
		output[i] = input[i];
	}
	output[strlen(input)] = EOS;
}

DataConvert::Function:: fstrcopy(input[],output[],maxdest = sizeof(output)){
	output[0] = EOS;
	strcat(output,input,maxdest);
	output[strlen(input)] = EOS;
}

DataConvert::Function:: bool:IsDecimalNumber(string[]){
	new value = 0;
	return !(sscanf(string,"d",value));
}

DataConvert::Function:: bool:IsHexNumber(string[]){
	new value = 0;
	return !(sscanf(string,"x",value));
}

DataConvert::Function:: bool:IsBinaryNumber(string[]){
	new value = 0;
	return !(sscanf(string,"b",value));
}

DataConvert::Function:: bool:IsOctalNumber(string[]){
	new value = 0;
	return !(sscanf(string,"o",value));
}

DataConvert::Function:: bool:IsFloatNumber(string[]){
	new Float:value = 0;
	return !(sscanf(string,"f",value));
}

DataConvert::Function:: bool:swap_string(string1[],string2[],dest1 = sizeof(string1),dest2 = sizeof(string2)){
	if(dest1 < strlen(string2) || dest2 < strlen(string1)) return false;
	new swap_size = dest1;
	if(dest2 > swap_size) swap_size = dest2;
	for(new i = 0; i < swap_size; i++){
		swap_int(string1[i],string2[i]);
	}
	return true;
}

DataConvert::Function:: GetFileID(File:handle){
	return (handle == INVALID_FILE_HANDLE)?INVALID_FILE_ID:((GetFileHandleID(handle)-FIRST_FILE_ADDRESS)/32);
}

DataConvert::Function:: BinToInt(string[]){
	new value = 0;
	if(sscanf(string,"b",value)) return 0;
	return value;
}

DataConvert::Function:: HexToBin(string[]){
	new DC_Result[BIN_NUMBER_LEN], value = 0;
	sscanf(string,"x",value);
	format(DC_Result,sizeof(DC_Result),"%b",value);
	return DC_Result;
}

DataConvert::Function:: BinToHex(string[]){
	new DC_Result[HEX_NUMBER_LEN], value = 0;
	if(sscanf(string,"b",value)){
		format(DC_Result,sizeof(DC_Result),"0");
	} else {
		format(DC_Result,sizeof(DC_Result),"%06x",value);
	}
	return DC_Result;
}

DataConvert::Function:: BinToHexEx(string[]){
	new DC_Result[HEX_NUMBER_LEN], value = 0;
	if(sscanf(string,"b",value)){
		format(DC_Result,sizeof(DC_Result),"0");
	} else {
		format(DC_Result,sizeof(DC_Result),"%x",value);
	}
	return DC_Result;
}

DataConvert::Function:: IntToBin(value){
	new DC_Result[BIN_NUMBER_LEN];
	format(DC_Result,sizeof(DC_Result),"%b",value);
	return DC_Result;
}

DataConvert::Function:: bool:StringToBool(string[]){
	return (string[0] != '0');
}

DataConvert::Function:: HexToInt(string[]){
	new value = 0;
	if(sscanf(string,"x",value)) return 0;
	return value;
}

DataConvert::Function:: IntToHex(value){
	new DC_Result[HEX_NUMBER_LEN];
	format(DC_Result,sizeof(DC_Result),"%06x",value);
	return DC_Result;
}

DataConvert::Function:: IntToHexEx(value){
	new DC_Result[HEX_NUMBER_LEN];
	format(DC_Result,sizeof(DC_Result),"%x",value);
	return DC_Result;
}

DataConvert::Function:: OctToInt(string[]){
	new value = 0;
	if(sscanf(string,"o",value)) return 0;
	return value;
}

DataConvert::Function:: OctToHex(string[]){
	new DC_Result[HEX_NUMBER_LEN], value = 0;
	if(sscanf(string,"o",value)){
		format(DC_Result,sizeof(DC_Result),"0");
	} else {
		format(DC_Result,sizeof(DC_Result),"%x",value);
	}
	return DC_Result;
}

DataConvert::Function:: OctToBin(string[]){
	new DC_Result[BIN_NUMBER_LEN], value = 0;
	if(sscanf(string,"o",value)){
		format(DC_Result,sizeof(DC_Result),"0");
	} else {
		format(DC_Result,sizeof(DC_Result),"%b",value);
	}
	return DC_Result;
}

DataConvert::Function:: rot13(string[]){
	for(new i = 0, j = strlen(string); i < j; i++){
		if('a' <= string[i] <= 'z'){
			string[i] = (string[i] - 'a' + 13) % 26 + 'a';
		} else if('A' <= string[i] <= 'Z'){
			string[i] = (string[i] - 'A' + 13) % 26 + 'A';
		}
	}
}

DataConvert::Function:: code128(input[],output[]){
	for(new i = 0, j = strlen(input); i < j; i++){
		output[i] = (0xFF & input[i]+128);
	}
	output[strlen(input)] = EOS;
}

DataConvert::Function:: bool:ContainInSeries(value,...){
	for(new i = 1, j = numargs(); i < j; i++){
		if(getarg(i) == value) return true;
	}
	return false;
}

DataConvert::Function:: bool:ContainInSeriesFloat(Float:value,Float:...){
	for(new i = 1, j = numargs(); i < j; i++){
		if((Float:getarg(i)) == value) return true;
	}
	return false;
}

DataConvert::Function:: IntToOct(value){
	new DC_Result[OCT_NUMBER_LEN];
	IntToAny(value,DC_Result,8);
	return DC_Result;
}

DataConvert::Function:: HexToOct(string[]){
	new DC_Result[OCT_NUMBER_LEN];
	IntToAny(HexToInt(string),DC_Result,8);
	return DC_Result;
}

DataConvert::Function:: BinToOct(string[]){
	new DC_Result[OCT_NUMBER_LEN];
	IntToAny(BinToInt(string),DC_Result,8);
	return DC_Result;
}

DataConvert::Function:: strlower(string[]){
	for(new i = 0, j = strlen(string); i < j; i++){
		string[i] = tolower(string[i]);
	}
}

DataConvert::Function:: strupper(string[]){
	for(new i = 0, j = strlen(string); i < j; i++){
		string[i] = toupper(string[i]);
	}
}

DataConvert::Function:: StripNewLine(string[]){
	new len = strlen(string);
	if(string[0] == 0) return 0;
	if(string[(len-1)]=='\n' || string[(len-1)]=='\r'){
		string[(len-1)] = 0;
		if(string[0] == 0) return 0;
		if(string[(len-2)]=='\n' || string[(len-2)]=='\r'){
			string[(len-2)] = 0;
		}
	}
	return 1;
}

DataConvert::Function:: fcopy(input[],output[]){
	if(!fexist(input)) return 0;
	if(fexist(output)) return 0;
	new File:inpf = fopen(input,io_read),
		File:outf = fopen(output,io_write);
	if(!inpf || !outf) return 0;
	for(new i = flength(inpf); i > 0; i--){
		fputchar(outf,fgetchar(inpf,0,false),false);
	}
	fclose(inpf);
	fclose(outf);
	return 1;
}

#if !defined _YSF_included
	DataConvert::Function:: frename(oldname[],newname[]){
		if(!fexist(oldname)) return 0;
		if(fexist(newname)) return 0;
		if(!fcopy(oldname,newname))	return 0;
		fremove(oldname);
		return 1;
	}
#endif

DataConvert::Function:: strreplace(string[],const search[],const replacement[],bool:ignorecase = false,pos = 0,limit = -1,maxlength = sizeof(string)){
	if(limit == 0) return 0;

	new sublen = strlen(search),
		replen = strlen(replacement),
		bool:packed = ispacked(string),
		maxlen = maxlength,
		len = strlen(string),
		count = 0;
    
	if(packed) maxlen *= 4;
    if(!sublen) return 0;
    
	while(-1 != (pos = strfind(string, search, ignorecase, pos))){
		strdel(string, pos, pos + sublen);
		len -= sublen;
		
		if(replen && len + replen < maxlen){
			strins(string, replacement, pos, maxlength);

			pos += replen;
			len += replen;
		}

		// Is there a limit of number of replacements, if so, did we break it?
		if(limit != -1 && ++count >= limit)	break;
	}

	return count;
}

//base64 by Shiny
DataConvert::Function:: base64_decode(input[],output[]){
	new SpaceReplacer = '=', len = strlen(input), Position;
	
	for(new i = 0; i < len; i++){
		new char_[2];
		char_[0] = DataConvert::Decoder[input[i++]];
		char_[1] = DataConvert::Decoder[input[i]];
		char_[0] = (char_[0] << 2) | ((char_[1] >> 4) & 3);
		Position += format(output[Position], len, "%c", char_[0]);
		
		if(++i < len){
			char_[0] = input[i];
			if(char_[0] == SpaceReplacer) break;
			char_[0] = DataConvert::Decoder[input[i]];
			char_[1] = ((char_[1] << 4) & 240) | ((char_[0] >> 2) & 15);
			Position += format(output[Position], len, "%c", char_[1]);
		}
		
		if(++i < len){
			char_[1] = input[i];
			if(char_[1] == SpaceReplacer) break;
			char_[1] = DataConvert::Decoder[input[i]];
			char_[0] = ((char_[0] << 6) & 192) | char_[1];
			Position += format(output[Position], len, "%c", char_[0]);
		}
	}
}

DataConvert::Function:: base64_encode(input[],output[]){
	new SpaceReplacer = '=', len = strlen(input), Position;
	
	for(new i = 0 ; i < len ; i++){
		new Character;
		Character = (input[i] >> 2) & 63;
		Position += format(output[Position], len, "%c", DataConvert::LettersTable[Character]);
		Character = (input[i] << 4) & 63;
		
		if(++i < len){
			Character |= (input[i] >> 4) & 15;
		}
		Position += format(output[Position], len, "%c", DataConvert::LettersTable[Character]);

		if(i < len){
			Character = (input[i] << 2) & 63;
			
			if(++i < len){
				Character |= (input[i] >> 6) & 3;
			}
			Position += format(output[Position], len, "%c", DataConvert::LettersTable[Character]);
		} else {
			i++;
			Position += format(output[Position], len, "%c", SpaceReplacer);
		}

		if(i < len){
			Character = input[i] & 63;
			Position += format(output[Position], len, "%c", DataConvert::LettersTable[Character]);
		} else {
			Position += format(output[Position], len, "%c", SpaceReplacer);
		}
	}
}

#define e:base64 					base64_encode
#define d:base64 					base64_decode

DataConvert::Public:: DataConvert::OnRequestResponse(index, response_code, data[]){
	if(response_code == 200){
		if(isnull(data)) return 0;
		new DataConvert::version[16], DataConvert::ver, DataConvert::file_ver = Data_Convert_Version;
		strmid(DataConvert::version,data,0,strlen(data));
		DataConvert::ver = strval(DataConvert::version);
		if(DataConvert::ver > DataConvert::file_ver){
			print(" ");
			printf("[ADM] Info: Please update your DataConvert include to v%d.%d.%d from here:",(DataConvert::ver / 10000),((DataConvert::ver % 10000) / 100),((DataConvert::ver % 10000) % 100));
			print("http://8.ct8.pl/d/DataConvert.inc");
			print(" ");
		}
	}
	return 1;
}

DataConvert::Function:: DataConvert::Init(){
	SetSVarInt("ADM:DataConvert:ACTIVE",DataConvert::GetActiveCount()+1);
	
	//Welcome to DataConvert
	new DataConvert::ver_name[16];
	format(DataConvert::ver_name,sizeof(DataConvert::ver_name),"%d.%d.%d",(Data_Convert_Version / 10000),((Data_Convert_Version % 10000) / 100),((Data_Convert_Version % 10000) % 100));
	if(GetSVarInt("ADM:DataConvert:INIT") == 0){
		SetSVarInt("ADM:DataConvert:INIT",1);
		printf("[ADM] Info: Load Data Convert v%s for SA:MP %s by Abyss Morgan",DataConvert::ver_name,DataConvert::SAMP_VERSION);
	}
	
	if(GetSVarInt("ADM:DataConvert:VERCHECK") == 0){
		SetSVarInt("ADM:DataConvert:VERCHECK",1);
		new DataConvert::send_data[50];
		format(DataConvert::send_data,sizeof(DataConvert::send_data),"8.ct8.pl/dc/check.php?version=%d",Data_Convert_Version);
		HTTP(14,HTTP_GET,DataConvert::send_data,"","DataConvert_OnRequestResponse");
	}
}

DataConvert::Function:: DataConvert::Exit(){
	SetSVarInt("ADM:DataConvert:ACTIVE",DataConvert::GetActiveCount()-1);
	
	//Goodbye DataConvert
	new DataConvert::ver_name[16];
	format(DataConvert::ver_name,sizeof(DataConvert::ver_name),"%d.%d.%d",(Data_Convert_Version / 10000),((Data_Convert_Version % 10000) / 100),((Data_Convert_Version % 10000) % 100));
	if(DataConvert::GetActiveCount() == 0){
		SetSVarInt("ADM:DataConvert:INIT",0);
		printf("[ADM] Info: Unload Data Convert v%s for SA:MP %s by Abyss Morgan",DataConvert::ver_name,DataConvert::SAMP_VERSION);
	}
}

new bool:CRC_DataConvertInit = true,
	bool:CRC_DataConvertExit = true;
	
//Hook: OnFilterScriptInit
public OnFilterScriptInit(){
	if(CRC_DataConvertInit){
		CRC_DataConvertInit = false;
		DataConvert::Init();
	}
	#if defined DataConvert_OnFilterScriptInit
		DataConvert::OnFilterScriptInit();
	#endif
	return 1;
}

#if defined _ALS_OnFilterScriptInit
	#undef OnFilterScriptInit
#else
	#define _ALS_OnFilterScriptInit
#endif
#define OnFilterScriptInit DataConvert_OnFilterScriptInit
#if defined DataConvert_OnFilterScriptInit
	forward DataConvert_OnFilterScriptInit();
#endif

//Hook: OnGameModeInit
public OnGameModeInit(){
	if(CRC_DataConvertInit){
		CRC_DataConvertInit = false;
		DataConvert::Init();
	}
	#if defined DataConvert_OnGameModeInit
		DataConvert::OnGameModeInit();
	#endif
	return 1;
}

#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif
#define OnGameModeInit DataConvert_OnGameModeInit
#if defined DataConvert_OnGameModeInit
	forward DataConvert_OnGameModeInit();
#endif

//Hook: OnFilterScriptExit
public OnFilterScriptExit(){
	if(CRC_DataConvertExit){
		CRC_DataConvertExit = false;
		DataConvert::Exit();
	}
	#if defined DataConvert_OnFilterScriptExit
		DataConvert::OnFilterScriptExit();
	#endif
	return 1;
}

#if defined _ALS_OnFilterScriptExit
	#undef OnFilterScriptExit
#else
	#define _ALS_OnFilterScriptExit
#endif
#define OnFilterScriptExit DataConvert_OnFilterScriptExit
#if defined DataConvert_OnFilterScriptExit
	forward DataConvert_OnFilterScriptExit();
#endif

//Hook: OnGameModeExit
public OnGameModeExit(){
	if(CRC_DataConvertExit){
		CRC_DataConvertExit = false;
		DataConvert::Exit();
	}
	#if defined DataConvert_OnGameModeExit
		DataConvert::OnGameModeExit();
	#endif
	return 1;
}

#if defined _ALS_OnGameModeExit
	#undef OnGameModeExit
#else
	#define _ALS_OnGameModeExit
#endif
#define OnGameModeExit DataConvert_OnGameModeExit
#if defined DataConvert_OnGameModeExit
	forward DataConvert_OnGameModeExit();
#endif

//EOF