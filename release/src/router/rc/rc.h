/*
 * Router rc control script
 *
 * Copyright 2005, Broadcom Corporation
 * All Rights Reserved.
 *
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: rc.h,v 1.39 2005/03/29 02:00:06 honor Exp $
 */

#ifndef __RC_H__
#define __RC_H__

#define _GNU_SOURCE

#include <rtconfig.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h> // !!TB
#include <string.h>
#include <signal.h>
#include <syslog.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <net/if.h>
#include <signal.h>

#include <bcmnvram.h>
#include <bcmparams.h>
#include <utils.h>
#include <shutils.h>
#include <shared.h>

#include "sysdeps.h"
#include <linux/version.h>
#ifdef HND_ROUTER
#include <cms_image.h>
#include <bcm_imgif.h>
#endif

#ifdef RTCONFIG_REALTEK
#include "../../../../../realtek/rtl819x/linux-3.10/drivers/char/asus_ate.h"
#endif

#if defined(RTCONFIG_USB)
#include <usb_info.h>
#endif

#ifdef RTCONFIG_LETSENCRYPT
#include "letsencrypt_config.h"
#include "letsencrypt_control.h"
#endif

#ifdef RTCONFIG_OPENVPN
#include "openvpn_config.h"
#include "openvpn_control.h"
#endif

#ifdef RTCONFIG_PARENTALCTRL
#include "pc.h"
#endif

#ifdef RTCONFIG_INTERNETCTRL
#include "ic.h"
#endif

#define IFUP (IFF_UP | IFF_RUNNING | IFF_BROADCAST | IFF_MULTICAST)

#if LINUX_KERNEL_VERSION >= KERNEL_VERSION(3,2,0)
#define USBCOMMON_MOD	"usb-common"
#endif
#define USBCORE_MOD	"usbcore"
#if defined (RTCONFIG_USB_XHCI)
#if defined(RTCONFIG_ETRON_XHCI)
#define USB30_MOD	"etxhci-hcd"
#else
#define USB30_MOD	"xhci-hcd"
#endif
#else
#endif

#if defined(RTN56UB1) || defined(RTN56UB2) || defined(RTAC1200GA1) || defined(RTAC1200GU) //for MT7621
#define USB20_MOD	"xhci-hcd"
#elif defined(RTCONFIG_QCN550X) && LINUX_KERNEL_VERSION >= KERNEL_VERSION(4,4,0)
#define USB20_MOD	"ehci-ath79"
#else
#define USB20_MOD	"ehci-hcd"
#endif



#if defined(RTCONFIG_SOC_IPQ8064)
#if LINUX_KERNEL_VERSION >= KERNEL_VERSION(4,4,0)
#define PRE_XHCI_KMODS	"phy-qcom-dwc3 dwc3-of-simple dwc3"
#define POST_XHCI_KMODS	"xhci-pci xhci-plat-hcd"
#else
#define PRE_XHCI_KMODS	""
#define POST_XHCI_KMODS	"udc-core dwc3-ipq"
#endif
#elif defined(RTCONFIG_SOC_IPQ8074)
#if defined(RTCONFIG_GLOBAL_INI)
/* SPF10.0 */
#define PRE_XHCI_KMODS	"phy-msm-qusb phy-msm-ssusb-qmp phy-qcom-dwc3 usb-f-qdss dwc3-of-simple dwc3"
#else
/* SPF7.0 ~ SPF8.0 */
#define PRE_XHCI_KMODS	"phy-msm-qusb phy-msm-ssusb-qmp phy-qcom-dwc3 dwc3-of-simple dwc3"
#endif
#define POST_XHCI_KMODS	"xhci-pci xhci-plat-hcd"
#elif defined(RTCONFIG_SOC_IPQ40XX)
#define PRE_XHCI_KMODS	"phy-qca-baldur phy-qca-uniphy dwc3-ipq40xx dwc3"
#define POST_XHCI_KMODS	""
#else
#define PRE_XHCI_KMODS	""
#define POST_XHCI_KMODS	""
#endif

#if defined(RTCONFIG_SOC_IPQ40XX)
#define USB_DWC3	"dwc3"
#define USB_DWC3_IPQ	"dwc3-ipq40xx"
#define USB_PHY1        "phy-qca-baldur"
#define USB_PHY2        "phy-qca-uniphy"
#endif

#define USBSTORAGE_MOD	"usb-storage"
#define SCSI_MOD	"scsi_mod"
#define SD_MOD		"sd_mod"
#define SG_MOD		"sg"
#ifdef LINUX26
#define USBOHCI_MOD	"ohci-hcd"
#define USBUHCI_MOD	"uhci-hcd"
#define USBPRINTER_MOD	"usblp"
#define SCSI_WAIT_MOD	"scsi_wait_scan"
#define USBFS		"usbfs"
#else
#define USBOHCI_MOD	"usb-ohci"
#define USBUHCI_MOD	"usb-uhci"
#define USBPRINTER_MOD	"printer"
#define USBFS		"usbdevfs"
#endif

#ifdef RTCONFIG_IPV6
extern char wan6face[];
#endif


#if defined(RTCONFIG_QCA) && defined(RTCONFIG_WIFI_SON)
#ifdef RTCONFIG_DETWAN
#define CONFIGURED_WAN_NIC      nvram_safe_get("detwan_ifname")
#else
#define CONFIGURED_WAN_NIC      DEFAULT_WAN_NIC
#endif

#if defined(MAPAC1300) || defined(MAPAC2200) || defined(VZWAC1300) || defined(SHAC1300)
#define DEFAULT_WAN_NIC "eth0"
#define DEFAULT_LAN_NIC "eth1"
#define DEFAULT_WAN_PORT_SHIFT  5
#define DEFAULT_LAN_PORT_SHIFT  4
#elif defined(MAPAC1750)
#define DEFAULT_WAN_NIC "vlan2"
#define DEFAULT_LAN_NIC "vlan1"
#define DEFAULT_WAN_PORT_SHIFT  2
#define DEFAULT_LAN_PORT_SHIFT  3
#else
#error Defaine Default WAN/LAN NIC!
#endif
#endif /* QCA & WIFI_SON */

/* services.c */
extern int g_reboot;
extern int wan_phyid;
#if defined(RTCONFIG_JFFS2) || defined(RTCONFIG_JFFSV1) || defined(RTCONFIG_BRCM_NAND_JFFS2)
extern int jffs2_fail;
#endif

#if defined(RTCONFIG_AMAS)
static inline int is_cap(){
#ifdef RTCONFIG_MASTER_DET
	if (nvram_match("cfg_master", "1") && (is_router_mode() || access_point_mode()))
#else
	if (is_router_mode())
#endif
		return 1;

	return 0;
}

static inline char *node_str(){
	if(is_cap())
		return "C";

	return "R";
}
#endif

#ifdef RTCONFIG_BCMARM
#ifndef LINUX_KERNEL_VERSION
#define LINUX_KERNEL_VERSION LINUX_VERSION_CODE
#endif

static inline int before(int ver1, int ver2)
{
	return (ver1-ver2) < 0;
}

#define After(ver2, ver1)	before(ver1, ver2)
#endif

#if defined(LINUX30) || LINUX_VERSION_CODE > KERNEL_VERSION(2,6,34)
#define DAYS_PARAM	" --kerneltz --weekdays "
#else
#define DAYS_PARAM	" --days "
#endif

#define LOGNAME get_productid()
#ifdef RTCONFIG_USB_MODEM
#define is_phyconnected() (nvram_match("link_wan", "1") || nvram_match("link_wan1", "1"))
#else
#define is_phyconnected() (nvram_match("link_wan","1"))
#endif

#define NAT_RULES	"/tmp/nat_rules"
#define REDIRECT_RULES	"/tmp/redirect_rules"

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(ary) (sizeof(ary) / sizeof((ary)[0]))
#endif

#define XSTR(s) STR(s)
#define STR(s) #s

//	#define DEBUG_IPTFILE
//	#define DEBUG_RCTEST
//	#define DEBUG_NOISY

