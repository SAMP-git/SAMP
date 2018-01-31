/****************************************************************************************************
 *                                                                                                  *
 *                                          )(   DINI   )(                                          *
 *                                                                                                  *
 * Copyright © 2018 DracoBlue (http://dracoblue.com). All rights reserved.                          *
 * Contributors: Abyss Morgan                                                                       *
 *                                                                                                  *
 * Website:  http://8.ct8.pl                                                                        *
 * Download: http://8.ct8.pl/r/download                                                             *
 *                                                                                                  *
 * Plugins: None                                                                                    *
 * Modules: None                                                                                    *
 *                                                                                                  *
 * File Version: 2.0.0                                                                              *
 *                                                                                                  *
 * Functions:                                                                                       *
 * DINI::Exist(filename[]);                                                                         *
 * DINI::Remove(filename[]);                                                                        *
 * DINI::Create(filename[]);                                                                        *
 * DINI::IsSet(filename[],key[]);                                                                   *
 * DINI::UnSet(filename[],key[]);                                                                   *
 * DINI::GetString(filename[],key[]);                                                               *
 * DINI::SetString(filename[],key[],value[]);                                                       *
 * DINI::GetInt(filename[],key[]);                                                                  *
 * DINI::SetInt(filename[],key[],value);                                                            *
 * DINI::GetFloat(filename[],key[]);                                                                *
 * DINI::SetFloat(filename[],key[],Float:value);                                                    *
 * DINI::GetBool(filename[],key[]);                                                                 *
 * DINI::SetBool(filename[],key[],bool:value);                                                      *
 * DINI::StripNewLine(string[]);                                                                    *
 * DINI::CopyTextFile(oldname[],newname[]);                                                         *
 *                                                                                                  *
 ****************************************************************************************************/

#if defined _dini_included
	#endinput
#endif
#define _dini_included

#define DINI::						dini_

#if defined MAX_STRING
	#define DINI_MAX_STRING			MAX_STRING
#else
	#define DINI_MAX_STRING			512
#endif

#define dini_Exist(%0)				fexist(%0)
#define dini_Remove(%0)				fremove(%0)

stock DINI::Create(filename[]){
	if(DINI::Exist(filename)) return 0;
	new File:fhnd;
	fhnd = fopen(filename,io_write);
	if(fhnd){
		fwrite(fhnd,"");
		fclose(fhnd);
		return 1;
	}
	return 0;
}

stock DINI::StripNewLine(string[]){
	new len = strlen(string);
	if(string[0] == 0) return 0;
	if((string[len - 1] == '\n') || (string[len - 1] == '\r')){
		string[len - 1] = 0;
		if(string[0] == 0) return 0;
		if((string[len - 2] == '\n') || (string[len - 2] == '\r')) string[len - 2] = 0;
	}
	return 1;
}

stock DINI::CopyTextFile(oldname[],newname[]){
	if(!DINI::Exist(oldname)) return 0;
	new File:inpf = fopen(oldname,io_read), File:outf;
	if(!inpf) return 0;
	outf = fopen(newname,io_write);
	if(!outf){
		fclose(inpf);
		return 0;
	}
	new tmpres[DINI_MAX_STRING];
	while(fread(inpf,tmpres)){
		DINI::StripNewLine(tmpres);
		format(tmpres,sizeof(tmpres),"%s\r\n",tmpres);
		fwrite(outf,tmpres);
	}
	fclose(inpf);
	fclose(outf);
	return 1;
}

stock DINI::GetString(filename[],key[]){
	new tmpres[DINI_MAX_STRING], key_length = strlen(key);
	if(key_length == 0 || key_length + 2 > DINI_MAX_STRING) return tmpres;
	
	new File:inpf = fopen(filename,io_read);
	if(!inpf) return tmpres;
	
	while(fread(inpf,tmpres)){
		if(tmpres[key_length] == '=' && !strcmp(tmpres,key,true,key_length)){
			DINI::StripNewLine(tmpres);
			strmid(tmpres,tmpres,key_length + 1,strlen(tmpres),DINI_MAX_STRING);
			fclose(inpf);
			return tmpres;
		}
	}
	fclose(inpf);
	return tmpres;
}


