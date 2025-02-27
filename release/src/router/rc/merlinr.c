/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 * Copyright 2019-2021, paldier <paldier@hotmail.com>.
 * Copyright 2019-2021, lostlonger<lostlonger.g@gmail.com>.
 * All Rights Reserved.
 * 
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include "rc.h"
#include <shared.h>
#include <shutils.h>
#if defined(RTCONFIG_LANTIQ)
#include <lantiq.h>
#elif defined(RTCONFIG_QCA)
#include <qca.h>
#endif
#include "merlinr.h"
#include <curl/curl.h>


void merlinr_insmod(){
	eval("insmod", "nfnetlink");
	eval("insmod", "ip_set");
	eval("insmod", "ip_set_bitmap_ip");
	eval("insmod", "ip_set_bitmap_ipmac");
	eval("insmod", "ip_set_bitmap_port");
	eval("insmod", "ip_set_hash_ip");
	eval("insmod", "ip_set_hash_ipport");
	eval("insmod", "ip_set_hash_ipportip");
	eval("insmod", "ip_set_hash_ipportnet");
	eval("insmod", "ip_set_hash_ipmac");
	eval("insmod", "ip_set_hash_ipmark");
	eval("insmod", "ip_set_hash_net");
	eval("insmod", "ip_set_hash_netport");
	eval("insmod", "ip_set_hash_netiface");
	eval("insmod", "ip_set_hash_netnet");
	eval("insmod", "ip_set_hash_netportnet");
	eval("insmod", "ip_set_hash_mac");
	eval("insmod", "ip_set_list_set");
	eval("insmod", "nf_tproxy_core");
	eval("insmod", "xt_TPROXY");
	eval("insmod", "xt_set");
}
#if defined(TUFAX3000) || defined(RTAX58U)
void enable_4t4r_ax58()
{
//ensure that the hardware support 4t4r
	if(!strcmp(nvram_get("1:sw_rxchain_mask"), "0xf") ){
//4t4r
		nvram_set("1:sw_txchain_mask", "0xf");
		nvram_set("wl1_txchain", "15");
		nvram_commit();
	} else {
//2t2r
		nvram_set("1:sw_txchain_mask", "0x9");
		nvram_set("wl1_txchain", "9");
		nvram_commit();
	}
}
void enable_4t4r()
{
//unlock 4t4r for all regions, not just china
//ensure that the hardware support 4t4r
	if(!strcmp(cfe_nvram_get("1:sw_rxchain_mask"), "0xf") && strcmp(cfe_nvram_get("1:sw_txchain_mask"), "0xf")){
		if ( !pids("envrams") )
		{
		  system("/usr/sbin/envrams &> /dev/null");
		  usleep(100000);
		}
		ATE_BRCM_SET("1:sw_txchain_mask", "0xf");
		ATE_BRCM_COMMIT();
	}
}
#endif
void merlinr_init()
{
	_dprintf("############################ MerlinR init #################################\n");
#if defined(RTCONFIG_SOFTCENTER)
	nvram_set("sc_wan_sig", "0");
	nvram_set("sc_nat_sig", "0");
	nvram_set("sc_mount_sig", "0");
	nvram_set("sc_unmount_sig", "0");
	nvram_set("sc_services_sig", "0");	
#endif
#if defined(TUFAX3000)
	if(!nvram_get("wl1_bw_160"))
		nvram_set("wl1_bw_160", "0");
#endif
	merlinr_insmod();
}
void merlinr_init_done()
{
	_dprintf("############################ MerlinR init done #################################\n");
#ifdef RTCONFIG_SOFTCENTER
	if (!f_exists("/jffs/softcenter/scripts/ks_tar_install.sh") && nvram_match("sc_mount","0")){
		doSystem("/usr/sbin/jffsinit.sh &");
		logmessage("Softcenter/软件中心", "Installing/开始安装......");
		logmessage("Softcenter/软件中心", "Wait a minute/1分钟后完成安装");
		_dprintf("....softcenter ok....\n");
	} else if (f_exists("/jffs/softcenter/scripts/ks_tar_install.sh") && nvram_match("sc_mount","0"))
		nvram_set("sc_installed","1");
	//else if (!f_exists("/jffs/softcenter/scripts/ks_tar_intall.sh") && nvram_match("sc_mount","1"))
		//nvram_set("sc_installed","0");
	if(f_exists("/jffs/.asusrouter")){
		unlink("/jffs/.asusrouter");
		doSystem("sed -i '/softcenter-wan.sh/d' /jffs/scripts/wan-start");
		doSystem("sed -i '/softcenter-net.sh/d' /jffs/scripts/nat-start");
		doSystem("sed -i '/softcenter-mount.sh/d' /jffs/scripts/post-mount");

	}
	doSystem("dbus set softcenter_firmware_version=`nvram get extendno|cut -d \"_\" -f2|cut -d \"-\" -f1|cut -c2-6`");
#endif
#if defined(RTCONFIG_QCA)
	if(!nvram_get("bl_ver"))
		nvram_set("bl_ver", "1.0.0.0");
#elif defined(RTCONFIG_RALINK)
	if(!nvram_get("bl_ver"))
		nvram_set("bl_ver", nvram_get("blver"));
#elif defined(RTCONFIG_LANTIQ)
#if !defined(K3C)
	if(!nvram_get("bl_ver"))
		doSystem("nvram set bl_ver=`uboot_env --get --name bl_ver`");
#endif
#endif
	if(!nvram_get("modelname"))
#if defined(K3)
		nvram_set("modelname", "K3");
#elif defined(K3C)
		nvram_set("modelname", "K3C");
#elif defined(SBRAC1900P)
		nvram_set("modelname", "SBRAC1900P");
#elif defined(SBRAC3200P)
		nvram_set("modelname", "SBRAC3200P");
#elif defined(EA6700)
		nvram_set("modelname", "EA6700");
#elif defined(R8000P) || defined(R7900P)
		nvram_set("modelname", "R8000P");
#elif defined(RAX20)
		nvram_set("modelname", "RAX20");
#elif defined(RAX80)
		nvram_set("modelname", "RAX80");
#elif defined(RAX200)
		nvram_set("modelname", "RAX200");
#elif defined(BLUECAVE)
		nvram_set("modelname", "BLUECAVE");
#elif defined(RTAC68U)
		nvram_set("modelname", "RTAC68U");
#elif defined(RTAC3200)
		nvram_set("modelname", "RTAC3200");
#elif defined(RTAC3100)
		nvram_set("modelname", "RTAC3100");
#elif defined(RTAC88U)
		nvram_set("modelname", "RTAC88U");
#elif defined(RTAC5300)
		nvram_set("modelname", "RTAC5300");
#elif defined(GTAC2900)
		nvram_set("modelname", "GTAC2900");
#elif defined(GTAC5300)
		nvram_set("modelname", "GTAC5300");
#elif defined(RTAC86U)
		nvram_set("modelname", "RTAC86U");
#elif defined(RTACRH17)
		nvram_set("modelname", "RTACRH17");
#elif defined(RTAX55)
		nvram_set("modelname", "RTAX55");//ax55 and ax56 v2
#elif defined(RTAX56U)
		nvram_set("modelname", "RTAX56U");
#elif defined(TUFAX3000)
		nvram_set("modelname", "TUFAX3000");
#elif defined(RTAX58U)
		nvram_set("modelname", "RTAX58U");
#elif defined(RTAX68U)
		nvram_set("modelname", "RTAX68U");
#elif defined(RTAX82U)
		nvram_set("modelname", "RTAX82U")
#elif defined(RTAX86U)
		nvram_set("modelname", "RTAX86U")
#elif defined(RTAX88U)
		nvram_set("modelname", "RTAX88U");
#elif defined(RTAX89U)
		nvram_set("modelname", "RTAX89X");
#elif defined(GTAX11000)
		nvram_set("modelname", "GTAX11000");
#elif defined(TUFAC1750)
		nvram_set("modelname", "TUFAC1750");
#elif defined(RTAC85P)
		nvram_set("modelname", "RTAC85P");
#elif defined(RMAC2100)
		nvram_set("modelname", "RMAC2100");
#endif
#if defined(R8000P) || defined(R7900P)
	nvram_set("ping_target","www.taobao.com");
	nvram_commit();
#endif
#if defined(TUFAX3000) || defined(RTAX58U)
	enable_4t4r();
#elif defined(MERLINR_VER_MAJOR_X) && defined(RTAC86U)
	merlinr_patch_nvram();
#elif defined(GTAC2900) && defined(MERLINR_VER_MAJOR_X)
//ac86u <--> gtac2900
	patch_ac86();
	//patch_gt2900();
#endif
}