#ifdef DEBUG_NOISY
#define TRACE_PT(fmt, args...)		\
do {					\
	char psn[16];			\
	pid_t pid;			\
	pid = getpid();			\
	psname(pid, psn, sizeof(psn));	\
	_dprintf("[%d %s:%s +%ld] " fmt,\
		pid, psn, __func__, uptime(), ##args);	\
} while(0)
#else
#define TRACE_PT(args...) do { } while(0)
#endif

#ifdef RTCONFIG_RESTRICT_GUI
#define BIT_RES_GUI	"0x100"
#endif

#define PROC_SCSI_ROOT	"/proc/scsi"
#define USB_STORAGE	"usb-storage"

#ifndef MAX_WAIT_FILE
#define MAX_WAIT_FILE 5
#endif
#define PPP_DIR "/tmp/ppp/peers"
#define PPP_CONF_FOR_3G "/tmp/ppp/peers/3g"

#ifdef RTCONFIG_USB_BECEEM
#define BECEEM_DIR "/tmp/Beceem_firmware"
#define WIMAX_CONF "/tmp/wimax.conf"
#define WIMAX_LOG "/tmp/wimax.log"
#endif

#define NOTIFY_DIR "/tmp/notify"
#define NOTIFY_TYPE_USB "usb"

#define BOOT		0
#define REDIAL		1
#define CONNECTING	2

#define PPPOE0		0
#define PPPOE1		1

#define DSL_WAN_VID		3880		//0x0F28
#define DSL_WAN_VIF		"vlan3880"

#define GOT_IP			0x01
#define RELEASE_IP		0x02
#define	GET_IP_ERROR		0x03
#define RELEASE_WAN_CONTROL	0x04
#define USB_DATA_ACCESS		0x05	//For WRTSL54GS
#define USB_CONNECT		0x06	//For WRTSL54GS
#define USB_DISCONNECT		0x07	//For WRTSL54GS

/* USB attached SCSI protocol */
#if LINUX_KERNEL_VERSION >= KERNEL_VERSION(3,15,0)
#define MODPROBE__UAS		modprobe("uas")
#define MODPROBE_R__UAS		modprobe_r("uas")
#else
#define MODPROBE__UAS		do {} while(0)
#define MODPROBE_R__UAS		do {} while(0)
#endif

#define SERIAL_NUMBER_LENGTH	12	//ATE need

#define SET_LED(val)	do { } while(0)

typedef enum { IPT_TABLE_NAT, IPT_TABLE_FILTER, IPT_TABLE_MANGLE } ipt_table_t;

#define IFUP (IFF_UP | IFF_RUNNING | IFF_BROADCAST | IFF_MULTICAST)

#define	IPT_V4	0x01
#define	IPT_V6	0x02
#define	IPT_ANY_AF	(IPT_V4 | IPT_V6)
#define	IPT_AF_IS_EMPTY(f)	((f & IPT_ANY_AF) == 0)

extern struct nvram_tuple router_defaults[];
extern struct nvram_tuple router_state_defaults[];

/* alert_mail.c */
extern void alert_mail_service();

/* ate.c */
extern int asus_ate_command(const char *command, const char *value, const char *value2);
extern int ate_dev_status(void);
extern int isValidMacAddr(const char* mac);
extern int isValidCountryCode(const char *Ccode);
extern int isValidRegrev(const char *regrev);
extern int isValidSN(const char *sn);
extern int isNumber(const char *num);
extern int pincheck(const char *a);
extern int isValidChannel(int is_2G, char *channel);
extern int setPSK(const char *psk);
extern int getPSK(void);
#if defined(RTCONFIG_CFEZ) && defined(RTCONFIG_BCMARM)
extern void start_envrams(void);
extern int chk_envrams_proc(void);
#endif
extern int ate_run_arpstrom(void);
#ifdef BLUECAVE
extern int setCentralLedLv(int lv);
#endif
extern int ate_get_fw_upgrade_state(void);
extern void set_IpAddr_Lan(const char *);
extern void get_IpAddr_Lan();
extern void set_MRFLAG(const char *);
extern void get_MRFLAG();

/* tcode_rc.c */
#ifdef RTCONFIG_TCODE
extern int config_tcode(int type);
#endif

/* ate-XXX.c */
#if defined(CONFIG_BCMWL5)
extern int ATE_BRCM_FACTORY_MODE(void);
static inline int ate_factory_mode(void) { return ATE_BRCM_FACTORY_MODE(); }
#else
extern int IS_ATE_FACTORY_MODE(void);
static inline int ate_factory_mode(void) { return IS_ATE_FACTORY_MODE(); }
#endif
#ifdef RTCONFIG_FANCTRL
#if defined(RTCONFIG_QCA)
extern int __setFanOnOff(const int onoff);
extern void setFanOnOff(const int onoff);
#endif
extern int setFanOn(void);
extern int setFanOff(void);
extern int getFanSpeed(void);
#else
#if defined(RTCONFIG_QCA)
static inline void setFanOnOff(const int onoff) { }
#endif
#endif
extern int cpu_temperature(int *t, long arg);
extern int aqr_temperature(int *t, long arg);
extern void ate_temperature_record(void);
#if defined(RTCONFIG_WIFI_QCN5024_QCN5054)
extern int stress_pktgen_main(int argc, char *argv[]);
#endif
#if defined(RTCONFIG_SOC_IPQ8074)
extern int test_bl_updater_main(int argc, char *argv[]);
#endif

/* shared/boardapi.c */
extern int wanport_ctrl(int ctrl);
extern int lanport_ctrl(int ctrl);
extern int wanport_status(int wan_unit);

/* board API under sysdeps directory */
extern void set_factory_mode();
#if defined(RTCONFIG_OPENPLUS_TFAT) || defined(RTCONFIG_OPENPLUSPARAGON_NTFS) || defined(RTCONFIG_OPENPLUSTUXERA_NTFS) || defined(RTCONFIG_OPENPLUSPARAGON_HFS) || defined(RTCONFIG_OPENPLUSTUXERA_HFS)
extern void set_fs_coexist();
#endif
extern int _dump_powertable();
extern void ate_commit_bootlog(char *err_code);
extern int setAllLedOn(void);
#if defined(RPAC53) || defined(RT4GAC68U)
extern int setAllOrangeLedOn(void);
#endif
extern int setAllLedOff(void);
#if defined(RTCONFIG_WPS_ALLLED_BTN) || defined(RTCONFIG_SW_CTRL_ALLLED)
extern void setAllLedNormal(void);
#endif
#ifdef RTCONFIG_SW_CTRL_ALLLED
extern void setAllLedBrightness(void);
#endif
extern int setATEModeLedOn(void);
extern int start_wps_method(void);
extern int stop_wps_method(void);
extern int is_wps_stopped(void);
extern int is_wps_success(void);
#if defined(RTCONFIG_AMAS) && defined(CONFIG_BCMWL5)
extern int start_wps_enr(void);
#endif
extern int setMAC_2G(const char *mac);
extern int setMAC_5G(const char *mac);
#if defined(RTCONFIG_NEW_REGULATION_DOMAIN)
extern int getRegSpec(void);
extern int getRegDomain_2G(void);
extern int getRegDomain_5G(void);
extern int setRegSpec(const char *regSpec, int do_write);
extern int setRegDomain_2G(const char *cc);
extern int setRegDomain_5G(const char *cc);
#else
extern int getCountryCode_2G(void);
extern int setCountryCode_2G(const char *cc);
#endif
extern int setCountryCode_5G(const char *cc);
extern int setSN(const char *SN);
extern int getSN(void);
extern int setPIN(const char *pin);
extern int getPIN(void);
extern int set40M_Channel_2G(char *channel);
extern int set40M_Channel_5G(char *channel);
extern int ResetDefault(void);
extern int getBootVer(void);
extern int getMAC_2G(void);
extern int getMAC_5G(void);
extern int GetPhyStatus(int verbose);
extern int Get_ChannelList_2G(void);
extern int Get_ChannelList_5G(void);
extern int Get_ChannelList_60G(void);
extern void Get_fail_ret(void);
extern void Get_fail_reboot_log(void);
extern void Get_fail_dev_log(void);
#ifdef RTCONFIG_ODMPID
extern int setMN(const char *MN);
extern int getMN(void);
#ifdef RTCONFIG_REALTEK
extern int getflashMN(char *modelname, int length);
#endif
#endif
extern int check_imagefile(char *fname);
#ifdef RTCONFIG_ATEUSB3_FORCE
extern int getForceU3(void);
extern int setForceU3(const char *val);
#endif
#if defined(RTCONFIG_RALINK)
extern int set_wantolan(void);
static inline void platform_start_ate_mode(void) { };
static inline int setWlOffLed(void) { return 0; }
#elif defined(RTCONFIG_QCA)
extern int get_imageheader_size(void);
extern void platform_start_ate_mode(void);
extern void set_uuid(void);
static inline int setWlOffLed(void) { return 0; }
#elif defined(RTCONFIG_ALPINE)
extern void platform_start_ate_mode(void);
extern void set_uuid(void);
static inline int setWlOffLed(void) { return 0; }
#elif defined(RTCONFIG_LANTIQ)
extern void platform_start_ate_mode(void);
extern void set_uuid(void);
static inline int setWlOffLed(void) { return 0; }
#elif defined(CONFIG_BCMWL5)
static inline void platform_start_ate_mode(void) { };
extern int setWlOffLed(void);
#elif  defined(RTCONFIG_REALTEK)
void platform_start_ate_mode(void);
extern int setWlOffLed(void);
#else
extern int setWlOffLed(void);
#error
#endif
extern int toggle_dfs_in_acs(int onoff);
extern int set_HwId(const char *HwId);
extern int get_HwId(void);
extern int set_HwVersion(const char *HwVer);
extern int get_HwVersion(void);
extern int set_HwBom(const char *HwBom);
extern int get_HwBom(void);
extern int set_DateCode(const char *DateCode);
extern int get_DateCode(void);

/* board API under sysdeps/ralink/ralink.c */
#ifdef RTCONFIG_RALINK
extern int FWRITE(const char *da, const char* str_hex);
extern int FREAD(unsigned int addr_sa, int len);
#if defined(RTN65U)
extern void ate_run_in(void);
#endif
extern int gen_ralink_config(int band, int is_iNIC);
extern int get_channel(int band);
extern int __need_to_start_wps_band(char *prefix);
extern int need_to_start_wps_band(int wps_band);
extern void start_wsc_pin_enrollee(void);
extern void stop_wsc(void);
extern void stop_wsc_both(void);
extern void start_wsc(void);
extern void wps_oob_both(void);
extern void wsc_user_commit();
extern int getrssi(int band);
extern int asuscfe(const char *PwqV, const char *IF);
extern int stainfo(int band);
extern int Set_SwitchPort_LEDs(const char *group, const char *action);
extern int ralink_mssid_mac_validate(const char *macaddr);
extern int wl_WscConfigured(int unit);
extern int Get_Device_Flags(void);
extern int Set_Device_Flags(const char *flags_str);
extern const char *get_wifname(int band);
extern const char *get_wpsifname(void);
extern void gen_ra_config(const char* wif);
extern int radio_ra(const char *wif, int band, int ctrl);
extern void set_wlpara_ra(const char* wif, int band);
extern int wlconf_ra(const char* wif);
extern void enable_apcli(char *aif, int wlc_band);
extern void apcli_start(void);
extern void stop_wds_ra(const char* lan_ifname, const char* wif);
extern void start_wds_ra(void);
#endif

/* board API under sysdeps/qca/qca.c */
#if defined(RTCONFIG_QCA)
extern int FWRITE(const char *da, const char* str_hex);
extern int FREAD(unsigned int addr_sa, int len);
extern void ate_run_in(void);
extern int calculate_bw_of_each_channel(int band);
extern int gen_ath_config(int band, int subnet);
extern int gen_nl80211_config(int band, int subnet);
extern int __need_to_start_wps_band(char *prefix);
extern int need_to_start_wps_band(int wps_band);
extern void stop_wsc(void);
extern void stop_wsc_both(void);
extern void start_wsc(void);
extern void wps_oob_both(void);
extern int create_tmp_sta(int unit, char *sta, char *ssid_prefix);
extern void destroy_tmp_sta(char *sta);
extern void start_wsc_enrollee_band(int band);
extern void start_wsc_enrollee(void);
extern void stop_wsc_enrollee_band(int band);
extern void stop_wsc_enrollee(void);
extern int wps_ascii_to_char_with_utf8(const char *output, const char *input, int outsize);
extern int getting_wps_enroll_conf(int unit, char *ssid, char *psk);
extern int getting_wps_result(int unit, char *bssid, char *ssid, char *key_mgmt);
extern void amas_save_wifi_para(void);
extern void wifi_clone(int unit);
extern char *getWscStatus_enrollee(int unit, char *buf, int buflen);
extern char *getWscStatus(int unit, char *buf, int buflen);
extern void wsc_user_commit();
extern void qca_wif_up(const char* wif);
#ifdef RTCONFIG_WIFI_SON
extern void hyfi_process(void);
#endif
#if defined(RTCONFIG_SOC_IPQ8074)
extern void beacon_counter_monitor(void);
extern void thermal_monitor(void);
#endif
extern void gen_qca_wifi_cfgs(void);
extern void set_wlpara_qca(const char* wif, int band);
extern int wlconf_qca(const char* wif);
extern int getEEPROM(unsigned char *outbuf, unsigned short *lenpt, char *area);
extern void hexdump(unsigned char *pt, unsigned short len);
extern void setCTL(const char *);
extern int verify_ctl_table(void);
extern char *getStaMAC(char *buf, int buflen);
extern char *get_qca_iwpriv(char *name, char *command);
extern unsigned int getPapState(int unit);
extern unsigned int getStaXRssi(int unit);
#ifdef RTCONFIG_CONCURRENTREPEATER
extern unsigned int get_conn_link_quality(int unit);
#endif
typedef unsigned int	u_int;
extern u_int ieee80211_mhz2ieee(u_int freq);
#if defined(RTCONFIG_WIFI_QCA9557_QCA9882) || defined(RTCONFIG_QCA953X) || defined(RTCONFIG_QCA956X) || defined(RTCONFIG_QCN550X)
#ifdef RTCONFIG_ART2_BUILDIN
extern void Set_ART2(void);
#else
extern void Set_ART2(const char *tftpd_ip);
#endif
extern void Get_EEPROM_X(char *command);
extern void Get_CalCompare(void);
#endif
extern int dis_steer(void);
#if defined(RTCONFIG_WIFI_QCA9990_QCA9990) || \
    defined(RTCONFIG_WIFI_QCA9994_QCA9994) || \
    defined(RTCONFIG_WIFI_QCN5024_QCN5054) || \
    defined(RTCONFIG_PCIE_AR9888) || defined(RTCONFIG_PCIE_QCA9888) || \
    defined(RTCONFIG_SOC_IPQ40XX)
extern void Set_Qcmbr(const char *value);
extern void Get_BData_X(const char *command);
extern int start_thermald(void);
#endif
#if defined(RTCONFIG_WIFI_QCN5024_QCN5054)
extern void Set_Ftm(const char *value);
#endif
#if defined(RTCONFIG_SOC_IPQ8074)
extern void Get_VoltUp(void);
extern void Set_VoltUp(const char *value);
extern void Get_L2Ceiling(void);
extern void Set_L2Ceiling(const char *value);
extern void Get_PwrCycleCnt(void);
extern void Set_PwrCycleCnt(const char *value);
extern void Get_AvgUptime(void);
extern void Set_AvgUptime(const char *value);
#endif
extern int country_to_code(char *ctry, int band, char *code_str, size_t len);
extern void acs_ch_weight_param(void);
extern void get_stainfo(int bssidx, int vifidx);
#endif	/* RTCONFIG_QCA */

#if defined(RTCONFIG_SOC_IPQ8074)
extern int upgrade_bootloader_v2(void);
extern int pre_firmware_upgrade(const char *fname);
#else	/* !RTCONFIG_SOC_IPQ8074 */
static inline int upgrade_bootloader_v2(void) { return 0; }
static inline int pre_firmware_upgrade(const char __attribute__((__unused__)) *fname) { return 0; }
#endif	/* RTCONFIG_SOC_IPQ8074 */

#if defined(RTCONFIG_QCA_MCSD)
extern int start_mcsd(FILE *fp);
extern void stop_mcsd(void);
#else
static inline int start_mcsd(FILE __attribute__((__unused__)) *fp) { return 0; }
static inline void stop_mcsd(void) { }
#endif

#ifdef RTCONFIG_CONCURRENTREPEATER
extern void start_wps_cli(void);
extern void stop_wps_cli(void);
#endif

#if defined(RTCONFIG_ALPINE) || defined(RTCONFIG_LANTIQ)
extern int start_nvram_txt(void);
#endif

/* board API under shared/sysdeps/alpine/private.c  */
#if defined(RTCONFIG_ALPINE)
extern int update_trx(char *imagefile);
#endif

/* board API under sysdeps/alpine/alpine.c */
#if defined(RTCONFIG_ALPINE)
extern int start_aqr107(void);
extern int FWRITE(const char *da, const char* str_hex);
extern int FREAD(unsigned int addr_sa, int len);
extern int gen_ath_config(int band, int is_iNIC,int subnet);
extern int __need_to_start_wps_band(char *prefix);
extern int need_to_start_wps_band(int wps_band);
extern int getEEPROM(unsigned char *outbuf, unsigned short *lenpt, char *area);
extern void hexdump(unsigned char *pt, unsigned short len);
extern void setCTL(const char *);
extern int verify_ctl_table(void);
#ifdef RTCONFIG_ATEUSB3_FORCE
extern int getForceU3(void);
extern int setForceU3(const char *val);
#endif
extern char *getStaMAC(char *buf, int buflen);
extern unsigned int getPapState(int unit);
typedef unsigned int	u_int;
extern u_int ieee80211_mhz2ieee(u_int freq);
#endif

/* board API under sysdeps/lantiq/lantiq.c */
#if defined(RTCONFIG_LANTIQ)

extern int get_wlan_service_status(int bssidx, int vifidx);
extern void set_wlan_service_status(int bssidx, int vifidx, int enabled);

extern char *wav_get_security_str(const char *auth, const char *crypto, int weptype);
extern char *wav_get_beacon_type(const char *crypto);
extern char *wav_get_encrypt(const char *crypto);
extern int start_repeater(void);
extern int set_wps_enable(int unit);
extern int set_all_wps_config(int configured);
extern void wps_oob(void);
extern void stop_wsc(void);
extern int FWRITE(const char *da, const char* str_hex);
extern int FREAD(unsigned int addr_sa, int len);
extern int gen_ath_config(int band, int is_iNIC,int subnet);
extern int __need_to_start_wps_band(char *prefix);
extern int need_to_start_wps_band(int wps_band);
extern int getWscStatusStr(int unit, char *buf, int buf_size);
extern int getEEPROM(unsigned char *outbuf, unsigned short *lenpt, char *area);
extern void hexdump(unsigned char *pt, unsigned short len);
extern void setCTL(const char *);
extern int verify_ctl_table(void);
#ifdef RTCONFIG_ATEUSB3_FORCE
extern int getForceU3(void);
extern int setForceU3(const char *val);
#endif
extern char *getStaMAC(char *buf, int buflen);
extern unsigned int getPapState(int unit);
typedef unsigned int	u_int;
extern u_int ieee80211_mhz2ieee(u_int freq);
extern int ppa_support(int wan_unit);
extern void usb_pwr_ctl(int onoff);
#endif
/* board API under sysdeps/qca/ctl.c */
#if defined(RTCONFIG_QCA)
extern const void *req_fw_hook(const char *filename, size_t *new_size);
#else
static inline const void *req_fw_hook(const char *filename, size_t *new_size) { return NULL; }
#endif

/* board API under sysdeps/init-broadcom.c sysdeps/broadcom sysdeps/tcode_brcm.c */
extern void init_others(void);
#if defined(CONFIG_BCMWL5) || defined(RTCONFIG_WIRELESSREPEATER)
extern int is_ure(int unit);
#endif
#ifdef CONFIG_BCMWL5
/* The below macros handle endian mis-matches between wl utility and wl driver. */
extern bool g_swap;
#define htod32(i) (g_swap?bcmswap32(i):(uint32)(i))
#define dtoh64(i) (g_swap?bcmswap64(i):(uint64)(i))
#define dtoh32(i) (g_swap?bcmswap32(i):(uint32)(i))
#define dtoh16(i) (g_swap?bcmswap16(i):(uint16)(i))
#define dtohchanspec(i) (g_swap?dtoh16(i):i)

extern void wl_vif_hwaddr_set(const char *name);
extern int wlconf(char *ifname, int unit, int subunit);
extern int wl_send_dif_event(const char *ifname, uint32 event);
extern int check_wl_client(char *ifname, int unit, int subunit);
extern void wlconf_post(const char *ifname);
extern void wlconf_pre();
extern void init_wl_compact(void);
extern void check_afterburner(void);
extern int set_wltxpower();
#ifdef RTCONFIG_BCMWL6
extern void led_bh_prep(int post);
extern int wl_check_chanspec();
extern void wl_check_5g_band_group();
#if defined(RTCONFIG_BCMWL6) && defined(RTCONFIG_PROXYSTA)
extern void reset_psr_hwaddr();
#endif
#endif
#ifdef RTCONFIG_BCM_7114
extern void ldo_patch();
#endif
#if defined(RTCONFIG_BCM7) || defined(RTCONFIG_BCM_7114) || defined(HND_ROUTER)
extern int wl_channel_valid(char *wif, int channel);
extern int wl_subband(char *wif, int idx);
#endif
#if defined(RTCONFIG_BCM_7114) || defined(HND_ROUTER)
extern void check_4366_dummy(void);
extern void check_4366_fabid(void);
#endif
extern void wl_dfs_radarthrs_config(char *ifname, int unit);
#if defined(RTCONFIG_BCM7) || defined(RTCONFIG_BCM_7114) || defined(HND_ROUTER)
extern int wlcscan_core_escan(char *ofile, char *wif);
#endif
extern int setRegrev_2G(const char *regrev);
extern int setRegrev_5G(const char *regrev);
#if defined(RTAC3200) || defined(RTAC5300) || defined(GTAC5300) || defined(RTCONFIG_HAS_5G_2)
extern int setMAC_5G_2(const char *mac);
extern int getMAC_5G_2(void);
extern int Get_ChannelList_5G_2(void);
#endif
extern int setCommit(void);
extern int setWaitTime(const char *wtime);
extern int setWiFi2G(const char *act);
extern int setWiFi5G(const char *act);
extern int getCountryCode_5G(void);
extern int getRegrev_2G(void);
extern int getRegrev_5G(void);
extern int getSSID(int unit);
extern void check_wl_country();
extern int wl_dfs_support(int unit);
#if defined(RTAC3200) || defined(RTAC68U) || defined(RTCONFIG_BCM_7114) || defined(HND_ROUTER)
extern void wl_disband5grp();
#endif
#ifdef RTCONFIG_BCMARM
extern int setWanLedMode1(void);
extern int setWanLedMode2(void);
extern void tweak_smp_affinity(int enable_samba);
#ifdef HND_ROUTER
extern void tweak_usb_affinity(int enable);
#endif
#endif
#ifdef WLCLMLOAD
extern int download_clmblob_files();
#endif
#ifdef RTAC68U
extern void check_cfe_ac68u();
extern void update_cfe();
extern int firmware_enc_crc_main(int argc, char *argv[]);
extern int fw_check_main(int argc, char *argv[]);
#endif
extern void check_asus_jffs(void);
extern void fw_check_pre(void);
#ifdef RTAC3200
extern void update_cfe_ac3200();
extern void update_cfe_ac3200_128k();
extern void bsd_defaults(void);
#endif
#ifdef GTAC2900
extern void update_cfe_ac2900();
#endif
#ifdef RTCONFIG_BCM_MFG
extern void brcm_mfg_init();
extern void brcm_mfg_services();
#endif
#ifdef HND_ROUTER
extern void fc_init();
extern void fc_fini();
extern void hnd_nat_ac_init(int bootup);
extern void setLANLedOn(void);
extern void setLANLedOff(void);
extern int mtd_erase_image_update();
extern int wait_to_forward_state(char *ifname);
#ifndef RTCONFIG_HND_ROUTER_AX
extern void wl_fail_db(int unit, int state, int count);
#endif
#endif
#ifdef RTCONFIG_BCMWL6
extern int hw_vht_cap();
#endif
extern int wl_control_channel(int unit);
#ifdef RTCONFIG_DPSTA
void set_dpsta_ifnames();
#endif
#ifdef RTAC86U
extern void hnd_cfe_check();
#endif
#ifdef RTCONFIG_RGBLED
extern int setRogRGBLedTest(int RGB);
#endif
extern void hnd_set_hwstp(void);
#endif

#ifdef RTCONFIG_AMAS
enum {
	AB_FLAG_NONE		= 0,
	AB_FLAG_SOFT		= 1,
	AB_FLAG_HARD_2PK	= 2,
	AB_FLAG_HARD_3PK	= 3,
	AB_FLAG_MAX
};
extern int set_amas_bdl(int flag);
extern int unset_amas_bdl(void);
extern int get_amas_bdl(void);
extern int get_amas_bdlkey(void);
extern int set_amas_bdlkey(const char *str);
extern int unset_amas_bdlkey(void);
extern int no_need_obd(void);
extern int no_need_obdeth(void);
#if defined(RTCONFIG_ETHOBD)
extern int no_need_obdeth(void);
#endif
extern int wait_wifi_ready(void);
#endif

#ifdef RTCONFIG_WIFI_SON
extern int start_cap(int c);
extern void start_re(int c);
extern int start_cmd(char *cmd);
extern int start_wsplcd(void);
extern int start_hyd(void);
#ifdef RTCONFIG_ETHBACKHAUL
extern void start_eth(int c);
extern int lldpcli_det(void);
extern int lldpcli_det2(void);
#endif
#if defined(RTCONFIG_HIDDEN_BACKHAUL)
extern int check_bh(char* iface);
extern void add_bh_network(void);
extern void del_bh_network(void);
extern void addXOR(char* plaintext, char* ciphertext);
extern void delXOR(char* ciphertext,int cipher_lens,char* plaintext);
extern void setting_hash_ap(char* src, int lens);
extern void get_mesh_ie(char *part1, char *part2);
extern void renew_bh(void);
#endif
extern void config_hive(int role,int band);
extern void stop_hyfi(void);
extern void start_hyfi(void);
extern int get_role(void);
extern void hyfi_process(void);
extern void start_wifimon_check(int delay);
extern void duplicate_5g2();
extern int hyd_exec;
#ifdef RTCONFIG_ETHBACKHAUL
extern void start_eth_bh_mon();
extern int ethbh_peer_detect(char *nic, char *timeout, char *msg);
extern int check_eth_time;
extern int eth_down_time;
#endif
#endif

/* sysdeps/dsl-*.c */
extern int check_tc_firmware_crc(void);
extern int truncate_trx(void);
extern void do_upgrade_adsldrv(void);
extern int compare_linux_image(void);

// init.c
extern int init_main(int argc, char *argv[]);
extern int reboothalt_main(int argc, char *argv[]);
extern int console_main(int argc, char *argv[]);
extern int limit_page_cache_ratio(int ratio);
extern int init_nvram(void);
extern int init_nvram2(void);
extern int init_nvram3(void);
extern void wl_defaults(void);
extern void wl_defaults_wps(void);
extern void restore_defaults_module(char *prefix);
extern void restore_defaults_wifi(int all);
extern void clean_vlan_ifnames(void);
extern int fixdmgfw_main(int argc, char *argv[]);

// init-qca.c
#if defined(RTCONFIG_GLOBAL_INI)
extern int __update_ini_file(const char *filename, char **params);
static inline int update_ini_file(const char *filename, char *param)
{
	char *params[] = { param, NULL };
	return __update_ini_file(filename, params);
}
#endif
#ifdef RTCONFIG_TAGGED_BASED_VLAN
extern void vlan_switch_accept_tagged(unsigned int port_list);
extern void vlan_switch_accept_untagged(unsigned int port_list);
extern void vlan_switch_accept_all(unsigned int port_list);
extern void vlan_switch_setup(int vlan_id, int vlan_prio, int lanportset);
extern int vlan_switch_pvid_setup(int *pvid_list, int *pprio_list, int size);
#endif
extern unsigned int num_of_mssid_support(unsigned int unit);
#ifdef RTCONFIG_BCMFA
extern void fa_nvram_adjust();
#endif

// format.c
extern void adjust_url_urlelist();
extern void adjust_ddns_config();
extern void adjust_access_restrict_config();
#if defined(RTCONFIG_VPN_FUSION)
extern void adjust_vpnc_config(void);
#endif
#if defined(RTCONFIG_NOTIFICATION_CENTER)
extern void force_off_push_msg(void);
#endif

// format.c
extern void adjust_url_urlelist();
extern void adjust_ddns_config();
extern void adjust_access_restrict_config();

// interface.c
extern int _ifconfig(const char *name, int flags, const char *addr, const char *netmask, const char *dstaddr, int mtu);
static inline int ifconfig(const char *name, int flags, const char *addr, const char *netmask)
{
	return _ifconfig(name, flags, addr, netmask, NULL, 0);
}
extern int _ifconfig_get(const char *name, int *flags, char *addr, char *netmask, char *dstaddr, int *mtu);
extern int ifconfig_mtu(const char *name, int mtu);
extern int route_add(char *name, int metric, char *dst, char *gateway, char *genmask);
extern int route_del(char *name, int metric, char *dst, char *gateway, char *genmask);
extern int start_vlan(void);
extern int stop_vlan(void);
extern int config_vlan(void);
extern int set_bonding_iface_hwaddr(const char *bond_if, const char *hwaddr);
extern void config_loopback(void);
#ifdef RTCONFIG_IPV6
extern int _ipv6_route_add(const char *name, int metric, const char *dst, const char *gateway, int flags);
extern int ipv6_route_add(const char *name, int metric, const char *dst, const char *gateway);
extern int ipv6_route_del(const char *name, int metric, const char *dst, const char *gateway);
extern int ipv6_mapaddr4(struct in6_addr *addr6, int ip6len, struct in_addr *addr4, int ip4mask);
#endif

// wan.c
extern int wan_prefix(char *ifname, char *prefix);
extern int wan_ifunit(char *wan_ifname);
extern int wanx_ifunit(char *wan_ifname);
extern int preset_wan_routes(char *wan_ifname);
extern int found_default_route(int wan_unit);
#ifdef RTCONFIG_QCA_PLC_UTILS
extern int autodet_plc_main(int argc, char *argv[]);
#endif
extern int autodet_main(int argc, char *argv[]);
extern int detwan_main(int argc, char *argv[]);
extern int dpdt_ant_main(int argc, char *argv[]);
extern int thermal_txpwr_main(int argc, char *argv[]);
extern void start_wan(void);
extern void stop_wan(void);
extern int add_multi_routes(int check_link);
extern int add_routes(char *prefix, char *var, char *ifname);
extern int del_routes(char *prefix, char *var, char *ifname);
extern void start_wan_if(int unit);
extern void stop_wan_if(int unit);
#ifdef RTCONFIG_IPV6
extern void stop_wan6(void);
#if 0
extern void start_ecmh(const char *wan_ifname);
extern void stop_ecmh(void);
#endif
extern void wan6_up(const char *wan_ifname);
extern void wan6_down(const char *wan_ifname);
extern void start_wan6(void);
extern void stop_wan6(void);
#endif
extern int do_ping_detect(int wan_unit, const char *target);
extern int do_dns_detect(int wan_unit);
#ifdef DSL_AC68U
extern int check_wan_if(int unit);
#endif

// lan.c
extern void update_lan_state(int state, int reason);
extern void set_et_qos_mode(void);
extern void start_wl(void);
extern void stop_wl(void);
#if defined(RTCONFIG_QCA)||defined(RTCONFIG_RALINK)
extern char *get_hwaddr(const char *ifname);
#endif
extern void set_hwaddr(const char *ifname, const char *mac);
#ifdef RTCONFIG_REALTEK
extern int wlconf_rtk(const char* ifname);
extern void gen_rtk_config(const char* ifname);
extern int rtk_chk_wlc_ssid(const char* ifname);
#endif /* RTCONFIG_REALTEK */
extern void start_lan(void);
extern void stop_lan(void);
extern void do_static_routes(int add);
extern void hotplug_net(void);
extern int radio_switch(int subunit);
extern int radio_main(int argc, char *argv[]);
extern int wldist_main(int argc, char *argv[]);
extern int update_lan_resolvconf(void);
extern void lan_up(char *lan_ifname);
extern void lan_down(char *lan_ifname);
extern void stop_lan_wl(void);
extern void start_lan_wl(void);
extern void restart_wl(void);
extern void lanaccess_mssid_ban(const char *ifname_in);
extern void lanaccess_wl(void);
#ifdef RTCONFIG_FBWIFI
extern void stop_fbwifi_check();
extern void start_fbwifi_check();
extern void restart_fbwifi_register();
#endif
extern void restart_wireless(void);
#ifdef RTCONFIG_BCM_7114
extern void stop_wl_bcm(void);
#endif
extern void start_wan_port(void);
extern void stop_wan_port(void);
extern void start_lan_port(int dt);
extern void stop_lan_port(void);
extern void start_lan_wlport(void);
extern void stop_lan_wlport(void);
extern int wl_dev_exist(void);
#if defined(RTCONFIG_RALINK) || defined(RTCONFIG_QCA) || defined(RTCONFIG_ALPINE) || defined(RTCONFIG_LANTIQ)
extern pid_t pid_from_file(char *pidfile);
#endif
extern int delay_main(int argc, char *argv[]);
#ifdef RTCONFIG_IPV6
extern void set_default_accept_ra(int flag);
extern void set_default_accept_ra_defrtr(int flag);
extern void set_intf_ipv6_dad(const char *ifname, int bridge, int flag);
extern void config_ipv6(int enable, int incl_wan);
#ifdef RTCONFIG_DUALWAN
extern void start_lan_ipv6(void);
extern void stop_lan_ipv6(void);
extern void restart_dnsmasq_ipv6(void);
#endif
extern void enable_ipv6(const char *ifname);
extern void disable_ipv6(const char *ifname);
extern void start_lan_ipv6(void);
extern void stop_lan_ipv6(void);
#endif
extern int set_bonding(const char *bond_if, const char *mode, const char *policy, char *hwaddr);
extern void remove_bonding(const char *bond_if);
#ifdef RTCONFIG_WIRELESSREPEATER
extern void start_lan_wlc(void);
extern void stop_lan_wlc(void);
#endif
#if defined(RTCONFIG_QCA)
extern void qca_wif_up(const char* wif);
extern void gen_qca_wifi_cfgs(void);
#endif
#ifdef HND_ROUTER
extern void wait_lan_port_to_forward_state(void);
#endif
#if defined(RTCONFIG_RALINK) && defined(RTCONFIG_WLMODULE_MT7615E_AP)
extern void start_wds_ra();
#endif

// firewall.c
extern int start_firewall(int wanunit, int lanunit);
extern void enable_ip_forward(void);
extern void convert_routes(void);
extern void start_default_filter(int lanunit);
extern int ipt_addr_compact(const char *s, int af, int strict);
extern void filter_setting(int wan_unit, char *lan_if, char *lan_ip, char *logaccept, char *logdrop);
#ifdef WEB_REDIRECT
extern void redirect_setting(void);
#endif
#ifdef RTCONFIG_REDIRECT_DNAME
extern void redirect_nat_setting(void);
#endif
#if defined(RTCONFIG_DUALWAN)
extern void set_load_balance(void);
#endif
extern void ip2class(char *lan_ip, char *netmask, char *buf);
#ifdef RTCONFIG_WIFI_SON
extern void set_cap_apmode_filter(void);
#endif
extern void write_extra_filter(FILE *fp);
#ifdef RTCONFIG_IPV6
extern void write_extra_filter6(FILE *fp);
#endif

/* pc.c */
#ifdef RTCONFIG_PARENTALCTRL
extern int pc_main(int argc, char *argv[]);
#endif

// pc_block.c
#ifdef RTCONFIG_PARENTALCTRL
extern int pc_block_main(int argc, char *argv[]);
extern void config_blocking_redirect(FILE *fp);
#endif

// pc_tmp.c
#ifdef RTCONFIG_PARENTALCTRL
extern int pc_tmp_main(int argc, char *argv[]);
#endif

/* ic.c */
#ifdef RTCONFIG_INTERNETCTRL
extern int ic_main(int argc, char *argv[]);
#endif

// ppp.c
extern int ipup_main(int argc, char **argv);
extern int ipdown_main(int argc, char **argv);
extern int ippreup_main(int argc, char **argv);
#ifdef RTCONFIG_IPV6
extern int ip6up_main(int argc, char **argv);
extern int ip6down_main(int argc, char **argv);
#endif
extern int authfail_main(int argc, char **argv);
extern int ppp_ifunit(char *ifname);
extern int ppp_linkunit(char *linkname);

// pppd.c
extern char *ppp_escape(char *src, char *buf, size_t size);
extern char *ppp_safe_escape(char *src, char *buf, size_t size);
extern int start_pppd(int unit);
extern void stop_pppd(int unit);
extern int start_demand_ppp(int unit, int wait);
extern int start_pppoe_relay(char *wan_if);
extern void stop_pppoe_relay(void);

// roamst.c
void rast_ipc_socket_thread(void);

// vpnc.c
#ifdef RTCONFIG_VPNC
extern int vpnc_ipup_main(int argc, char **argv);
extern int vpnc_ipdown_main(int argc, char **argv);
extern int vpnc_ippreup_main(int argc, char **argv);
extern int vpnc_authfail_main(int argc, char **argv);
#ifdef RTCONFIG_VPN_FUSION
extern void update_vpnc_state(const int vpnc_idx, const int state, const int reason);
extern void reset_vpnc_state(void);
extern int vpnc_ovpn_up_main(int argc, char **argv);
extern int vpnc_ovpn_down_main(int argc, char **argv);
extern int vpnc_ovpn_route_up_main(int argc, char **argv);
#else
extern void update_vpnc_state(char *prefix, int state, int reason);
#endif
extern int is_vpnc_dns_active(void);
#endif

/*rc_ipsec.c*/
#ifdef RTCONFIG_IPSEC
extern void rc_ipsec_nvram_convert_check(void);
extern void rc_ipsec_config_init(void);
extern void run_ipsec_firewall_scripts(void);
extern void rc_ipsec_nvram_convert_check(void);
#endif

// network.c
extern void set_host_domain_name(void);
extern void start_lan(void);
extern void stop_lan(void);
extern void do_static_routes(int add);
extern void start_wl(void);
extern void wan_up(const char *wan_ifname);
#ifdef RTCONFIG_IPV6
extern void wan6_up(const char *wan_ifname);
#endif
extern void wan_down(char *wan_ifname);
extern void update_wan_state(char *prefix, int state, int reason);
extern int update_resolvconf(void);

/* qos.c */
extern int start_iQos(void);
extern void stop_iQos(void);
extern void del_iQosRules(void);
extern int add_iQosRules(char *pcWANIF);
#ifdef CONFIG_BCMWL5
extern void del_EbtablesRules(void);
extern void add_EbtablesRules(void);
#endif
extern void ForceDisableWLan_bw(void);
extern int check_wl_guest_bw_enable();

/* rtstate.c */
extern void add_rc_support(char *feature);
extern void del_rc_support(char *feature);

// udhcpc.c
extern int udhcpc_wan(int argc, char **argv);
extern int udhcpc_lan(int argc, char **argv);
extern int start_udhcpc(char *wan_ifname, int unit, pid_t *ppid);
extern void stop_udhcpc(int unit);
extern int zcip_wan(int argc, char **argv);
extern int start_zcip(char *wan_ifname, int unit, pid_t *ppid);
extern void stop_zcip(int unit);

#ifdef RTCONFIG_IPV6
extern int dhcp6c_wan(int argc, char **argv);
extern int start_dhcp6c(void);
extern void stop_dhcp6c(void);
#ifdef RTCONFIG_6RELAYD
extern void stop_6relayd(void);
extern int start_6relayd(void);
#endif
#endif

#ifdef RTCONFIG_WPS
extern int wpsaide_main(int argc, char *argv[]);
#ifdef RTCONFIG_QCA
extern int get_wps_er_main(int argc, char *argv[]);
#if defined(RTCONFIG_CFG80211)
extern int vap_evhandler_main(int argc, char *argv[]);
#endif
#endif
extern int stop_wpsaide();
extern int start_wpsaide();
#endif

// auth.c
extern int start_auth(int unit, int wan_up);
extern int stop_auth(int unit, int wan_down);
extern int restart_auth(int unit);
#ifdef RTCONFIG_EAPOL
extern int wpacli_main(int argc, char *argv[]);
#endif

// mtd.c
extern int mtd_erase(const char *mtdname);
extern int mtd_unlock(const char *mtdname);
#ifdef RTCONFIG_BCMARM
#ifdef HND_ROUTER
extern int bca_sys_upgrade(const char *path);
extern int hnd_nvram_erase();
#else
extern int mtd_erase_old(const char *mtdname);
extern int mtd_write_main_old(int argc, char *argv[]);
extern int mtd_unlock_erase_main_old(int argc, char *argv[]);
extern int mtd_write(const char *path, const char *mtd);
#endif
#else
extern int mtd_write_main(int argc, char *argv[]);
extern int mtd_unlock_erase_main(int argc, char *argv[]);
#endif

// jffs2.c
#if defined(RTCONFIG_UBIFS)
extern void start_ubifs(void);
extern void stop_ubifs(int stop);
static inline void start_jffs2(void) { start_ubifs(); }
static inline void stop_jffs2(int stop) { stop_ubifs(stop); }
#elif defined(RTCONFIG_YAFFS)
extern void start_yaffs(void);
extern void stop_yaffs(int stop);
static inline void start_jffs2(void) { start_yaffs(); }
static inline void stop_jffs2(int stop) { stop_yaffs(stop); }
#elif defined(RTCONFIG_JFFS2) || defined(RTCONFIG_JFFSV1) || defined(RTCONFIG_BRCM_NAND_JFFS2)
extern void start_jffs2(void);
extern void stop_jffs2(int stop);
#else
static inline void start_jffs2(void) { }
static inline void stop_jffs2(int stop) { }
#endif
extern void userfs_prepare(const char *folder);

// watchdog.c
extern void led_control_normal(void);
#ifndef HND_ROUTER
extern void erase_nvram(void);
#endif
extern int init_toggle(void);
extern void btn_check(void);
extern int watchdog_main(int argc, char *argv[]);
#ifdef RTCONFIG_CONNTRACK
extern int pctime_main(int argc, char *argv[]);
#endif
extern int watchdog02_main(int argc, char *argv[]);
#ifdef SW_DEVLED
extern int sw_devled_main(int argc, char *argv[]);
#endif
extern int wdg_monitor_main(int argc, char *argv[]);
extern void init_wllc(void);
extern void rssi_check_unit(int unit);
#if defined(RTCONFIG_LED_BTN) || defined(RTCONFIG_WPS_ALLLED_BTN) || defined(RTCONFIG_TURBO_BTN)
extern void led_table_ctrl(int on_off);
#endif
extern void timecheck(void);

// check_watchdog.c
extern int check_watchdog_main(int argc, char *argv[]);

// usbled.c
extern int usbled_main(int argc, char *argv[]);

// phy_tempsense.c
#if defined(RTCONFIG_FANCTRL)
#if defined(CONFIG_BCMWL5)
extern int phy_tempsense_main(int argc, char *argv[]);
#endif
extern void restart_fanctrl(void);
#endif

#if defined(RTCONFIG_BCMWL6) && defined(RTCONFIG_PROXYSTA)
// arp.c
extern int send_arpreq(void);
// psta_monitor.c
extern int psta_monitor_main(int argc, char *argv[]);
#endif
#if defined(RTCONFIG_AMAS) && (defined(RTCONFIG_BCMWL6) || defined(RTCONFIG_LANTIQ) || defined(RTCONFIG_QCA) || defined(RTCONFIG_REALTEK))
// obd.c
extern int obd_main(int argc, char *argv[]);
extern void amas_wait_wifi_ready(void);
extern void obd_led_blink();
extern void obd_led_off();
#ifdef RTCONFIG_PRELINK
extern void obd_switch_re(int wifi);
extern int prelink_lock_acquire(int dbg_on);
extern void prelink_unlock(int lock_fd);
#endif
#endif
#if defined(RTCONFIG_AMAS) && defined(RTCONFIG_ETHOBD)
//obd_eth.c
extern int obdeth_main(int argc, char *argv[]);
extern int obd_monitor_main(int argc, char *argv[]);
extern unsigned char *data_aes_encrypt(unsigned char *key, unsigned char *data, size_t data_len, size_t *out_len);
extern int hex2str_x(unsigned char *hex, char *str, int hex_len);
#endif
#ifdef RTCONFIG_AMAS
extern unsigned char *gen_sha256_key(unsigned char *data,size_t data_len,size_t *out_len);
extern unsigned char *data_aes_decrypt(unsigned char *key, unsigned char *enc_data, size_t data_len, size_t *out_len);
extern int hex2str_x(unsigned char *hex, char *str, int hex_len);
extern int isNull (unsigned char *string, int len);
#endif

#ifdef RTCONFIG_QTN
extern int qtn_monitor_main(int argc, char *argv[]);
#endif

#ifdef RTCONFIG_LANTIQ
extern int wave_monitor_main(int argc, char *argv[]);
extern int start_wave_monitor(void);
extern void start_mcast_proxy(void);
extern void stop_mcast_proxy(void);
#endif

#ifdef RTCONFIG_QSR10G
extern int rpc_qcsapi_set_wlmaclist(int unit, int subunit);
extern int rpc_qtn_ready();
extern int start_ate_mode_qsr10g(void);
extern int qtn_monitor_main(void);
#endif
extern int radio_main(int argc, char *argv[]);

// ntp.c
extern int ntp_main(int argc, char *argv[]);

// common.c
extern void usage_exit(const char *cmd, const char *help) __attribute__ ((noreturn));
#define xstart(args...)	_xstart(args, NULL)
extern int _xstart(const char *cmd, ...);
extern void run_nvscript(const char *nv, const char *arg1, int wtime);
extern void run_userfile (char *folder, char *extension, const char *arg1, int wtime);
extern void setup_conntrack(void);
extern void inc_mac(char *mac, int plus);
extern void set_mac(const char *ifname, const char *nvname, int plus);
extern const char *default_wanif(void);
//	extern const char *default_wlif(void);
extern void simple_unlock(const char *name);
extern void simple_lock(const char *name);
extern void killall_tk(const char *name);
extern void kill_pid_tk(pid_t pid);
extern void kill_pidfile_tk(const char *pidfile);
extern void kill_pidfile_tk_g(const char *pidfile);
extern long fappend(FILE *out, const char *fname);
extern long fappend_file(const char *path, const char *fname);
extern void logmessage(char *logheader, char *fmt, ...);
extern char *trim_r(char *str);
extern void restart_lfp(void);
extern int get_meminfo_item(const char *name);
extern void setup_timezone(void);
extern int is_valid_hostname(const char *name);
extern int is_valid_domainname(const char *name);
extern void setup_ct_timeout(int connflag);
extern void setup_udp_timeout(int connflag);
extern void setup_ftp_conntrack(int port);
extern void setup_pt_conntrack(void);
extern void remove_conntrack(void);
extern int _pppstatus(const char *statusfile);
extern int pppstatus(int unit);
extern void time_zone_x_mapping(void);
extern void stop_if_misc(void);
extern int mssid_mac_validate(const char *macaddr);
#ifdef CONFIG_BCMWL5
extern int setup_dnsmq(int mode);
#endif
extern int rand_seed_by_time(void);
char *get_wpa_supplicant_pidfile(const char *ifname, char *buf, int size);
void kill_wifi_wpa_supplicant(int unit);


// ssh.c

// usb.c
#if defined(RTCONFIG_OPENPLUS_TFAT) \
		|| defined(RTCONFIG_OPENPLUSPARAGON_NTFS) || defined(RTCONFIG_OPENPLUSTUXERA_NTFS) \
		|| defined(RTCONFIG_OPENPLUSPARAGON_HFS) || defined(RTCONFIG_OPENPLUSTUXERA_HFS)
extern int fs_coexist();
#endif
#ifdef RTCONFIG_USB
FILE* fopen_or_warn(const char *path, const char *mode);
extern void hotplug_usb(void);
extern void add_usb_host_module(void);
#ifdef RTCONFIG_USB_MODEM
extern void add_usb_modem_modules(void);
#if defined(RTCONFIG_JFFS2) || defined(RTCONFIG_BRCM_NAND_JFFS2) || defined(RTCONFIG_UBIFS)
extern int modem_data_main(int argc, char *argv[]);
#endif
#endif
extern void start_usb(int orig);
extern void remove_usb_module(void);
extern void stop_usb_program(int mode);
#ifndef RTCONFIG_ERPTEST
extern void stop_usb();
#else
extern void restart_usb(int f_stop);
extern void stop_usb(int f_force);
#endif
#ifdef RTCONFIG_USB_PRINTER
extern void start_lpd();
extern void stop_lpd();
extern void start_u2ec();
extern void stop_u2ec();
extern void start_usblpsrv(void);
#endif
extern int ejusb_main(int argc, char *argv[]);
extern int __ejusb_main(const char *port_path, int unplug);
extern void webdav_account_default(void);
extern void remove_storage_main(int shutdn);
#ifndef RTCONFIG_NO_USBPORT
extern int start_usbled(void);
extern int stop_usbled(void);
#endif
extern void restart_nas_services(int stop, int start, int force);
extern void stop_nas_services(int force);
extern int sd_partition_num();
#endif
#ifdef RTCONFIG_CROND
extern void start_cron(void);
extern void stop_cron(void);
#endif
#ifdef RTCONFIG_QUAGGA
extern void stop_quagga(void);
extern int start_quagga(void);
#endif
extern void start_webdav(void);
#ifdef RTCONFIG_SAMBASRV
extern void create_custom_passwd(void);
extern void stop_samba(int force);
extern void start_samba(void);
#endif
#ifdef RTCONFIG_WEBDAV
extern void stop_webdav(void);
extern void stop_all_webdav(void);
#else
static inline void stop_all_webdav(void) { }
#endif
#ifdef RTCONFIG_FTP
extern void stop_ftpd(int force);
extern void start_ftpd(void);
#endif
#ifdef RTCONFIG_TFTP_SERVER
extern void stop_tftpd(int force);
extern void start_tftpd(void);
#endif
#ifdef RTCONFIG_CLOUDSYNC
extern void stop_cloudsync(int type);
extern void start_cloudsync(int fromUI);
#endif
#if defined(RTCONFIG_APP_PREINSTALLED) || defined(RTCONFIG_APP_NETINSTALLED)
extern int stop_app(void);
extern int start_app(void);
extern void usb_notify();
#endif

#ifdef RTCONFIG_DISK_MONITOR
extern void start_diskmon(void);
extern void stop_diskmon(void);
extern int diskmon_main(int argc, char *argv[]);
extern void remove_pool_error(const char *device, const char *flag);
#endif
extern int diskremove_main(int argc, char *argv[]);

#ifdef RTCONFIG_CIFS
extern void start_cifs(void);
extern void stop_cifs(void);
extern int mount_cifs_main(int argc, char *argv[]);
#else
static inline void start_cifs(void) {};
static inline void stop_cifs(void) {};
#endif

// linkmonitor.c
extern int linkmonitor_main(int argc, char *argv[]);

// vpn.c
extern void start_pptpd(void);
extern void stop_pptpd(void);

// vpnc.c
#ifdef RTCONFIG_VPNC
extern int start_vpnc(void);
extern void stop_vpnc(void);
#ifdef RTCONFIG_VPN_FUSION
extern void vpnc_init();
extern int stop_vpnc_by_unit(const int unit);
extern int start_vpnc_by_unit(const int unit);
extern int change_default_wan();
#if USE_IPTABLE_ROUTE_TARGE
extern int vpnc_active_dev_policy(const int policy_idx);
extern int vpnc_remove_tmp_policy_rule();
#else
extern int vpnc_set_dev_policy_rule();
#endif
#endif
#endif

// ovpn.c
extern int ovpn_up_main(int argc, char **argv);
extern int ovpn_down_main(int argc, char **argv);
extern int ovpn_route_up_main(int argc, char **argv);

// openvpn.c
#ifdef RTCONFIG_OPENVPN
extern void start_vpnclient(int clientNum);
extern void stop_vpnclient(int clientNum);
extern void start_vpnserver(int serverNum);
extern void stop_vpnserver(int serverNum);
extern void start_vpn_eas();
extern void stop_vpn_eas();
extern void run_vpn_firewall_scripts();
extern void write_vpn_dnsmasq_config(FILE*);
extern int write_vpn_resolv(FILE*);
//static inline void start_vpn_eas() { }
//#define write_vpn_resolv(f) (0)
extern void create_openvpn_passwd();
#endif

// wanduck.c
#if defined(RTCONFIG_LANWAN_LED) || defined(RTCONFIG_HND_ROUTER)
extern int update_wan_leds(int wan_unit, int link_wan_unit);
#endif
#if defined(RTCONFIG_LANWAN_LED) || defined(RTCONFIG_LAN4WAN_LED)
int LanWanLedCtrl(void);
#endif
extern int wanduck_main(int argc, char *argv[]);

// tcpcheck.c
extern int setupsocket(int sock);
extern void wakeme(int sig);
extern void goodconnect(int i);
extern void failedconnect(int i);
extern void subtime(struct timeval *a, struct timeval *b, struct timeval *res);
extern void setupset(fd_set *theset, int *numfds);
extern void waitforconnects();
extern int tcpcheck_main(int argc, char *argv[]);
extern int tcpcheck_retval(int timeout, char *host_port);

// readmem.c
#ifdef BUILD_READMEM
extern int readmem_main(int argc, char *argv[]);
#endif

// usb_devices.c
#ifdef RTCONFIG_USB
extern int asus_sd(const char *device_name, const char *action);
extern int asus_lp(const char *device_name, const char *action);
extern int asus_sr(const char *device_name, const char *action);
extern int asus_tty(const char *device_name, const char *action);
extern int asus_usb_interface(const char *device_name, const char *action);
extern int asus_sg(const char *device_name, const char *action);
extern int asus_usbbcm(const char *device_name, const char *action);
#ifdef RTCONFIG_USB_MODEM
extern int is_create_file_dongle(const unsigned int vid, const unsigned int pid);
#ifdef RTCONFIG_USB_BECEEM
extern int is_beceem_dongle(const int mode, const unsigned int vid, const unsigned int pid);
extern int is_samsung_dongle(const int mode, const unsigned int vid, const unsigned int pid);
extern int is_gct_dongle(const int mode, const unsigned int vid, const unsigned int pid);
extern int write_beceem_conf(const char *eth_node);
extern int write_gct_conf(void);
#endif
extern int is_android_phone(const int mode, const unsigned int vid, const unsigned int pid);
extern int is_storage_cd(const unsigned int vid, const unsigned int pid);
extern int is_gobi_dongle(const unsigned int vid, const unsigned int pid);
extern int write_3g_conf(FILE *fp, int dno, int aut, const unsigned int vid, const unsigned int pid);
extern int init_3g_param(const char *port_path, const unsigned int vid, const unsigned int pid);
extern int write_3g_ppp_conf(int modem_unit);
#endif
#endif

#ifdef RTCONFIG_DSL
//dsl.c
extern void dsl_configure(int req);
extern void start_dsl(void);
extern void stop_dsl(void);
extern void remove_dsl_autodet(void);
extern void dsl_defaults(void);
#endif

//services.c
extern void write_static_leases(FILE *fp);
#ifdef RTCONFIG_DHCP_OVERRIDE
extern int restart_dnsmasq(int need_link_DownUp);
#endif
extern void start_dnsmasq(void);
extern void stop_dnsmasq(void);
extern void reload_dnsmasq(void);
#if defined(RTCONFIG_TR069) || defined(RTCONFIG_AMAS)
extern int dnsmasq_script_main(int argc, char **argv);
#endif
#ifdef RTCONFIG_DNSPRIVACY
extern void start_stubby(void);
extern void stop_stubby(void);
#endif
extern int ddns_updated_main(int argc, char *argv[]);
#ifdef RTCONFIG_IPV6
extern void add_ip6_lanaddr(void);
extern void start_ipv6_tunnel(void);
extern void stop_ipv6_tunnel(void);
extern void start_ipv6(void);
extern void stop_ipv6(void);
extern void ipv6_sysconf(const char *ifname, const char *name, int value);
extern int ipv6_getconf(const char *ifname, const char *name);
#ifdef RTCONFIG_DUALWAN
extern void restart_dnsmasq_ipv6();
#endif
#endif
extern int wps_band_radio_off(int wps_band);
#ifdef CONFIG_BCMWL5
#ifdef RTCONFIG_FANCTRL
extern int stop_phy_tempsense(void);
extern int start_phy_tempsense(void);
#endif
#ifdef RTCONFIG_WLCEVENTD
extern int start_wlceventd(void);
extern void stop_wlceventd(void);
#endif
#ifdef RTCONFIG_HSPOT
extern void stop_hspotap(void);
extern int start_hspotap(void);
#endif
extern void start_igmp_proxy(void);
extern void stop_igmp_proxy(void);
#if defined(BCM_BSD)
extern int start_bsd(void);
extern void stop_bsd(void);
#endif
#ifdef BCM_ASPMD
extern int start_aspmd(void);
extern void stop_aspmd(void);
#endif /* BCM_ASPMD */
#ifdef BCM_EVENTD
extern int start_eventd(void);
extern void stop_eventd(void);
#endif
#ifdef BCM_APPEVENTD
extern int start_appeventd(void);
extern void stop_appeventd(void);
#endif
extern int start_eapd(void);
extern void stop_eapd(void);
extern int start_nas(void);
extern void stop_nas(void);
#ifdef RTCONFIG_BCMWL6
extern void stop_acsd(void);
extern int start_acsd();
extern void set_acs_ifnames();
#ifdef RTCONFIG_PROXYSTA
extern int stop_psta_monitor();
extern int start_psta_monitor();
#endif
extern int wl_igs_enabled(void);
#endif
#ifdef RTCONFIG_DHDAP
extern int start_dhd_monitor(void);
extern int stop_dhd_monitor(void);
#endif
#if defined(BCA_HNDROUTER) && defined(MCPD_PROXY)
extern void start_mcpd_proxy(void);
extern void stop_mcpd_proxy(void);
extern void restart_mcpd_proxy(void);
#endif
#endif

#ifdef RTCONFIG_AMAS
extern void stop_obd(void);
extern void start_obd(void);
#endif
#ifdef RTCONFIG_ETHOBD
extern void stop_obd_monitor(void);
extern void start_obd_monitor(void);
extern void stop_eth_obd(void);
extern void start_eth_obd(void);
#endif
#ifdef RTCONFIG_CFGSYNC
extern void update_macfilter_relist();
#endif

extern int start_nat_rules(void);
extern int stop_nat_rules(void);
extern int start_syslogd(void);
extern void stop_syslogd(void);
#ifdef RTCONFIG_RSYSLOGD
static inline void reload_syslogd(void) {};
#else
extern void reload_syslogd(void);
extern int start_klogd(void);
extern void stop_klogd(void);
#endif
extern int start_logger(void);
extern void start_dfs(void);
extern void handle_notifications(void);
#ifdef RTL_WTDOG
extern void stop_rtl_watchdog(void);
extern void start_rtl_watchdog(void);
#endif
extern void stop_watchdog(void);
extern void stop_check_watchdog(void);
extern void stop_watchdog02(void);
extern int restart_dualwan(void);
extern int start_watchdog(void);
extern int start_check_watchdog(void);
extern int start_watchdog02(void);
#ifdef SW_DEVLED
extern int start_sw_devled(void);
extern void stop_sw_devled(void);
#endif
#if defined(RTAC1200G) || defined(RTAC1200GP)
extern void stop_wdg_monitor(void);
extern int start_wdg_monitor(void);
#endif
extern int get_apps_name(const char *string);
extern int run_app_script(const char *pkg_name, const char *pkg_action);
extern void set_hostname(void);
extern int _start_telnetd(int force);
extern int start_telnetd(void);
extern void stop_telnetd(void);
#ifdef RTCONFIG_SSH
extern int start_sshd(void);
extern void stop_sshd(void);
#endif
#ifdef RTCONFIG_WTFAST
extern void start_wtfast(void);
extern void stop_wtfast(void);
#endif
#ifdef RTCONFIG_TCPLUGIN
extern void start_qmacc(void);
extern void stop_qmacc(void);
#endif
extern void start_hotplug2(void);
extern void stop_services(void);
extern void stop_services_mfg(void);
extern void stop_logger(void);
extern void setup_passwd(void);
extern void create_passwd(void);
extern int start_services(void);
#ifdef RTCONFIG_FBWIFI
extern void start_httpd_uam();
extern void stop_httpd_uam();
extern void stop_fbwifi_register();
extern void clean_certificaion_rules();
extern void stop_fbwifi();
extern void my_mkdir(char *path);
extern void start_fbwifi();
extern void restart_fbwifi();
extern void set_fbwifi_profile(void);
extern void overwrite_fbwifi_ssid(void);
#endif
#if defined(RTCONFIG_RGBLED)
extern void start_aurargb(void);
#endif
extern void check_services(void);
extern int no_need_to_start_wps(void);
extern int wps_band_radio_off(int wps_band);
extern void check_wps_enable();
extern int start_wanduck(void);
extern void stop_wanduck(void);
extern void stop_ntpc(void);
extern int start_ntpc(void);
#ifdef RTCONFIG_UPNPC
extern int start_miniupnpc(void);
extern void stop_miniupnpc(void);
#endif
#ifdef RTCONFIG_BONJOUR
extern int start_netmonitor(void);
extern void stop_netmonitor(void);
#endif
extern void stop_networkmap(void);
extern int start_networkmap(int bootwait);
extern int stop_wps(void);
extern int start_wps(void);
extern void stop_upnp(void);
extern void start_upnp(void);
extern void reload_upnp(void);
extern void stop_ddns(void);
extern int start_ddns(void);
extern void refresh_ntpc(void);
extern void start_hotplug2(void);
extern void stop_hotplug2(void);
extern void stop_lltd(void);
extern void stop_httpd(void);
extern void stop_rstats(void);
extern void stop_autodet(void);
extern void start_autodet(void);
extern int wps_band_ssid_broadcast_off(int wps_band);
#ifdef RTCONFIG_QCA_PLC_UTILS
extern void start_plcdet(void);
#endif
extern void start_httpd(void);
extern int wl_wpsPincheck(char *pin_string);
extern int start_wps_pbc(int unit);
#if defined(RTCONFIG_RALINK)
extern int exec_8021x_start(int band, int is_iNIC);
extern int exec_8021x_stop(int band, int is_iNIC);
extern int start_8021x(void);
extern int stop_8021x(void);
#endif
extern int firmware_check_main(int argc, char *argv[]);
#ifdef RTCONFIG_HTTPS
extern int rsasign_check_main(int argc, char *argv[]);
extern int rsarootca_check_main(int argc, char *argv[]);
extern char *pwdec(const char *input, char *output, int output_len);
extern char *pwdec_dsl(char *input);
#endif
#ifdef RTCONFIG_ISP_CUSTOMIZE
extern int package_verify_main(int argc, char *argv[]);
#endif
extern int service_main(int argc, char *argv[]);
#ifdef RTCONFIG_DSL
extern void stop_spectrum(void);
extern int check_tc_upgrade(void);
extern int start_tc_upgrade(void);
#ifdef RTCONFIG_DSL_TCLINUX
extern void start_dsl_autodet(void);
extern void stop_dsl_autodet(void);
extern void stop_dsl_diag(void);
extern int start_dsl_diag(void);
#endif
#endif
#ifdef RTCONFIG_FRS_LIVE_UPDATE
extern int firmware_check_update_main(int argc, char *argv[]);
#endif
#ifdef RTCONFIG_FRS_FEEDBACK
extern void start_sendfeedback(void);
#ifdef RTCONFIG_DBLOG
extern void start_senddblog(char *path);
extern void start_dblog(int option);
extern void stop_dblog(void);
#endif /* RTCONFIG_DBLOG */
#ifdef RTCONFIG_DSL_TCLINUX
extern void start_sendDSLdiag(void);
#endif
#endif /* RTCONFIG_FRS_FEEDBACK */
#ifdef RTCONFIG_SNMPD
extern void start_snmpd(void);
extern void stop_snmpd(void);
#endif
extern int ddns_custom_updated_main(int argc, char *argv[]);
extern void setup_leds(void);
#ifdef RTCONFIG_TIMEMACHINE
extern int start_timemachine(void);
extern void stop_timemachine(int force);
extern int start_afpd(void);
extern void stop_afpd(int force);
extern int start_cnid_metad(void);
extern void stop_cnid_metad(int force);
extern int start_avahi_daemon(void);
extern void stop_avahi_daemon(void);
#endif
#ifdef RTCONFIG_QUAGGA
extern void stop_quagga(void);
extern int start_quagga(void);
#endif
#ifdef RTCONFIG_RGBLED
extern void start_aurargb(void);
#endif
#if defined(RTCONFIG_MDNS)
extern int generate_mdns_config(void);
extern int generate_afpd_service_config(void);
extern int generate_adisk_service_config(void);
extern int generate_itune_service_config(void);
extern int start_mdns(void);
extern void stop_mdns(void);
extern void restart_mdns(void);
extern int mkdir_if_none(const char *path);
#endif
extern void start_snooper(void);
extern void stop_snooper(void);
#if defined(CONFIG_BCMWL5) && !defined(HND_ROUTER) && defined(RTCONFIG_DUALWAN)
extern int restart_dualwan(void);
#endif
#ifdef __CONFIG_NORTON__
extern int start_norton(void);
extern int stop_norton(void);
#endif

#ifdef RTCONFIG_MEDIA_SERVER
void force_stop_dms(void);
void stop_mt_daapd(int force);
void start_dms(void);
void start_mt_daapd(void);
void set_invoke_later(int flag);
int get_invoke_later(void);
#endif	/* RTCONFIG_MEDIA_SERVER */

#if defined(CONFIG_BCMWL5) \
		|| (defined(RTCONFIG_RALINK) && defined(RTCONFIG_WIRELESSREPEATER)) \
		|| defined(RTCONFIG_QCA) || defined(RTCONFIG_REALTEK) \
		|| defined(RTCONFIG_QSR10G) || defined(RTCONFIG_LANTIQ)
void start_wlcscan(void);
void stop_wlcscan(void);
#endif
#ifdef RTCONFIG_WIRELESSREPEATER
void start_wlcconnect(void);
void stop_wlcconnect(void);
void repeater_nat_setting(void);
void repeater_filter_setting(int mode);
#if defined(RTCONFIG_RALINK)
void apcli_start(void);
int site_survey_for_channel(int n, const char *wif, int *HT_EXT);
#endif
#ifdef RTCONFIG_AMAS
int amas_lanctrl_main(void);
int amas_bhctrl_main(void);
void start_amas_wlcconnect(void);
void stop_amas_wlcconnect(void);
extern int amas_wlcconnect_main(void);
void start_amas_bhctrl(void);
void stop_amas_bhctrl(void);
extern int amas_bhctrl_main(void);
void start_amas_lanctrl(void);
void stop_amas_lanctrl(void);
void start_amas_lldpd(void);
void stop_amas_lldpd(void);
extern void gen_lldpd_if(char *bind_ifnames);
void set_pre_sysdep_config(int iftype);
void set_post_sysdep_config(int iftype);
int get_radar_status(int bssidx);
int Pty_procedure_check(int unit, int wlif_count);
#ifdef RTCONFIG_BHCOST_OPT
void apply_config_to_driver(int band);
#else
void apply_config_to_driver();
#endif
extern int amas_ssd_main(void);
void start_amas_ssd(void);
void stop_amas_ssd(void);
#ifdef RTCONFIG_BHCOST_OPT
void start_amas_status(void);
void stop_amas_status(void);
extern int amas_status_main(void);
extern int amas_misc_main(void);
void start_amas_misc(void);
void stop_amas_misc(void);
#endif
#endif
#endif	/* RTCONFIG_WIRELESSREPEATER */
#if defined(RTCONFIG_QCA_LBD)
extern void duplicate_wl_ifaces(void);
#endif

#ifdef RTCONFIG_PARENTALCTRL
extern void stop_pc_block(void);
extern void start_pc_block(void);
#endif
#ifdef RTCONFIG_HTTPS
extern int check_rsasign(char *fname);
#endif

#ifdef RTCONFIG_COOVACHILLI
extern void chilli_addif2br0(const char *ifname);
extern void main_config(void);
extern void chilli_config(void);
extern void chilli_config_CP(void);
extern void hotspotsys_config(void);
extern void stop_chilli(void);
extern void stop_CP(void);
extern void Checkifnames(char *nvifnames, char *ifname);
extern void bridge_ifByA(char *ifs, char *, int);
extern void DN2tmpfile(char *name);
extern void start_CP(void);
extern void start_chilli(void);
void chilli_localUser_passcode(void);
void chilli_localUser(void);
#endif
#ifdef RTCONFIG_CAPTIVE_PORTAL
extern void start_uam_srv(void);
extern void set_captive_portal_adv_wl(void);
extern void set_captive_portal_wl(void);
extern void overwrite_captive_portal_ssid(void);
extern void overwrite_captive_portal_adv_ssid(void);
#endif
#if defined(RTCONFIG_COOVACHILLI)
extern int restart_coovachilli_if_conflicts(char *wan_ip, char *wan_mask);
#endif
#ifdef RTCONFIG_PERMISSION_MANAGEMENT
extern void PMS_Init_Database();
#endif
#if defined (RTCONFIG_BT_CONN)
extern void start_bluetooth_service(void);
extern void stop_bluetooth_service(void);
#if defined(RTCONFIG_AMAS) && defined(RTCONFIG_PRELINK)
extern void ble_rename_ssid(void);
extern int check_bluetooth_device(const char *bt_dev);
#endif
#endif	/* RTCONFIG_BT_CONN */
#if defined (RTCONFIG_DETWAN)
extern int string_remove(char *string, const char *match);
#endif
#ifdef RTCONFIG_CFGSYNC
extern void stop_cfgsync(void);
extern int start_cfgsync(void);
extern void send_event_to_cfgmnt(int event_id);
#ifdef RTCONFIG_CONNDIAG
extern int conn_diag_main(int argc, char *argv[]);
extern int diag_data_main(int argc, char *argv[]);
extern void stop_conn_diag(void);
extern void start_conn_diag(void);
#endif
#if defined(RTCONFIG_WIFI_DRV_DISABLE) /* for IPQ40XX */
extern int setDisableWifiDrv(const char *);
extern int getDisableWifiDrv(void);
#endif
#endif
#ifdef RTCONFIG_NOTIFICATION_CENTER
extern int start_wlc_nt(void);
extern void stop_wlc_nt(void);
extern int stop_notification_center(void);
#endif
#ifdef RTCONFIG_PROTECTION_SERVER
extern int start_ptcsrv(void);
extern void stop_ptcsrv(void);
#endif
#ifdef RTCONFIG_NETOOL
extern int start_netool(void);
extern void stop_netool(void);
#endif
#ifdef LANTIQ_BSD
extern int start_bsd(void);
extern void stop_bsd(void);
#endif


//wireless.c
extern int wlcscan_main(void);
extern void repeater_pap_disable(void);
extern int wlcconnect_main(void);
#if defined(RTCONFIG_BLINK_LED)
extern void update_wifi_led_state_in_wlcmode(void);
#else
static inline void update_wifi_led_state_in_wlcmode(void) { }
#endif
#if defined(RTCONFIG_QCA) || defined(RTCONFIG_RALINK)
extern int dump_powertable(void);
#endif

//speedtest.c
extern int speedtest_main(int argc, char **argv);
extern void wan_bandwidth_detect(void);
extern int speedtest();

#if defined(RTCONFIG_BWDPI)
extern int bwdpi_main(int argc, char **argv);
extern int bwdpi_check_main(int argc, char **argv);
extern int bwdpi_wred_alive_main(int argc, char **argv);
extern int show_wrs_main(int argc, char **argv);
extern int rsasign_sig_check_main(int argc, char *argv[]);
extern int bwdpi_db_10_main(int argc, char **argv);
extern void stop_bwdpi_check();
extern void start_bwdpi_check();
extern void stop_bwdpi_wred_alive();
extern void extract_data(char *path, FILE *fp);
extern int merge_log(char *path, int len);
extern void stop_dpi_engine_service(int forced);
extern void start_dpi_engine_service();
extern void start_wrs_wbl_service();
extern void setup_wrs_conf();
extern void auto_sig_check();
extern void sqlite_db_check();
extern void tm_eula_check();
extern int tm_qos_main(char *cmd);
extern int qosd_main(char *cmd);
extern int wrs_main(char *cmd);
extern int stat_main(char *mode, char *name, char *dura, char *date);
extern int clear_user_domain(void);
extern int web_history_main(char *MAC, char *page);
extern int wrs_app_main(char *cmd);
extern int set_cc(char *cmd);
extern int set_vp(char *cmd);
extern int get_vp(char *cmd);
extern int data_collect_main(char *cmd, char *path);
extern int device_main();
extern int device_info_main(char *MAC, char *ipaddr);
extern int wrs_url_main();
extern int rewrite_main(char *path1, char *path2, char *path3);
extern int extract_data_main(char *path);
extern int get_anomaly_main(char *cmd);
extern int get_app_patrol_main();
extern void web_history_save();
extern void AiProtectionMonitor_mail_log();
extern int get_fw_mesh_user(void **output, unsigned int *buf_used_len);
extern int get_fw_mesh_extender(void **output, unsigned int *buf_used_len);
extern int mesh_set_user(char *macstr, char *ipstr, uint8_t action);
extern int mesh_set_extender(char *macstr, uint8_t action);
#endif

/* amas_lib.c */
#ifdef RTCONFIG_AMAS
extern int amas_lib_device_ip_query(char *mac, char *ip);
extern int amas_lib_main(int argc, char **argv);
extern void stop_amas_lib();
extern void start_amas_lib();
extern int amaslib_lease_main(int argc, char **argv);
extern void amaslib_check();
extern int amas_ipc_main(int argc, char *argv[]);
#endif

// tcode_rc.c
extern int config_tcode(int type);

// hour_monitor.c
extern int hour_monitor_main(int argc, char **argv);
extern int hour_monitor_function_check();
extern void check_hour_monitor_service();
extern void hm_traffic_analyzer_save();
extern void hm_traffic_limiter_save();

// tagged_based_vlan.c
#if defined(RTCONFIG_TAGGED_BASED_VLAN)
extern char *get_info_by_subnet(char *subnet_name, int field, char *result);
extern int vlan_enable(void);
extern void clean_vlan_config(void);
extern void set_default_vlan_config(int vlan_id_tmp, int vlan_prio_tmp, int lanportset);
extern void set_vlan_config(int index, int vlan_id_tmp, int vlan_prio_tmp, int lanportset, int wlmap, char *subnet_name, char *vlan_if);
extern int check_if_exist_vlan_ifnames(char *ifname_in);
extern void get_lan_if_for_vlan(char *ifname,int size);
extern void get_default_vlaninfo(int *vid, int *prio, int *portlist);
extern void pvid_info_get_brtac828(int *pvid_array);
extern void config_PVID(void);
extern void start_tagged_based_vlan(char *input);
extern void stop_vlan_ifnames(void);
extern void stop_vlan_wl_ifnames(void);
extern int check_used_subnet(char *subnet_name, char *brif);
extern int check_internet(char *name);
extern int check_intranet_only(char *name);
extern void vlan_subnet_dnsmasq_conf(FILE *fp);
extern void vlan_subnet_filter_input(FILE *fp);
extern void vlan_subnet_filter_forward(FILE *fp, char *wan_if);
extern int check_exist_subnet_access_rule(int index, int subnet_group_tmp);
extern void vlan_subnet_deny_input(FILE *fp);
extern void vlan_subnet_deny_forward(FILE *fp);
extern void vlan_lanaccess_mssid_ban(const char *limited_ifname, char *ip, char *netmask);
extern void vlan_lanaccess_wl(void);
extern int get_vlan_info_by_lanX(char *lan_prefix, int *vid, int *prio, int *portlist);
extern void vlan_if_allow_list_set(unsigned int wan_allow_list, unsigned int lan_allow_list, unsigned int wl_allow_list);
extern int iptv_and_dualwan_info_get(int *iptv_vids,int size, unsigned int *wan_deny_list, unsigned int *lan_deny_list);
extern void cp_str2list( char * if_list, unsigned int *wl_allow_list);
extern int captive_protal_info_get(unsigned int *wl_allow_list);
extern int init_tagged_based_vlan(void);
extern int find_brifname_by_wlifname(char *wl_ifname, char *brif_name, int size);
extern void vlan_subnet_dnsmasq_conf(FILE *fp);
#else
static inline int find_brifname_by_wlifname(char __attribute__((__unused__)) *wl_ifname, char __attribute__((__unused__)) *brif_name, int __attribute__((__unused__)) size) { return 0; }
#endif

// amas_wgn.c
#ifdef RTCONFIG_AMAS_WGN
extern void wgn_init(void);
extern void wgn_start(void);
extern void wgn_stop(void);
extern void wgn_filter_forward(FILE *fp);
extern void wgn_filter_input(FILE *fp);
extern void wgn_dnsmasq_conf(FILE *fp);
extern void wgn_check_subnet_conflict(void);
extern void wgn_check_avalible_brif(void);
extern void wgn_hotplug_net(char *interface, int action /* 0:del, 1:add */);
extern void wgn_sysdep_swtich_set(int vid);
extern void wgn_sysdep_swtich_unset(int vid);
extern int wgn_is_wds_guest_vlan(char *ifname);
extern int wgn_if_check_used(char *ifname);
extern int wgn_is_enabled(void);
extern char* wgn_all_lan_ifnames(void);
extern char* wgn_guest_lan_ifnames(char *ret_ifnames, size_t ret_ifnames_bsize);
extern char* wgn_guest_lan_ipaddr(const char *guest_wlif, char *result, size_t result_bsize);
extern char* wgn_guest_lan_netmask(const char *guest_wlif, char *result, size_t result_bsize);
#endif

// traffic_limiter.c
#ifdef RTCONFIG_TRAFFIC_LIMITER
extern void traffic_limiter_sendSMS(const char *type, int unit);
extern void traffic_limiter_limitdata_check(void);
extern int traffic_limiter_wanduck_check(int unit);
extern void reset_traffic_limiter_counter(int force);
extern void init_traffic_limiter(void);
#endif


#ifdef RTCONFIG_USB_MODEM
#ifdef RTCONFIG_INTERNAL_GOBI
extern int lteled_main(int argc, char **argv);
extern int start_lteled(void);
extern void stop_lteled(void);
#endif
#endif

#ifdef RTCONFIG_TOR
extern void start_Tor_proxy(void);
#endif
#ifdef RTCONFIG_CLOUDCHECK
void stop_cloudcheck(void);
void start_cloudcheck(void);
#endif

#ifdef RTCONFIG_IPERF
// monitor.c
extern int monitor_main(int argc, char *argv[]);
#endif

#ifdef RTCONFIG_TR069
extern int start_tr(void);
extern void stop_tr(void);
extern int tr_lease_main(int argc, char *argv[]);
#endif

#ifdef RTCONFIG_NEW_USER_LOW_RSSI
extern void stop_roamast(void);
extern void start_roamast(void);
extern int roam_assistant_main(int argc, char *argv[]);
#endif

#ifdef RTCONFIG_DHCP_OVERRIDE
extern int detectWAN_arp_main(int argc, char **argv);
#endif

#if defined(RTCONFIG_KEY_GUARD)
extern void stop_keyguard(void);
extern void start_keyguard(void);
void start_ecoguard(void)
extern int keyguard_main(int argc, char *argv[]);
#endif
extern void start_ecoguard(void);

extern void start_ecoguard(void);

#ifdef BTN_SETUP
enum BTNSETUP_STATE
{
	BTNSETUP_NONE=0,
	BTNSETUP_DETECT,
	BTNSETUP_START,
	BTNSETUP_DATAEXCHANGE,
	BTNSETUP_DATAEXCHANGE_FINISH,
	BTNSETUP_DATAEXCHANGE_EXTEND,
	BTNSETUP_FINISH
};
#endif

/* led_monitor.c */
#if defined(RTCONFIG_CONCURRENTREPEATER)
extern int re_wpsc_main(void);
extern int stop_re_wpsc();
extern int start_re_wpsc();

typedef struct led_state_s {
	int id;
	int color;
	int state;
	unsigned long flash_interval;
	unsigned long next_switch_time;
	short changed;
} led_state_t;

extern int led_monitor_main(int argc, char *argv[]);
extern int stop_led_monitor();
extern int start_led_monitor();
extern int set_off_led(led_state_t *led);
extern int set_on_led(led_state_t *led);
#ifdef RTCONFIG_REALTEK
extern void update_gpiomode(int gpio, int mode);
#endif
#if defined(RTCONFIG_RALINK)
extern int air_monitor_main(int argc, char *argv[]);
#endif

enum LED_STATUS
{
	LED_BOOTING = 0,
	LED_BOOTED,
	LED_BOOTED_APMODE,
	LED_WIFI_2G_DOWN,
	LED_WIFI_5G_DOWN,
	LED_WPS_START,
	LED_WPS_SCANNING,
	LED_WPS_2G_SCANNING,
	LED_WPS_5G_SCANNING,
	LED_WPS_FAIL,
	LED_WPS_PROCESSING,
	LED_WPS_RESTART_WL,
	LED_RESTART_WL,
	LED_RESTART_WL_DONE,
	LED_FIRMWARE_UPGRADE,
	LED_FACTORY_RESET,
	LED_AP_WPS_START
};
#endif

#ifdef RTCONFIG_TUNNEL
extern void start_mastiff();
extern void stop_mastiff();
extern void start_aae_sip_conn(int sdk_init);
extern void stop_aae_sip_conn(int sdk_deinit);
extern void stop_aae_gently();
#endif
#ifdef RTCONFIG_HAPDEVENT
extern int start_hapdevent(void);
extern void stop_hapdevent(void);
#endif

#if defined(RTCONFIG_TCODE) && defined(CONFIG_BCMWL5)
extern char *cfe_nvram_get(const char *name);
static INLINE int
cfe_nvram_match(char *name, char *match) {
	const char *value = cfe_nvram_get(name);
	return (value && !strcmp(value, match));
}
extern char *cfe_nvram_safe_get(const char *name);
extern char *cfe_nvram_get_raw(const char *name);
extern char *cfe_nvram_safe_get_raw(const char *name);
extern int cfe_nvram_set(const char *name);
extern int refresh_cfe_nvram();
extern int factory_debug();
#if !(defined(RTCONFIG_CFEZ) && defined(RTCONFIG_BCMARM))
extern char *ATE_BRCM_PREFIX(void);
extern int ATE_BRCM_SET(const char *name, const char *value);
extern int ATE_BRCM_UNSET(const char *name);
extern void ATE_BRCM_COMMIT(void);
#endif
#endif

#if defined(RTCONFIG_LACP)
void config_lacp(void);
#endif

#if defined(CONFIG_BCMWL5) && defined(RTCONFIG_DUALWAN)
int dualwan_control(int argc, char *argv[]);
#endif

#ifdef RTCONFIG_PORT_BASED_VLAN
// vlan.c
extern int vlan_enable(void);
extern int check_if_exist_vlan_ifnames(char *ifname);
extern void start_vlan_ifnames(void);
extern void stop_vlan_ifnames(void);
extern void start_vlan_wl_ifnames(void);
extern void restart_vlan_wl(void);
extern void vlan_lanaccess_wl(void);
extern void set_port_based_vlan_config(char *interface);
extern void set_vlan_ifnames(int index, int wlmap, char *subnet_name, char *vlan_if);
extern void stop_vlan_wl_ifnames(void);
extern void start_vlan_wl(void);
#endif
#ifdef RTCONFIG_TAGGED_BASED_VLAN
// vlan.c
extern int vlan_enable(void);
extern int check_if_exist_vlan_ifnames(char *ifname);
extern void start_vlan_ifnames(void);
extern void stop_vlan_ifnames(void);
extern void start_vlan_wl_ifnames(void);
extern void restart_vlan_wl(void);
extern void vlan_lanaccess_wl(void);
extern void set_port_based_vlan_config(char *interface);
extern void set_vlan_ifnames(int index, int wlmap, char *subnet_name, char *vlan_if);
extern void stop_vlan_wl_ifnames(void);
extern void start_vlan_wl(void);
#endif

#ifdef RTCONFIG_TAGGED_BASED_VLAN
void set_vlan_config( 	int index,
						int vlan_id_tmp,
						int vlan_prio_tmp,
						int lanportset,
						int wlmap,
						char *subnet_name,
						char *vlan_if );
void start_tagged_based_vlan(char *);
#endif

#if defined(RTCONFIG_RALINK)
extern int dump_txbftable(void);
#endif

// timemachine.c
extern void find_backup_mac_date(char *mpname);
extern void write_timemachine_tokeninfo(char *mpname);

// nt_mail.c
#ifdef RTCONFIG_NOTIFICATION_CENTER
extern void alert_mail_service();
extern void am_setup_email_conf();
extern void am_setup_email_info();
#endif

#ifdef RTCONFIG_LETSENCRYPT
// letsencrypt.c
extern int start_letsencrypt(void);
extern int stop_letsencrypt(void);
extern int le_acme_main(int argc, char **argv);
extern int copy_le_certificate(char *dst_cert, char *dst_key);
extern int is_correct_le_certificate(char *cert_path);
extern void run_le_fw_script(void);
#endif

// netool.c
#ifdef RTCONFIG_NETOOL
extern int netool_main(int argc, char **argv);
#endif

// erp_monitor.c
#if !(defined(RTCONFIG_QCA) || defined(RTCONFIG_RALINK) || defined(RTCONFIG_REALTEK)) \
 ||  (defined(RTCONFIG_SOC_IPQ8074))
extern int erp_monitor_main(int argc, char **argv);
extern void stop_erp_monitor();
extern void start_erp_monitor();
#endif

#ifdef RTCONFIG_USB_SWAP
extern int stop_usb_swap(char *path);
extern int start_usb_swap(char *path);
#endif

#ifdef RTCONFIG_HD_SPINDOWN
void start_usb_idle(void);
void stop_usb_idle(void);
#endif

#ifdef RTCONFIG_UPLOADER
void start_uploader(void);
void stop_uploader(void);
#endif

// adtbw.c
#ifdef RTCONFIG_ADTBW
extern int adtbw_main(int argc, char **argv);
extern void stop_adtbw();
extern void start_adtbw();
#endif

// natnl_api.c
#ifdef RTCONFIG_TUNNEL
extern void start_aae();
#endif

// asm1042.c
#ifdef RTN65U
extern void asm1042_upgrade(int);
#endif

// private.c
#if defined(RTCONFIG_NOTIFICATION_CENTER)
extern void oauth_google_gen_token_email(void);
extern void oauth_google_drive_gen_token(void);
extern void oauth_google_update_token(void);
extern int oauth_google_send_message(const char* receiver, const char* subject, const char* message, const char* attached_files[], int attached_files_count);
extern void oauth_google_check_token_status(void);
extern void oauth_google_drive_check_token_status(void);

#endif

#ifdef RTCONFIG_UUPLUGIN
extern void start_uu();
extern void stop_uu();
#endif

#if defined(RTCONFIG_QCA_LBD)
#define LBD_PATH "/tmp/lbd.conf"
extern int gen_lbd_config_file(void);
extern void stop_qca_lbd(void);
extern void start_qca_lbd(void);
#endif

#ifdef RTCONFIG_ISP_CUSTOMIZE
extern int check_package_sign(char *fname, char *fsign);
extern char *find_customize_setting_by_name(const char *name);
extern int get_file_hash(const char *file, char *out, int *len);
extern int get_package_hash(char *out, int *len);
extern int get_package_version(char *out, int *len);
extern int delete_file(const char *file);
extern int delete_package();
extern int verify_package(char *out, int *len);
extern void load_customize_package();
extern void package_restore_defaults();
#endif

// dsl_fb.c
#ifdef RTCONFIG_FRS_FEEDBACK
extern int do_feedback(const char* feedback_file, char* attach_cmd);
#endif

#if defined(RTCONFIG_BCM_7114) || defined(HND_ROUTER)
typedef struct probe_4366_param_s {
	int bECode_2G;
	int bECode_5G;
	int bECode_5G_2;
	int bECode_fabid;
} probe_4366_param_t;
#endif /* RTCONFIG_BCM_7114 || HND_ROUTER */

#if defined(RTAX88U)
typedef struct probe_PCIE_param_s {
	int bPCIE_down;
} probe_PCIE_param_t;
#endif /* RTAX88U */

#ifdef RTCONFIG_ASUSCTRL
/* asusctrl */
extern void asus_ctrl_enband5grp();
extern int asus_ctrl_en(int cid);
extern int asus_ctrl_ignore();
#ifdef RTCONFIG_BCMARM
extern int asus_ctrl_write(char *asusctrl);
#else
static inline int asus_ctrl_write(char *asusctrl) { return 0; }
#endif
#endif

#ifdef RTCONFIG_BCMARM
typedef struct WiFi_temperature_s {
	double t2g;
	double t5g;
	double t5g2;
} WiFi_temperature_t;
double get_cpu_temp();
int get_wifi_temps(WiFi_temperature_t *wt);
#endif /* RTCONFIG_BCMARM */

#ifdef RTCONFIG_GN_WBL
extern void add_GN_WBL_EBTbrouteRule();
extern void add_GN_WBL_ChainRule(FILE *fp);
extern void add_GN_WBL_ForwardRule(FILE *fp);
#ifdef RTCONFIG_LANTIQ
extern void GN_WBL_restart();
#endif
#endif

#endif	/* __RC_H__ */

