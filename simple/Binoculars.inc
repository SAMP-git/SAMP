/*
#include <ADM/3DTryg>
*/

CMD:binoculars(playerid){
	new szLIST[3096], buffer[128], cnt = 0;
	Tryg3DForeach(i){
		if(playerid == i) continue;
		if(IsPlayerSpawned(i)){
			if(IsElementOnPlayerScreen(playerid,i)){
				format(buffer,sizeof(buffer),"{FFFFFF}%d. {00FF00}%s\n",i,GetPlayerNick(i));
				strcat(szLIST,buffer);
				cnt++;
			}
		}
	}
	if(cnt == 0) szLIST = "{FFFFFF}I do not see anyone nearby\n";
	ShowPlayerDialog(playerid,DIALOG_INFO,DIALOG_STYLE_LIST,"Binoculars",szLIST,"{00FF00}Exit","");
	return 1;
}