#define FWUPDATE_DBG(fmt,args...) \
        if(1) { \
                char info[1024]; \
                snprintf(info, sizeof(info), "echo \"[FWUPDATE][%s:(%d)]"fmt"\" >> /tmp/webs_upgrade.log", __FUNCTION__, __LINE__, ##args); \
                system(info); \
        }

int str_split(char* buf, char** s, int s_size) {
	int curr = 0;
	char* token = strtok(buf, ".");
	while(token && curr < s_size) {
		s[curr++] = token;
		token = strtok(NULL, ".");
	}
	return curr;
}

int versioncmp(char *cur_fwver, char *fwver) {
	char buf1[20] = {0}, buf2[20] = {0};
	char* s1[20] = {0};
	char* s2[20] = {0};
	char* end;
	int i, n1, n2, s1_len, s2_len, s_max, rlt;
	strncpy(buf1, cur_fwver, strlen(cur_fwver));
	strncpy(buf2, fwver, strlen(fwver));
	//fprintf(stderr, "%s\n", buf1);
	//fprintf(stderr, "%s\n", buf2);

	s1_len = str_split(buf1, s1, 10);
	s2_len = str_split(buf2, s2, 10);

	//fprintf(stderr, "s1=%d, s2=%d\n", s1_len, s2_len);

	s_max = (s1_len > s2_len ? s1_len: s2_len);

	rlt = 0;
	for(i = 0; i < s_max; i++) {
		if(!s1[i]) {
			s1[i] = "0";
		}
		if(!s2[i]) {
			s2[i] = "0";
		}

		n1 = strtol(s1[i], &end, 10);
		if(*end) {
			//parse error, but not break hear
			rlt = 1;
		}

		n2 = strtol(s2[i], &end, 10);
		if(*end) {
			if (1 == rlt) {
				//both error
				rlt = 0;
				break;
			}
			rlt = -1;
			break;
		}

		if(1 == rlt) {
			break;
		}

		if (n1 == n2) {
			continue;
		} else if(n1 < n2) {
			rlt = 1;
			break;
		} else {
			rlt = -1;
			break;
		}
	}
	return rlt;
}

size_t getcontentlengthfunc(void *ptr, size_t size, size_t nmemb, void *stream) {
	int r;
	long len = 0;

	/* _snscanf() is Win32 specific */
	// r = _snscanf(ptr, size * nmemb, "Content-Length: %ld\n", &len);
	r = sscanf(ptr, "Content-Length: %ld\n", &len);
	if (r) /* Microsoft: we don't read the specs */
		*((long *) stream) = len;

	return size * nmemb;
}

size_t wirtefunc(void *ptr, size_t size, size_t nmemb, void *stream)
{
	return fwrite(ptr, size, nmemb, stream);
}
#if 0
size_t readfunc(void *ptr, size_t size, size_t nmemb, void *stream)
{
	FILE *f = stream;
	size_t n;

	if (ferror(f))
		return CURL_READFUNC_ABORT;

	n = fread(ptr, size, nmemb, f) * size;

	return n;
}
#endif
int curl_download_file(CURL *curlhandle, const char * remotepath, const char * localpath, long timeout, long tries)
{
	FILE *f;
	curl_off_t local_file_len = -1 ;
	long filesize =0 ;

	CURLcode r = CURLE_GOT_NOTHING;
	//int c;
	struct stat file_info;
	int use_resume = 0;
	//if(access(localpath,F_OK) ==0)

	if(stat(localpath, &file_info) == 0) 
	{
		local_file_len =  file_info.st_size;
		use_resume  = 1;
	}
	f = fopen(localpath, "ab+"); 
	if (f == NULL) {
		perror(NULL);
		return 0;
	}

	//curl_easy_setopt(curlhandle, CURLOPT_UPLOAD, 1L);

	curl_easy_setopt(curlhandle, CURLOPT_URL, remotepath);
	curl_easy_setopt(curlhandle, CURLOPT_CONNECTTIMEOUT, timeout);
	curl_easy_setopt(curlhandle, CURLOPT_HEADERFUNCTION, getcontentlengthfunc);
	curl_easy_setopt(curlhandle, CURLOPT_HEADERDATA, &filesize);
	curl_easy_setopt(curlhandle, CURLOPT_RESUME_FROM_LARGE, use_resume?local_file_len:0);
	curl_easy_setopt(curlhandle, CURLOPT_SSL_VERIFYPEER, 0);
	curl_easy_setopt(curlhandle, CURLOPT_SSL_VERIFYHOST, 0);
	curl_easy_setopt(curlhandle, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1); 
	curl_easy_setopt(curlhandle, CURLOPT_WRITEDATA, f);
	curl_easy_setopt(curlhandle, CURLOPT_WRITEFUNCTION, wirtefunc);

	//curl_easy_setopt(curlhandle, CURLOPT_READFUNCTION, readfunc);
	//curl_easy_setopt(curlhandle, CURLOPT_READDATA, f);
	curl_easy_setopt(curlhandle, CURLOPT_NOPROGRESS, 1);
	curl_easy_setopt(curlhandle, CURLOPT_VERBOSE, 1);

	r = curl_easy_perform(curlhandle);
	fclose(f);

	if (r == CURLE_OK)
		return 1;
	else {
		fprintf(stderr, "%s\n", curl_easy_strerror(r));
		return 0;
	}
}

int merlinr_firmware_check_update_main(int argc, char *argv[])
{
	//char notetxt[]="/tmp/release_note.txt";
	//char downloadphp[]="download.php";
	//char sqdownloadphp[]="SQ_download.php";
	//char wlanutdatetxt[]="/tmp/wlan_utdate.txt";
	//char wlan_update_mrflag1[]="wlan_update_mrflag1.zip";
	FILE *fpupdate;
	int download;
	char url[100];
	char log[200];
	char serverurl[]="https://update.paldier.com";
	char serverupdate[]="wlan_update_v2.zip";
	char localupdate[]="/tmp/wlan_update.txt";
	char releasenote[]="/tmp/release_note0.txt";
	char model[20], modelname[20], fsver[10], fwver[10], tag[10];
	char cur_fwver[10];
	char *tmp_fwver=nvram_get("extendno");
	char info[100];
	nvram_set("webs_state_update", "0");
	nvram_set("webs_state_flag", "0");
	nvram_set("webs_state_error", "0");
	nvram_set("webs_state_odm", "0");
	nvram_set("webs_state_url", "");
#ifdef RTCONFIG_AMAS
	nvram_set("cfg_check", "0");
	nvram_set("cfg_upgrade", "0");
#endif
	unlink("/tmp/webs_upgrade.log");
	unlink("/tmp/wlan_update.txt");
	unlink("/tmp/release_note0.txt");
	sscanf(tmp_fwver, "%*[A-Z0-9]_%[A-Z0-9.]-%*[a-z0-9]", cur_fwver);
	CURL *curlhandle = NULL;

	curl_global_init(CURL_GLOBAL_ALL);
	curlhandle = curl_easy_init();
	snprintf(url, sizeof(url), "%s/%s", serverurl, serverupdate);
	//snprintf(log, sizeof(log), "echo \"[FWUPDATE]---- update dl_path_info for general %s/%s ----\" >> /tmp/webs_upgrade.log", serverurl, serverupdate);
	FWUPDATE_DBG("---- update dl_path_info for general %s/%s ----", serverurl, serverupdate);
	download=curl_download_file(curlhandle , url,localupdate,8,3);
	//system(log);
	//_dprintf("%d\n",download);
	if(download)
	{
		fpupdate = fopen(localupdate, "r");
		if (!fpupdate)
			goto GODONE;
		//BLUECAVE#K3C#3004384#R7.0#g13e704e
		char buffer[1024];
		while(NULL!=fgets(buffer,sizeof(buffer),fpupdate)){
			sscanf(buffer,"%[A-Z0-9-]#%[A-Z0-9]#%[0-9]#%[A-Z0-9.]#%[a-z0-9]",model,modelname,fsver,fwver,tag);
			_dprintf("%s#%s#%s#%s\n",model,modelname,fsver,fwver);
			if(!strcmp(model, nvram_get("productid")) && !strcmp(modelname, nvram_safe_get("modelname"))){
				if((strstr(cur_fwver, "B") && strstr(fwver, "B"))||(strstr(cur_fwver, "R") && strstr(fwver, "R"))||(strstr(cur_fwver, "X") && strstr(fwver, "X"))){
					//_dprintf("%s#%s\n",fwver,cur_fwver);
					if(versioncmp((cur_fwver+1),(fwver+1))==1){
						nvram_set("webs_state_url", "");
#if (defined(RTAC82U) && !defined(RTCONFIG_AMAS)) || defined(RTAC3200) || defined(RTAC85P) || defined(RMAC2100)
						snprintf(info,sizeof(info),"3004_382_%s_%s-%s",modelname,fwver,tag);
#elif defined(BLUECAVE)
						snprintf(info,sizeof(info),"3004_384_%s_%s-%s",modelname,fwver,tag);
#else
						snprintf(info,sizeof(info),"3004_386_%s_%s-%s",modelname,fwver,tag);
#endif
						FWUPDATE_DBG("---- current version : %s ----", nvram_get("extendno"));
						FWUPDATE_DBG("---- productid : %s_%s-%s ----", modelname, fwver, tag);
						nvram_set("webs_state_info", info);
						nvram_set("webs_state_REQinfo", info);
						nvram_set("webs_state_flag", "1");
						nvram_set("webs_state_update", "1");
#ifdef RTCONFIG_AMAS
//						nvram_set("cfg_check", "9");
//						nvram_set("cfg_upgrade", "0");
#endif
						memset(url,'\0',sizeof(url));
						memset(log,'\0',sizeof(log));
						char releasenote_file[100];
						snprintf(releasenote_file, sizeof(releasenote_file), "%s_%s_%s_note.zip", nvram_get("productid"), nvram_get("webs_state_info"),nvram_get("preferred_lang"));
						snprintf(url, sizeof(url), "%s/%s", serverurl, releasenote_file);
						//snprintf(log, sizeof(log), "echo \"[FWUPDATE]---- download real release note %s/%s ----\" >> /tmp/webs_upgrade.log", serverurl, releasenote_file);
						//system(log);
						FWUPDATE_DBG("---- download real release note %s/%s ----", serverurl, releasenote_file);
						download=curl_download_file(curlhandle , url,releasenote,8,3);
						if(download ==0 ){
							memset(url,'\0',sizeof(url));
							snprintf(releasenote_file, sizeof(releasenote_file), "%s_%s_US_note.zip", nvram_get("productid"), nvram_get("webs_state_info"));
							snprintf(url, sizeof(url), "%s/%s", serverurl, releasenote_file);
							//snprintf(log, sizeof(log), "echo \"[FWUPDATE]---- download real release note %s/%s ----\" >> /tmp/webs_upgrade.log", serverurl, releasenote_file);
							//system(log);
							FWUPDATE_DBG("---- download real release note %s/%s ----", serverurl, releasenote_file);
							curl_download_file(curlhandle , url,releasenote,8,3);
						}
						FWUPDATE_DBG("---- firmware check update finish ----");
						return 0;
#if 0
						if(strstr(nt_center,"nt_center")){
							if((!nvram_get("webs_last_info"))||(strcmp(nvram_get("webs_last_info"), nvram_get("webs_state_info")))){
								system("Notify_Event2NC \"$SYS_FW_NWE_VERSION_AVAILABLE_EVENT\" \"{\\\"fw_ver\\\":\\\"$update_webs_state_info\\\"}\"");
								nvram_set("webs_last_info", nvram_get("webs_state_info"));
							}
						}
#endif
					}
				}
			}
		}
	}

	curl_easy_cleanup(curlhandle);
	curl_global_cleanup();

GODONE:
#if (defined(RTAC82U) && !defined(RTCONFIG_AMAS)) || defined(RTAC3200) || defined(RTAC85P) || defined(RMAC2100)
	snprintf(info,sizeof(info),"3004_382_%s",nvram_get("extendno"));
#elif defined(BLUECAVE)
	snprintf(info,sizeof(info),"3004_384_%s",nvram_get("extendno"));
#else
	snprintf(info,sizeof(info),"3004_386_%s",nvram_get("extendno"));
#endif
	nvram_set("webs_state_url", "");
	nvram_set("webs_state_flag", "0");
	nvram_set("webs_state_error", "1");
	nvram_set("webs_state_odm", "0");
	nvram_set("webs_state_update", "1");
	nvram_set("webs_state_info", info);
	nvram_set("webs_state_REQinfo", info);
	nvram_set("webs_state_upgrade", "");
#ifdef RTCONFIG_AMAS
	nvram_set("cfg_check", "9");
	nvram_set("cfg_upgrade", "0");
#endif
	FWUPDATE_DBG("---- firmware check update finish ----");
	return 0;
}
#if !defined(BLUECAVE)
void exec_uu_merlinr()
{
	FILE *fp;
	char buf[128];
	int download,i;
	char *dup_pattern, *g, *gg;
	char p[2][100];
	if(nvram_get_int("sw_mode") == 1){
		add_rc_support("uu_accel");
		mkdir("/tmp/uu", 0755);
		download = system("wget -t 2 -T 30 --dns-timeout=120 --header=Accept:text/plain -q --no-check-certificate 'https://router.uu.163.com/api/script/monitor?type=asuswrt-merlin' -O /tmp/uu/script_url");
		if (!download){
			_dprintf("download uuplugin script info successfully\n");
			if ((fp = fopen("/tmp/uu/script_url", "r"))!=NULL){
				fgets(buf, 128, fp);
				fclose(fp);
				unlink("/tmp/uu/script_url");
				i=0;
				g = dup_pattern = strdup(buf);
				gg = strtok( g, "," );
				while (gg != NULL)
				{
					if (gg!=NULL){
						strcpy(p[i], gg);
						i++;
						++download;
						gg = strtok( NULL, "," );
					}
				}
				if ( download > 0 )
				//if ( download == 2 )
				{
					_dprintf("URL: %s\n",p[0]);
					_dprintf("MD5: %s\n",p[1]);
					if ( !doSystem("wget -t 2 -T 30 --dns-timeout=120 --header=Accept:text/plain -q --no-check-certificate %s -O /tmp/uu/uuplugin_monitor.sh", p[0]))
					{
						_dprintf("download uuplugin script successfully\n");
						if ((fp = fopen("/tmp/uu/uuplugin_monitor.config", "w"))){
							fprintf(fp, "router=asuswrt-merlin\n");
							fprintf(fp, "model=\n");
							fclose(fp);
						}
						if((fp=popen("md5sum /tmp/uu/uuplugin_monitor.sh | sed 's/[ ][ ]*/ /g' | cut -d' ' -f1", "r")))
						{
							memset(buf,'\0',sizeof(buf));
							if((fread(buf, 1, 128, fp)))
							{
								buf[32]='\0';
								buf[33]='\0';
								if ( !strcasecmp(buf, p[1]))
								{
									pid_t pid;
									char *uu_argv[] = { "/tmp/uu/uuplugin_monitor.sh", NULL };
									_dprintf("prepare to execute uuplugin stript...\n");
									chmod("/tmp/uu/uuplugin_monitor.sh", 0755);
									_eval(uu_argv, NULL, 0, &pid);
								}
							}
							pclose(fp);
						}
					}
				}
			}
			free(dup_pattern);
		}
	}
}
#endif

#if defined(RTCONFIG_SOFTCENTER)
void softcenter_eval(int sig)
{
	//1=wan,2=nat,3=mount
	pid_t pid;
	char path[100], action[10], sc[]="/jffs/softcenter/bin";
	if(SOFTCENTER_WAN == sig){
		snprintf(path, sizeof(path), "%s/softcenter-wan.sh", sc);
		snprintf(action, sizeof(action), "start");
	} else if (SOFTCENTER_NAT == sig){
		snprintf(path, sizeof(path), "%s/softcenter-net.sh", sc);
		snprintf(action, sizeof(action), "start_nat");
	} else if (SOFTCENTER_MOUNT == sig){
		snprintf(path, sizeof(path), "%s/softcenter-mount.sh", sc);
		snprintf(action, sizeof(action), "start");
	} else if (SOFTCENTER_SERVICES == sig){
		snprintf(path, sizeof(path), "%s/softcenter-services.sh", sc);
		snprintf(action, sizeof(action), "start");
	//enable it after 1.3.0
	//} else if (SOFTCENTER_UNMOUNT == sig){
	//	snprintf(path, sizeof(path), "%s/softcenter-unmount.sh", sc);
	//	snprintf(action, sizeof(action), "unmount");
	} else {
		logmessage("Softcenter", "sig=%d, bug?",sig);
		return;
	}
	char *eval_argv[] = { path, action, NULL };
	_eval(eval_argv, NULL, 0, &pid);
}
#endif