stock DINI::SetString(filename[],key[],value[]){
	new key_length = strlen(key), value_length = strlen(value);
	if(key_length == 0 || key_length + value_length + 2 > DINI_MAX_STRING) return 0;
	
	new File:inpf, File:outf, tmpres[DINI_MAX_STRING], bool:wasset = false;
	
	format(tmpres,sizeof(tmpres),"%s.part",filename);
	DINI::Remove(tmpres);
	
	inpf = fopen(filename,io_read);
	if(!inpf) return 0;
	
	outf = fopen(tmpres,io_write);
	if(!outf){
		fclose(inpf);
		return 0;
	}
	
	while(fread(inpf,tmpres)){
		if(!wasset && tmpres[key_length] == '=' && !strcmp(tmpres,key,true,key_length)){
			format(tmpres,sizeof(tmpres),"%s=%s",key,value);
			wasset = true;
		} else {
			DINI::StripNewLine(tmpres);
		}
		fwrite(outf,tmpres);
		fwrite(outf,"\r\n");
	}

	if(!wasset){
		format(tmpres,sizeof(tmpres),"%s=%s",key,value);
		fwrite(outf,tmpres);
		fwrite(outf,"\r\n");
	}

	fclose(inpf);
	fclose(outf);

	format(tmpres,sizeof(tmpres),"%s.part",filename);
	if(DINI::CopyTextFile(tmpres,filename)){
		return DINI::Remove(tmpres);
	}
	return 0;
}

stock DINI::IsSet(filename[],key[]){
	new key_length = strlen(key);
	if(key_length == 0 || key_length + 2 > DINI_MAX_STRING) return 0;
	
	new File:inpf = fopen(filename,io_read);
	if(!inpf) return 0;
	
	new tmpres[DINI_MAX_STRING];
	while(fread(inpf,tmpres)){
		if(tmpres[key_length] == '=' && !strcmp(tmpres,key,true,key_length)){
			fclose(inpf);
			return 1;
		}
	}
	fclose(inpf);
	return 0;
}

stock DINI::UnSet(filename[],key[]){
	new key_length = strlen(key);
	if(key_length == 0 || key_length + 2 > DINI_MAX_STRING) return 0;
	
	new File:inpf, File:outf, tmpres[DINI_MAX_STRING];
	
	format(tmpres,DINI_MAX_STRING,"%s.part",filename);
	DINI::Remove(tmpres);
	
	inpf = fopen(filename,io_read);
	if(!inpf) return 0;
	
	outf = fopen(tmpres,io_write);
	if(!outf){
		fclose(inpf);
		return 0;
	}
	
	while(fread(inpf,tmpres)){
		if(!(tmpres[key_length] == '=' && !strcmp(tmpres,key,true,key_length))){
			DINI::StripNewLine(tmpres);
			fwrite(outf,tmpres);
			fwrite(outf,"\r\n");
		}
	}
	
	fclose(inpf);
	fclose(outf);

	format(tmpres,DINI_MAX_STRING,"%s.part",filename);
	if(DINI::CopyTextFile(tmpres,filename)){
		return DINI::Remove(tmpres);
	}
	return 0;
}

stock DINI::GetInt(filename[],key[]){
	return strval(DINI::GetString(filename,key));
}

stock DINI::SetInt(filename[],key[],value){
	new valuestring[DINI_MAX_STRING];
	format(valuestring,DINI_MAX_STRING,"%d",value);
	return DINI::SetString(filename,key,valuestring);
}

stock Float:DINI::GetFloat(filename[],key[]){
	return floatstr(DINI::GetString(filename,key));
}

stock DINI::SetFloat(filename[],key[],Float:value){
	new valuestring[DINI_MAX_STRING];
	format(valuestring,DINI_MAX_STRING,"%f",value);
	return DINI::SetString(filename,key,valuestring);
}

stock bool:DINI::GetBool(filename[],key[]){
	return bool:strval(DINI::GetString(filename,key));
}

stock DINI::SetBool(filename[],key[],bool:value){
	if(value){
		return DINI::SetString(filename,key,"1");
	}
	return DINI::SetString(filename,key,"0");
}

//Old syntax
#define dini_Exists					DINI::Exist
#define dini_Isset					DINI::IsSet
#define dini_Unset					DINI::UnSet
#define dini_Get					DINI::GetString
#define dini_Set					DINI::SetString
#define dini_Int					DINI::GetInt
#define dini_IntSet 				DINI::SetInt
#define dini_Float 					DINI::GetFloat
#define dini_FloatSet 				DINI::SetFloat
#define dini_Bool 					DINI::GetBool
#define dini_BoolSet 				DINI::SetBool
#define dini_fcopytextfile			DINI::CopyTextFile

//EOF